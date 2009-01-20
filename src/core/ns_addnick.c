/* NickServ : ADDNICK (Core)
 *
 * (C) 2009 Xanadu IRC Services Team
 *
 * Based on the original code of Anope by the Anope Team.
 * Based on the original code of Epona by Lara.
 * Based on the original code of Services by Andy Church.
 *
 * $Id: ns_addnick.c 1.0 01-17-2009 $
 */

#include "module.h"

int do_addnick(User * u);
void myNickServHelp(User * u);
int do_mynicks(User * u);
int do_glist(User * u);

NickAlias *makealias(const char *nick, NickCore * nc);

int do_group(User * u);

int AnopeInit(int argc, char **argv)
{
    Command *c;

    moduleAddAuthor("Xanadu");
    moduleAddVersion("$Id: ns_addnick.c 1.0 01-17-2009 $");
    moduleSetType(CORE);

    c = createCommand("ADDNICK", do_addnick, NULL, NS_HELP_ADDNICK, -1, -1, -1, -1);
    moduleAddCommand(NICKSERV, c, MOD_UNIQUE);

    c = createCommand("GROUP", do_group, NULL, -1, -1, -1, -1, -1);
    moduleAddCommand(NICKSERV, c, MOD_UNIQUE);

    c = createCommand("MYNICKS", do_mynicks, NULL, -1, NS_HELP_MYNICKS, -1, NS_ADMIN_HELP_MYNICKS, NS_ADMIN_HELP_MYNICKS);
    moduleAddCommand(NICKSERV, c, MOD_UNIQUE);

    c = createCommand("GLIST", do_glist, NULL, -1, -1, -1, -1, -1);
    moduleAddCommand(NICKSERV, c, MOD_UNIQUE);

    moduleSetNickHelp(myNickServHelp);

    return MOD_CONT;
}

void AnopeFini(void)
{

}

void myNickServHelp(User * u)
{
    notice_lang(s_NickServ, u, NS_HELP_CMD_ADDNICK);
    notice_lang(s_NickServ, u, NS_HELP_CMD_MYNICKS);
}

int do_addnick(User * u)
{
    NickAlias *na, *target;
    NickCore *nc;
    char *nick = strtok(NULL, " ");
    char *pass = strtok(NULL, " ");
    int i;
    char tsbuf[16];
    char modes[512];
    int len;

    if (NSEmailReg && (findrequestnick(u->nick))) {
        notice_lang(s_NickServ, u, NS_REQUESTED);
        return MOD_CONT;
    }

    if (readonly) {
        notice_lang(s_NickServ, u, NS_ADDNICK_DISABLED);
        return MOD_CONT;
    }
    if (checkDefCon(DEFCON_NO_NEW_NICKS)) {
        notice_lang(s_NickServ, u, OPER_DEFCON_DENIED);
        return MOD_CONT;
    }

    if (RestrictOperNicks) {
        for (i = 0; i < RootNumber; i++) {
            if (stristr(u->nick, ServicesRoots[i]) && !is_oper(u)) {
                notice_lang(s_NickServ, u, NICK_CANNOT_BE_REGISTERED,
                            u->nick);
                return MOD_CONT;
            }
        }
        for (i = 0; i < servadmins.count && (nc = servadmins.list[i]); i++) {
            if (stristr(u->nick, nc->display) && !is_oper(u)) {
                notice_lang(s_NickServ, u, NICK_CANNOT_BE_REGISTERED,
                            u->nick);
                return MOD_CONT;
            }
        }
        for (i = 0; i < servopers.count && (nc = servopers.list[i]); i++) {
            if (stristr(u->nick, nc->display) && !is_oper(u)) {
                notice_lang(s_NickServ, u, NICK_CANNOT_BE_REGISTERED,
                            u->nick);
                return MOD_CONT;
            }
        }
    }

    if (!nick || !pass) {
        syntax_error(s_NickServ, u, "ADDNICK", NS_ADDNICK_SYNTAX);
    } else if (!(target = findnick(nick))) {
        notice_lang(s_NickServ, u, NICK_X_NOT_REGISTERED, nick);
    } else if (time(NULL) < u->lastnickreg + NSRegDelay) {
        notice_lang(s_NickServ, u, NS_ADDNICK_PLEASE_WAIT, NSRegDelay);
    } else if (u->na && (u->na->status & NS_VERBOTEN)) {
        alog("%s: %s@%s tried to use ADDNICK from forbidden nick %s.",
             s_NickServ, u->username, u->host, u->nick);
        notice_lang(s_NickServ, u, NICK_X_FORBIDDEN, u->nick);
    } else if (u->na && (u->na->nc->flags & NI_SUSPENDED)) {
        alog("%s: %s!%s@%s tried to use ADDNICK from suspended nick %s.",
             s_NickServ, u->nick, u->username, u->host, target->nick);
        notice_lang(s_NickServ, u, NICK_X_SUSPENDED, u->nick);
    } else if (u->na && NSNoGroupChange) {
        notice_lang(s_NickServ, u, NS_ADDNICK_CHANGE_DISABLED, s_NickServ);
    } else if (u->na && !nick_identified(u)) {
        notice_lang(s_NickServ, u, NICK_IDENTIFY_REQUIRED, s_NickServ);
    } else if (target && (target->nc->flags & NI_SUSPENDED)) {
        alog("%s: %s!%s@%s tried to use GROUP from SUSPENDED nick %s",
             s_NickServ, u->nick, u->username, u->host, target->nick);
        notice_lang(s_NickServ, u, NICK_X_SUSPENDED, target->nick);
    } else if (target->status & NS_VERBOTEN) {
        notice_lang(s_NickServ, u, NICK_X_FORBIDDEN, nick);
    } else if (u->na && target->nc == u->na->nc) {
        notice_lang(s_NickServ, u, NS_ADDNICK_SAME, target->nick);
    } else if (NSMaxAliases && (target->nc->aliases.count >= NSMaxAliases)
               && !nick_is_services_admin(target->nc)) {
        notice_lang(s_NickServ, u, NS_ADDNICK_TOO_MANY, target->nick,
                    s_NickServ, s_NickServ);
    } else if (enc_check_password(pass, target->nc->pass) != 1) {
        alog("%s: Failed ADDNICK for %s!%s@%s (invalid password).",
             s_NickServ, u->nick, u->username, u->host);
        notice_lang(s_NickServ, u, PASSWORD_INCORRECT);
        bad_password(u);
    } else {
        /* If the nick is already registered, drop it.
         * If not, check that it is valid.
         */
        if (u->na) {
            delnick(u->na);
        } else {
            int prefixlen = strlen(NSGuestNickPrefix);
            int nicklen = strlen(u->nick);

            if (nicklen <= prefixlen + 7 && nicklen >= prefixlen + 1
                && stristr(u->nick, NSGuestNickPrefix) == u->nick
                && strspn(u->nick + prefixlen,
                          "1234567890") == nicklen - prefixlen) {
                notice_lang(s_NickServ, u, NICK_CANNOT_BE_REGISTERED,
                            u->nick);
                return MOD_CONT;
            }
        }
        na = makealias(u->nick, target->nc);

        if (na) {
            na->last_usermask =
                scalloc(strlen(common_get_vident(u)) +
                        strlen(common_get_vhost(u)) + 2, 1);
            sprintf(na->last_usermask, "%s@%s", common_get_vident(u),
                    common_get_vhost(u));
            na->last_realname = sstrdup(u->realname);
            na->time_registered = na->last_seen = time(NULL);
            na->status = (int16) (NS_IDENTIFIED | NS_RECOGNIZED);

            if (!(na->nc->flags & NI_SERVICES_ROOT)) {
                for (i = 0; i < RootNumber; i++) {
                    if (!stricmp(ServicesRoots[i], u->nick)) {
                        na->nc->flags |= NI_SERVICES_ROOT;
                        break;
                    }
                }
            }

            u->na = na;
            na->u = u;

#ifdef USE_RDB
            /* Is this really needed? Since this is a new alias it will get
             * its unique id on the next update, since it was previously
             * deleted by delnick. Must observe...
             */
            if (rdb_open()) {
                rdb_save_ns_alias(na);
                rdb_close();
            }
#endif
            send_event(EVENT_GROUP, 1, u->nick);
            alog("%s: %s!%s@%s makes %s join group of %s (%s) (e-mail: %s)", s_NickServ, u->nick, u->username, u->host, u->nick, target->nick, target->nc->display, (target->nc->email ? target->nc->email : "none"));
            notice_lang(s_NickServ, u, NS_ADDNICK_SUCCESS, target->nick);

            u->lastnickreg = time(NULL);
            snprintf(tsbuf, sizeof(tsbuf), "%lu",
                     (unsigned long int) u->timestamp);
            if (ircd->modeonreg) {
                len = strlen(ircd->modeonreg);
                strncpy(modes,ircd->modeonreg,512);
	       if(ircd->rootmodeonid && is_services_root(u)) { 
                    strncat(modes,ircd->rootmodeonid,512-len);
	        } else if(ircd->adminmodeonid && is_services_admin(u)) {
                    strncat(modes,ircd->adminmodeonid,512-len);
	        } else if(ircd->opermodeonid && is_services_oper(u)) {
                    strncat(modes,ircd->opermodeonid,512-len);
                }
                if (ircd->tsonmode) {
                    common_svsmode(u, modes, tsbuf);
                } else {
                    common_svsmode(u, modes, NULL);
                }
            }

            check_memos(u);
        } else {
            alog("%s: makealias(%s) failed", s_NickServ, u->nick);
            notice_lang(s_NickServ, u, NS_ADDNICK_FAILED);
        }
    }
    return MOD_CONT;
}

NickAlias *makealias(const char *nick, NickCore * nc)
{
    NickAlias *na;

    /* Just need to make the alias */
    na = scalloc(1, sizeof(NickAlias));
    na->nick = sstrdup(nick);
    na->nc = nc;
    slist_add(&nc->aliases, na);
    alpha_insert_alias(na);
    return na;
}

int do_group(User * u)
{
    notice_lang(s_NickServ, u, OBSOLETE_COMMAND, "ADDNICK");
    return MOD_CONT;
}

int do_mynicks(User * u)
{
    char *nick = strtok(NULL, " ");

    NickAlias *na, *na2;
    int is_servadmin = is_services_admin(u);
    int nick_ided = nick_identified(u);
    int i;

    if ((nick ? (stricmp(nick, u->nick) ? !is_servadmin : !nick_ided)
         : !nick_ided)) {
        notice_lang(s_NickServ, u,
                    (nick_ided ? ACCESS_DENIED :
                     NICK_IDENTIFY_REQUIRED), s_NickServ);
    } else if ((!nick ? !(na = u->na) : !(na = findnick(nick)))) {
        notice_lang(s_NickServ, u,
                    (!nick ? NICK_NOT_REGISTERED : NICK_X_NOT_REGISTERED),
                    nick);
    } else if (na->status & NS_VERBOTEN) {
        notice_lang(s_NickServ, u, NICK_X_FORBIDDEN, na->nick);
    } else {
        time_t expt;
        struct tm *tm;
        char buf[BUFSIZE];
        int wont_expire;

        notice_lang(s_NickServ, u,
                    nick ? NS_MYNICKS_HEADER_X : NS_MYNICKS_HEADER,
                    na->nc->display);
        for (i = 0; i < na->nc->aliases.count; i++) {
            na2 = na->nc->aliases.list[i];
            if (na2->nc == na->nc) {
                if (!(wont_expire = na2->status & NS_NO_EXPIRE)) {
                    expt = na2->last_seen + NSExpire;
                    tm = localtime(&expt);
                    strftime_lang(buf, sizeof(buf), na2->u,
                                  STRFTIME_DATE_TIME_FORMAT, tm);
                }
                notice_lang(s_NickServ, u,
                            ((is_services_admin(u) && !wont_expire)
                             ? NS_MYNICKS_REPLY_ADMIN : NS_MYNICKS_REPLY),
                            (wont_expire ? '!' : ' '), na2->nick, buf);
            }
        }
        notice_lang(s_NickServ, u, NS_MYNICKS_FOOTER,
                    na->nc->aliases.count);
    }
    return MOD_CONT;
}

int do_glist(User * u)
{
    notice_lang(s_NickServ, u, OBSOLETE_COMMAND, "MYNICKS");
    return MOD_CONT;
}

/* Definitions of IRC message functions and list of messages.
 *
 * (C) 2003-2007 Anope Team
 * Contact us at info@anope.org
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Epona by Lara.
 * Based on the original code of Services by Andy Church.
 *
 * $Id: messages.c 1265 2007-08-26 15:33:06Z geniusdex $
 *
 */

#include "services.h"
#include "messages.h"
#include "language.h"

int servernum;

/*************************************************************************/

int m_nickcoll(char *user)
{
    if (!skeleton && !readonly)
        introduce_user(user);
    return MOD_CONT;
}

/*************************************************************************/

int m_away(char *source, char *msg)
{
    User *u;

    u = finduser(source);

    if (u && msg == 0)          /* un-away */
        check_memos(u);
    return MOD_CONT;
}

/*************************************************************************/

int m_kill(char *nick, char *msg)
{
    BotInfo *bi;

    /* Recover if someone kills us. */
    /* use nickIsServices() to reduce the number of lines of code  - TSL */
    if (nickIsServices(nick, 0)) {
        if (!readonly && !skeleton)
            introduce_user(nick);
    } else if (s_BotServ && (bi = findbot(nick))) {
        if (!readonly && !skeleton) {
            introduce_user(nick);
            bot_rejoin_all(bi);
        }
    } else {
        do_kill(nick, msg);
    }
    return MOD_CONT;
}

/*************************************************************************/

int m_time(char *source, int ac, char **av)
{
    time_t t;
    struct tm *tm;
    char buf[64];

    if (!source) {
        return MOD_CONT;
    }

    time(&t);
    tm = localtime(&t);
    strftime(buf, sizeof(buf), "%a %b %d %H:%M:%S %Y %Z", tm);
    anope_cmd_391(source, buf);
    return MOD_CONT;
}

/*************************************************************************/

int m_motd(char *source)
{
    FILE *f;
    char buf[BUFSIZE];

    if (!source) {
        return MOD_CONT;
    }

    f = fopen(MOTDFilename, "r");
    if (f) {
        anope_cmd_375(source);
        while (fgets(buf, sizeof(buf), f)) {
            buf[strlen(buf) - 1] = 0;
            anope_cmd_372(source, buf);
        }
        fclose(f);
        anope_cmd_376(source);
    } else {
        anope_cmd_372_error(source);
    }
    return MOD_CONT;
}

/*************************************************************************/

int m_privmsg(char *source, char *receiver, char *msg)
{
    char *s;
    time_t starttime, stoptime; /* When processing started and finished */

    BotInfo *bi;
    ChannelInfo *ci;
    User *u;

    if (!source || !*source || !*receiver || !receiver || !msg) {
        return MOD_CONT;
    }

    u = finduser(source);

    if (!u) {
        alog("%s: user record for %s not found", msg, source);
        anope_cmd_notice(receiver, source,
                         getstring(NULL, USER_RECORD_NOT_FOUND));
        return MOD_CONT;
    }

    if (*receiver == '#') {
        if (s_BotServ && (ci = cs_findchan(receiver))) {
            /* Some paranoia checks */
            if (!(ci->flags & CI_VERBOTEN) && ci->c && ci->bi) {
                botchanmsgs(u, ci, msg);
            }
        }
    } else {
        /* Check if we should ignore.  Operators always get through. */
        if (allow_ignore && !is_oper(u)) {
            IgnoreData *ign = get_ignore(source);
            if (ign && ign->time > time(NULL)) {
                alog("Ignored message from %s: \"%s\"", source, inbuf);
                return MOD_CONT;
            }
        }

        /* If a server is specified (nick@server format), make sure it matches
         * us, and strip it off. */
        s = strchr(receiver, '@');
        if (s) {
            *s++ = 0;
            if (stricmp(s, ServerName) != 0)
                return MOD_CONT;
        } else if (UseStrictPrivMsg) {
            if (debug) {
                alog("Ignored PRIVMSG without @ from %s", source);
            }
            notice_lang(receiver, u, INVALID_TARGET, receiver, receiver,
                        ServerName, receiver);
            return MOD_CONT;
        }

        starttime = time(NULL);

        if ((stricmp(receiver, s_OperServ) == 0)
            || (s_OperServAlias
                && (stricmp(receiver, s_OperServAlias) == 0))) {
            if (!is_oper(u) && OSOpersOnly) {
                notice_lang(s_OperServ, u, ACCESS_DENIED);
                if (WallBadOS)
                    anope_cmd_global(s_OperServ,
                                     "Denied access to %s from %s!%s@%s (non-oper)",
                                     s_OperServ, u->nick, u->username,
                                     u->host);
            } else {
                operserv(u, msg);
            }
        } else if ((stricmp(receiver, s_NickServ) == 0)
                   || (s_NickServAlias
                       && (stricmp(receiver, s_NickServAlias) == 0))) {
            nickserv(u, msg);
        } else if ((stricmp(receiver, s_ChanServ) == 0)
                   || (s_ChanServAlias
                       && (stricmp(receiver, s_ChanServAlias) == 0))) {
            if (!is_oper(u) && CSOpersOnly)
                notice_lang(s_ChanServ, u, ACCESS_DENIED);
            else
                chanserv(u, msg);
        } else if ((stricmp(receiver, s_MemoServ) == 0)
                   || (s_MemoServAlias
                       && (stricmp(receiver, s_MemoServAlias) == 0))) {
            memoserv(u, msg);
        } else if (s_HostServ && ((stricmp(receiver, s_HostServ) == 0)
                                  || (s_HostServAlias
                                      &&
                                      (stricmp(receiver, s_HostServAlias)
                                       == 0)))) {
            hostserv(u, msg);
        } else if (s_HelpServ && ((stricmp(receiver, s_HelpServ) == 0)
                                  || (s_HelpServAlias
                                      &&
                                      (stricmp(receiver, s_HelpServAlias)
                                       == 0)))) {
            helpserv(u, msg);
        } else if (s_BotServ && ((stricmp(receiver, s_BotServ) == 0)
                                 || (s_BotServAlias
                                     && (stricmp(receiver, s_BotServAlias)
                                         == 0)))) {
            botserv(u, msg);
        } else if (s_BotServ && (bi = findbot(receiver))) {
            botmsgs(u, bi, msg);
        }

        /* Add to ignore list if the command took a significant amount of time. */
        if (allow_ignore) {
            stoptime = time(NULL);
            if (stoptime > starttime && *source && !strchr(source, '.'))
                add_ignore(source, stoptime - starttime);
        }
    }
    return MOD_CONT;
}

/*************************************************************************/

int m_stats(char *source, int ac, char **av)
{
    int i;
    User *u;
    NickCore *nc;

    if (ac < 1)
        return MOD_CONT;

    switch (*av[0]) {
    case 'l':
        u = finduser(source);

        if (u && is_oper(u)) {

            if (servernum == 1) {
                anope_cmd_211
                    ("%s Server SendBuf SentBytes SentMsgs RecvBuf "
                     "RecvBytes RecvMsgs ConnTime", source);
                anope_cmd_211("%s %s %d %d %d %d %d %d %ld", source,
                              RemoteServer, write_buffer_len(),
                              total_written, -1, read_buffer_len(),
                              total_read, -1, time(NULL) - start_time);
            } else if (servernum == 2) {
                anope_cmd_211
                    ("%s Server SendBuf SentBytes SentMsgs RecvBuf "
                     "RecvBytes RecvMsgs ConnTime", source);
                anope_cmd_211("%s %s %d %d %d %d %d %d %ld", source,
                              RemoteServer2, write_buffer_len(),
                              total_written, -1, read_buffer_len(),
                              total_read, -1, time(NULL) - start_time);
            } else if (servernum == 3) {
                anope_cmd_211
                    ("%s Server SendBuf SentBytes SentMsgs RecvBuf "
                     "RecvBytes RecvMsgs ConnTime", source);
                anope_cmd_211("%s %s %d %d %d %d %d %d %ld", source,
                              RemoteServer3, write_buffer_len(),
                              total_written, -1, read_buffer_len(),
                              total_read, -1, time(NULL) - start_time);
            }
        }

        anope_cmd_219(source, av[0]);
        break;
    case 'o':
    case 'O':
/* Check whether the user is an operator */
        u = finduser(source);
        if (u && !is_oper(u) && HideStatsO) {
            anope_cmd_219(source, av[0]);
        } else {
            for (i = 0; i < RootNumber; i++)
                anope_cmd_243("%s O * * %s Root 0", source,
                              ServicesRoots[i]);
            for (i = 0; i < servadmins.count && (nc = servadmins.list[i]);
                 i++)
                anope_cmd_243("%s O * * %s Admin 0", source, nc->display);
            for (i = 0; i < servopers.count && (nc = servopers.list[i]);
                 i++)
                anope_cmd_243("%s O * * %s Oper 0", source, nc->display);

            anope_cmd_219(source, av[0]);
        }

        break;

    case 'u':{
            int uptime = time(NULL) - start_time;
            anope_cmd_242("%s :Services up %d day%s, %02d:%02d:%02d",
                          source, uptime / 86400,
                          (uptime / 86400 == 1) ? "" : "s",
                          (uptime / 3600) % 24, (uptime / 60) % 60,
                          uptime % 60);
            anope_cmd_250("%s :Current users: %d (%d ops); maximum %d",
                          source, usercnt, opcnt, maxusercnt);
            anope_cmd_219(source, av[0]);
            break;
        }                       /* case 'u' */

    default:
        anope_cmd_219(source, av[0]);
        break;
    }
    return MOD_CONT;
}

/*************************************************************************/

int m_version(char *source, int ac, char **av)
{
    if (source) {
        anope_cmd_351(source);
    }
    return MOD_CONT;
}


/*************************************************************************/

int m_whois(char *source, char *who)
{
    BotInfo *bi;
    NickAlias *na;
    const char *clientdesc;

    if (source && who) {
        if (stricmp(who, s_NickServ) == 0)
            clientdesc = desc_NickServ;
        else if (stricmp(who, s_ChanServ) == 0)
            clientdesc = desc_ChanServ;
        else if (stricmp(who, s_MemoServ) == 0)
            clientdesc = desc_MemoServ;
        else if (s_BotServ && stricmp(who, s_BotServ) == 0)
            clientdesc = desc_BotServ;
        else if (s_HostServ && stricmp(who, s_HostServ) == 0)
            clientdesc = desc_HostServ;
        else if (stricmp(who, s_HelpServ) == 0)
            clientdesc = desc_HelpServ;
        else if (stricmp(who, s_OperServ) == 0)
            clientdesc = desc_OperServ;
        else if (stricmp(who, s_GlobalNoticer) == 0)
            clientdesc = desc_GlobalNoticer;
        else if (s_DevNull && stricmp(who, s_DevNull) == 0)
            clientdesc = desc_DevNull;
        else if (s_BotServ && (bi = findbot(who))) {
            /* Bots are handled separately */
            anope_cmd_311("%s %s %s %s * :%s", source, bi->nick,
                          bi->user, bi->host, bi->real);
            anope_cmd_307("%s %s :is a registered nick", source, bi->nick);
            anope_cmd_312("%s %s %s :%s", source, bi->nick, ServerName,
                          ServerDesc);
            anope_cmd_317("%s %s %ld %ld :seconds idle, signon time",
                          source, bi->nick, time(NULL) - bi->lastmsg,
                          start_time);
            anope_cmd_318(source, bi->nick);
            return MOD_CONT;
        } else if (!(ircd->svshold && UseSVSHOLD) && (na = findnick(who))
                   && (na->status & NS_KILL_HELD)) {
            /* We have a nick enforcer client here that we need to respond to.
             * We can't just say it doesn't exist here, even tho it does for
             * other servers :) -GD
             */
            anope_cmd_311("%s %s %s %s * :Services Enforcer", source,
                          na->nick, NSEnforcerUser, NSEnforcerHost);
            anope_cmd_312("%s %s %s :%s", source, na->nick, ServerName,
                          ServerDesc);
            anope_cmd_318(source, na->nick);
            return MOD_CONT;
        } else {
            anope_cmd_401(source, who);
            return MOD_CONT;
        }
        anope_cmd_311("%s %s %s %s * :%s", source, who,
                      ServiceUser, ServiceHost, clientdesc);
        anope_cmd_312("%s %s %s :%s", source, who, ServerName, ServerDesc);
        anope_cmd_317("%s %s %ld %ld :seconds idle, signon time", source,
                      who, time(NULL) - start_time, start_time);
        anope_cmd_318(source, who);
    }
    return MOD_CONT;
}

/* NULL route messages */
int anope_event_null(char *source, int ac, char **av)
{
    return MOD_CONT;
}

/* *INDENT-OFF* */
void moduleAddMsgs(void) {
    Message *m;
    m = createMessage("STATS",     m_stats); addCoreMessage(IRCD,m);
    m = createMessage("TIME",      m_time); addCoreMessage(IRCD,m);
    m = createMessage("VERSION",   m_version); addCoreMessage(IRCD,m);
}

/*************************************************************************/

Message *find_message(const char *name)
{
    Message *m;
    m = findMessage(IRCD, name);
    return m;
}

/*************************************************************************/

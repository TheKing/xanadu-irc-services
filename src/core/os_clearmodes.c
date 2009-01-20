/* OperServ core functions
 *
 * (C) 2003-2007 Anope Team
 * Contact us at info@anope.org
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Epona by Lara.
 * Based on the original code of Services by Andy Church. 
 * 
 * $Id: os_clearmodes.c 1265 2007-08-26 15:33:06Z geniusdex $
 *
 */
/*************************************************************************/

#include "module.h"

int do_clearmodes(User * u);
void myOperServHelp(User * u);

/**
 * Create the command, and tell anope about it.
 * @param argc Argument count
 * @param argv Argument list
 * @return MOD_CONT to allow the module, MOD_STOP to stop it
 **/
int AnopeInit(int argc, char **argv)
{
    Command *c;

    moduleAddAuthor("Anope");
    moduleAddVersion
        ("$Id: os_clearmodes.c 1265 2007-08-26 15:33:06Z geniusdex $");
    moduleSetType(CORE);

    c = createCommand("CLEARMODES", do_clearmodes, is_services_oper,
                      OPER_HELP_CLEARMODES, -1, -1, -1, -1);
    moduleAddCommand(OPERSERV, c, MOD_UNIQUE);

    moduleSetOperHelp(myOperServHelp);

    return MOD_CONT;
}

/**
 * Unload the module
 **/
void AnopeFini(void)
{

}


/**
 * Add the help response to anopes /os help output.
 * @param u The user who is requesting help
 **/
void myOperServHelp(User * u)
{
    if (is_services_oper(u)) {
        notice_lang(s_OperServ, u, OPER_HELP_CMD_CLEARMODES);
    }
}

/**
 * The /os clearmodes command.
 * @param u The user who issued the command
 * @param MOD_CONT to continue processing other modules, MOD_STOP to stop processing.
 **/
int do_clearmodes(User * u)
{
    char *s;
    int i;
    char *argv[2];
    char *chan = strtok(NULL, " ");
    Channel *c;
    int all = 0;
    int count;                  /* For saving ban info */
    char **bans;                /* For saving ban info */
    int exceptcount;            /* For saving except info */
    char **excepts;             /* For saving except info */
    int invitecount;            /* For saving invite info */
    char **invites;             /* For saving invite info */
    struct c_userlist *cu, *next;

    if (!chan) {
        syntax_error(s_OperServ, u, "CLEARMODES", OPER_CLEARMODES_SYNTAX);
        return MOD_CONT;
    } else if (!(c = findchan(chan))) {
        notice_lang(s_OperServ, u, CHAN_X_NOT_IN_USE, chan);
        return MOD_CONT;
    } else if (c->bouncy_modes) {
        notice_lang(s_OperServ, u, OPER_BOUNCY_MODES_U_LINE);
        return MOD_CONT;
    } else {
        s = strtok(NULL, " ");
        if (s) {
            if (stricmp(s, "ALL") == 0) {
                all = 1;
            } else {
                syntax_error(s_OperServ, u, "CLEARMODES",
                             OPER_CLEARMODES_SYNTAX);
                return MOD_CONT;
            }
        }

        if (WallOSClearmodes) {
            anope_cmd_global(s_OperServ, "%s used CLEARMODES%s on %s",
                             u->nick, all ? " ALL" : "", chan);
        }
        if (all) {
            /* Clear mode +o */
            if (ircd->svsmode_ucmode) {
                anope_cmd_svsmode_chan(c->name, "-o", NULL);
                for (cu = c->users; cu; cu = next) {
                    next = cu->next;
                    if (!chan_has_user_status(c, cu->user, CUS_OP)) {
                        continue;
                    }
                    argv[0] = sstrdup("-o");
                    argv[1] = cu->user->nick;
                    chan_set_modes(s_OperServ, c, 2, argv, 0);
                    free(argv[0]);
                }
            } else {
                for (cu = c->users; cu; cu = next) {
                    next = cu->next;
                    if (!chan_has_user_status(c, cu->user, CUS_OP))
                        continue;
                    argv[0] = sstrdup("-o");
                    argv[1] = cu->user->nick;
                    anope_cmd_mode(s_OperServ, c->name, "-o %s",
                                   cu->user->nick);
                    chan_set_modes(s_OperServ, c, 2, argv, 0);
                    free(argv[0]);
                }
            }

            if (ircd->svsmode_ucmode) {
                anope_cmd_svsmode_chan(c->name, "-v", NULL);
                for (cu = c->users; cu; cu = next) {
                    next = cu->next;
                    if (!chan_has_user_status(c, cu->user, CUS_VOICE)) {
                        continue;
                    }
                    argv[0] = sstrdup("-v");
                    argv[1] = cu->user->nick;
                    chan_set_modes(s_OperServ, c, 2, argv, 0);
                    free(argv[0]);
                }
            } else {
                /* Clear mode +v */
                for (cu = c->users; cu; cu = next) {
                    next = cu->next;
                    if (!chan_has_user_status(c, cu->user, CUS_VOICE))
                        continue;
                    argv[0] = sstrdup("-v");
                    argv[1] = sstrdup(cu->user->nick);
                    anope_cmd_mode(s_OperServ, c->name, "-v %s",
                                   cu->user->nick);
                    chan_set_modes(s_OperServ, c, 2, argv, 0);
                    free(argv[0]);
                }
            }

            /* Clear mode +h */
            if (ircd->svsmode_ucmode && ircd->halfop) {
                anope_cmd_svsmode_chan(c->name, "-h", NULL);
                for (cu = c->users; cu; cu = next) {
                    next = cu->next;
                    if (!chan_has_user_status(c, cu->user, CUS_HALFOP)) {
                        continue;
                    }
                    argv[0] = sstrdup("-h");
                    argv[1] = cu->user->nick;
                    chan_set_modes(s_OperServ, c, 2, argv, 0);
                    free(argv[0]);
                }
            } else {
                for (cu = c->users; cu; cu = next) {
                    next = cu->next;
                    if (!chan_has_user_status(c, cu->user, CUS_HALFOP))
                        continue;
                    argv[0] = sstrdup("-h");
                    argv[1] = sstrdup(cu->user->nick);
                    anope_cmd_mode(s_OperServ, c->name, "-h %s",
                                   cu->user->nick);
                    chan_set_modes(s_OperServ, c, 2, argv, 0);
                    free(argv[0]);
                }
            }
            /* Clear mode Owners */
            if (ircd->svsmode_ucmode && ircd->owner) {
                anope_cmd_svsmode_chan(c->name, ircd->ownerunset, NULL);
                for (cu = c->users; cu; cu = next) {
                    next = cu->next;
                    if (!chan_has_user_status(c, cu->user, CUS_HALFOP)) {
                        continue;
                    }
                    argv[0] = sstrdup(ircd->ownerunset);
                    argv[1] = cu->user->nick;
                    chan_set_modes(s_OperServ, c, 2, argv, 0);
                    free(argv[0]);
                }
            } else {
                for (cu = c->users; cu; cu = next) {
                    next = cu->next;
                    if (!chan_has_user_status(c, cu->user, CUS_OWNER))
                        continue;
                    argv[0] = sstrdup(ircd->ownerunset);
                    argv[1] = sstrdup(cu->user->nick);
                    anope_cmd_mode(s_OperServ, c->name, "%s %s",
                                   ircd->ownerunset, cu->user->nick);
                    chan_set_modes(s_OperServ, c, 2, argv, 0);
                    free(argv[0]);
                }
            }
            /* Clear mode protected or admins */
            if (ircd->svsmode_ucmode && (ircd->protect || ircd->admin)) {

                anope_cmd_svsmode_chan(c->name, ircd->adminunset, NULL);
                for (cu = c->users; cu; cu = next) {
                    next = cu->next;
                    if (!chan_has_user_status(c, cu->user, CUS_HALFOP)) {
                        continue;
                    }
                    argv[0] = sstrdup(ircd->adminunset);
                    argv[1] = cu->user->nick;
                    chan_set_modes(s_OperServ, c, 2, argv, 0);
                    free(argv[0]);
                }
            } else {
                for (cu = c->users; cu; cu = next) {
                    next = cu->next;
                    if (!chan_has_user_status(c, cu->user, CUS_PROTECT))
                        continue;
                    argv[0] = sstrdup(ircd->adminunset);
                    argv[1] = sstrdup(cu->user->nick);
                    anope_cmd_mode(s_OperServ, c->name, "%s %s",
                                   ircd->adminunset, cu->user->nick);
                    chan_set_modes(s_OperServ, c, 2, argv, 0);
                    free(argv[0]);
                }
            }


        }

        if (c->mode) {
            /* Clear modes the bulk of the modes */
            anope_cmd_mode(s_OperServ, c->name, "%s", ircd->modestoremove);
            argv[0] = sstrdup(ircd->modestoremove);
            chan_set_modes(s_OperServ, c, 1, argv, 0);
            free(argv[0]);

            /* to prevent the internals from complaining send -k, -L, -f by themselves if we need
               to send them - TSL */
            if (c->key) {
                anope_cmd_mode(s_OperServ, c->name, "-k %s", c->key);
                argv[0] = sstrdup("-k");
                argv[1] = c->key;
                chan_set_modes(s_OperServ, c, 2, argv, 0);
                free(argv[0]);
            }
            if (ircd->Lmode && c->redirect) {
                anope_cmd_mode(s_OperServ, c->name, "-L %s", c->redirect);
                argv[0] = sstrdup("-L");
                argv[1] = c->redirect;
                chan_set_modes(s_OperServ, c, 2, argv, 0);
                free(argv[0]);
            }
            if (ircd->fmode && c->flood) {
                if (flood_mode_char_remove) {
                    anope_cmd_mode(s_OperServ, c->name, "%s %s",
                                   flood_mode_char_remove, c->flood);
                    argv[0] = sstrdup(flood_mode_char_remove);
                    argv[1] = c->flood;
                    chan_set_modes(s_OperServ, c, 2, argv, 0);
                    free(argv[0]);
                } else {
                    if (debug) {
                        alog("debug: flood_mode_char_remove was not set unable to remove flood/throttle modes");
                    }
                }
            }
        }

        /* Clear bans */
        count = c->bancount;
        bans = scalloc(sizeof(char *) * count, 1);

        for (i = 0; i < count; i++)
            bans[i] = sstrdup(c->bans[i]);

        for (i = 0; i < count; i++) {
            argv[0] = sstrdup("-b");
            argv[1] = bans[i];
            anope_cmd_mode(s_OperServ, c->name, "-b %s", argv[1]);
            chan_set_modes(s_OperServ, c, 2, argv, 0);
            free(argv[1]);
            free(argv[0]);
        }

        free(bans);

        excepts = NULL;

        if (ircd->except) {
            /* Clear excepts */
            exceptcount = c->exceptcount;
            excepts = scalloc(sizeof(char *) * exceptcount, 1);

            for (i = 0; i < exceptcount; i++)
                excepts[i] = sstrdup(c->excepts[i]);

            for (i = 0; i < exceptcount; i++) {
                argv[0] = sstrdup("-e");
                argv[1] = excepts[i];
                anope_cmd_mode(s_OperServ, c->name, "-e %s", argv[1]);
                chan_set_modes(s_OperServ, c, 2, argv, 0);
                free(argv[1]);
                free(argv[0]);
            }

            if (excepts) {
                free(excepts);
            }
        }

        if (ircd->invitemode) {
            /* Clear invites */
            invitecount = c->invitecount;
            invites = scalloc(sizeof(char *) * invitecount, 1);

            for (i = 0; i < invitecount; i++)
                invites[i] = sstrdup(c->invite[i]);

            for (i = 0; i < invitecount; i++) {
                argv[0] = sstrdup("-I");
                argv[1] = invites[i];
                anope_cmd_mode(s_OperServ, c->name, "-I %s", argv[1]);
                chan_set_modes(s_OperServ, c, 2, argv, 0);
                free(argv[1]);
                free(argv[0]);
            }

            free(invites);
        }

    }

    if (all) {
        notice_lang(s_OperServ, u, OPER_CLEARMODES_ALL_DONE, chan);
    } else {
        notice_lang(s_OperServ, u, OPER_CLEARMODES_DONE, chan);
    }
    return MOD_CONT;
}

/* Viagra IRCD functions
 *
 * (C) 2003-2007 Anope Team
 * Contact us at info@anope.org
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Epona by Lara.
 * Based on the original code of Services by Andy Church. 
 * 
 * $Id: viagra.c 1291 2007-08-30 20:59:14Z drstein $
 *
 */

#include "services.h"
#include "pseudo.h"
#include "viagra.h"

IRCDVar myIrcd[] = {
    {"ViagraIRCd 1.3.*",        /* ircd name */
     "+ioS",                    /* adminserv mode */
     "+oS",                     /* nickserv mode */
     "+oS",                     /* chanserv mode */
     "+oS",                     /* memoserv mode */
     "+oS",                     /* hostserv mode */
     "+ioS",                    /* operserv mode */
     "+oS",                     /* botserv mode  */
     "+oS",                     /* helpserv mode */
     "+i",                      /* Dev/Null mode */
     "+ioS",                    /* Global mode   */
     "+ioS",                    /* adminserv alias mode */
     "+oS",                     /* nickserv alias mode */
     "+oS",                     /* chanserv alias mode */
     "+oS",                     /* memoserv alias mode */
     "+ioS",                    /* hostserv alias mode */
     "+ioS",                    /* operserv alias mode */
     "+oS",                     /* botserv alias mode  */
     "+oS",                     /* helpserv alias mode */
     "+iS",                     /* Dev/Null alias mode */
     "+ioS",                    /* Global alias mode   */
     "+qS",                     /* Used by BotServ Bots */
     5,                         /* Chan Max Symbols   */
     "-ciklmnpstOR",            /* Modes to Remove */
     "+ao",                     /* Channel Umode used by Botserv bots */
     1,                         /* SVSNICK */
     1,                         /* Vhost  */
     1,                         /* Has Owner */
     "+q",                      /* Mode to set for an owner */
     "-q",                      /* Mode to unset for an owner */
     "+a",                      /* Mode to set for channel admin */
     "-a",                      /* Mode to unset for channel admin */
     "+rd",                     /* Mode On Reg          */
     NULL,                      /* Mode on ID for Roots */
     NULL,                      /* Mode on ID for Admins */
     NULL,                      /* Mode on ID for Opers */
     "-r+d",                    /* Mode on UnReg        */
     "-r+d",                    /* Mode on Nick Change  */
     1,                         /* Supports SGlines     */
     1,                         /* Supports SQlines     */
     1,                         /* Supports SZlines     */
     1,                         /* Supports Halfop +h   */
     3,                         /* Number of server args */
     0,                         /* Join 2 Set           */
     0,                         /* Join 2 Message       */
     1,                         /* Has exceptions +e    */
     0,                         /* TS Topic Forward     */
     0,                         /* TS Topci Backward    */
     0,                         /* Protected Umode      */
     1,                         /* Has Admin            */
     0,                         /* Chan SQlines         */
     1,                         /* Quit on Kill         */
     1,                         /* SVSMODE unban        */
     1,                         /* Has Protect          */
     0,                         /* Reverse              */
     1,                         /* Chan Reg             */
     CMODE_r,                   /* Channel Mode         */
     1,                         /* vidents              */
     0,                         /* svshold              */
     1,                         /* time stamp on mode   */
     1,                         /* NICKIP               */
     0,                         /* O:LINE               */
     1,                         /* UMODE               */
     1,                         /* VHOST ON NICK        */
     1,                         /* Change RealName      */
     0,                         /* No Knock             */
     0,                         /* Admin Only           */
     DEFAULT_MLOCK,             /* Default MLOCK       */
     UMODE_x,                   /* Vhost Mode           */
     0,                         /* +f                   */
     0,                         /* +L                   */
     0,
     0,
     1,
     1,                         /* No Knock requires +i */
     NULL,                      /* CAPAB Chan Modes             */
     0,                         /* We support TOKENS */
     1,                         /* TOKENS are CASE inSensitive */
     0,                         /* TIME STAMPS are BASE64 */
     0,                         /* +I support */
     0,                         /* SJOIN ban char */
     0,                         /* SJOIN except char */
     0,                         /* SJOIN invite char */
     0,                         /* Can remove User Channel Modes with SVSMODE */
     0,                         /* Sglines are not enforced until user reconnects */
     "x",                       /* vhost char */
     0,                         /* ts6 */
     1,                         /* support helper umode */
     0,                         /* p10 */
     NULL,                      /* character set             */
     1,                         /* reports sync state */
     }
    ,
    {NULL}
};


IRCDCAPAB myIrcdcap[] = {
    {
     CAPAB_NOQUIT,              /* NOQUIT       */
     CAPAB_TSMODE,              /* TSMODE       */
     CAPAB_UNCONNECT,           /* UNCONNECT    */
     CAPAB_NICKIP,              /* NICKIP       */
     CAPAB_NSJOIN,              /* SJOIN        */
     CAPAB_ZIP,                 /* ZIP          */
     CAPAB_BURST,               /* BURST        */
     CAPAB_TS5,                 /* TS5          */
     CAPAB_TS3,                 /* TS3          */
     CAPAB_DKEY,                /* DKEY         */
     0,                         /* PT4          */
     0,                         /* SCS          */
     0,                         /* QS           */
     0,                         /* UID          */
     0,                         /* KNOCK        */
     0,                         /* CLIENT       */
     0,                         /* IPV6         */
     0,                         /* SSJ5         */
     0,                         /* SN2          */
     0,                         /* TOKEN        */
     0,                         /* VHOST        */
     0,                         /* SSJ3         */
     0,                         /* NICK2        */
     0,                         /* UMODE2       */
     0,                         /* VL           */
     0,                         /* TLKEXT       */
     CAPAB_DODKEY,              /* DODKEY       */
     CAPAB_DOZIP,               /* DOZIP        */
     0, 0, 0}
};




void viagra_set_umode(User * user, int ac, char **av)
{
    int add = 1;                /* 1 if adding modes, 0 if deleting */
    char *modes = av[0];

    ac--;

    if (debug)
        alog("debug: Changing mode for %s to %s", user->nick, modes);

    while (*modes) {

        /* This looks better, much better than "add ? (do_add) : (do_remove)".
         * At least this is readable without paying much attention :) -GD
         */
        if (add)
            user->mode |= umodes[(int) *modes];
        else
            user->mode &= ~umodes[(int) *modes];

        switch (*modes++) {
        case '+':
            add = 1;
            break;
        case '-':
            add = 0;
            break;
        case 'd':
            if (ac == 0) {
                alog("user: umode +d with no parameter (?) for user %s",
                     user->nick);
                break;
            }

            ac--;
            av++;
            user->svid = strtoul(*av, NULL, 0);
            break;
        case 'o':
            if (add) {
                opcnt++;
                if (WallOper) {
                    xanadu_cmd_global(s_OperServ,
                                     "\2%s\2 is now an IRC operator.",
                                     user->nick);
                }
                display_news(user, NEWS_OPER);
            } else {
                opcnt--;
            }
            break;
        case 'r':
            if (add && !nick_identified(user)) {
                common_svsmode(user, "-r", NULL);
                user->mode &= ~UMODE_r;
            }
            break;
        case 'x':
            update_host(user);
            break;
        }
    }
}

unsigned long umodes[128] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, UMODE_A, 0, UMODE_C, 0, 0, 0, 0, 0, UMODE_I, 0, 0, 0, 0, UMODE_N,
    UMODE_O,
    0,
    UMODE_Q,
    UMODE_R,
    UMODE_S, UMODE_T, 0, 0, 0, 0, 0,
    0,
    0, 0, 0, 0, 0,
    0, UMODE_a, UMODE_b, UMODE_c, UMODE_d, UMODE_e, UMODE_f,
    UMODE_g,
    UMODE_h, UMODE_i, 0, 0, 0, 0, UMODE_n, UMODE_o,
    0,
    0, UMODE_r, UMODE_s, 0, 0, 0, UMODE_w,
    UMODE_x,
    0,
    0,
    0, 0, 0, 0, 0
};


char myCsmodes[128] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

    0,
    0,
    0, 0, 0,
    'h',                        /* (37) % Channel halfops */
    0,                          /* (38) & bans */
    0, 0, 0,
    'q',

    'v', 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

    'o', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'a', 0
};

CMMode myCmmodes[128] = {
    {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL},
    {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL},
    {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL},
    {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL},
    {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL},
    {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL},
    {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL},
    {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL},
    {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL},
    {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL},
    {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL},
    {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL},
    {NULL},
    {NULL},
    {add_ban, del_ban},
    {NULL},
    {NULL},
    {add_exception, del_exception},
    {NULL},
    {NULL},
    {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL},
    {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL},
    {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL}
};



CBMode myCbmodes[128] = {
    {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0},
    {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0},
    {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0},
    {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0},
    {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0},
    {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0},
    {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0},
    {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0},
    {0},
    {0},                        /* A */
    {0},                        /* B */
    {0},                        /* C */
    {0},                        /* D */
    {0},                        /* E */
    {0},                        /* F */
    {0},                        /* G */
    {CMODE_H, 0, NULL, NULL},   /* H */
    {0},                        /* I */
    {0},                        /* J */
    {0},                        /* K */
    {0},                        /* L */
    {CMODE_M, 0, NULL, NULL},   /* M */
    {CMODE_N, 0, NULL, NULL},   /* N */
    {CMODE_O, CBM_NO_USER_MLOCK, NULL, NULL},
    {CMODE_P, 0, NULL, NULL},   /* P */
    {0},                        /* Q */
    {CMODE_R, 0, NULL, NULL},   /* R */
    {CMODE_S, 0, NULL, NULL},   /* S */
    {0},                        /* T */
    {0},                        /* U */
    {0},                        /* V */
    {0},                        /* W */
    {0},                        /* X */
    {0},                        /* Y */
    {0},                        /* Z */
    {0}, {0}, {0}, {0}, {0}, {0},
    {0},                        /* a */
    {0},                        /* b */
    {CMODE_c, 0, NULL, NULL},
    {0},                        /* d */
    {0},                        /* e */
    {0},                        /* f */
    {0},                        /* g */
    {0},                        /* h */
    {CMODE_i, 0, NULL, NULL},
    {0},                        /* j */
    {CMODE_k, 0, chan_set_key, cs_set_key},
    {CMODE_l, CBM_MINUS_NO_ARG, set_limit, cs_set_limit},
    {CMODE_m, 0, NULL, NULL},
    {CMODE_n, 0, NULL, NULL},
    {0},                        /* o */
    {CMODE_p, 0, NULL, NULL},
    {0},                        /* q */
    {CMODE_r, CBM_NO_MLOCK, NULL, NULL},
    {CMODE_s, 0, NULL, NULL},
    {CMODE_t, 0, NULL, NULL},
    {0},
    {0},                        /* v */
    {0},                        /* w */
    {CMODE_x, 0, NULL, NULL},   /* x */
    {0},                        /* y */
    {0},                        /* z */
    {0}, {0}, {0}, {0}
};

CBModeInfo myCbmodeinfos[] = {
    {'c', CMODE_c, 0, NULL, NULL},
    {'i', CMODE_i, 0, NULL, NULL},
    {'k', CMODE_k, 0, get_key, cs_get_key},
    {'l', CMODE_l, CBM_MINUS_NO_ARG, get_limit, cs_get_limit},
    {'m', CMODE_m, 0, NULL, NULL},
    {'n', CMODE_n, 0, NULL, NULL},
    {'p', CMODE_p, 0, NULL, NULL},
    {'r', CMODE_r, 0, NULL, NULL},
    {'s', CMODE_s, 0, NULL, NULL},
    {'t', CMODE_t, 0, NULL, NULL},
    {'x', CMODE_x, 0, NULL, NULL},
    {'H', CMODE_H, 0, NULL, NULL},
    {'M', CMODE_M, 0, NULL, NULL},
    {'N', CMODE_N, 0, NULL, NULL},
    {'O', CMODE_O, 0, NULL, NULL},
    {'P', CMODE_P, 0, NULL, NULL},
    {'R', CMODE_R, 0, NULL, NULL},
    {'S', CMODE_S, 0, NULL, NULL},
    {0}
};

CUMode myCumodes[128] = {
    {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0},
    {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0},
    {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0},
    {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0},

    {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0},
    {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0},
    {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0},
    {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0},

    {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0},
    {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0},
    {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0},
    {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0},

    {0},

    {CUS_PROTECT, CUF_PROTECT_BOTSERV, check_valid_op},
    {0},                        /* b */
    {0},                        /* c */
    {0},                        /* d */
    {0},                        /* e */
    {0},                        /* f */
    {0},                        /* g */
    {CUS_HALFOP, 0, check_valid_op},
    {0},                        /* i */
    {0},                        /* j */
    {0},                        /* k */
    {0},                        /* l */
    {0},                        /* m */
    {0},                        /* n */
    {CUS_OP, CUF_PROTECT_BOTSERV, check_valid_op},
    {0},                        /* p */
    {CUS_OWNER, 0, check_valid_op},
    {0},                        /* r */
    {0},                        /* s */
    {0},                        /* t */
    {0},                        /* u */
    {CUS_VOICE, 0, NULL},
    {0},                        /* w */
    {0},                        /* x */
    {0},                        /* y */
    {0},                        /* z */
    {0}, {0}, {0}, {0}, {0}
};


void viagra_cmd_bot_unban(ChannelInfo * ci, char *nick)
{
    send_cmd(ServerName, "SVSMODE %s -b %s", ci->name, nick);
}

int xanadu_event_setname(char *source, int ac, char **av)
{
    User *u;

    if (ac != 1)
        return MOD_CONT;

    u = finduser(source);
    if (!u) {
        if (debug) {
            alog("debug: SETNAME for nonexistent user %s", source);
        }
        return MOD_CONT;
    }

    change_user_realname(u, av[0]);
    return MOD_CONT;
}

int xanadu_event_sjoin(char *source, int ac, char **av)
{
    do_sjoin(source, ac, av);
    return MOD_CONT;
}

int xanadu_event_chgname(char *source, int ac, char **av)
{
    User *u;

    if (ac != 2)
        return MOD_CONT;

    u = finduser(av[0]);
    if (!u) {
        if (debug) {
            alog("debug: CHGNAME for nonexistent user %s", av[0]);
        }
        return MOD_CONT;
    }

    change_user_realname(u, av[1]);
    return MOD_CONT;
}

int xanadu_event_setident(char *source, int ac, char **av)
{
    User *u;

    if (ac != 1)
        return MOD_CONT;

    u = finduser(source);
    if (!u) {
        if (debug) {
            alog("debug: SETIDENT for nonexistent user %s", source);
        }
        return MOD_CONT;
    }

    change_user_username(u, av[0]);
    return MOD_CONT;
}

int xanadu_event_chgident(char *source, int ac, char **av)
{
    User *u;

    if (ac != 2)
        return MOD_CONT;

    u = finduser(av[0]);
    if (!u) {
        if (debug) {
            alog("debug: CHGIDENT for nonexistent user %s", av[0]);
        }
        return MOD_CONT;
    }

    change_user_username(u, av[1]);
    return MOD_CONT;
}

/*
 * sethost
 *  parv[0] = sender
 *  parv[1] = newhost
 */
int xanadu_event_sethost(char *source, int ac, char **av)
{
    User *u;

    if (ac != 1)
        return MOD_CONT;

    u = finduser(source);
    if (!u) {
        if (debug) {
            alog("debug: SETHOST for nonexistent user %s", source);
        }
        return MOD_CONT;
    }

    change_user_host(u, av[0]);
    return MOD_CONT;
}

int xanadu_event_nick(char *source, int ac, char **av)
{
    User *user;

    if (ac != 2) {
        user = do_nick(source, av[0], av[4], av[5], av[6], av[9],
                       strtoul(av[2], NULL, 10), strtoul(av[7], NULL, 0),
                       strtoul(av[8], NULL, 0), "*", NULL);
        if (user) {
            xanadu_set_umode(user, 1, &av[3]);
        }
    } else {
        do_nick(source, av[0], NULL, NULL, NULL, NULL,
                strtoul(av[1], NULL, 10), 0, 0, NULL, NULL);
    }
    return MOD_CONT;
}

int xanadu_event_vs(char *source, int ac, char **av)
{
    User *u;

    if (ac != 2)
        return MOD_CONT;

    u = finduser(av[0]);
    if (!u) {
        if (debug) {
            alog("debug: VS for nonexistent user %s", av[0]);
        }
        return MOD_CONT;
    }

    change_user_host(u, av[1]);
    return MOD_CONT;

}

int xanadu_event_chghost(char *source, int ac, char **av)
{
    User *u;

    if (ac != 2)
        return MOD_CONT;

    u = finduser(av[0]);
    if (!u) {
        if (debug) {
            alog("debug: CHGHOST for nonexistent user %s", av[0]);
        }
        return MOD_CONT;
    }

    change_user_host(u, av[1]);
    return MOD_CONT;
}

int xanadu_event_436(char *source, int ac, char **av)
{
    if (ac < 1)
        return MOD_CONT;

    m_nickcoll(av[0]);
    return MOD_CONT;
}

int xanadu_event_notice(char *source, int ac, char **av)
{
    return MOD_CONT;
}

int xanadu_event_pass(char *source, int ac, char **av)
{
    return MOD_CONT;
}

int xanadu_event_svinfo(char *source, int ac, char **av)
{
    return MOD_CONT;
}

/*
 * gnotice
 *  parv[0] = sender prefix
 *  parv[1] = message text
 */
int xanadu_event_gnotice(char *source, int ac, char **av)
{
    return MOD_CONT;
}

int xanadu_event_sqline(char *source, int ac, char **av)
{
    return MOD_CONT;
}

int xanadu_event_burst(char *source, int ac, char **av)
{
    Server *s;
    s = findserver(servlist, source);
    if (!ac) {
        /* for future use  - start burst */
    } else {
        /* If we found a server with the given source, that one just
         * finished bursting. If there was no source, then our uplink
         * server finished bursting. -GD
         */
        if (!s && serv_uplink)
            s = serv_uplink;
        finish_sync(s, 1);
    }
    return MOD_CONT;
}

int xanadu_event_tctrl(char *source, int ac, char **av)
{
    return MOD_CONT;
}

/*
 * error
 *  parv[0] = sender prefix
 *  parv[*] = parameters
 */
int xanadu_event_error(char *source, int ac, char **av)
{
    if (ac >= 1) {
        if (debug) {
            alog("debug: %s", av[0]);
        }
    }
    return MOD_CONT;
}

/* *INDENT-OFF* */
void moduleAddIRCDMsgs(void) {
    Message *m;

    updateProtectDetails("PROTECT","PROTECTME","protect","deprotect","AUTOPROTECT","+a","-a");
 
    m = createMessage("401",       xanadu_event_null); addCoreMessage(IRCD,m);
    m = createMessage("402",       xanadu_event_null); addCoreMessage(IRCD,m);
    m = createMessage("436",       xanadu_event_436); addCoreMessage(IRCD,m);
    m = createMessage("AWAY",      xanadu_event_away); addCoreMessage(IRCD,m);
    m = createMessage("INVITE",    xanadu_event_invite); addCoreMessage(IRCD,m);
    m = createMessage("JOIN",      xanadu_event_join); addCoreMessage(IRCD,m);
    m = createMessage("KICK",      xanadu_event_kick); addCoreMessage(IRCD,m);
    m = createMessage("KILL",      xanadu_event_kill); addCoreMessage(IRCD,m);
    m = createMessage("MODE",      xanadu_event_mode); addCoreMessage(IRCD,m);
    m = createMessage("MOTD",      xanadu_event_motd); addCoreMessage(IRCD,m);
    m = createMessage("NICK",      xanadu_event_nick); addCoreMessage(IRCD,m);
    m = createMessage("NOTICE",    xanadu_event_notice); addCoreMessage(IRCD,m);
    m = createMessage("PART",      xanadu_event_part); addCoreMessage(IRCD,m);
    m = createMessage("PASS",      xanadu_event_pass); addCoreMessage(IRCD,m);
    m = createMessage("PING",      xanadu_event_ping); addCoreMessage(IRCD,m);
    m = createMessage("PRIVMSG",   xanadu_event_privmsg); addCoreMessage(IRCD,m);
    m = createMessage("QUIT",      xanadu_event_quit); addCoreMessage(IRCD,m);
    m = createMessage("SERVER",    xanadu_event_server); addCoreMessage(IRCD,m);
    m = createMessage("SQUIT",     xanadu_event_squit); addCoreMessage(IRCD,m);
    m = createMessage("TOPIC",     xanadu_event_topic); addCoreMessage(IRCD,m);
    m = createMessage("WALLOPS",   xanadu_event_null); addCoreMessage(IRCD,m);
    m = createMessage("WHOIS",     xanadu_event_whois); addCoreMessage(IRCD,m);
    m = createMessage("AKILL",     xanadu_event_null); addCoreMessage(IRCD,m);
    m = createMessage("GLOBOPS",   xanadu_event_null); addCoreMessage(IRCD,m);
    m = createMessage("GNOTICE",   xanadu_event_gnotice); addCoreMessage(IRCD,m);
    m = createMessage("GOPER",     xanadu_event_null); addCoreMessage(IRCD,m);
    m = createMessage("RAKILL",    xanadu_event_null); addCoreMessage(IRCD,m);
    m = createMessage("SILENCE",   xanadu_event_null); addCoreMessage(IRCD,m);
    m = createMessage("SVSKILL",   xanadu_event_null); addCoreMessage(IRCD,m);
    m = createMessage("SVSMODE",   xanadu_event_mode); addCoreMessage(IRCD,m);
    m = createMessage("SVSNICK",   xanadu_event_null); addCoreMessage(IRCD,m);
    m = createMessage("SVSNOOP",   xanadu_event_null); addCoreMessage(IRCD,m);
    m = createMessage("SQLINE",    xanadu_event_sqline); addCoreMessage(IRCD,m);
    m = createMessage("UNSQLINE",  xanadu_event_null); addCoreMessage(IRCD,m);
    m = createMessage("CAPAB", 	   xanadu_event_capab); addCoreMessage(IRCD,m);
    m = createMessage("CS",        xanadu_event_cs); addCoreMessage(IRCD,m);
    m = createMessage("HS",        xanadu_event_hs); addCoreMessage(IRCD,m);
    m = createMessage("MS",        xanadu_event_ms); addCoreMessage(IRCD,m);
    m = createMessage("NS",        xanadu_event_ns); addCoreMessage(IRCD,m);
    m = createMessage("OS",        xanadu_event_os); addCoreMessage(IRCD,m);
    m = createMessage("RS",        xanadu_event_null); addCoreMessage(IRCD,m);
    m = createMessage("SGLINE",    xanadu_event_null); addCoreMessage(IRCD,m);
    m = createMessage("SJOIN",     xanadu_event_sjoin); addCoreMessage(IRCD,m);
    m = createMessage("SS",        xanadu_event_null); addCoreMessage(IRCD,m);
    m = createMessage("SVINFO",    xanadu_event_svinfo); addCoreMessage(IRCD,m);
    m = createMessage("SZLINE",    xanadu_event_null); addCoreMessage(IRCD,m);
    m = createMessage("UNSGLINE",  xanadu_event_null); addCoreMessage(IRCD,m);
    m = createMessage("UNSZLINE",  xanadu_event_null); addCoreMessage(IRCD,m);
    m = createMessage("CHGHOST",   xanadu_event_chghost); addCoreMessage(IRCD,m);
    m = createMessage("CHGIDENT",  xanadu_event_chgident); addCoreMessage(IRCD,m);
    m = createMessage("CHGNAME",   xanadu_event_chgname); addCoreMessage(IRCD,m);
    m = createMessage("SETHOST",   xanadu_event_sethost); addCoreMessage(IRCD,m);
    m = createMessage("SETIDENT",  xanadu_event_setident); addCoreMessage(IRCD,m);
    m = createMessage("SETNAME",   xanadu_event_setname); addCoreMessage(IRCD,m);
    m = createMessage("VS",        xanadu_event_vs); addCoreMessage(IRCD,m);
    m = createMessage("BURST",     xanadu_event_burst); addCoreMessage(IRCD,m);
    m = createMessage("TCTRL",     xanadu_event_tctrl); addCoreMessage(IRCD,m);
    m = createMessage("ERROR",     xanadu_event_error); addCoreMessage(IRCD,m);
    m = createMessage("REHASH",    xanadu_event_rehash); addCoreMessage(IRCD,m);
    m = createMessage("ADMIN",     xanadu_event_admin); addCoreMessage(IRCD,m);
    m = createMessage("SNOTICE",   xanadu_event_snotice); addCoreMessage(IRCD,m);
}

/* *INDENT-ON* */


/* SQLINE */
void viagra_cmd_sqline(char *mask, char *reason)
{
    if (!mask || !reason) {
        return;
    }

    send_cmd(NULL, "SQLINE %s :%s", mask, reason);
}

void viagra_cmd_unsgline(char *mask)
{
    send_cmd(NULL, "UNSGLINE 0 :%s", mask);
}

void viagra_cmd_unszline(char *mask)
{
    send_cmd(NULL, "UNSZLINE 0 %s", mask);
}

void viagra_cmd_szline(char *mask, char *reason, char *whom)
{
    send_cmd(NULL, "SZLINE %s :%s", mask, reason);
}

void viagra_cmd_svsnoop(char *server, int set)
{
    send_cmd(NULL, "SVSNOOP %s %s", server, (set ? "+" : "-"));
}

void viagra_cmd_svsadmin(char *server, int set)
{
    viagra_cmd_svsnoop(server, set);
}

void viagra_cmd_sgline(char *mask, char *reason)
{
    send_cmd(NULL, "SGLINE %d :%s:%s", (int)strlen(mask), mask, reason);
}

void viagra_cmd_remove_akill(char *user, char *host)
{
    send_cmd(NULL, "RAKILL %s %s", host, user);
}

/* PART */
void viagra_cmd_part(char *nick, char *chan, char *buf)
{
    if (!nick || !chan) {
        return;
    }

    if (buf) {
        send_cmd(nick, "PART %s :%s", chan, buf);
    } else {
        send_cmd(nick, "PART %s", chan);
    }
}

void viagra_cmd_topic(char *whosets, char *chan, char *whosetit,
                      char *topic, time_t when)
{
    send_cmd(whosets, "TOPIC %s %s %lu :%s", chan, whosetit,
             (unsigned long int) when, topic);
}

void viagra_cmd_vhost_off(User * u)
{
    send_cmd(NULL, "SVSMODE %s -x", u->nick);
    notice_lang(s_HostServ, u, HOST_OFF_UNREAL, u->nick, ircd->vhostchar);
}

void viagra_cmd_vhost_on(char *nick, char *vIdent, char *vhost)
{
    if (vIdent) {
        send_cmd(NULL, "CHGIDENT %s %s", nick, vIdent);
    }
    send_cmd(NULL, "SVSMODE %s +x", nick);
    send_cmd(NULL, "SVSCHGHOST %s %s", nick, vhost);
}

void viagra_cmd_unsqline(char *user)
{
    send_cmd(NULL, "UNSQLINE %s", user);
}

void viagra_cmd_join(char *user, char *channel, time_t chantime)
{
    send_cmd(user, "SJOIN %ld %s", (long int) chantime, channel);
}


/*
 * m_akill()
 *  parv[1]=host
 *  parv[2]=user
 *  parv[3]=length
 *  parv[4]=akiller
 *  parv[5]=time set
 *  parv[6]=reason
 */
void viagra_cmd_akill(char *user, char *host, char *who, time_t when,
                      time_t expires, char *reason)
{
    send_cmd(NULL, "AKILL %s %s %d %s %ld :%s", host, user, 86400 * 2, who,
             (long int) time(NULL), reason);
}


/*
 * svskill
 *  parv[0] = servername
 *  parv[1] = client
 *  parv[2] = nick stamp
 *  parv[3] = kill message
 */
void viagra_cmd_svskill(char *source, char *user, char *buf)
{
    if (buf) {
        send_cmd(source, "SVSKILL %s :%s", user, buf);
    }
    return;
}

void viagra_cmd_mode(char *source, char *dest, char *buf)
{
    if (!buf) {
        return;
    }

    if (ircdcap->tsmode) {
        if (uplink_capab & ircdcap->tsmode) {
            send_cmd(source, "MODE %s 0 %s", dest, buf);
        } else {
            send_cmd(source, "MODE %s %s", dest, buf);
        }
    } else {
        send_cmd(source, "MODE %s %s", dest, buf);
    }
}

/* QUIT */
void viagra_cmd_quit(char *source, char *buf)
{
    if (buf) {
        send_cmd(source, "QUIT :%s", buf);
    } else {
        send_cmd(source, "QUIT");
    }
}

int xanadu_event_away(char *source, int ac, char **av)
{
    if (!source) {
        return MOD_CONT;
    }
    m_away(source, (ac ? av[0] : NULL));
    return MOD_CONT;
}

int xanadu_event_ping(char *source, int ac, char **av)
{
    if (ac < 1)
        return MOD_CONT;
    viagra_cmd_pong(ac > 1 ? av[1] : ServerName, av[0]);
    return MOD_CONT;
}

void viagra_cmd_svsmode(User * u, int ac, char **av)
{
    send_cmd(ServerName, "SVSMODE %s %ld %s%s%s", u->nick,
             (long int) u->timestamp, av[0], (ac == 2 ? " " : ""),
             (ac == 2 ? av[1] : ""));
}

void viagra_cmd_squit(char *servname, char *message)
{
    send_cmd(NULL, "SQUIT %s :%s", servname, message);
}

/* PONG */
void viagra_cmd_pong(char *servname, char *who)
{
    send_cmd(servname, "PONG %s", who);
}

/*
 * svinfo
 *  parv[0] = sender prefix
 *  parv[1] = TS_CURRENT for the server
 *  parv[2] = TS_MIN for the server
 *  parv[3] = server is standalone or connected to non-TS only
 *  parv[4] = server's idea of UTC time
 */
void viagra_cmd_svinfo()
{
    send_cmd(NULL, "SVINFO 5 3 0 :%ld", (long int) time(NULL));
}

/* CAPAB */
void viagra_cmd_capab()
{
    send_cmd(NULL, "CAPAB TS5 NOQUIT SSJOIN BURST UNCONNECT NICKIP");
}

/* PASS */
void viagra_cmd_pass(char *pass)
{
    send_cmd(NULL, "PASS %s :TS", pass);
}

/* SERVER */
void viagra_cmd_server(char *servname, int hop, char *descript)
{
    send_cmd(NULL, "SERVER %s %d :%s", servname, hop, descript);
}

void viagra_cmd_burst()
{
    send_cmd(NULL, "BURST");
}

void viagra_cmd_connect(int servernum)
{
    me_server =
        new_server(NULL, ServerName, ServerDesc, SERVER_ISME, NULL);

    if (servernum == 1) {
        viagra_cmd_pass(RemotePassword);
    } else if (servernum == 2) {
        viagra_cmd_pass(RemotePassword2);
    } else if (servernum == 3) {
        viagra_cmd_pass(RemotePassword3);
    }
    viagra_cmd_capab();
    viagra_cmd_server(ServerName, 1, ServerDesc);
    viagra_cmd_svinfo();
    viagra_cmd_burst();
}

/* EVENT : OS */
int xanadu_event_os(char *source, int ac, char **av)
{
    if (ac < 1)
        return MOD_CONT;
    m_privmsg(source, s_OperServ, av[0]);
    return MOD_CONT;
}

/* EVENT : NS */
int xanadu_event_ns(char *source, int ac, char **av)
{
    if (ac < 1)
        return MOD_CONT;
    m_privmsg(source, s_NickServ, av[0]);
    return MOD_CONT;
}

/* EVENT : MS */
int xanadu_event_ms(char *source, int ac, char **av)
{
    if (ac < 1)
        return MOD_CONT;
    m_privmsg(source, s_MemoServ, av[0]);
    return MOD_CONT;
}

/* EVENT : HS */
int xanadu_event_hs(char *source, int ac, char **av)
{
    if (ac < 1)
        return MOD_CONT;
    m_privmsg(source, s_HostServ, av[0]);
    return MOD_CONT;
}

/* EVENT : CS */
int xanadu_event_cs(char *source, int ac, char **av)
{
    if (ac < 1)
        return MOD_CONT;
    m_privmsg(source, s_ChanServ, av[0]);
    return MOD_CONT;
}

int xanadu_event_server(char *source, int ac, char **av)
{
    if (!stricmp(av[1], "1")) {
        uplink = sstrdup(av[0]);
    }
    do_server(source, av[0], av[1], av[2], NULL);
    return MOD_CONT;
}


int xanadu_event_privmsg(char *source, int ac, char **av)
{
    if (ac != 2)
        return MOD_CONT;
    m_privmsg(source, av[0], av[1]);
    return MOD_CONT;
}

int xanadu_event_part(char *source, int ac, char **av)
{
    if (ac < 1 || ac > 2)
        return MOD_CONT;
    do_part(source, ac, av);
    return MOD_CONT;
}

int xanadu_event_whois(char *source, int ac, char **av)
{
    if (source && ac >= 1) {
        m_whois(source, av[0]);
    }
    return MOD_CONT;
}

int xanadu_event_topic(char *source, int ac, char **av)
{
    if (ac != 4)
        return MOD_CONT;
    do_topic(source, ac, av);
    return MOD_CONT;
}

int xanadu_event_squit(char *source, int ac, char **av)
{
    if (ac != 2)
        return MOD_CONT;
    do_squit(source, ac, av);
    return MOD_CONT;
}

int xanadu_event_quit(char *source, int ac, char **av)
{
    if (ac != 1)
        return MOD_CONT;
    do_quit(source, ac, av);
    return MOD_CONT;
}


int xanadu_event_mode(char *source, int ac, char **av)
{
    if (ac < 2)
        return MOD_CONT;

    if (*av[0] == '#' || *av[0] == '&') {
        do_cmode(source, ac, av);
    } else {
        do_umode(source, ac, av);
    }
    return MOD_CONT;
}

int xanadu_event_kill(char *source, int ac, char **av)
{
    if (ac != 2)
        return MOD_CONT;

    m_kill(av[0], av[1]);
    return MOD_CONT;
}

int xanadu_event_kick(char *source, int ac, char **av)
{
    if (ac != 3)
        return MOD_CONT;
    do_kick(source, ac, av);
    return MOD_CONT;
}

int xanadu_event_join(char *source, int ac, char **av)
{
    if (ac != 1)
        return MOD_CONT;
    do_join(source, ac, av);
    return MOD_CONT;
}

int xanadu_event_motd(char *source, int ac, char **av)
{
    if (!source) {
        return MOD_CONT;
    }

    m_motd(source);
    return MOD_CONT;
}

void viagra_cmd_notice_ops(char *source, char *dest, char *buf)
{
    if (buf) {
        send_cmd(NULL, "NOTICE @%s :%s", dest, buf);
    }
    return;
}

void viagra_cmd_notice(char *source, char *dest, char *buf)
{
    if (!buf) {
        return;
    }

    if (NSDefFlags & NI_MSG) {
        viagra_cmd_privmsg2(source, dest, buf);
    } else {
        send_cmd(source, "NOTICE %s :%s", dest, buf);
    }
}

void viagra_cmd_notice2(char *source, char *dest, char *msg)
{
    send_cmd(source, "NOTICE %s :%s", dest, msg);
}

void viagra_cmd_privmsg(char *source, char *dest, char *buf)
{
    if (!buf) {
        return;
    }

    send_cmd(source, "PRIVMSG %s :%s", dest, buf);
}

void viagra_cmd_privmsg2(char *source, char *dest, char *msg)
{
    send_cmd(source, "PRIVMSG %s :%s", dest, msg);
}

void viagra_cmd_serv_notice(char *source, char *dest, char *msg)
{
    send_cmd(source, "NOTICE $%s :%s", dest, msg);
}

void viagra_cmd_serv_privmsg(char *source, char *dest, char *msg)
{
    send_cmd(source, "PRIVMSG $%s :%s", dest, msg);
}

/* GLOBOPS */
void viagra_cmd_global(char *source, char *buf)
{
    if (!buf) {
        return;
    }

    send_cmd(source ? source : ServerName, "GLOBOPS :%s", buf);
}

/* 391 */
void viagra_cmd_391(char *source, char *timestr)
{
    if (!timestr) {
        return;
    }
    send_cmd(NULL, "391 :%s %s :%s", source, ServerName, timestr);
}

/* 250 */
void viagra_cmd_250(char *buf)
{
    if (!buf) {
        return;
    }

    send_cmd(NULL, "250 %s", buf);
}

/* 307 */
void viagra_cmd_307(char *buf)
{
    if (!buf) {
        return;
    }

    send_cmd(ServerName, "307 %s", buf);
}

/* 311 */
void viagra_cmd_311(char *buf)
{
    if (!buf) {
        return;
    }

    send_cmd(ServerName, "311 %s", buf);
}

/* 312 */
void viagra_cmd_312(char *buf)
{
    if (!buf) {
        return;
    }

    send_cmd(ServerName, "312 %s", buf);
}

/* 317 */
void viagra_cmd_317(char *buf)
{
    if (!buf) {
        return;
    }

    send_cmd(ServerName, "317 %s", buf);
}

/* 219 */
void viagra_cmd_219(char *source, char *letter)
{
    if (!source) {
        return;
    }

    if (letter) {
        send_cmd(NULL, "219 %s %c :End of /STATS report.", source,
                 *letter);
    } else {
        send_cmd(NULL, "219 %s l :End of /STATS report.", source);
    }
}

/* 401 */
void viagra_cmd_401(char *source, char *who)
{
    if (!source || !who) {
        return;
    }
    send_cmd(ServerName, "401 %s %s :No such service.", source, who);
}

/* 318 */
void viagra_cmd_318(char *source, char *who)
{
    if (!source || !who) {
        return;
    }

    send_cmd(ServerName, "318 %s %s :End of /WHOIS list.", source, who);
}

/* 242 */
void viagra_cmd_242(char *buf)
{
    if (!buf) {
        return;
    }

    send_cmd(NULL, "242 %s", buf);
}

/* 243 */
void viagra_cmd_243(char *buf)
{
    if (!buf) {
        return;
    }

    send_cmd(NULL, "243 %s", buf);
}

/* 211 */
void viagra_cmd_211(char *buf)
{
    if (!buf) {
        return;
    }

    send_cmd(NULL, "211 %s", buf);
}

void viagra_cmd_351(char *source)
{
    send_cmd(ServerName, "351 %s Xanadu-%s %s :%s - %s (%s) -- %s",
             source, version_number, ServerName, ircd->name, version_flags,
             EncModule, version_build);
}

void viagra_cmd_bot_nick(char *nick, char *user, char *host, char *real,
                         char *modes)
{
    EnforceQlinedNick(nick, s_BotServ);
    send_cmd(NULL, "NICK %s 1 %ld %s %s %s %s 0 0 :%s", nick,
             (long int) time(NULL), modes, user, host, ServerName, real);
    viagra_cmd_sqline(nick, "Reserved for services");
}

void viagra_cmd_kick(char *source, char *chan, char *user, char *buf)
{
    if (buf) {
        send_cmd(source, "KICK %s %s :%s", chan, user, buf);
    } else {
        send_cmd(source, "KICK %s %s", chan, user);
    }
}

void viagra_cmd_nick(char *nick, char *name, char *modes)
{
    EnforceQlinedNick(nick, NULL);
    send_cmd(NULL, "NICK %s 1 %ld %s %s %s %s 0 0 :%s", nick,
             (long int) time(NULL), modes, ServiceUser, ServiceHost,
             ServerName, name);
    viagra_cmd_sqline(nick, "Reserved for services");
}

void viagra_cmd_372(char *source, char *msg)
{
    send_cmd(ServerName, "372 %s :- %s", source, msg);
}

void viagra_cmd_372_error(char *source)
{
    send_cmd(ServerName, "422 %s :- MOTD file not found!  Please "
             "contact your IRC administrator.", source);
}

void viagra_cmd_375(char *source)
{
    send_cmd(ServerName, "375 %s :- %s Message of the Day",
             source, ServerName);
}

void viagra_cmd_376(char *source)
{
    send_cmd(ServerName, "376 %s :End of /MOTD command.", source);
}

/* INVITE */
void viagra_cmd_invite(char *source, char *chan, char *nick)
{
    if (!source || !chan || !nick) {
        return;
    }

    send_cmd(source, "INVITE %s %s", nick, chan);
}

void viagra_cmd_bot_chan_mode(char *nick, char *chan)
{
    xanadu_cmd_mode(nick, chan, "%s %s", ircd->botchanumode, nick);
}

int xanadu_event_capab(char *source, int ac, char **av)
{
    capab_parse(ac, av);
    return MOD_CONT;
}

/* SVSHOLD - set */
void viagra_cmd_svshold(char *nick)
{
    /* Not supported by this IRCD */
}

/* SVSHOLD - release */
void viagra_cmd_release_svshold(char *nick)
{
    /* Not Supported by this IRCD */
}

/* SVSNICK */
void viagra_cmd_svsnick(char *source, char *guest, time_t when)
{
    if (!source || !guest) {
        return;
    }
    send_cmd(NULL, "SVSNICK %s %s :%ld", source, guest, (long int) when);
}

void viagra_cmd_guest_nick(char *nick, char *user, char *host, char *real,
                           char *modes)
{
    send_cmd(NULL, "NICK %s 1 %ld %s %s %s %s 0 0 :%s", nick,
             (long int) time(NULL), modes, user, host, ServerName, real);
}

void viagra_cmd_svso(char *source, char *nick, char *flag)
{
    /* Not Supported by this IRCD */
}


/* SVSMODE -b */
void viagra_cmd_unban(char *name, char *nick)
{
    viagra_cmd_svsmode_chan(name, "-b", nick);
}

/* SVSMODE channel modes */

void viagra_cmd_svsmode_chan(char *name, char *mode, char *nick)
{
    if (nick) {
        send_cmd(ServerName, "SVSMODE %s %s %s", name, mode, nick);
    } else {
        send_cmd(ServerName, "SVSMODE %s %s", name, mode);
    }
}

/* SVSMODE +d */
/* sent if svid is something weird */
void viagra_cmd_svid_umode(char *nick, time_t ts)
{
    send_cmd(ServerName, "SVSMODE %s %lu +d 1", nick,
             (unsigned long int) ts);
}

/* SVSMODE +d */
/* nc_change was = 1, and there is no na->status */
void viagra_cmd_nc_change(User * u)
{
    common_svsmode(u, "+d", "1");
}

/* SVSMODE +d */
void viagra_cmd_svid_umode2(User * u, char *ts)
{
    /* not used by bahamut ircds */
}

void viagra_cmd_svid_umode3(User * u, char *ts)
{
    if (u->svid != u->timestamp) {
        common_svsmode(u, "+rd", ts);
    } else {
        common_svsmode(u, "+r", NULL);
    }
}

/* NICK <newnick>  */
void viagra_cmd_chg_nick(char *oldnick, char *newnick)
{
    if (!oldnick || !newnick) {
        return;
    }

    send_cmd(oldnick, "NICK %s", newnick);
}

/*
 * svsjoin
 *  parv[0] = sender
 *  parv[1] = nick to make join
 *  parv[2] = channel(s) to join
 */
void viagra_cmd_svsjoin(char *source, char *nick, char *chan, char *param)
{
    send_cmd(source, "SVSJOIN %s :%s", nick, chan);
}

/*
 * svspart
 *  parv[0] = sender
 *  parv[1] = nick to make part
 *  parv[2] = channel(s) to part
 */
void viagra_cmd_svspart(char *source, char *nick, char *chan)
{
    send_cmd(source, "SVSPART %s :%s", nick, chan);
}

void viagra_cmd_swhois(char *source, char *who, char *mask)
{
    /* not supported */
}

int viagra_flood_mode_check(char *value)
{
    return 0;
}

int xanadu_event_rehash(char *source, int ac, char **av)
{
    return MOD_CONT;
}

int xanadu_event_admin(char *source, int ac, char **av)
{
    return MOD_CONT;
}

int xanadu_event_snotice(char *source, int ac, char **av)
{
    return MOD_CONT;
}

int xanadu_event_invite(char *source, int ac, char **av)
{
    return MOD_CONT;
}

void viagra_cmd_eob()
{
    send_cmd(NULL, "BURST 0");
}

void viagra_cmd_jupe(char *jserver, char *who, char *reason)
{
    char rbuf[256];

    snprintf(rbuf, sizeof(rbuf), "Juped by %s%s%s", who,
             reason ? ": " : "", reason ? reason : "");

    if (findserver(servlist, jserver))
        viagra_cmd_squit(jserver, rbuf);
    viagra_cmd_server(jserver, 2, rbuf);
    new_server(me_server, jserver, rbuf, SERVER_JUPED, NULL);
}

/* GLOBOPS - to handle old WALLOPS */
void viagra_cmd_global_legacy(char *source, char *fmt)
{
    send_cmd(source ? source : ServerName, "GLOBOPS :%s", fmt);
}

/* 
  1 = valid nick
  0 = nick is in valid
*/
int viagra_valid_nick(char *nick)
{
    /* no hard coded invalid nicks */
    return 1;
}

/* 
  1 = valid chan
  0 = chan is in valid
*/
int viagra_valid_chan(char *chan)
{
    /* no hard coded invalid chans */
    return 1;
}


void viagra_cmd_ctcp(char *source, char *dest, char *buf)
{
    char *s;

    if (!buf) {
        return;
    } else {
        s = normalizeBuffer(buf);
    }

    send_cmd(source, "NOTICE %s :\1%s \1", dest, s);
    free(s);
}


/**
 * Tell anope which function we want to perform each task inside of anope.
 * These prototypes must match what anope expects.
 **/
void moduleAddAnopeCmds()
{
    pmodule_cmd_svsnoop(viagra_cmd_svsnoop);
    pmodule_cmd_remove_akill(viagra_cmd_remove_akill);
    pmodule_cmd_topic(viagra_cmd_topic);
    pmodule_cmd_vhost_off(viagra_cmd_vhost_off);
    pmodule_cmd_akill(viagra_cmd_akill);
    pmodule_cmd_svskill(viagra_cmd_svskill);
    pmodule_cmd_svsmode(viagra_cmd_svsmode);
    pmodule_cmd_372(viagra_cmd_372);
    pmodule_cmd_372_error(viagra_cmd_372_error);
    pmodule_cmd_375(viagra_cmd_375);
    pmodule_cmd_376(viagra_cmd_376);
    pmodule_cmd_nick(viagra_cmd_nick);
    pmodule_cmd_guest_nick(viagra_cmd_guest_nick);
    pmodule_cmd_mode(viagra_cmd_mode);
    pmodule_cmd_bot_nick(viagra_cmd_bot_nick);
    pmodule_cmd_kick(viagra_cmd_kick);
    pmodule_cmd_notice_ops(viagra_cmd_notice_ops);
    pmodule_cmd_notice(viagra_cmd_notice);
    pmodule_cmd_notice2(viagra_cmd_notice2);
    pmodule_cmd_privmsg(viagra_cmd_privmsg);
    pmodule_cmd_privmsg2(viagra_cmd_privmsg2);
    pmodule_cmd_serv_notice(viagra_cmd_serv_notice);
    pmodule_cmd_serv_privmsg(viagra_cmd_serv_privmsg);
    pmodule_cmd_bot_chan_mode(viagra_cmd_bot_chan_mode);
    pmodule_cmd_351(viagra_cmd_351);
    pmodule_cmd_quit(viagra_cmd_quit);
    pmodule_cmd_pong(viagra_cmd_pong);
    pmodule_cmd_join(viagra_cmd_join);
    pmodule_cmd_unsqline(viagra_cmd_unsqline);
    pmodule_cmd_invite(viagra_cmd_invite);
    pmodule_cmd_part(viagra_cmd_part);
    pmodule_cmd_391(viagra_cmd_391);
    pmodule_cmd_250(viagra_cmd_250);
    pmodule_cmd_307(viagra_cmd_307);
    pmodule_cmd_311(viagra_cmd_311);
    pmodule_cmd_312(viagra_cmd_312);
    pmodule_cmd_317(viagra_cmd_317);
    pmodule_cmd_219(viagra_cmd_219);
    pmodule_cmd_401(viagra_cmd_401);
    pmodule_cmd_318(viagra_cmd_318);
    pmodule_cmd_242(viagra_cmd_242);
    pmodule_cmd_243(viagra_cmd_243);
    pmodule_cmd_211(viagra_cmd_211);
    pmodule_cmd_global(viagra_cmd_global);
    pmodule_cmd_global_legacy(viagra_cmd_global_legacy);
    pmodule_cmd_sqline(viagra_cmd_sqline);
    pmodule_cmd_squit(viagra_cmd_squit);
    pmodule_cmd_svso(viagra_cmd_svso);
    pmodule_cmd_chg_nick(viagra_cmd_chg_nick);
    pmodule_cmd_svsnick(viagra_cmd_svsnick);
    pmodule_cmd_vhost_on(viagra_cmd_vhost_on);
    pmodule_cmd_connect(viagra_cmd_connect);
    pmodule_cmd_svshold(viagra_cmd_svshold);
    pmodule_cmd_release_svshold(viagra_cmd_release_svshold);
    pmodule_cmd_unsgline(viagra_cmd_unsgline);
    pmodule_cmd_unszline(viagra_cmd_unszline);
    pmodule_cmd_szline(viagra_cmd_szline);
    pmodule_cmd_sgline(viagra_cmd_sgline);
    pmodule_cmd_unban(viagra_cmd_unban);
    pmodule_cmd_svsmode_chan(viagra_cmd_svsmode_chan);
    pmodule_cmd_svid_umode(viagra_cmd_svid_umode);
    pmodule_cmd_nc_change(viagra_cmd_nc_change);
    pmodule_cmd_svid_umode2(viagra_cmd_svid_umode2);
    pmodule_cmd_svid_umode3(viagra_cmd_svid_umode3);
    pmodule_cmd_svsjoin(viagra_cmd_svsjoin);
    pmodule_cmd_svspart(viagra_cmd_svspart);
    pmodule_cmd_swhois(viagra_cmd_swhois);
    pmodule_cmd_eob(viagra_cmd_eob);
    pmodule_flood_mode_check(viagra_flood_mode_check);
    pmodule_cmd_jupe(viagra_cmd_jupe);
    pmodule_valid_nick(viagra_valid_nick);
    pmodule_valid_chan(viagra_valid_chan);
    pmodule_cmd_ctcp(viagra_cmd_ctcp);
    pmodule_set_umode(viagra_set_umode);
}

/** 
 * Now tell anope how to use us.
 **/
int AnopeInit(int argc, char **argv)
{

    moduleAddAuthor("Anope");
    moduleAddVersion("$Id: viagra.c 1291 2007-08-30 20:59:14Z drstein $");
    moduleSetType(PROTOCOL);

    pmodule_ircd_version("ViagraIRCd 1.3.x");
    pmodule_ircd_cap(myIrcdcap);
    pmodule_ircd_var(myIrcd);
    pmodule_ircd_cbmodeinfos(myCbmodeinfos);
    pmodule_ircd_cumodes(myCumodes);
    pmodule_ircd_flood_mode_char_set("");
    pmodule_ircd_flood_mode_char_remove("");
    pmodule_ircd_cbmodes(myCbmodes);
    pmodule_ircd_cmmodes(myCmmodes);
    pmodule_ircd_csmodes(myCsmodes);
    pmodule_ircd_useTSMode(0);

        /** Deal with modes anope _needs_ to know **/
    pmodule_invis_umode(UMODE_i);
    pmodule_oper_umode(UMODE_o);
    pmodule_invite_cmode(CMODE_i);
    pmodule_secret_cmode(CMODE_s);
    pmodule_private_cmode(CMODE_p);
    pmodule_key_mode(CMODE_k);
    pmodule_limit_mode(CMODE_l);

    moduleAddAnopeCmds();
    moduleAddIRCDMsgs();

    return MOD_CONT;
}

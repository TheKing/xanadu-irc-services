/* BotServ core fantasy functions
 *
 * (C) 2003-2007 Anope Team
 * Contact us at info@anope.org
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Epona by Lara.
 * Based on the original code of Services by Andy Church. 
 * 
 * $Id: bs_fantasy_unban.c 1265 2007-08-26 15:33:06Z geniusdex $
 *
 */
/*************************************************************************/

#include "module.h"

int do_fantasy(int argc, char **argv);

/**
 * Create the hook, and tell anope about it.
 * @param argc Argument count
 * @param argv Argument list
 * @return MOD_CONT to allow the module, MOD_STOP to stop it
 **/
int AnopeInit(int argc, char **argv)
{
    EvtHook *hook;

    moduleAddAuthor("Anope");
    moduleAddVersion
        ("$Id: bs_fantasy_unban.c 1265 2007-08-26 15:33:06Z geniusdex $");
    moduleSetType(CORE);

    hook = createEventHook(EVENT_BOT_FANTASY, do_fantasy);
    moduleAddEventHook(hook);

    return MOD_CONT;
}

/**
 * Unload the module
 **/
void AnopeFini(void)
{

}

/**
 * Handle unban fantasy command.
 * @param argc Argument count
 * @param argv Argument list
 * @return MOD_CONT or MOD_STOP
 **/
int do_fantasy(int argc, char **argv)
{
    User *u;
    ChannelInfo *ci;
    char *target = NULL;

    if (argc < 3)
        return MOD_CONT;

    if (stricmp(argv[0], "unban") == 0) {
        u = finduser(argv[1]);
        ci = cs_findchan(argv[2]);
        if (!u || !ci || !check_access(u, ci, CA_UNBAN))
            return MOD_CONT;

        if (argc >= 4)
            target = myStrGetToken(argv[3], ' ', 0);
        if (!target)
            common_unban(ci, u->nick);
        else
            common_unban(ci, target);
    }

    return MOD_CONT;
}

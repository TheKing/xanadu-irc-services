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
 * $Id: bs_fantasy_owner.c 1265 2007-08-26 15:33:06Z geniusdex $
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
        ("$Id: bs_fantasy_owner.c 1265 2007-08-26 15:33:06Z geniusdex $");
    moduleSetType(CORE);

    /* No need to load of we don't support owner */
    if (!ircd->owner) {
        alog("Your ircd doesn't support the owner channelmode; bs_fantasy_owner won't be loaded");
        return MOD_STOP;
    }

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
 * Handle owner/deowner fantasy commands.
 * @param argc Argument count
 * @param argv Argument list
 * @return MOD_CONT or MOD_STOP
 **/
int do_fantasy(int argc, char **argv)
{
    User *u;
    ChannelInfo *ci;

    if (argc < 3)
        return MOD_CONT;

    if (stricmp(argv[0], "deowner") == 0) {
        u = finduser(argv[1]);
        ci = cs_findchan(argv[2]);
        if (!u || !ci)
            return MOD_CONT;

        if (is_founder(u, ci))
            bot_raw_mode(u, ci, ircd->ownerunset, u->nick);
    } else if (stricmp(argv[0], "owner") == 0) {
        u = finduser(argv[1]);
        ci = cs_findchan(argv[2]);
        if (!u || !ci)
            return MOD_CONT;

        if (is_founder(u, ci))
            bot_raw_mode(u, ci, ircd->ownerset, u->nick);
    }

    return MOD_CONT;
}

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
 * $Id: os_shutdown.c 1265 2007-08-26 15:33:06Z geniusdex $
 *
 */
/*************************************************************************/

#include "module.h"

int do_shutdown(User * u);
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
    moduleAddVersion("$Id: os_shutdown.c 1265 2007-08-26 15:33:06Z geniusdex $");
    moduleSetType(CORE);

    c = createCommand("SHUTDOWN", do_shutdown, is_services_root,
                      OPER_HELP_SHUTDOWN, -1, -1, -1, -1);
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
    if (is_services_root(u)) {
        notice_lang(s_OperServ, u, OPER_HELP_CMD_SHUTDOWN);
    }
}

/**
 * The /os shutdown command.
 * @param u The user who issued the command
 * @param MOD_CONT to continue processing other modules, MOD_STOP to stop processing.
 **/
int do_shutdown(User * u)
{
    quitmsg = calloc(32 + strlen(u->nick), 1);
    if (!quitmsg)
        quitmsg = "SHUTDOWN command received, but out of memory!";
    else
        sprintf(quitmsg, "SHUTDOWN command received from %s", u->nick);

    if (GlobalOnCycle) {
        oper_global(NULL, "%s", GlobalOnCycleMessage);
    }
    save_data = 1;
    delayed_quit = 1;
    return MOD_CONT;
}

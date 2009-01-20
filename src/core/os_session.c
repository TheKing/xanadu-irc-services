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
 * $Id: os_session.c 1265 2007-08-26 15:33:06Z geniusdex $
 *
 */
/*************************************************************************/

#include "module.h"

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
    moduleAddVersion("$Id: os_session.c 1265 2007-08-26 15:33:06Z geniusdex $");
    moduleSetType(CORE);

    /**
     * do_session/do_exception are exported from sessions.c - we just want to provide an interface.
     **/
    c = createCommand("SESSION", do_session, is_services_oper,
                      OPER_HELP_SESSION, -1, -1, -1, -1);
    moduleAddCommand(OPERSERV, c, MOD_UNIQUE);
    c = createCommand("EXCEPTION", do_exception, is_services_oper,
                      OPER_HELP_EXCEPTION, -1, -1, -1, -1);
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
        notice_lang(s_OperServ, u, OPER_HELP_CMD_SESSION);
        notice_lang(s_OperServ, u, OPER_HELP_CMD_EXCEPTION);
    }
}

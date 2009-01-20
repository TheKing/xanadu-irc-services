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
 * $Id: os_opernews.c 1265 2007-08-26 15:33:06Z geniusdex $
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
    moduleAddVersion("$Id: os_opernews.c 1265 2007-08-26 15:33:06Z geniusdex $");
    moduleSetType(CORE);

    /** 
     * For some unknown reason, do_opernews is actaully defined in news.c
     * we can look at moving it here later
     **/
    c = createCommand("OPERNEWS", do_opernews, is_services_admin,
                      NEWS_HELP_OPER, -1, -1, -1, -1);
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
    if (is_services_admin(u)) {
        notice_lang(s_OperServ, u, OPER_HELP_CMD_OPERNEWS);
    }
}

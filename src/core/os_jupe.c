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
 * $Id: os_jupe.c 1265 2007-08-26 15:33:06Z geniusdex $
 *
 */
/*************************************************************************/

#include "module.h"

int do_jupe(User * u);
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
    moduleAddVersion("$Id: os_jupe.c 1265 2007-08-26 15:33:06Z geniusdex $");
    moduleSetType(CORE);

    c = createCommand("JUPE", do_jupe, is_services_admin, OPER_HELP_JUPE,
                      -1, -1, -1, -1);
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
        notice_lang(s_OperServ, u, OPER_HELP_CMD_JUPE);
    }
}

/**
 * The /os jupe command.
 * @param u The user who issued the command
 * @param MOD_CONT to continue processing other modules, MOD_STOP to stop processing.
 **/
int do_jupe(User * u)
{
    char *jserver = strtok(NULL, " ");
    char *reason = strtok(NULL, "");

    if (!jserver) {
        syntax_error(s_OperServ, u, "JUPE", OPER_JUPE_SYNTAX);
    } else {
        if (!isValidHost(jserver, 3)) {
            notice_lang(s_OperServ, u, OPER_JUPE_HOST_ERROR);
        } else {
            xanadu_cmd_jupe(jserver, u->nick, reason);

            if (WallOSJupe)
                xanadu_cmd_global(s_OperServ, "\2%s\2 used JUPE on \2%s\2",
                                 u->nick, jserver);
        }
    }
    return MOD_CONT;
}

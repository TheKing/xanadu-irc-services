/* MemoServ core functions
 *
 * (C) 2003-2007 Anope Team
 * Contact us at info@anope.org
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Epona by Lara.
 * Based on the original code of Services by Andy Church. 
 * 
 * $Id: ms_help.c 1265 2007-08-26 15:33:06Z geniusdex $
 *
 */
/*************************************************************************/

#include "module.h"

int do_help(User * u);

/**
 * Create the help command, and tell anope about it.
 * @param argc Argument count
 * @param argv Argument list
 * @return MOD_CONT to allow the module, MOD_STOP to stop it
 **/
int AnopeInit(int argc, char **argv)
{
    Command *c;

    moduleAddAuthor("Anope");
    moduleAddVersion("$Id: ms_help.c 1265 2007-08-26 15:33:06Z geniusdex $");
    moduleSetType(CORE);
    c = createCommand("HELP", do_help, NULL, -1, -1, -1, -1, -1);
    moduleAddCommand(MEMOSERV, c, MOD_UNIQUE);

    return MOD_CONT;
}

/**
 * Unload the module
 **/
void AnopeFini(void)
{

}



/**
 * The /ms help command.
 * @param u The user who issued the command
 * @param MOD_CONT to continue processing other modules, MOD_STOP to stop processing.
 **/
int do_help(User * u)
{
    char *cmd = strtok(NULL, "");

    if (!cmd) {
        notice_help(s_MemoServ, u, MEMO_HELP_HEADER);
        moduleDisplayHelp(3, u);
        notice_help(s_MemoServ, u, MEMO_HELP_FOOTER, s_ChanServ);
    } else {
        mod_help_cmd(s_MemoServ, u, MEMOSERV, cmd);
    }
    return MOD_CONT;
}

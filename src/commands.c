/* Routines for looking up commands in a *Serv command list.
 *
 * (C) 2003-2007 Anope Team
 * Contact us at info@anope.org
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Epona by Lara.
 * Based on the original code of Services by Andy Church. 
 * 
 * $Id: commands.c 1282 2007-08-28 14:20:44Z geniusdex $ 
 *
 */

#include "services.h"
#include "commands.h"
#include "language.h"

/*************************************************************************/

/**
 * Return the Command corresponding to the given name, or NULL if no such
 * command exists.
 * @param list Command struct
 * @param cmd Command to look up
 * @return Command Struct for the given cmd
 */
Command *lookup_cmd(Command * list, char *cmd)
{
    Command *c;

    for (c = list; c->name; c++) {
        if (stricmp(c->name, cmd) == 0) {
            return c;
        }
    }
    return NULL;
}

/*************************************************************************/

/**
 * Run the routine for the given command, if it exists and the user has
 * privilege to do so; if not, print an appropriate error message.
 * @param services Services Client
 * @param u User Struct
 * @param list Command struct
 * @param cmd Command
 * @return void
 */
void run_cmd(char *service, User * u, Command * list, char *cmd)
{
    Command *c = lookup_cmd(list, cmd);
    do_run_cmd(service, u, c, cmd);
}

/*************************************************************************/

/**
 * Run the routine for the given command, if it exists and the user has
 * privilege to do so; if not, print an appropriate error message.
 * @param services Services Client
 * @param u User Struct
 * @param Command Hash Table
 * @param cmd Command
 * @return void
 */
void mod_run_cmd(char *service, User * u, CommandHash * cmdTable[],
                 const char *cmd)
{
    Command *c = findCommand(cmdTable, cmd);
    do_run_cmd(service, u, c, cmd);
}


/*************************************************************************/

/**
 * Run the given command
 * @param services Services Client
 * @param u User Struct
 * @param c Command Struct
 * @param cmd Command
 * @return void
 */
void do_run_cmd(char *service, User * u, Command * c, const char *cmd)
{
    int retVal = 0;
    Command *current;

    if (c && c->routine) {
        if ((checkDefCon(DEFCON_OPER_ONLY)
             || checkDefCon(DEFCON_SILENT_OPER_ONLY)) && !is_oper(u)) {
            if (!checkDefCon(DEFCON_SILENT_OPER_ONLY)) {
                notice_lang(service, u, OPER_DEFCON_DENIED);
            }
        } else {
            mod_current_module_name = c->mod_name;
            mod_current_module = NULL;
            if ((c->has_priv == NULL) || c->has_priv(u)) {
                retVal = c->routine(u);
                mod_current_module_name = NULL;
                if (retVal == MOD_CONT) {
                    current = c->next;
                    while (current && retVal == MOD_CONT) {
                        mod_current_module_name = current->mod_name;
                        mod_current_module = NULL;
                        retVal = current->routine(u);
                        mod_current_module_name = NULL;
                        current = current->next;
                    }
                }
            } else {
                notice_lang(service, u, ACCESS_DENIED);
                alog("Access denied for %s with service %s and command %s",
                     u->nick, service, cmd);
            }
            mod_current_module_name = NULL;
        }
    } else {
        if ((!checkDefCon(DEFCON_SILENT_OPER_ONLY)) || is_oper(u)) {
            notice_lang(service, u, UNKNOWN_COMMAND_HELP, cmd, service);
        }
    }
}

/*************************************************************************/

/**
 * Output the 'Limited to' line for the given command
 * @param service Services Client
 * @param u User Struct
 * @param c Command Struct
 * @return void
 */
void do_help_limited(char *service, User * u, Command * c)
{
    if (c->has_priv == is_services_oper)
        notice_lang(service, u, HELP_LIMIT_SERV_OPER);
    else if (c->has_priv == is_services_admin)
        notice_lang(service, u, HELP_LIMIT_SERV_ADMIN);
    else if (c->has_priv == is_services_root)
        notice_lang(service, u, HELP_LIMIT_SERV_ROOT);
    else if (c->has_priv == is_oper)
        notice_lang(service, u, HELP_LIMIT_IRC_OPER);
    else if (c->has_priv == is_host_setter)
        notice_lang(service, u, HELP_LIMIT_HOST_SETTER);
    else if (c->has_priv == is_host_remover)
        notice_lang(service, u, HELP_LIMIT_HOST_REMOVER);
}

/*************************************************************************/

/**
 * Print a help message for the given command.
 * @param services Services Client
 * @param u User Struct
 * @param c Command Struct
 * @param cmd Command
 * @return void
 */
void do_help_cmd(char *service, User * u, Command * c, const char *cmd)
{
    Command *current;
    int has_had_help = 0;
    int cont = MOD_CONT;
    const char *p1 = NULL, *p2 = NULL, *p3 = NULL, *p4 = NULL;
    Module *calling_module = mod_current_module;
    char *calling_module_name = mod_current_module_name;

    for (current = c; (current) && (cont == MOD_CONT);
         current = current->next) {
        mod_current_module_name = current->mod_name;
        if (mod_current_module_name)
            mod_current_module = findModule(mod_current_module_name);
        else
            mod_current_module = NULL;

        p1 = current->help_param1;
        p2 = current->help_param2;
        p3 = current->help_param3;
        p4 = current->help_param4;
        if (current->helpmsg_all >= 0) {
            notice_help(service, u, current->helpmsg_all, p1, p2, p3, p4);
            has_had_help = 1;
        } else if (current->all_help) {
            cont = current->all_help(u);
            has_had_help = 1;
        }
        if (is_services_root(u)) {
            if (current->helpmsg_root >= 0) {
                notice_help(service, u, current->helpmsg_root, p1, p2, p3,
                            p4);
                has_had_help = 1;
            } else if (current->root_help) {
                cont = current->root_help(u);
                has_had_help = 1;
            }
        } else if (is_services_admin(u)) {
            if (current->helpmsg_admin >= 0) {
                notice_help(service, u, current->helpmsg_admin, p1, p2, p3,
                            p4);
                has_had_help = 1;
            } else if (current->admin_help) {
                cont = current->admin_help(u);
                has_had_help = 1;
            }
        } else if (is_services_oper(u)) {
            if (current->helpmsg_oper >= 0) {
                notice_help(service, u, current->helpmsg_oper, p1, p2, p3,
                            p4);
                has_had_help = 1;
            } else if (current->oper_help) {
                cont = current->oper_help(u);
                has_had_help = 1;
            }
        } else {
            if (current->helpmsg_reg >= 0) {
                notice_help(service, u, current->helpmsg_reg, p1, p2, p3,
                            p4);
                has_had_help = 1;
            } else if (current->regular_help) {
                cont = current->regular_help(u);
                has_had_help = 1;
            }
        }
    }
    if (has_had_help == 0) {
        notice_lang(service, u, NO_HELP_AVAILABLE, cmd);
    } else {
        do_help_limited(service, u, c);
    }

    mod_current_module = calling_module;
    mod_current_module_name = calling_module_name;
}

/*************************************************************************/

/**
 * Find the Help Command
 * @param services Services Client
 * @param u User Struct
 * @param c Command Struct
 * @param cmd Command
 * @return void
 */
void help_cmd(char *service, User * u, Command * list, char *cmd)
{
    Command *c = lookup_cmd(list, cmd);
    do_help_cmd(service, u, c, cmd);
}

/*************************************************************************/

/**
 * Find the Help Command
 * @param services Services Client
 * @param u User Struct
 * @param Command Hash Table
 * @param cmd Command
 * @return void
 */
void mod_help_cmd(char *service, User * u, CommandHash * cmdTable[],
                  const char *cmd)
{
    Command *c = findCommand(cmdTable, cmd);
    do_help_cmd(service, u, c, cmd);
}

/*************************************************************************/

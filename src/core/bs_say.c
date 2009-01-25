/* BotServ core functions
 *
 * (C) 2003-2007 Anope Team
 * Contact us at info@anope.org
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Epona by Lara.
 * Based on the original code of Services by Andy Church. 
 * 
 * $Id: bs_say.c 1265 2007-08-26 15:33:06Z geniusdex $
 *
 */
/*************************************************************************/

#include "module.h"

int do_say(User * u);
void myBotServHelp(User * u);

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
    moduleAddVersion("$Id: bs_say.c 1265 2007-08-26 15:33:06Z geniusdex $");
    moduleSetType(CORE);
    c = createCommand("SAY", do_say, NULL, BOT_HELP_SAY, -1, -1, -1, -1);
    moduleAddCommand(BOTSERV, c, MOD_UNIQUE);

    moduleSetBotHelp(myBotServHelp);

    return MOD_CONT;
}

/**
 * Unload the module
 **/
void AnopeFini(void)
{

}



/**
 * Add the help response to Anopes /bs help output.
 * @param u The user who is requesting help
 **/
void myBotServHelp(User * u)
{
    notice_lang(s_BotServ, u, BOT_HELP_CMD_SAY);
}

/**
 * The /bs say command.
 * @param u The user who issued the command
 * @param MOD_CONT to continue processing other modules, MOD_STOP to stop processing.
 **/
int do_say(User * u)
{
    ChannelInfo *ci;

    char *chan = strtok(NULL, " ");
    char *text = strtok(NULL, "");

    if (!chan || !text)
        syntax_error(s_BotServ, u, "SAY", BOT_SAY_SYNTAX);
    else if (!(ci = cs_findchan(chan)))
        notice_lang(s_BotServ, u, CHAN_X_NOT_REGISTERED, chan);
    else if (ci->flags & CI_VERBOTEN)
        notice_lang(s_BotServ, u, CHAN_X_FORBIDDEN, chan);
    else if (!ci->bi)
        notice_help(s_BotServ, u, BOT_NOT_ASSIGNED);
    else if (!ci->c || ci->c->usercount < BSMinUsers)
        notice_lang(s_BotServ, u, BOT_NOT_ON_CHANNEL, ci->name);
    else if (!check_access(u, ci, CA_SAY))
        notice_lang(s_BotServ, u, ACCESS_DENIED);
    else {
        if (text[0] != '\001') {
            xanadu_cmd_privmsg(ci->bi->nick, ci->name, "%s", text);
            ci->bi->lastmsg = time(NULL);
            if (logchan && LogBot)
                xanadu_cmd_privmsg(ci->bi->nick, LogChannel,
                                  "SAY %s %s %s", u->nick, ci->name, text);
        } else {
            syntax_error(s_BotServ, u, "SAY", BOT_SAY_SYNTAX);
        }
    }
    return MOD_CONT;
}

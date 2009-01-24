/* ChanServ : FLAGS (Core)
 *
 * (C) 2009 Xanadu IRC Services Team
 *
 * Based on the original code of Anope by the Anope Team.
 * Based on the original code of Epona by Lara.
 * Based on the original code of Services by Andy Church.
 *
 * $Id: cs_flags.c 1.0 01-23-2009 $
 */


int AnopeInit(int argc, char **argv)
{
    Command *c;

    moduleAddAuthor("Xanadu");
    moduleAddVersion("$Id: cs_flags.c TheKing 1-23-2009");
    moduleSetType(CORE);
    c = createCommand("FLAGS", do_cmd_flags, NULL, CHAN_HELP_FLAGS, -1, -1, -1, -1);
    moduleAddCommand(CHANSERV, c, MOD_UNIQUE);
    moduleSetChanHelp(myChanServHelp);

    return MOD_CONT;

}

void AnopeFini(void)
{

}

void myChanServHelp(User * u)
{

notice_lang(s_ChanServ, u, "lawlz");

}



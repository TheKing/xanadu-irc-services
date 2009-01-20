/**
 * The ChanServ SUSPEND command. - Source
 *
 ***********
 * Module Name    : bs_fantasy_ext
 * Author         : Viper <Viper@Absurd-IRC.net>
 * Creation Date  : 09/09/2007
 *
 * More info on http://modules.anope.org and http://forum.anope.org
 ***********
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 ***********
 *
 * Last Updated   : 09/09/2007
 *
 **/

#ifdef ENABLE_SUSPEND
#include "suspend.h"

/**
 * The suspend command.
 * @param u The user who issued the command
 * @param ci The channel we will be suspending.
 * @param reason The reason given for suspending the channel.
 * @param MOD_CONT to continue processing other modules, MOD_STOP to stop processing.
 **/
int do_suspend(User * u, ChannelInfo *ci, char *reason) {
	Channel *c;

	if (!ci) {
		moduleNoticeLang(ci->bi->nick, u, LANG_SUSPEND_SYNTAX);
		return MOD_CONT;
	}

	if (ForceForbidReason && !reason) {
		moduleNoticeLang(ci->bi->nick, u, LANG_SUSPEND_NO_REASON);
		return MOD_CONT;
	}

	if (readonly) {
		notice_lang(ci->bi->nick, u, READ_ONLY_MODE);
		return MOD_CONT;
	}

	ci->flags |= CI_SUSPENDED;
	ci->forbidby = sstrdup(u->nick);
	if (reason)
		ci->forbidreason = sstrdup(reason);

	if ((c = findchan(ci->name))) {
		struct c_userlist *cu, *next;
		char *av[3];

		for (cu = c->users; cu; cu = next) {
			next = cu->next;

			if (is_oper(cu->user))
				continue;

			av[0] = c->name;
			av[1] = cu->user->nick;
			av[2] = reason ? reason : "CHAN_SUSPEND_REASON";
			anope_cmd_kick(ci->bi->nick, av[0], av[1], av[2]);
			do_kick(ci->bi->nick, 3, av);
		}
	}

	if (WallForbid)
		anope_cmd_global(ci->bi->nick, "\2%s\2 used SUSPEND on channel \2%s\2",
				u->nick, ci->name);

	alog("%s: %s set SUSPEND for channel %s", ci->bi->nick, u->nick, ci->name);
	notice_lang(ci->bi->nick, u, CHAN_SUSPEND_SUCCEEDED, ci->name);
	send_event(EVENT_CHAN_SUSPENDED, 1, ci->name);

	return MOD_CONT;
}
#endif

/* EOF */

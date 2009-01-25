/**
 * Misc IRCoper commands the module adds - Source
 *
 ***********
 * Module Name    : bs_fantasy_ext
 * Author         : Viper <Viper@Absurd-IRC.net>
 * Creation Date  : 21/07/2006
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
 * Last Updated   : 27/02/2007
 *
 **/

#include "misc_oper.h"

#ifdef ENABLE_KILL
int my_do_kill(User *u, Channel *c, char *target, char *reason) {
	ChannelInfo *ci = c->ci;
	User *u2;
	int is_same = 0, exists = 1, ok = 1, checked = 0;

	if (!target) {
		target = u->nick;
	}

	is_same = (target == u->nick) ? 1 : (stricmp(target, u->nick) == 0);
	if (is_same) {
		if (!(reason))
			reason = "WOEHHHAAAAAAAA.... LOOK I CAN FLYY !!";
		u2 = u;
		exists = 1;
	} else
		exists = ((u2 = finduser(target)) ? 1 : 0);

	if (!exists)
		return MOD_CONT;

/* is_oper(u)
 * is_services_oper(u)
 * is_services_admin(u)
 * is_services_root(u)
 */

	/* gernal access control */
	if (!is_oper(u) && OSOpersOnly)
		notice_lang(ci->bi->nick, u, ACCESS_DENIED);
	else if (!is_services_oper(u) && !is_services_admin(u) && !is_services_root(u))
		notice_lang(ci->bi->nick, u, ACCESS_DENIED);
	else {
		/* check if target has greater access then executor
		 * there s probably an easier/shorter way to check this, but this keeps it easy */
		if (is_services_root(u)) {
			checked = 1;
			if (is_services_root(u2) && !(is_same))
				ok = 0;
		}
		if (is_services_admin(u) && !checked) {
			checked = 1;
			if ((is_services_admin(u2) || is_services_root(u2)) && !is_same)
				ok = 0;
		}
		if (is_services_oper(u) && !checked) {
			checked = 1;
			if ((is_services_oper(u2) || is_services_admin(u2) || is_services_root(u2)) && !(is_same))
				ok = 0;
		}

		if (!ok)
			notice_lang(ci->bi->nick, u, PERMISSION_DENIED);
		else {
			char killreason[600];
			if (reason) {
				snprintf(killreason, sizeof(killreason), "%s (Requested by %s)", reason, u->nick);
				kill_user(s_OperServ, u2->nick, killreason);
				alog("[bs_fantasy_ext] %s used !KILL to forcefully disconnect %s from the network. Reason: %s", u->nick, u2->nick, reason);
			} else {
				snprintf(killreason, sizeof(killreason), "Requested by %s", u->nick);
				kill_user(s_OperServ, u2->nick, killreason);
				alog("[bs_fantasy_ext] %s used !KILL to forcefully disconnect %s from the network. No reason specified.", u->nick, u2->nick);
			}
		}
	}

	return MOD_CONT;
}
#endif


#ifdef ENABLE_MODE
int do_mode(User * u, Channel *c, char *modes) {
	ChannelInfo *ci = c->ci;
	int ac;
	char **av;

	if (!modes) {
		moduleNoticeLang(ci->bi->nick, u, LANG_MODE_SYNTAX, BSFantasyCharacter);
		return MOD_CONT;
	}

	if (c->bouncy_modes) {
		notice_lang(ci->bi->nick, u, OPER_BOUNCY_MODES_U_LINE);
		return MOD_CONT;
	} else if ((ircd->adminmode) && (!is_services_admin(u)) && (c->mode & ircd->adminmode)) {
		notice_lang(ci->bi->nick, u, PERMISSION_DENIED);
		return MOD_CONT;
	} else {
		xanadu_cmd_mode(s_OperServ, c->name, "%s", modes);

		ac = split_buf(modes, &av, 1);
		chan_set_modes(s_OperServ, c, ac, av, -1);
		free(av);

		if (WallOSMode)
			xanadu_cmd_global(ci->bi->nick, "%s used !MODE %s on %s", u->nick, modes, c->name);

		alog("[bs_fantasy_ext] %s used !MODE to set %s on channel %s", u->nick, modes, c->name);
	}
	return MOD_CONT;
}
#endif

/* EOF */

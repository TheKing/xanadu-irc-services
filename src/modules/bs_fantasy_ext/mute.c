/**
 * Main functions for handling the mute and unmute commands - Source
 *
 ***********
 * Module Name    : bs_fantasy_ext
 * Author         : Viper <Viper@Absurd-IRC.net>
 * Creation Date  : 14/09/2006
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
 * Last Updated   : 08/02/2008
 *
 **/

#ifdef ENABLE_MUTEUNMUTE
#include "mute.h"

int do_mute(User * u, Channel *c, char *params) {
	ChannelInfo *ci = c->ci;
	User *u2;
	int is_same, exists;

	if (!params) {
		params = u->nick;
	}

	is_same = (params == u->nick) ? 1 : (stricmp(params, u->nick) == 0);

	if (is_same) {
		u2 = u;
		exists = 1;
	} else
		exists = ((u2 = finduser(params)) ? 1 : 0);

	if (!is_same ? !check_access(u, ci, CA_BAN) : !check_access(u, ci, CA_BANME)) {
		notice_lang(ci->bi->nick, u, ACCESS_DENIED);
	} else if (!is_same && exists && (ci->flags & CI_PEACE) && (get_access(u2, ci) >= get_access(u, ci))) {
		notice_lang(ci->bi->nick, u, PERMISSION_DENIED);
	} else if (exists && ((ircd->protectedumode && is_protected(u2)) && !is_founder(u, ci))) {
		notice_lang(ci->bi->nick, u, PERMISSION_DENIED);
	/**
	 * Dont ban the user on channels where he is excepted
	 * to prevent services <-> server wars.
	 **/
	} else if (exists && (ircd->except && is_excepted(ci, u2))) {
		notice_lang(ci->bi->nick, u, CHAN_EXCEPTED, u2->nick, ci->name);
	} else if (!exists && (ircd->except && is_excepted_mask(ci, params))) {
		notice_lang(ci->bi->nick, u, CHAN_EXCEPTED, params, ci->name);
	} else if (exists && RestrictKB && ((!is_founder(u, ci) && is_services_oper(u2)) ||
			(is_founder(u, ci) && is_services_admin(u2)))) {
		notice_lang(ci->bi->nick, u, PERMISSION_DENIED);
	} else {
		if (!stricmp(params, ci->bi->nick))
			bot_raw_kick(u, ci, u->nick, "Oops!");

		else {
			struct c_userlist *cu = NULL;
			struct c_userlist *next;
			char mask[BUFSIZE];
			int ok = 1;

			if (exists) {
				do_up_down(u2, c, "down", 0);
				get_idealban(ci, u2, mask, sizeof(mask));
			}

			else if (my_match_wild_nocase("*@*", params)) {
				/* If we get a *@* target we need to add the *!... */
				if (!my_match_wild_nocase("*!*@*", params)) {
					char buf[BUFSIZE];
					snprintf(buf, BUFSIZE, "*!%s", params);
					free(params);
					params = sstrdup(buf);
				}

#if CPU_USAGE_REDUCTION < 10
				cu = c->users;

				/* Check to make sure were are not going to ban an admin here... */
				while ((cu) && ok) {
					next = cu->next;
					if (match_usermask(params, cu->user)) {
						if (ircd->protectedumode && is_protected(cu->user) && !is_founder(u, ci)) {
							notice_lang(ci->bi->nick, u, PERMISSION_DENIED);
							ok = 0;
						} else if ((ci->flags & CI_PEACE) && (get_access(cu->user, ci) >= get_access(u, ci))) {
							notice_lang(ci->bi->nick, u, PERMISSION_DENIED);
							ok = 0;
						} else if (ircd->except && is_excepted(ci, cu->user)) {
							notice_lang(ci->bi->nick, u, CHAN_EXCEPTED, cu->user->nick, ci->name);
							ok = 0;
						} else if (RestrictKB && ((!is_founder(u, ci) && is_services_oper(cu->user)) ||
								(is_founder(u, ci) && is_services_admin(cu->user)))) {
							notice_lang(ci->bi->nick, u, PERMISSION_DENIED);
							ok = 0;
						}
					}
					cu = next;
				}
#endif
			} else {
				moduleNoticeLang(ci->bi->nick, u, LANG_REQ_NICK_OR_MASK);
				ok = 0;
			}

			if (ok) {
				char buf[BUFSIZE];
				char *av[2];

				if (!stricmp(IRCDModule, "unreal32")) {
					if (exists)
						snprintf(buf, sizeof(buf), "~q:%s", mask);
					else
						snprintf(buf, sizeof(buf), "~q:%s", params);

					av[0] = sstrdup("+b");
					av[1] = sstrdup(buf);

				} else if (!stricmp(IRCDModule, "charybdis")) {
					av[0] = sstrdup("+q");
					if (exists)
						av[1] = sstrdup(mask);
					else
						av[1] = sstrdup(params);

				} else {
					av[0] = sstrdup("+b");
					if (exists)
						av[1] = sstrdup(mask);
					else
						av[1] = sstrdup(params);
				}

				xanadu_cmd_mode(ci->bi->nick,c ->name, "%s %s", av[0], av[1]);
				chan_set_modes(ci->bi->nick, c, 2, av, 1);

#if CPU_USAGE_REDUCTION < 5
				if (!exists) {
					cu = c->users;

					while ((cu)) {
						next = cu->next;
						if (match_usermask(params, cu->user))
							do_up_down(cu->user, c, "down", 0);
						cu = next;
					}
				}
#endif

				if (av[0]) free(av[0]);
				if (av[1]) free(av[1]);
			}
		}
	}

	return MOD_CONT;
}

int do_unmute(User * u, Channel *c, char *params) {
	ChannelInfo *ci = c->ci;
	User *u2;
	int is_same, exists;
	char *av[2];
	char buf[256];

	if (!params) {
		params = u->nick;
	}

	is_same = (params == u->nick) ? 1 : (stricmp(params, u->nick) == 0);

	if (is_same) {
		u2 = u;
		exists = 1;
	} else
		exists = ((u2 = finduser(params)) ? 1 : 0);

	if (!check_access(u, ci, CA_UNBAN)) {
		notice_lang(ci->bi->nick, u, ACCESS_DENIED);
	} else {
		char mask[BUFSIZE];

		if (exists) {
			get_idealban(ci, u2, mask, sizeof(mask));
		}

		if (!stricmp(IRCDModule, "unreal32")) {
			if (exists)
				snprintf(buf, sizeof(buf), "~q:%s", mask);
			else
				snprintf(buf, sizeof(buf), "~q:%s", params);

			av[0] = sstrdup("-b");
			av[1] = sstrdup(buf);

		} else if (!stricmp(IRCDModule, "charybdis")) {
			av[0] = sstrdup("-q");
			if (exists)
				av[1] = sstrdup(mask);
			else
				av[1] = sstrdup(params);

		} else {
			av[0] = sstrdup("-b");
			if (exists)
				av[1] = sstrdup(mask);
			else
				av[1] = sstrdup(params);
		}

		xanadu_cmd_mode(ci->bi->nick,c ->name, "%s %s", av[0], av[1]);
		chan_set_modes(ci->bi->nick, c, 2, av, 1);


		if (av[0]) free(av[0]);
		if (av[1]) free(av[1]);
	}

	return MOD_CONT;
}
#endif


/* EOF */

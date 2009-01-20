/**
 * Methods to modify and access the ChanServ XOP lists. - Source
 *
 ***********
 * Module Name    : bs_fantasy_ext
 * Author         : Viper <Viper@Absurd-IRC.net>
 * Creation Date  : 21/07/2006
 *
 * More info on http://modules.anope.org and http://forum.anope.org
 ***********
 *
 * Based on the code of Anope by The Anope Dev Team
 *
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

#ifdef ENABLE_XOP
#include "xop.h"

/* Here we handle all changes or list requests to the access list with xop enabled */
int do_xop(User *u, char *xname, int xlev, int *xmsgs,  Channel *c, char *cmd, char *nick) {
	char event_access[BUFSIZE];

	ChannelInfo *ci = c->ci;
	NickAlias *na;
	NickCore *nc;

	int i;
	int change = 0;
	short ulev;
	int is_list = (cmd && stricmp(cmd, "LIST") == 0);
	int is_servadmin = is_services_admin(u);
	ChanAccess *access;

	/* If CLEAR, we don't need any parameters.
	 * If LIST, we don't *require* any parameters, but we can take any.
	 * If DEL or ADD we require a nick. */
	if (!cmd || ((is_list || !stricmp(cmd, "CLEAR")) ? 0 : !nick)) {
		syntax_error(ci->bi->nick, u, xname, xmsgs[0]);
	} else if (!(ci->flags & CI_XOP)) {
		notice_lang(ci->bi->nick, u, CHAN_XOP_ACCESS, s_ChanServ);
	} else if (stricmp(cmd, "ADD") == 0) {
		if (readonly) {
			notice_lang(ci->bi->nick, u, xmsgs[1]);
			return MOD_CONT;
		}

		ulev = get_access(u, ci);

		if ((xlev >= ulev || ulev < ACCESS_AOP) && !is_servadmin) {
			notice_lang(ci->bi->nick, u, PERMISSION_DENIED);
			return MOD_CONT;
		}

		na = findnick(nick);
		if (!na) {
			notice_lang(ci->bi->nick, u, xmsgs[2]);
			return MOD_CONT;
		} else if (na->status & NS_VERBOTEN) {
			notice_lang(ci->bi->nick, u, NICK_X_FORBIDDEN, na->nick);
			return MOD_CONT;
		}

		nc = na->nc;
		for (access = ci->access, i = 0; i < ci->accesscount;
			access++, i++) {
			if (access->nc == nc) {
				/**
				* Patch provided by PopCorn to prevert AOP's reducing SOP's levels
				**/
				if ((access->level >= ulev) && (!is_servadmin)) {
					notice_lang(ci->bi->nick, u, PERMISSION_DENIED);
					return MOD_CONT;
				}
				change++;
				break;
			}
		}

		if (!change) {
			if (i < CSAccessMax) {
				ci->accesscount++;
				ci->access = srealloc(ci->access, sizeof(ChanAccess) * ci->accesscount);
			} else {
				notice_lang(s_ChanServ, u, CHAN_XOP_REACHED_LIMIT, CSAccessMax);
				return MOD_CONT;
			}

			access = &ci->access[i];
			access->nc = nc;
		}

		access->in_use = 1;
		access->level = xlev;
		access->last_seen = 0;

		alog("%s: %s!%s@%s (level %d) %s access level %d to %s (group %s) on channel %s", ci->bi->nick, u->nick, u->username, u->host, ulev, change ? "changed" : "set", access->level, na->nick, nc->display, ci->name);

		snprintf(event_access, BUFSIZE, "%d", access->level);

		if (!change) {
			send_event(EVENT_ACCESS_ADD, 4, ci->name, u->nick, na->nick, event_access);
			notice_lang(ci->bi->nick, u, xmsgs[3], access->nc->display, ci->name);
		} else {
			send_event(EVENT_ACCESS_CHANGE, 4, ci->name, u->nick, na->nick, event_access);
			notice_lang(ci->bi->nick, u, xmsgs[4], access->nc->display, ci->name);
		}

	} else if (stricmp(cmd, "DEL") == 0) {
		int deleted, a, b;
		if (readonly) {
			notice_lang(ci->bi->nick, u, xmsgs[1]);
			return MOD_CONT;
		}

		if (ci->accesscount == 0) {
			notice_lang(ci->bi->nick, u, xmsgs[11], ci->name);
			return MOD_CONT;
		}

		ulev = get_access(u, ci);

		if ((xlev >= ulev || ulev < ACCESS_AOP) && !is_servadmin) {
			notice_lang(ci->bi->nick, u, PERMISSION_DENIED);
			return MOD_CONT;
		}

		/* Special case: is it a number/list?  Only do search if it isn't. */
		if (isdigit(*nick) && strspn(nick, "1234567890,-") == strlen(nick)) {
			int count, last = -1, perm = 0;
			deleted = process_numlist(nick, &count, xop_del_callback, u, ci, &last, &perm, ulev, xlev);
			if (!deleted) {
				if (perm) {
					notice_lang(ci->bi->nick, u, PERMISSION_DENIED);
				} else if (count == 1) {
					notice_lang(ci->bi->nick, u, xmsgs[5], last, ci->name);
				} else {
					notice_lang(ci->bi->nick, u, xmsgs[7], ci->name);
				}
			} else if (deleted == 1) {
				notice_lang(ci->bi->nick, u, xmsgs[9], ci->name);
			} else {
				notice_lang(ci->bi->nick, u, xmsgs[10], deleted, ci->name);
			}
		} else {
			na = findnick(nick);
			if (!na) {
				notice_lang(ci->bi->nick, u, NICK_X_NOT_REGISTERED, nick);
				return MOD_CONT;
			}
			nc = na->nc;

			for (i = 0; i < ci->accesscount; i++)
				if (ci->access[i].nc == nc && ci->access[i].level == xlev)
					break;

			if (i == ci->accesscount) {
				notice_lang(ci->bi->nick, u, xmsgs[6], nick, ci->name);
				return MOD_CONT;
			}

			access = &ci->access[i];
			if (!is_servadmin && ulev <= access->level) {
				deleted = 0;
				notice_lang(ci->bi->nick, u, PERMISSION_DENIED);
			} else {
				notice_lang(ci->bi->nick, u, xmsgs[8], access->nc->display, ci->name);
				access->nc = NULL;
				access->in_use = 0;
				send_event(EVENT_ACCESS_DEL, 3, ci->name, u->nick, na->nick);
				deleted = 1;
			}
		}
		if (deleted) {
			/* Reordering - DrStein */
			for (b = 0; b < ci->accesscount; b++) {
				if (ci->access[b].in_use) {
					for (a = 0; a < ci->accesscount; a++) {
						if (a > b)
							break;
						if (!ci->access[a].in_use) {
							ci->access[a].in_use = 1;
							ci->access[a].level = ci->access[b].level;
							ci->access[a].nc = ci->access[b].nc;
							ci->access[a].last_seen =
							ci->access[b].last_seen;
							ci->access[b].nc = NULL;
							ci->access[b].in_use = 0;
							break;
						}
					}
				}
			}

			/* After reordering only the entries at the end could still be empty.
			 * We ll free the places no longer in use... */
			for (i = ci->accesscount - 1; i >= 0; i--) {
				if (ci->access[i].in_use == 1)
					break;

				ci->accesscount--;
			}
			ci->access = srealloc(ci->access,sizeof(ChanAccess) * ci->accesscount);
		}
	} else if (stricmp(cmd, "LIST") == 0) {
		int sent_header = 0;

		ulev = get_access(u, ci);

		if (!is_servadmin && ulev < ACCESS_AOP) {
			notice_lang(ci->bi->nick, u, ACCESS_DENIED);
			return MOD_CONT;
		}

		if (ci->accesscount == 0) {
			notice_lang(ci->bi->nick, u, xmsgs[11], ci->name);
			return MOD_CONT;
		}

		if (nick && strspn(nick, "1234567890,-") == strlen(nick)) {
			process_numlist(nick, NULL, xop_list_callback, u, ci, &sent_header, xlev, xmsgs[12]);
		} else {
			for (i = 0; i < ci->accesscount; i++) {
				if (nick && ci->access[i].nc && !my_match_wild_nocase(nick, ci->access[i].nc->display))
					continue;
				xop_list(u, i, ci, &sent_header, xlev, xmsgs[12]);
			}
		}
		if (!sent_header)
			notice_lang(ci->bi->nick, u, xmsgs[7], ci->name);
	} else if (stricmp(cmd, "CLEAR") == 0) {
		if (readonly) {
			notice_lang(ci->bi->nick, u, CHAN_ACCESS_DISABLED);
			return MOD_CONT;
		}

		if (ci->accesscount == 0) {
			notice_lang(ci->bi->nick, u, CHAN_ACCESS_LIST_EMPTY, ci->name);
			return MOD_CONT;
		}

		if (!is_servadmin && !is_founder(u, ci)) {
			notice_lang(ci->bi->nick, u, PERMISSION_DENIED);
			return MOD_CONT;
		}

		for (i = 0; i < ci->accesscount; i++) {
			if (ci->access[i].in_use && ci->access[i].level == xlev) {
				ci->access[i].nc = NULL;
				ci->access[i].in_use = 0;
			}
		}

		send_event(EVENT_ACCESS_CLEAR, 2, ci->name, u->nick);

		notice_lang(ci->bi->nick, u, xmsgs[13], ci->name);
	} else {
		syntax_error(ci->bi->nick, u, xname, xmsgs[0]);
	}
	return MOD_CONT;
}

/* `last' is set to the last index this routine was called with
 * `perm' is incremented whenever a permission-denied error occurs
 */
static int xop_del(User * u, ChannelInfo * ci, ChanAccess * access, int *perm, int uacc, int xlev) {
	char *nick = access->nc->display;
	if (!access->in_use || access->level != xlev)
		return 0;
	if (!is_services_admin(u) && uacc <= access->level) {
		(*perm)++;
		return 0;
	}
	access->nc = NULL;
	access->in_use = 0;
	send_event(EVENT_ACCESS_DEL, 3, ci->name, u->nick, nick);
	return 1;
}

static int xop_del_callback(User *u, int num, va_list args) {
	ChannelInfo *ci = va_arg(args, ChannelInfo *);
	int *last = va_arg(args, int *);
	int *perm = va_arg(args, int *);
	int uacc = va_arg(args, int);
	int xlev = va_arg(args, int);

	if (num < 1 || num > ci->accesscount)
		return 0;
	*last = num;

	return xop_del(u, ci, &ci->access[num - 1], perm, uacc, xlev);
}


static int xop_list(User *u, int index, ChannelInfo *ci, int *sent_header, int xlev, int xmsg) {
	ChanAccess *access = &ci->access[index];

	if (!access->in_use || access->level != xlev)
		return 0;

	if (!*sent_header) {
		notice_lang(ci->bi->nick, u, xmsg, ci->name);
		*sent_header = 1;
	}

	notice_lang(ci->bi->nick, u, CHAN_XOP_LIST_FORMAT, index + 1, access->nc->display);
	return 1;
}

static int xop_list_callback(User *u, int num, va_list args) {
	ChannelInfo *ci = va_arg(args, ChannelInfo *);
	int *sent_header = va_arg(args, int *);
	int xlev = va_arg(args, int);
	int xmsg = va_arg(args, int);

	if (num < 1 || num > ci->accesscount)
		return 0;

	return xop_list(u, num - 1, ci, sent_header, xlev, xmsg);
}
#endif

/* EOF */

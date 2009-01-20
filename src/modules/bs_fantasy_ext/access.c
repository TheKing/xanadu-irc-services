/**
 * Methods to modify and access the ChanServ access list. - Source
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

#ifdef ENABLE_ACCESS
#include "access.h"

int do_access(User *u, Channel *c, char *cmd, char *nick, char *s) {
	char event_access[BUFSIZE];

	ChannelInfo *ci = c->ci;
	NickAlias *na = NULL;
	NickCore *nc;
	ChanAccess *access;

	int i;
	int level = 0, ulev;
	int is_list = (cmd && stricmp(cmd, "LIST") == 0);
	int is_servadmin = is_services_admin(u);

	/* If LIST, we don't *require* any parameters, but we can take any.
	 * If DEL, we require a nick and no level.
	 * Else (ADD), we require a level (which implies a nick). */
	if (!cmd || ((is_list || !stricmp(cmd, "CLEAR")) ? 0 : (stricmp(cmd, "DEL") == 0) ? (!nick || s) : !s)) {
		moduleNoticeLang(ci->bi->nick, u, LANG_ACCESS_SYNTAX, BSFantasyCharacter);
	/* We still allow LIST in xOP mode, but not others */
	} else if ((ci->flags & CI_XOP) && !is_list) {
		if (ircd->halfop)
			notice_lang(ci->bi->nick, u, CHAN_ACCESS_XOP_HOP, s_ChanServ);
		else
			notice_lang(ci->bi->nick, u, CHAN_ACCESS_XOP, s_ChanServ);
	} else if (((is_list && !check_access(u, ci, CA_ACCESS_LIST)) || (!is_list
		&& !check_access(u, ci, CA_ACCESS_CHANGE))) && !is_servadmin) {
		notice_lang(ci->bi->nick, u, ACCESS_DENIED);
	} else if (stricmp(cmd, "ADD") == 0) {
		if (readonly) {
			notice_lang(ci->bi->nick, u, CHAN_ACCESS_DISABLED);
			return MOD_CONT;
		}

		level = atoi(s);
		ulev = get_access(u, ci);

		if (!is_servadmin && level >= ulev) {
			notice_lang(ci->bi->nick, u, PERMISSION_DENIED);
			return MOD_CONT;
		}

		if (level == 0) {
			notice_lang(ci->bi->nick, u, CHAN_ACCESS_LEVEL_NONZERO);
			return MOD_CONT;
		} else if (level <= ACCESS_INVALID || level >= ACCESS_FOUNDER) {
			notice_lang(ci->bi->nick, u, CHAN_ACCESS_LEVEL_RANGE, ACCESS_INVALID + 1, ACCESS_FOUNDER - 1);
			return MOD_CONT;
		}

		na = findnick(nick);
		if (!na) {
			notice_lang(ci->bi->nick, u, CHAN_ACCESS_NICKS_ONLY);
			return MOD_CONT;
		}
		if (na->status & NS_VERBOTEN) {
			notice_lang(ci->bi->nick, u, NICK_X_FORBIDDEN, nick);
			return MOD_CONT;
		}

		nc = na->nc;
		for (access = ci->access, i = 0; i < ci->accesscount; access++, i++) {
			if (access->nc == nc) {
				/* Don't allow lowering from a level >= ulev */
				if (!is_servadmin && access->level >= ulev) {
					notice_lang(ci->bi->nick, u, PERMISSION_DENIED);
					return MOD_CONT;
				}
				if (access->level == level) {
					notice_lang(ci->bi->nick, u, CHAN_ACCESS_LEVEL_UNCHANGED, access->nc->display, ci->name, level);
					return MOD_CONT;
				}
				access->level = level;
				snprintf(event_access, BUFSIZE, "%d", access->level);
				send_event(EVENT_ACCESS_CHANGE, 4, ci->name, u->nick, na->nick, event_access);
				alog("%s: %s!%s@%s (level %d) set access level %d to %s (group %s) on channel %s", ci->bi->nick, u->nick, u->username, u->host, ulev, access->level, na->nick, nc->display, ci->name);
				notice_lang(ci->bi->nick, u, CHAN_ACCESS_LEVEL_CHANGED, access->nc->display, ci->name, level);
				return MOD_CONT;
			}
		}

		/* All entries should be in use so we no longer need
		 * to go over the entire list..
		for (i = 0; i < ci->accesscount; i++) {
			if (!ci->access[i].in_use)
				break;
		}
		*/

		if (i == ci->accesscount) {
			if (i < CSAccessMax) {
				ci->accesscount++;
				ci->access = srealloc(ci->access, sizeof(ChanAccess) * ci->accesscount);
			} else {
				notice_lang(ci->bi->nick, u, CHAN_ACCESS_REACHED_LIMIT, CSAccessMax);
				return MOD_CONT;
			}
		}

		access = &ci->access[i];
		access->nc = nc;
		access->in_use = 1;
		access->level = level;
		access->last_seen = 0;

		snprintf(event_access, BUFSIZE, "%d", access->level);
		send_event(EVENT_ACCESS_ADD, 4, ci->name, u->nick, na->nick, event_access);
		alog("%s: %s!%s@%s (level %d) set access level %d to %s (group %s) on channel %s", ci->bi->nick, u->nick, u->username, u->host, ulev, access->level, na->nick, nc->display, ci->name);
		notice_lang(ci->bi->nick, u, CHAN_ACCESS_ADDED, nc->display, ci->name, access->level);
    } else if (stricmp(cmd, "DEL") == 0) {
		int deleted, a, b;
		if (readonly) {
			notice_lang(ci->bi->nick, u, CHAN_ACCESS_DISABLED);
			return MOD_CONT;
		}

		if (ci->accesscount == 0) {
			notice_lang(ci->bi->nick, u, CHAN_ACCESS_LIST_EMPTY, ci->name);
			return MOD_CONT;
		}

		/* Special case: is it a number/list?  Only do search if it isn't. */
		if (isdigit(*nick) && strspn(nick, "1234567890,-") == strlen(nick)) {
			int count, last = -1, perm = 0;
			deleted = process_numlist(nick, &count, access_del_callback, u, ci, &last, &perm, get_access(u, ci));
			if (!deleted) {
				if (perm) {
					notice_lang(ci->bi->nick, u, PERMISSION_DENIED);
				} else if (count == 1) {
					notice_lang(ci->bi->nick, u, CHAN_ACCESS_NO_SUCH_ENTRY, last, ci->name);
				} else {
					notice_lang(ci->bi->nick, u, CHAN_ACCESS_NO_MATCH, ci->name);
				}
			} else if (deleted == 1) {
				notice_lang(ci->bi->nick, u, CHAN_ACCESS_DELETED_ONE, ci->name);
			} else {
				notice_lang(ci->bi->nick, u, CHAN_ACCESS_DELETED_SEVERAL, deleted, ci->name);
			}
		} else {
			na = findnick(nick);
			if (!na) {
				notice_lang(ci->bi->nick, u, NICK_X_NOT_REGISTERED, nick);
				return MOD_CONT;
			}
			nc = na->nc;
			for (i = 0; i < ci->accesscount; i++) {
				if (ci->access[i].nc == nc)
					break;
			}
			if (i == ci->accesscount) {
				notice_lang(ci->bi->nick, u, CHAN_ACCESS_NOT_FOUND, nick, ci->name);
				return MOD_CONT;
			}
			access = &ci->access[i];
			if (!is_servadmin && get_access(u, ci) <= access->level) {
				deleted = 0;
				notice_lang(ci->bi->nick, u, PERMISSION_DENIED);
			} else {
				notice_lang(ci->bi->nick, u, CHAN_ACCESS_DELETED, access->nc->display, ci->name);
				alog("%s: %s!%s@%s (level %d) deleted access of %s (group %s) on %s", ci->bi->nick, u->nick, u->username, u->host, get_access(u, ci), na->nick, access->nc->display, ci->name);
				access->nc = NULL;
				access->in_use = 0;
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

			/* We don't know the nick if someone used numbers, so we trigger the event without
		 	 * nick param. We just do this once, even if someone enters a range. -Certus */
			if (na)
				send_event(EVENT_ACCESS_DEL, 3, ci->name, u->nick, na->nick);
			else
				send_event(EVENT_ACCESS_DEL, 2, ci->name, u->nick);
		}
	} else if (stricmp(cmd, "LIST") == 0) {
		int sent_header = 0;

		if (ci->accesscount == 0) {
			notice_lang(ci->bi->nick, u, CHAN_ACCESS_LIST_EMPTY, ci->name);
			return MOD_CONT;
		}
		if (nick && strspn(nick, "1234567890,-") == strlen(nick)) {
			process_numlist(nick, NULL, access_list_callback, u, ci, &sent_header);
		} else {
			for (i = 0; i < ci->accesscount; i++) {
				if (nick && ci->access[i].nc && !my_match_wild_nocase(nick, ci->access[i].nc->display))
					continue;
				access_list(u, i, ci, &sent_header);
			}
		}
		if (!sent_header) {
			notice_lang(ci->bi->nick, u, CHAN_ACCESS_NO_MATCH, ci->name);
		} else {
			notice_lang(ci->bi->nick, u, CHAN_ACCESS_LIST_FOOTER, ci->name);
		}
	} else if (stricmp(cmd, "CLEAR") == 0) {
		if (readonly) {
			notice_lang(ci->bi->nick, u, CHAN_ACCESS_DISABLED);
			return MOD_CONT;
		}

		if (!is_servadmin && !is_founder(u, ci)) {
			notice_lang(ci->bi->nick, u, PERMISSION_DENIED);
			return MOD_CONT;
		}

		free(ci->access);
		ci->access = NULL;
		ci->accesscount = 0;

		send_event(EVENT_ACCESS_CLEAR, 2, ci->name, u->nick);

		notice_lang(ci->bi->nick, u, CHAN_ACCESS_CLEAR, ci->name);
		alog("%s: %s!%s@%s (level %d) cleared access list on %s", ci->bi->nick, u->nick, u->username,
			u->host, get_access(u, ci), ci->name);

	} else {
		moduleNoticeLang(ci->bi->nick, u, LANG_ACCESS_SYNTAX, BSFantasyCharacter);
	}
	return MOD_CONT;
}

static int access_del(User *u, ChannelInfo *ci, ChanAccess *access, int *perm, int uacc) {
	char *nick = access->nc->display;

	if (!access->in_use)
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

static int access_del_callback(User *u, int num, va_list args) {
	ChannelInfo *ci = va_arg(args, ChannelInfo *);
	int *last = va_arg(args, int *);
	int *perm = va_arg(args, int *);
	int uacc = va_arg(args, int);
	if (num < 1 || num > ci->accesscount)
		return 0;
	*last = num;
	return access_del(u, ci, &ci->access[num - 1], perm, uacc);
}


static int access_list(User *u, int index, ChannelInfo *ci, int *sent_header) {
	ChanAccess *access = &ci->access[index];
	const char *xop;

	if (!access->in_use)
		return 0;

	if (!*sent_header) {
		notice_lang(ci->bi->nick, u, CHAN_ACCESS_LIST_HEADER, ci->name);
		*sent_header = 1;
	}

	if (ci->flags & CI_XOP) {
		xop = get_xop_level(access->level);
		notice_lang(ci->bi->nick, u, CHAN_ACCESS_LIST_XOP_FORMAT, index + 1, xop, access->nc->display);
	} else {
		notice_lang(ci->bi->nick, u, CHAN_ACCESS_LIST_AXS_FORMAT, index + 1, access->level, access->nc->display);
	}
	return 1;
}

static int access_list_callback(User *u, int num, va_list args) {
	ChannelInfo *ci = va_arg(args, ChannelInfo *);
	int *sent_header = va_arg(args, int *);
	if (num < 1 || num > ci->accesscount)
		return 0;
	return access_list(u, num - 1, ci, sent_header);
}

int do_levels(User *u, Channel *c, char *cmd, char *what, short level) {
	ChannelInfo *ci = c->ci;
	int i;

	if (ci->flags & CI_XOP) {
		notice_lang(ci->bi->nick, u, CHAN_LEVELS_XOP);
	} else if (!is_founder(u, ci) && !is_services_admin(u)) {
		notice_lang(ci->bi->nick, u, ACCESS_DENIED);
	} else if (stricmp(cmd, "SET") == 0) {

		if (level <= ACCESS_INVALID || level >= ACCESS_FOUNDER) {
			notice_lang(ci->bi->nick, u, CHAN_LEVELS_RANGE, ACCESS_INVALID + 1, ACCESS_FOUNDER - 1);
			return MOD_CONT;
		}

		for (i = 0; levelinfo[i].what >= 0; i++) {
			if (stricmp(levelinfo[i].name, what) == 0) {
				ci->levels[levelinfo[i].what] = level;

				alog("%s: %s!%s@%s set level %s on channel %s to %d", ci->bi->nick, u->nick, u->username, u->host,
					levelinfo[i].name, ci->name, level);
				notice_lang(ci->bi->nick, u, CHAN_LEVELS_CHANGED, levelinfo[i].name, ci->name, level);
				return MOD_CONT;
			}
		}

		notice_lang(ci->bi->nick, u, CHAN_LEVELS_UNKNOWN, what, s_ChanServ);

	} else if (stricmp(cmd, "DIS") == 0 || stricmp(cmd, "DISABLE") == 0) {
		for (i = 0; levelinfo[i].what >= 0; i++) {
			if (stricmp(levelinfo[i].name, what) == 0) {
				ci->levels[levelinfo[i].what] = ACCESS_INVALID;

				alog("%s: %s!%s@%s disabled level %s on channel %s", ci->bi->nick, u->nick, u->username, u->host,
					levelinfo[i].name, ci->name);
				notice_lang(ci->bi->nick, u, CHAN_LEVELS_DISABLED, levelinfo[i].name, ci->name);
				return MOD_CONT;
			}
		}

		notice_lang(ci->bi->nick, u, CHAN_LEVELS_UNKNOWN, what, s_ChanServ);
	} else if (stricmp(cmd, "LIST") == 0) {
		int i;

		notice_lang(ci->bi->nick, u, CHAN_LEVELS_LIST_HEADER, ci->name);

		if (!levelinfo_maxwidth) {
			for (i = 0; levelinfo[i].what >= 0; i++) {
				int len = strlen(levelinfo[i].name);
				if (len > levelinfo_maxwidth)
					levelinfo_maxwidth = len;
			}
		}

		for (i = 0; levelinfo[i].what >= 0; i++) {
			int j = ci->levels[levelinfo[i].what];

			if (j == ACCESS_INVALID) {
				j = levelinfo[i].what;

				if (j == CA_AUTOOP || j == CA_AUTODEOP || j == CA_AUTOVOICE || j == CA_NOJOIN) {
					notice_lang(ci->bi->nick, u, CHAN_LEVELS_LIST_DISABLED, levelinfo_maxwidth, levelinfo[i].name);
				} else {
					notice_lang(ci->bi->nick, u, CHAN_LEVELS_LIST_DISABLED, levelinfo_maxwidth, levelinfo[i].name);
				}
			} else if (j == ACCESS_FOUNDER) {
				notice_lang(ci->bi->nick, u, CHAN_LEVELS_LIST_FOUNDER, levelinfo_maxwidth, levelinfo[i].name);
			} else {
				notice_lang(ci->bi->nick, u, CHAN_LEVELS_LIST_NORMAL, levelinfo_maxwidth, levelinfo[i].name, j);
			}
		}

	} else if (stricmp(cmd, "RESET") == 0) {
		reset_levels(ci);

		alog("%s: %s!%s@%s reset levels definitions on channel %s", ci->bi->nick, u->nick, u->username, u->host, ci->name);
		notice_lang(ci->bi->nick, u, CHAN_LEVELS_RESET, ci->name);
	} else {
		moduleNoticeLang(ci->bi->nick, u, LANG_LEVELS_SYNTAX, BSFantasyCharacter);
	}
	return MOD_CONT;
}
#endif

/* EOF */

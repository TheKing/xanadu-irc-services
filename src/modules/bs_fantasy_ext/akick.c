/**
 * Methods to modify and access the ChanServ akick list. - Source
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
 * Last Updated   : 14/02/2008
 *
 **/

#ifdef ENABLE_AKICK
#include "akick.h"

int do_akick(User * u, Channel *c, char *cmd, char *mask, char *reason) {
	ChannelInfo *ci = c->ci;
	AutoKick *akick;
	int i;
	struct c_userlist *cu = NULL;
	struct c_userlist *next;
	char *argv[3];
	int count = 0;

	if (!cmd || (!mask && (!stricmp(cmd, "ADD") || !stricmp(cmd, "STICK")
		|| !stricmp(cmd, "UNSTICK") || !stricmp(cmd, "DEL")))) {
		moduleNoticeLang(ci->bi->nick, u, LANG_AKICK_SYNTAX, BSFantasyCharacter);
	} else if (!check_access(u, ci, CA_AKICK) && !is_services_admin(u)) {
		notice_lang(ci->bi->nick, u, ACCESS_DENIED);
	} else if (stricmp(cmd, "ADD") == 0) {
		NickAlias *na = findnick(mask);
		User *target = finduser(mask);
		NickCore *nc = NULL;
		char *nick, *user, *host;
		int freemask = 0;

		if (readonly) {
			notice_lang(ci->bi->nick, u, CHAN_AKICK_DISABLED);
			return MOD_CONT;
		}

		if (!na) {
			if (target) {
				char tmp[BUFSIZE];
				mask = NULL;
				freemask = 1;
				get_idealban(ci, target, tmp, BUFSIZE);
				mask = sstrdup(tmp);
			} else {
				split_usermask(mask, &nick, &user, &host);
				mask = scalloc(strlen(nick) + strlen(user) + strlen(host) + 3, 1);
				freemask = 1;
				sprintf(mask, "%s!%s@%s", nick, user, host);

				free(nick);
				free(user);
				free(host);
			}
		} else {
			if (na->status & NS_VERBOTEN) {
				notice_lang(ci->bi->nick, u, NICK_X_FORBIDDEN, mask);
				return MOD_CONT;
			}
			nc = na->nc;
		}

		/* Check excepts BEFORE we get this far */
		if (ircd->except) {
			if (is_excepted_mask(ci, mask) == 1) {
				notice_lang(ci->bi->nick, u, CHAN_EXCEPTED, mask, c->name);
				if (freemask) free(mask);
				return MOD_CONT;
			}
		}

		for (akick = ci->akick, i = 0; i < ci->akickcount; akick++, i++) {
			if (!(akick->flags & AK_USED))
				continue;
			if ((akick->flags & AK_ISNICK) ? akick->u.nc == nc : stricmp(akick->u.mask, mask) == 0) {
				notice_lang(ci->bi->nick, u, CHAN_AKICK_ALREADY_EXISTS, (akick->flags & AK_ISNICK) ? akick->u.nc->
					display : akick->u.mask, c->name);
				if (freemask) free(mask);
				return MOD_CONT;
			}
		}

		/* All entries should be in use so we don't have to go over
		 * the entire list. We simply add new entries at the end. */
		if (ci->akickcount >= CSAutokickMax) {
			notice_lang(s_ChanServ, u, CHAN_AKICK_REACHED_LIMIT, CSAutokickMax);
			if (freemask) free(mask);
			return MOD_CONT;
		}
		ci->akickcount++;
		ci->akick = srealloc(ci->akick, sizeof(AutoKick) * ci->akickcount);

		akick = &ci->akick[i];
		akick->flags = AK_USED;
		if (nc) {
			akick->flags |= AK_ISNICK;
			akick->u.nc = nc;
		} else {
			akick->u.mask = sstrdup(mask);
		}
		akick->creator = sstrdup(u->nick);
		akick->addtime = time(NULL);
		if (reason) {
			if (strlen(reason) > 200)
				reason[200] = '\0';
			akick->reason = sstrdup(reason);
		} else {
			akick->reason = NULL;
		}

		/* Auto ENFORCE #63 */
		if (c) {
			cu = c->users;
			while (cu) {
				next = cu->next;
				if (check_kick(cu->user, c->name, c->creation_time)) {
					argv[0] = sstrdup(c->name);
					argv[1] = sstrdup(cu->user->nick);
					if (akick->reason)
						argv[2] = sstrdup(akick->reason);
					else
						argv[2] = sstrdup("none");

					do_kick(ci->bi->nick, 3, argv);

					free(argv[2]);
					free(argv[1]);
					free(argv[0]);
					count++;

				}
				cu = next;
			}
		}
		notice_lang(ci->bi->nick, u, CHAN_AKICK_ADDED, mask, c->name);

		if (count)
			notice_lang(ci->bi->nick, u, CHAN_AKICK_ENFORCE_DONE, c->name, count);

		if (freemask) free(mask);
			return MOD_CONT;

	} else if (stricmp(cmd, "STICK") == 0) {
		NickAlias *na;
		NickCore *nc;

		if (readonly) {
			notice_lang(ci->bi->nick, u, CHAN_AKICK_DISABLED);
			return MOD_CONT;
		}

		if (ci->akickcount == 0) {
			notice_lang(ci->bi->nick, u, CHAN_AKICK_LIST_EMPTY, ci->name);
			return MOD_CONT;
		}

		na = findnick(mask);
		nc = (na ? na->nc : NULL);

		for (akick = ci->akick, i = 0; i < ci->akickcount; akick++, i++) {
			if (!(akick->flags & AK_USED) || (akick->flags & AK_ISNICK))
				continue;
			if (!stricmp(akick->u.mask, mask))
				break;
		}

		if (i == ci->akickcount) {
			notice_lang(ci->bi->nick, u, CHAN_AKICK_NOT_FOUND, mask, ci->name);
			return MOD_CONT;
		}

		akick->flags |= AK_STUCK;
		notice_lang(ci->bi->nick, u, CHAN_AKICK_STUCK, akick->u.mask, ci->name);

		if (ci->c)
			stick_mask(ci, akick);

	} else if (stricmp(cmd, "UNSTICK") == 0) {
		NickAlias *na;
		NickCore *nc;

		if (readonly) {
			notice_lang(ci->bi->nick, u, CHAN_AKICK_DISABLED);
			return MOD_CONT;
		}

		if (ci->akickcount == 0) {
			notice_lang(ci->bi->nick, u, CHAN_AKICK_LIST_EMPTY, ci->name);
			return MOD_CONT;
		}

		na = findnick(mask);
		nc = (na ? na->nc : NULL);

		for (akick = ci->akick, i = 0; i < ci->akickcount; akick++, i++) {
			if (!(akick->flags & AK_USED) || (akick->flags & AK_ISNICK))
				continue;
			if (!stricmp(akick->u.mask, mask))
				break;
		}

		if (i == ci->akickcount) {
			notice_lang(ci->bi->nick, u, CHAN_AKICK_NOT_FOUND, mask, ci->name);
			return MOD_CONT;
		}

		akick->flags &= ~AK_STUCK;
		notice_lang(ci->bi->nick, u, CHAN_AKICK_UNSTUCK, akick->u.mask, ci->name);

	} else if (stricmp(cmd, "DEL") == 0) {
		int deleted, a, b;

		if (readonly) {
			notice_lang(ci->bi->nick, u, CHAN_AKICK_DISABLED);
			return MOD_CONT;
		}

		if (ci->akickcount == 0) {
			notice_lang(ci->bi->nick, u, CHAN_AKICK_LIST_EMPTY, c->name);
			return MOD_CONT;
		}

		/* Special case: is it a number/list?  Only do search if it isn't. */
		if (isdigit(*mask) && strspn(mask, "1234567890,-") == strlen(mask)) {
			int count, last = -1;
			deleted = process_numlist(mask, &count, akick_del_callback, u, ci, &last);
			if (!deleted) {
				if (count == 1) {
					notice_lang(ci->bi->nick, u, CHAN_AKICK_NO_SUCH_ENTRY, last, ci->name);
				} else {
					notice_lang(ci->bi->nick, u, CHAN_AKICK_NO_MATCH, ci->name);
				}
			} else if (deleted == 1) {
				notice_lang(ci->bi->nick, u, CHAN_AKICK_DELETED_ONE, ci->name);
			} else {
				notice_lang(ci->bi->nick, u, CHAN_AKICK_DELETED_SEVERAL, deleted, ci->name);
			}
		} else {
			NickAlias *na = findnick(mask);
			NickCore *nc = (na ? na->nc : NULL);

			for (akick = ci->akick, i = 0; i < ci->akickcount; akick++, i++) {
				if (!(akick->flags & AK_USED))
					continue;
				if (((akick->flags & AK_ISNICK) && akick->u.nc == nc) || (!(akick->flags & AK_ISNICK)
					&& stricmp(akick->u.mask, mask) == 0))
					break;
			}
			if (i == ci->akickcount) {
				notice_lang(ci->bi->nick, u, CHAN_AKICK_NOT_FOUND, mask, c->name);
				return MOD_CONT;
			}
			notice_lang(ci->bi->nick, u, CHAN_AKICK_DELETED, mask, c->name);
			akick_del(u, akick);
			deleted = 1;
		}
		if (deleted) {
			/* Reordering - DrStein */
			for (b = 0; b < ci->akickcount; b++) {
				if (ci->akick[b].flags & AK_USED) {
					for (a = 0; a < ci->akickcount; a++) {
					if (a > b)
						break;
						if (!(ci->akick[a].flags & AK_USED)) {
							ci->akick[a].flags = ci->akick[b].flags;
							if (ci->akick[b].flags & AK_ISNICK) {
								ci->akick[a].u.nc = ci->akick[b].u.nc;
							} else {
								ci->akick[a].u.mask = sstrdup(ci->akick[b].u.mask);
							}
							/* maybe we should first check whether there
							is a reason before we sstdrup it -Certus */
							if (ci->akick[b].reason)
								ci->akick[a].reason = sstrdup(ci->akick[b].reason);
							else
								ci->akick[a].reason = NULL;
							ci->akick[a].creator = sstrdup(ci->akick[b].creator);
							ci->akick[a].addtime = ci->akick[b].addtime;

							akick_del(u, &ci->akick[b]);
							break;
						}
					}
				}
			}

			/* After reordering only the entries at the end could still be empty.
			 * We ll free the places no longer in use... - Viper */
			for (i = ci->akickcount - 1; i >= 0; i--) {
				if (ci->akick[i].flags & AK_USED)
					break;

				ci->akickcount--;
			}
			ci->akick = srealloc(ci->akick,sizeof(AutoKick) * ci->akickcount);
		}
    } else if (stricmp(cmd, "LIST") == 0) {
		int sent_header = 0;

		if (ci->akickcount == 0) {
			notice_lang(ci->bi->nick, u, CHAN_AKICK_LIST_EMPTY, c->name);
			return MOD_CONT;
		}
		if (mask && isdigit(*mask) && strspn(mask, "1234567890,-") == strlen(mask)) {
			process_numlist(mask, NULL, akick_list_callback, u, ci, &sent_header);
		} else {
			for (akick = ci->akick, i = 0; i < ci->akickcount; akick++, i++) {
				if (!(akick->flags & AK_USED))
					continue;
				if (mask) {
					if (!(akick->flags & AK_ISNICK) && !my_match_wild_nocase(mask, akick->u.mask))
						continue;
					if ((akick->flags & AK_ISNICK) && !my_match_wild_nocase(mask, akick->u.nc->display))
						continue;
				}
				akick_list(u, i, ci, &sent_header);
			}
		}
		if (!sent_header)
		notice_lang(ci->bi->nick, u, CHAN_AKICK_NO_MATCH, c->name);

	} else if (stricmp(cmd, "VIEW") == 0) {
		int sent_header = 0;

		if (ci->akickcount == 0) {
			notice_lang(ci->bi->nick, u, CHAN_AKICK_LIST_EMPTY, c->name);
			return MOD_CONT;
		}
		if (mask && isdigit(*mask) && strspn(mask, "1234567890,-") == strlen(mask)) {
			process_numlist(mask, NULL, akick_view_callback, u, ci, &sent_header);
		} else {
			for (akick = ci->akick, i = 0; i < ci->akickcount; akick++, i++) {
				if (!(akick->flags & AK_USED))
					continue;
				if (mask) {
					if (!(akick->flags & AK_ISNICK) && !my_match_wild_nocase(mask, akick->u.mask))
						continue;
					if ((akick->flags & AK_ISNICK) && !my_match_wild_nocase(mask, akick->u.nc->display))
						continue;
				}
				akick_view(u, i, ci, &sent_header);
			}
		}
		if (!sent_header)
		notice_lang(ci->bi->nick, u, CHAN_AKICK_NO_MATCH, c->name);

	} else if (stricmp(cmd, "ENFORCE") == 0) {
		struct c_userlist *cu = NULL;
		struct c_userlist *next;
		char *argv[3];
		int count = 0;

		if (!c) {
			notice_lang(ci->bi->nick, u, CHAN_X_NOT_IN_USE, ci->name);
			return MOD_CONT;
		}

		cu = c->users;

		while (cu) {
			next = cu->next;
			if (check_kick(cu->user, c->name, c->creation_time)) {
				argv[0] = sstrdup(c->name);
				argv[1] = sstrdup(cu->user->nick);
				argv[2] = sstrdup(CSAutokickReason);

				do_kick(ci->bi->nick, 3, argv);

				free(argv[2]);
				free(argv[1]);
				free(argv[0]);

				count++;
			}
			cu = next;
		}

		notice_lang(ci->bi->nick, u, CHAN_AKICK_ENFORCE_DONE, ci->name, count);

	} else if (stricmp(cmd, "CLEAR") == 0) {

		if (readonly) {
			notice_lang(ci->bi->nick, u, CHAN_AKICK_DISABLED);
			return MOD_CONT;
		}

		for (akick = ci->akick, i = 0; i < ci->akickcount; akick++, i++) {
			if (!(akick->flags & AK_USED))
				continue;
			akick_del(u, akick);
		}

		free(ci->akick);
		ci->akick = NULL;
		ci->akickcount = 0;

		notice_lang(ci->bi->nick, u, CHAN_AKICK_CLEAR, ci->name);

	} else {
		moduleNoticeLang(ci->bi->nick, u, LANG_AKICK_SYNTAX, BSFantasyCharacter);
	}
	return MOD_CONT;
}

static int akick_del(User * u, AutoKick * akick) {
	if (!(akick->flags & AK_USED))
		return 0;
	if (akick->flags & AK_ISNICK) {
		akick->u.nc = NULL;
	} else {
		free(akick->u.mask);
		akick->u.mask = NULL;
	}
	if (akick->reason) {
		free(akick->reason);
		akick->reason = NULL;
	}
	if (akick->creator) {
		free(akick->creator);
		akick->creator = NULL;
	}
	akick->addtime = 0;
	akick->flags = 0;
	return 1;
}

static int akick_del_callback(User * u, int num, va_list args) {
	ChannelInfo *ci = va_arg(args, ChannelInfo *);
	int *last = va_arg(args, int *);
	*last = num;
	if (num < 1 || num > ci->akickcount)
		return 0;
	return akick_del(u, &ci->akick[num - 1]);
}

static int akick_list(User * u, int index, ChannelInfo * ci, int *sent_header) {
	AutoKick *akick = &ci->akick[index];

	if (!(akick->flags & AK_USED))
		return 0;
	if (!*sent_header) {
		notice_lang(ci->bi->nick, u, CHAN_AKICK_LIST_HEADER, ci->name);
		*sent_header = 1;
	}

	notice_lang(ci->bi->nick, u, CHAN_AKICK_LIST_FORMAT, index + 1, ((akick->flags & AK_ISNICK) ? akick->u.nc->
		display : akick->u.mask), (akick->reason ? akick->reason : getstring(u->na, NO_REASON)));
	return 1;
}

static int akick_list_callback(User * u, int num, va_list args) {
	ChannelInfo *ci = va_arg(args, ChannelInfo *);
	int *sent_header = va_arg(args, int *);
	if (num < 1 || num > ci->akickcount)
		return 0;
	return akick_list(u, num - 1, ci, sent_header);
}

static int akick_view(User * u, int index, ChannelInfo * ci, int *sent_header) {
	AutoKick *akick = &ci->akick[index];
	char timebuf[64];
	struct tm tm;

	if (!(akick->flags & AK_USED))
		return 0;
	if (!*sent_header) {
		notice_lang(ci->bi->nick, u, CHAN_AKICK_LIST_HEADER, ci->name);
		*sent_header = 1;
	}

	if (akick->addtime) {
		tm = *localtime(&akick->addtime);
		strftime_lang(timebuf, sizeof(timebuf), u, STRFTIME_SHORT_DATE_FORMAT, &tm);
	} else {
		snprintf(timebuf, sizeof(timebuf), getstring(u->na, UNKNOWN));
	}

	notice_lang(ci->bi->nick, u, ((akick->flags & AK_STUCK) ? CHAN_AKICK_VIEW_FORMAT_STUCK :
		CHAN_AKICK_VIEW_FORMAT), index + 1, ((akick->flags & AK_ISNICK) ? akick->u.nc->display : akick->u.mask),
		akick->creator ? akick->creator : getstring(u->na, UNKNOWN), timebuf, (akick->reason ? akick->
		reason : getstring(u->na, NO_REASON)));
	return 1;
}

static int akick_view_callback(User * u, int num, va_list args) {
	ChannelInfo *ci = va_arg(args, ChannelInfo *);
	int *sent_header = va_arg(args, int *);
	if (num < 1 || num > ci->akickcount)
		return 0;
	return akick_view(u, num - 1, ci, sent_header);
}
#endif

/* EOF */

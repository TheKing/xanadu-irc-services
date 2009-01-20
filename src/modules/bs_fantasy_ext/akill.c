/**
 * Methods to modify and access the OperServ akill list. - Source
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

#ifdef ENABLE_AKILL
#include "akill.h"

/* Manage the AKILL list. */
int do_akill(User * u, Channel *c, char *cmd, char *expiry, char *mask, char *reason) {
	ChannelInfo *ci = c->ci;
	char breason[BUFSIZE];

	if (!cmd)
		cmd = "";

	if (!stricmp(cmd, "ADD")) {
		int deleted = 0;
		time_t expires;

		expires = expiry ? dotime(expiry) : AutokillExpiry;
		/* If the expiry given does not contain a final letter, it's in days,
		 * said the doc. Ah well.
		 */
		if (expiry && isdigit(expiry[strlen(expiry) - 1]))
			expires *= 86400;
		/* Do not allow less than a minute expiry time */
		if (expires != 0 && expires < 60) {
			notice_lang(ci->bi->nick, u, BAD_EXPIRY_TIME);
			return MOD_CONT;
		} else if (expires > 0) {
			expires += time(NULL);
		}

		if (mask && reason) {
			/* We first do some sanity check on the proposed mask. */
			if (strchr(mask, '!')) {
				notice_lang(ci->bi->nick, u, OPER_AKILL_NO_NICK);
				return MOD_CONT;
			}

			if (!strchr(mask, '@')) {
				notice_lang(ci->bi->nick, u, BAD_USERHOST_MASK);
				return MOD_CONT;
			}

			if (mask && strspn(mask, "~@.*?") == strlen(mask)) {
				notice_lang(ci->bi->nick, u, USERHOST_MASK_TOO_WIDE, mask);
				return MOD_CONT;
			}

			/**
			* Changed sprintf() to snprintf()and increased the size of
			* breason to match bufsize
			* -Rob
			**/
			if (AddAkiller) {
				snprintf(breason, sizeof(breason), "[%s] %s", u->nick, reason);
				reason = sstrdup(breason);
			}

			deleted = add_akill(u, mask, u->nick, expires, reason);
			if (deleted < 0) {
				return MOD_CONT;
			} else if (deleted) {
				notice_lang(ci->bi->nick, u, OPER_AKILL_DELETED_SEVERAL, deleted);
			}
			notice_lang(ci->bi->nick, u, OPER_AKILL_ADDED, mask);

			alog("[bs_fantasy_ext] %s added an AKILL for %s (%s).", u->nick, mask, reason);

			if (WallOSAkill) {
				char buf[128];

				if (!expires) {
					strcpy(buf, "does not expire");
				} else {
					int wall_expiry = expires - time(NULL);
					char *s = NULL;

					if (wall_expiry >= 86400) {
						wall_expiry /= 86400;
						s = "day";
					} else if (wall_expiry >= 3600) {
						wall_expiry /= 3600;
						s = "hour";
					} else if (wall_expiry >= 60) {
						wall_expiry /= 60;
						s = "minute";
					}

					snprintf(buf, sizeof(buf), "expires in %d %s%s", wall_expiry, s, (wall_expiry == 1) ? "" : "s");
				}

				anope_cmd_global(ci->bi->nick, "%s added an AKILL for %s (%s) (%s)", u->nick, mask, reason, buf);
			}

			if (readonly) {
				notice_lang(ci->bi->nick, u, READ_ONLY_MODE);
			}
			/* memleak fixed in the core, forgot it here - Thx Trystan */
            if (AddAkiller) {
                free(reason);
            }
		} else {
			syntax_error(ci->bi->nick, u, "AKILL", OPER_AKILL_SYNTAX);
		}

	} else if (!stricmp(cmd, "DEL")) {
		int res = 0;

		if (!mask) {
			syntax_error(ci->bi->nick, u, "AKILL", OPER_AKILL_SYNTAX);
			return MOD_CONT;
		}

		if (akills.count == 0) {
			notice_lang(ci->bi->nick, u, OPER_AKILL_LIST_EMPTY);
			return MOD_CONT;
		}

		if (isdigit(*mask) && strspn(mask, "1234567890,-") == strlen(mask)) {
			/* Deleting a range */
			res = slist_delete_range(&akills, mask, NULL);
			if (res == 0) {
				notice_lang(ci->bi->nick, u, OPER_AKILL_NO_MATCH);
				return MOD_CONT;
			} else if (res == 1) {
				notice_lang(ci->bi->nick, u, OPER_AKILL_DELETED_ONE);
			} else {
				notice_lang(ci->bi->nick, u, OPER_AKILL_DELETED_SEVERAL,res);
			}

			alog("[bs_fantasy_ext] %s deleted %d AKILLs.", u->nick, res);
		} else {
			if ((res = slist_indexof(&akills, mask)) == -1) {
				notice_lang(ci->bi->nick, u, OPER_AKILL_NOT_FOUND, mask);
				return MOD_CONT;
			}

			slist_delete(&akills, res);
			notice_lang(ci->bi->nick, u, OPER_AKILL_DELETED, mask);

			alog("[bs_fantasy_ext] %s deleted an AKILL for %s.", u->nick, mask);
		}

		if (readonly)
			notice_lang(ci->bi->nick, u, READ_ONLY_MODE);

	} else if (!stricmp(cmd, "LIST")) {
		int res, sent_header = 0;

		if (akills.count == 0) {
			notice_lang(ci->bi->nick, u, OPER_AKILL_LIST_EMPTY);
			alog("[bs_fantasy_ext] %s requested the AKILL list.", u->nick);
			return MOD_CONT;
		}

		if (!mask || (isdigit(*mask) && strspn(mask, "1234567890,-") == strlen(mask))) {
			res = slist_enum(&akills, mask, &akill_list_callback, u, ci, &sent_header);
			if (res == 0) {
				notice_lang(ci->bi->nick, u, OPER_AKILL_NO_MATCH);
				alog("[bs_fantasy_ext] %s requested the AKILL list entrie(s) %s.", u->nick, mask);
				return MOD_CONT;
			} else {
				notice_lang(ci->bi->nick, u, END_OF_ANY_LIST, "Akill");
			}
		} else {
			int i;
			char amask[BUFSIZE];

			for (i = 0; i < akills.count; i++) {
				snprintf(amask, sizeof(amask), "%s@%s", ((Akill *) akills.list[i])->user, ((Akill *) akills.list[i])->host);
				if (!stricmp(mask, amask) || my_match_wild_nocase(mask, amask))
					akill_list(i + 1, akills.list[i], u, &sent_header, ci);
			}

			if (!sent_header)
				notice_lang(ci->bi->nick, u, OPER_AKILL_NO_MATCH);
			else {
				notice_lang(ci->bi->nick, u, END_OF_ANY_LIST, "Akill");
			}
		}

		alog("[bs_fantasy_ext] %s requested the AKILL list.", u->nick);
	} else if (!stricmp(cmd, "VIEW")) {
		int res, sent_header = 0;

		if (akills.count == 0) {
			notice_lang(ci->bi->nick, u, OPER_AKILL_LIST_EMPTY);
			return MOD_CONT;
		}

		if (!mask || (isdigit(*mask) && strspn(mask, "1234567890,-") == strlen(mask))) {
			res = slist_enum(&akills, mask, &akill_view_callback, u, ci, &sent_header);
		if (res == 0) {
			notice_lang(ci->bi->nick, u, OPER_AKILL_NO_MATCH);
			return MOD_CONT;
			}
		} else {
			int i;
			char amask[BUFSIZE];

			for (i = 0; i < akills.count; i++) {
				snprintf(amask, sizeof(amask), "%s@%s", ((Akill *) akills.list[i])->user, ((Akill *) akills.list[i])->host);
				if (!stricmp(mask, amask) || my_match_wild_nocase(mask, amask))
					akill_view(i + 1, akills.list[i], u, &sent_header, ci);
		}

		if (!sent_header)
			notice_lang(ci->bi->nick, u, OPER_AKILL_NO_MATCH);
		}
	} else if (!stricmp(cmd, "CLEAR")) {
		slist_clear(&akills, 1);
		notice_lang(ci->bi->nick, u, OPER_AKILL_CLEAR);

		alog("[bs_fantasy_ext] %s cleared the AKILL list.", u->nick);
	} else {
		syntax_error(ci->bi->nick, u, "AKILL", OPER_AKILL_SYNTAX);
	}
	return MOD_CONT;
}

/* Lists an AKILL entry, prefixing it with the header if needed */
static int akill_list(int number, Akill * ak, User * u, int *sent_header, ChannelInfo *ci) {
	char mask[BUFSIZE];

	if (!ak)
	return 0;

	if (!*sent_header) {
		notice_lang(ci->bi->nick, u, OPER_AKILL_LIST_HEADER);
		*sent_header = 1;
	}

	snprintf(mask, sizeof(mask), "%s@%s", ak->user, ak->host);
	notice_lang(ci->bi->nick, u, OPER_AKILL_LIST_FORMAT, number, mask, ak->reason);

	return 1;
}

static int akill_view(int number, Akill * ak, User * u, int *sent_header, ChannelInfo *ci) {
	char mask[BUFSIZE];
	char timebuf[32], expirebuf[256];
	struct tm tm;

	if (!ak)
	return 0;

	if (!*sent_header) {
		notice_lang(ci->bi->nick, u, OPER_AKILL_VIEW_HEADER);
		*sent_header = 1;
	}

	snprintf(mask, sizeof(mask), "%s@%s", ak->user, ak->host);
	tm = *localtime(&ak->seton);
	strftime_lang(timebuf, sizeof(timebuf), u, STRFTIME_SHORT_DATE_FORMAT, &tm);
	expire_left(u->na, expirebuf, sizeof(expirebuf), ak->expires);
	notice_lang(ci->bi->nick, u, OPER_AKILL_VIEW_FORMAT, number, mask, ak->by, timebuf, expirebuf, ak->reason);

	return 1;
}

/* Lists an AKILL entry, prefixing it with the header if needed */
static int akill_list_callback(SList * slist, int number, void *item, va_list args) {
	User *u = va_arg(args, User *);
	ChannelInfo *ci = va_arg(args, ChannelInfo *);
	int *sent_header = va_arg(args, int *);

	return akill_list(number, item, u, sent_header, ci);
}

/* Callback for enumeration purposes */
static int akill_view_callback(SList * slist, int number, void *item, va_list args) {
	User *u = va_arg(args, User *);
	ChannelInfo *ci = va_arg(args, ChannelInfo *);
	int *sent_header = va_arg(args, int *);

	return akill_view(number, item, u, sent_header, ci);
}
#endif

/* EOF */

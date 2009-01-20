/**
 * Methods to modify and access the Services ingore list. - Source
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
 * Last Updated   : 27/02/2007
 *
 **/

#ifdef ENABLE_IGNORE
#include "ignore.h"

int do_ignoreuser(User * u, Channel *c, char *cmd, char *nick, char *time) {
	ChannelInfo *ci = c->ci;
	int t;

	if (!cmd) {
		notice_lang(s_OperServ, u, OPER_IGNORE_SYNTAX);
		return MOD_CONT;
	}

	if (!stricmp(cmd, "ADD")) {
		if (!nick) {
			notice_lang(ci->bi->nick, u, OPER_IGNORE_SYNTAX);
			return MOD_CONT;
		} else if (!time) {
			notice_lang(ci->bi->nick, u, OPER_IGNORE_SYNTAX);
			return MOD_CONT;
		} else {
			t = dotime(time);

			if (t <= -1) {
				notice_lang(ci->bi->nick, u, OPER_IGNORE_VALID_TIME);
				return MOD_CONT;
			} else if (t == 0) {
				t = 157248000;  /* if 0 is given, we set time to 157248000 seconds == 5 years (let's hope the next restart will  be before that time ;-)) */
				add_ignore(nick, t);
				notice_lang(ci->bi->nick, u, OPER_IGNORE_PERM_DONE, nick);
				alog("[bs_fantasy_ext] %s added %s permanently to the ignore list.", u->nick, nick);
			} else {
				add_ignore(nick, t);
				notice_lang(ci->bi->nick, u, OPER_IGNORE_TIME_DONE, nick, time);
				alog("[bs_fantasy_ext] %s added %s to the ignore list for %d seconds.", u->nick, nick, t);
			}
		}
	} else if (!stricmp(cmd, "LIST")) {
			do_ignorelist(u, ci);
			alog("[bs_fantasy_ext] %s requested the ignore list.", u->nick);
	}

	else if (!stricmp(cmd, "DEL")) {
		if (!nick) {
			notice_lang(ci->bi->nick, u, OPER_IGNORE_SYNTAX);
		} else {
			if (get_ignore(nick) == 0) {
				notice_lang(ci->bi->nick, u, OPER_IGNORE_LIST_NOMATCH, nick);
				return MOD_CONT;
			} else {
				delete_ignore(nick);
				notice_lang(ci->bi->nick, u, OPER_IGNORE_DEL_DONE, nick);
				alog("[bs_fantasy_ext] %s deleted %s from the ignore list.", u->nick, nick);
			}
		}
	} else if (!stricmp(cmd, "CLEAR")) {
		do_clearignore(u, ci);
		alog("[bs_fantasy_ext] %s cleared the ignore list.", u->nick);

	} else
		notice_lang(ci->bi->nick, u, OPER_IGNORE_SYNTAX);
	return MOD_CONT;
}

/* shows the Services ignore list */
static int do_ignorelist(User * u, ChannelInfo *ci) {
	int sent_header = 0;
	IgnoreData *id;
	int i;

	for (i = 0; i < 256; i++) {
		for (id = ignore[i]; id; id = id->next) {
			if (!sent_header) {
				notice_lang(ci->bi->nick, u, OPER_IGNORE_LIST);
				sent_header = 1;
			}
			notice_user(ci->bi->nick, u, "%s", id->who);
		}
	}
	if (!sent_header)
		notice_lang(ci->bi->nick, u, OPER_IGNORE_LIST_EMPTY);
	return MOD_CONT;
}

/* deletes a nick from the ignore list  */
static void delete_ignore(const char *nick) {
	IgnoreData *ign, *prev;
	IgnoreData **whichlist;

	if (!nick || !*nick) {
		return;
	}

	whichlist = &ignore[tolower(nick[0])];

	for (ign = *whichlist, prev = NULL; ign; prev = ign, ign = ign->next) {
		if (stricmp(ign->who, nick) == 0)
			break;
	}
	/* If the ignore was not found, bail out -GD */
	if (!ign)
		return;
	if (prev)
		prev->next = ign->next;
	else
		*whichlist = ign->next;
	free(ign);
	ign = NULL;
}

/* Cleares the Services ignore list */
static int do_clearignore(User * u, ChannelInfo *ci) {
	IgnoreData *id = NULL, *next = NULL;
	int i;
	for (i = 0; i < 256; i++) {
		for (id = ignore[i]; id; id = next) {
			next = id->next;
			free(id);
			if (!next) {
				ignore[i] = NULL;
			}
		}
	}
	notice_lang(ci->bi->nick, u, OPER_IGNORE_LIST_CLEARED);
	return MOD_CONT;
}
#endif

/* EOF */

/**
 * Methods to modify and access the BotServ badwords list. - Source
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

#ifdef ENABLE_BADWORDS
#include "badwords.h"

int do_badwords(User * u, Channel *c, char *cmd, char *word, char *style) {
	ChannelInfo *ci = c->ci;
	BadWord *bw;

	int i;
	int need_args = (cmd && (!stricmp(cmd, "LIST") || !stricmp(cmd, "CLEAR")));

	if (!cmd || (need_args ? 0 : !word)) {
		moduleNoticeLang(ci->bi->nick, u, LANG_BADWORDS_SYNTAX, BSFantasyCharacter);
	} else if (!check_access(u, ci, CA_BADWORDS) && (!need_args || !is_services_admin(u))) {
		notice_lang(c->ci->bi->nick, u, ACCESS_DENIED);
	} else if (stricmp(cmd, "ADD") == 0) {
		int type = BW_ANY;

		if (readonly) {
			notice_lang(c->ci->bi->nick, u, BOT_BADWORDS_DISABLED);
			return MOD_CONT;
		}

		if (style) {
			if (!stricmp(style, "SINGLE"))
				type = BW_SINGLE;
			else if (!stricmp(style, "START"))
				type = BW_START;
			else if (!stricmp(style, "END"))
				type = BW_END;
		}

		for (bw = ci->badwords, i = 0; i < ci->bwcount; bw++, i++) {
			if (bw->word && ((BSCaseSensitive && (!strcmp(bw->word, word))) || (!BSCaseSensitive
				&& (!stricmp(bw->word, word))))) {
				notice_lang(c->ci->bi->nick, u, BOT_BADWORDS_ALREADY_EXISTS, bw->word, ci->name);
				return MOD_CONT;
			}
		}

		for (i = 0; i < ci->bwcount; i++) {
			if (!ci->badwords[i].in_use)
				break;
		}
		if (i == ci->bwcount) {
			if (i < BSBadWordsMax) {
				ci->bwcount++;
				ci->badwords = srealloc(ci->badwords, sizeof(BadWord) * ci->bwcount);
			} else {
				notice_lang(c->ci->bi->nick, u, BOT_BADWORDS_REACHED_LIMIT, BSBadWordsMax);
				return MOD_CONT;
			}
		}
		bw = &ci->badwords[i];
		bw->in_use = 1;
		bw->word = sstrdup(word);
		bw->type = type;

		notice_lang(c->ci->bi->nick, u, BOT_BADWORDS_ADDED, bw->word, ci->name);

	} else if (stricmp(cmd, "DEL") == 0) {
		int deleted = 0, a, b;

		if (readonly) {
			notice_lang(c->ci->bi->nick, u, BOT_BADWORDS_DISABLED);
			return MOD_CONT;
		}

		/* Special case: is it a number/list?  Only do search if it isn't. */
		if (isdigit(*word) && strspn(word, "1234567890,-") == strlen(word)) {
			int count, last = -1;
			deleted = process_numlist(word, &count, badwords_del_callback, u, ci, &last);
			if (!deleted) {
				if (count == 1) {
					notice_lang(c->ci->bi->nick, u, BOT_BADWORDS_NO_SUCH_ENTRY, last, ci->name);
				} else {
					notice_lang(c->ci->bi->nick, u, BOT_BADWORDS_NO_MATCH, ci->name);
				}
			} else if (deleted == 1) {
				notice_lang(c->ci->bi->nick, u, BOT_BADWORDS_DELETED_ONE, ci->name);
			} else {
				notice_lang(c->ci->bi->nick, u, BOT_BADWORDS_DELETED_SEVERAL, deleted, ci->name);
			}
		} else {
			for (i = 0; i < ci->bwcount; i++) {
				if (ci->badwords[i].in_use && !stricmp(ci->badwords[i].word, word))
					break;
			}
			if (i == ci->bwcount) {
				notice_lang(c->ci->bi->nick, u, BOT_BADWORDS_NOT_FOUND, word, ci->name);
				return MOD_CONT;
			}
			bw = &ci->badwords[i];
			notice_lang(c->ci->bi->nick, u, BOT_BADWORDS_DELETED, bw->word, ci->name);
			if (bw->word)
				free(bw->word);
			bw->word = NULL;
			bw->in_use = 0;
			deleted = 1;
		}

		if (deleted) {
			/* Reordering - DrStein */
			for (b = 0; b < ci->bwcount; b++) {
				if (ci->badwords[b].in_use) {
					for (a = 0; a < ci->bwcount; a++) {
						if (a > b)
							break;
						if (!(ci->badwords[a].in_use)) {
							ci->badwords[a].in_use = ci->badwords[b].in_use;
							ci->badwords[a].type = ci->badwords[b].type;
							if (ci->badwords[b].word) {
								ci->badwords[a].word = sstrdup(ci->badwords[b].word);
								free(ci->badwords[b].word);
							}
							ci->badwords[b].word = NULL;
							ci->badwords[b].in_use = 0;
							break;
						}
					}
				}
			}
			/* After reordering only the entries at the end could still be empty.
			 * We ll free the places no longer in use... - Viper */
			for (i = ci->bwcount - 1; i >= 0; i--) {
				if (ci->badwords[i].in_use)
					break;
				ci->bwcount--;
			}
			ci->badwords = srealloc(ci->badwords,sizeof(BadWord) * ci->bwcount);
		}

	} else if (stricmp(cmd, "LIST") == 0) {
		int sent_header = 0;

		if (ci->bwcount == 0) {
			notice_lang(c->ci->bi->nick, u, BOT_BADWORDS_LIST_EMPTY, ci->name);
			return MOD_CONT;
		}
		if (word && strspn(word, "1234567890,-") == strlen(word)) {
			process_numlist(word, NULL, badwords_list_callback, u, ci, &sent_header);
		} else {
			for (i = 0; i < ci->bwcount; i++) {
				if (!(ci->badwords[i].in_use))
					continue;
				if (word && ci->badwords[i].word && !my_match_wild_nocase(word, ci->badwords[i].word))
					continue;
				badwords_list(u, i, ci, &sent_header);
			}
		}
		if (!sent_header)
			notice_lang(c->ci->bi->nick, u, BOT_BADWORDS_NO_MATCH, ci->name);

    } else if (stricmp(cmd, "CLEAR") == 0) {
		if (readonly) {
			notice_lang(c->ci->bi->nick, u, BOT_BADWORDS_DISABLED);
			return MOD_CONT;
		}

		for (i = 0; i < ci->bwcount; i++)
			if (ci->badwords[i].word)
				free(ci->badwords[i].word);

		free(ci->badwords);
		ci->badwords = NULL;
		ci->bwcount = 0;

		notice_lang(c->ci->bi->nick, u, BOT_BADWORDS_CLEAR);

	} else {
		moduleNoticeLang(ci->bi->nick, u, LANG_BADWORDS_SYNTAX, BSFantasyCharacter);
	}
	return MOD_CONT;
}

static int badwords_del_callback(User * u, int num, va_list args) {
	BadWord *bw;
	ChannelInfo *ci = va_arg(args, ChannelInfo *);
	int *last = va_arg(args, int *);
	*last = num;

	if (num < 1 || num > ci->bwcount)
		return 0;

	bw = &ci->badwords[num - 1];
	if (bw->word)
		free(bw->word);
	bw->word = NULL;
	bw->in_use = 0;

	return 1;
}

static int badwords_list(User * u, int index, ChannelInfo * ci, int *sent_header) {
	BadWord *bw = &ci->badwords[index];

	if (!bw->in_use)
		return 0;
	if (!*sent_header) {
		notice_lang(ci->bi->nick, u, BOT_BADWORDS_LIST_HEADER, ci->name);
		*sent_header = 1;
	}

	notice_lang(ci->bi->nick, u, BOT_BADWORDS_LIST_FORMAT, index + 1, bw->word, ((bw->type == BW_SINGLE) ? "(SINGLE)" :
		((bw->type == BW_START) ? "(START)" : ((bw->type == BW_END) ? "(END)" :
		((bw->type == BW_ANY) ? "(ANY)" : ""))) ));
	return 1;
}

static int badwords_list_callback(User * u, int num, va_list args) {
	ChannelInfo *ci = va_arg(args, ChannelInfo *);
	int *sent_header = va_arg(args, int *);
	if (num < 1 || num > ci->bwcount)
		return 0;
	return badwords_list(u, num - 1, ci, sent_header);
}
#endif

/* EOF */


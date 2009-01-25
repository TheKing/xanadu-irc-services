/**
 * Main routines for the fantasy shun and tshun commands - Source
 *
 ***********
 * Module Name    : bs_fantasy_ext
 * Author         : Viper <Viper@Absurd-IRC.net>
 * Creation Date  : 06/02/2008
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
 * Last Updated   : 03/04/2008
 *
 **/

#include "shun.h"

#ifdef ENABLE_SHUN
/**
 * We handle SHUN
 **/
int do_shun(User *u, Channel *c, char *arg) {
	ChannelInfo *ci = c->ci;
	User *u2;
	char breason[BUFSIZE];
	char *cmd, *target, *expiry = NULL, *reason = NULL, *temp;
	int index = 0, action = 0;
	time_t expires = 0;

	cmd = myStrGetToken(arg, ' ', index);

	if (!stricmp(cmd, "ADD")) {
		action = 0;
		index++;
	} else if (!stricmp(cmd, "DEL")) {
		action = 1;
		index++;
	}

	target = myStrGetToken(arg, ' ', index++);

	if (!action) {
		temp = myStrGetToken(arg, ' ', index);

		if (temp && *temp == '+') {
			expiry = temp;
			reason = myStrGetTokenRemainder(arg, ' ', ++index);
		} else {
			expiry = NULL;
			free(temp);
			reason = myStrGetTokenRemainder(arg, ' ', index);
		}

		expires = expiry ? dotime(expiry) : SHUNExpiry;
		/* If the expiry given does not contain a final letter, it's in days. */
		if (expiry && isdigit(expiry[strlen(expiry) - 1]))
			expires *= 86400;

		if (!reason) {
			char buf[BUFSIZE];
			snprintf(buf, BUFSIZE, "Shunned by %s", u->nick);
			reason = sstrdup(buf);
		} else {
			if (AddAkiller) {
				snprintf(breason, sizeof(breason), "[%s] %s", u->nick, reason);
				free(reason);
				reason = sstrdup(breason);
			}
		}
	}

	if (!target)
		moduleNoticeLang(ci->bi->nick, u, LANG_SHUN_SYNTAX);
	/* Do not allow less than a minute expiry time */
	else if (expires != 0 && expires < 60)
		notice_lang(ci->bi->nick, u, BAD_EXPIRY_TIME);
	else {
		if ((u2 = finduser(target))) {
			if (!action) {
				send_cmd(NULL, "TKL + s * %s %s %ld %ld :%s", u2->host, u->nick,
						(long int) time(NULL) + expires, time(NULL), reason);
				moduleNoticeLang(ci->bi->nick, u, LANG_SHUN_ADDED, "*", u2->host);

				if (WallOSAkill) {
					char buf[128];

					if (!expires) {
						strcpy(buf, "Does not expire");
					} else {
						int wall_expiry = expires;
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

						snprintf(buf, sizeof(buf), "expires in %d %s%s", wall_expiry, s ? s : "",
								(wall_expiry == 1) ? "" : "s");
					}

					xanadu_cmd_global(ci->bi->nick, "%s added a SHUN on %s [*@%s] (%s) (%s)",
									 u->nick, u2->nick, u2->host, reason, buf);
				}

			} else {
				send_cmd(NULL, "TKL - s * %s %s", u2->host, u->nick);
				moduleNoticeLang(ci->bi->nick, u, LANG_SHUN_DELETION, "*", u2->host);
				xanadu_cmd_global(ci->bi->nick, "%s removed SHUN on %s [*@%s] ", u->nick, u2->nick,
						u2->host);
			}
		} else if (my_match_wild_nocase("*@*", target) && !my_match_wild_nocase("*!*@*", target)) {
			char *uname, *host;

			uname = myStrGetToken(target, '@', 0);
			host = myStrGetToken(target, '@', 1);

			if (!action) {
				send_cmd(NULL, "TKL + s %s %s %s %ld %ld :%s", uname, host, u->nick,
						(long int) time(NULL) + expires, time(NULL), reason);
				moduleNoticeLang(ci->bi->nick, u, LANG_SHUN_ADDED, uname, host);

				if (WallOSAkill) {
					char buf[128];

					if (!expires) {
						strcpy(buf, "Does not expire");
					} else {
						int wall_expiry = expires;
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

						snprintf(buf, sizeof(buf), "expires in %d %s%s", wall_expiry, s ? s : "",
								(wall_expiry == 1) ? "" : "s");
					}

					xanadu_cmd_global(ci->bi->nick, "%s added a SHUN on %s@%s (%s) (%s)",
									 u->nick, uname, host, reason, buf);
				}

			} else {
				send_cmd(NULL, "TKL - s %s %s %s", uname, host, u->nick);
				moduleNoticeLang(ci->bi->nick, u, LANG_SHUN_DELETION, uname, host);
				xanadu_cmd_global(ci->bi->nick, "%s removed SHUN on %s@%s ", u->nick, uname, host);
			}

			free(uname);
			free(host);
		} else
			moduleNoticeLang(ci->bi->nick, u, LANG_SHUN_SYNTAX);
	}

	if (cmd) free(cmd);
	if (target) free(target);
	if (expiry) free(expiry);
	if (reason) free(reason);

	return MOD_CONT;
}
#endif

#ifdef ENABLE_TSHUN
/**
 * We handle TSHUN
 **/
int do_tshun(User *u, Channel *c, char *arg) {
	ChannelInfo *ci = c->ci;
	User *u2;
	char breason[BUFSIZE];
	char *cmd, *target, *reason = NULL;
	int index = 0, action = 0;

	cmd = myStrGetToken(arg, ' ', 0);

	if (!stricmp(cmd, "ADD")) {
		action = 0;
		index++;
	} else if (!stricmp(cmd, "DEL")) {
		action = 1;
		index++;
	}

	target = myStrGetToken(arg, ' ', index++);

	if (!action) {
		reason = myStrGetTokenRemainder(arg, ' ', index++);

		if (!reason) {
			char buf[BUFSIZE];
			snprintf(buf, BUFSIZE, "Temporarily Shunned by %s", u->nick);
			reason = sstrdup(buf);
		} else {
			if (AddAkiller) {
				snprintf(breason, sizeof(breason), "[%s] %s", u->nick, reason);
				free(reason);
				reason = sstrdup(breason);
			}
		}
	}

	if (!target)
		moduleNoticeLang(ci->bi->nick, u, LANG_TSHUN_SYNTAX);
	else if (!(u2 = finduser(target)))
		moduleNoticeLang(ci->bi->nick, u, LANG_TSHUN_TARGET_NEXIST, target);
	else if (is_oper(u2))
		moduleNoticeLang(ci->bi->nick, u, LANG_TSHUN_TARGET_OPER);
	else {
		if (!action) {
			send_cmd(NULL, "TEMPSHUN +%s %s", u2->nick, reason);
			moduleNoticeLang(ci->bi->nick, u, LANG_TSHUN_ADDED, u2->nick);
			xanadu_cmd_global(ci->bi->nick, "%s added a TEMPSHUN on %s (%s)", u->nick, u2->nick, reason);
		} else {
			send_cmd(NULL, "TEMPSHUN -%s", u2->nick);
			moduleNoticeLang(ci->bi->nick, u, LANG_TSHUN_DELETION, u2->nick);
			xanadu_cmd_global(ci->bi->nick, "%s removed TEMPSHUN on %s", u->nick, u2->nick);
		}
	}

	if (cmd) free(cmd);
	if (target) free(target);
	if (reason) free(reason);

	return MOD_CONT;
}
#endif

 /* EOF */

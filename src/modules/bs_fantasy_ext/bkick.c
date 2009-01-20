/**
 * Methods to modify the BotServ kick settings. - Source
 *
 ***********
 * Module Name    : bs_fantasy_ext
 * Author         : Viper <Viper@Absurd-IRC.net>
 * Creation Date: 21/07/2006
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
 * Last Updated: 22/07/2006
 *
 **/

#ifdef ENABLE_BKICK
#include "bkick.h"

int do_set_kick(User * u, ChannelInfo * ci, char *option, char *value, char *ttb, char *param1, char *param2) {
	if (readonly)
		notice_lang(s_BotServ, u, BOT_KICK_DISABLED);

	else if (!option || !value)
		syntax_error(ci->bi->nick, u, "KICK", BOT_KICK_SYNTAX);
	else if (stricmp(value, "ON") && stricmp(value, "OFF"))
		syntax_error(ci->bi->nick, u, "KICK", BOT_KICK_SYNTAX);
	else if (ci->flags & CI_VERBOTEN)
		notice_lang(ci->bi->nick, u, CHAN_X_FORBIDDEN, ci->name);
	else if (!is_services_admin(u) && !check_access(u, ci, CA_SET))
		notice_lang(ci->bi->nick, u, ACCESS_DENIED);
	else if (!ci->bi)
		notice_lang(s_BotServ, u, BOT_DOES_NOT_EXIST, ci->name);
	else {
		if (!stricmp(option, "BADWORDS")) {
			if (!stricmp(value, "ON")) {
				if (ttb) {
					ci->ttb[TTB_BADWORDS] = strtol(ttb, (char **) NULL, 10);
					/* Only error if errno returns ERANGE or EINVAL or we are less then 0 - TSL */
					if (errno == ERANGE || errno == EINVAL || ci->ttb[TTB_BADWORDS] < 0) {
						/* leaving the debug behind since we might want to know what these are */
						if (debug) {
							alog("debug: errno is %d ERANGE %d EINVAL %d ttb %d", errno, ERANGE, EINVAL,
							ci->ttb[TTB_BADWORDS]);
						}
						/* reset the value back to 0 - TSL */
						ci->ttb[TTB_BADWORDS] = 0;
						notice_lang(ci->bi->nick, u, BOT_KICK_BAD_TTB, ttb);
						return MOD_CONT;
					}
				} else {
					ci->ttb[TTB_BADWORDS] = 0;
				}
				ci->botflags |= BS_KICK_BADWORDS;
				if (ci->ttb[TTB_BADWORDS])
					notice_lang(ci->bi->nick, u, BOT_KICK_BADWORDS_ON_BAN, ci->ttb[TTB_BADWORDS]);
				else
					notice_lang(ci->bi->nick, u, BOT_KICK_BADWORDS_ON);
			} else {
				ci->botflags &= ~BS_KICK_BADWORDS;
				notice_lang(ci->bi->nick, u, BOT_KICK_BADWORDS_OFF);
			}
		} else if (!stricmp(option, "BOLDS")) {
			if (!stricmp(value, "ON")) {
				if (ttb) {
					ci->ttb[TTB_BOLDS] = strtol(ttb, (char **) NULL, 10);
					if (errno == ERANGE || errno == EINVAL || ci->ttb[TTB_BOLDS] < 0) {
						if (debug) {
							alog("debug: errno is %d ERANGE %d EINVAL %d ttb %d", errno, ERANGE, EINVAL,
							ci->ttb[TTB_BOLDS]);
						}
						ci->ttb[TTB_BOLDS] = 0;
						notice_lang(ci->bi->nick, u, BOT_KICK_BAD_TTB, ttb);
						return MOD_CONT;
					}
				} else
					ci->ttb[TTB_BOLDS] = 0;
				ci->botflags |= BS_KICK_BOLDS;
				if (ci->ttb[TTB_BOLDS])
					notice_lang(ci->bi->nick, u, BOT_KICK_BOLDS_ON_BAN, ci->ttb[TTB_BOLDS]);
				else
					notice_lang(ci->bi->nick, u, BOT_KICK_BOLDS_ON);
			} else {
				ci->botflags &= ~BS_KICK_BOLDS;
				notice_lang(ci->bi->nick, u, BOT_KICK_BOLDS_OFF);
			}
		} else if (!stricmp(option, "CAPS")) {
			if (!stricmp(value, "ON")) {
				char *min = param1;
				char *percent = param2;

				if (ttb) {
					ci->ttb[TTB_CAPS] = strtol(ttb, (char **) NULL, 10);
					if (errno == ERANGE || errno == EINVAL || ci->ttb[TTB_CAPS] < 0) {
						if (debug) {
							alog("debug: errno is %d ERANGE %d EINVAL %d ttb %d", errno, ERANGE, EINVAL,
							ci->ttb[TTB_CAPS]);
						}
						ci->ttb[TTB_CAPS] = 0;
						notice_lang(ci->bi->nick, u, BOT_KICK_BAD_TTB, ttb);
						return MOD_CONT;
					}
				} else
					ci->ttb[TTB_CAPS] = 0;

				if (!min)
					ci->capsmin = 10;
				else
					ci->capsmin = atol(min);
				if (ci->capsmin < 1)
					ci->capsmin = 10;

				if (!percent)
					ci->capspercent = 25;
				else
					ci->capspercent = atol(percent);
				if (ci->capspercent < 1 || ci->capspercent > 100)
					ci->capspercent = 25;

				ci->botflags |= BS_KICK_CAPS;
				if (ci->ttb[TTB_CAPS])
					notice_lang(ci->bi->nick, u, BOT_KICK_CAPS_ON_BAN, ci->capsmin, ci->capspercent,
					ci->ttb[TTB_CAPS]);
				else
					notice_lang(ci->bi->nick, u, BOT_KICK_CAPS_ON, ci->capsmin, ci->capspercent);
			} else {
				ci->botflags &= ~BS_KICK_CAPS;
				notice_lang(ci->bi->nick, u, BOT_KICK_CAPS_OFF);
			}
		} else if (!stricmp(option, "COLORS")) {
			if (!stricmp(value, "ON")) {
				if (ttb) {
					ci->ttb[TTB_COLORS] = strtol(ttb, (char **) NULL, 10);
					if (errno == ERANGE || errno == EINVAL || ci->ttb[TTB_COLORS] < 0) {
						if (debug) {
						alog("debug: errno is %d ERANGE %d EINVAL %d ttb %d", errno, ERANGE, EINVAL,
						ci->ttb[TTB_COLORS]);
						}
						ci->ttb[TTB_COLORS] = 0;
						notice_lang(ci->bi->nick, u, BOT_KICK_BAD_TTB, ttb);
						return MOD_CONT;
					}
				} else
					ci->ttb[TTB_COLORS] = 0;
				ci->botflags |= BS_KICK_COLORS;
				if (ci->ttb[TTB_COLORS])
					notice_lang(ci->bi->nick, u, BOT_KICK_COLORS_ON_BAN, ci->ttb[TTB_COLORS]);
				else
					notice_lang(ci->bi->nick, u, BOT_KICK_COLORS_ON);
			} else {
				ci->botflags &= ~BS_KICK_COLORS;
				notice_lang(s_BotServ, u, BOT_KICK_COLORS_OFF);
			}
		} else if (!stricmp(option, "FLOOD")) {
			if (!stricmp(value, "ON")) {
				char *lines = param1;
				char *secs = param2;

				if (ttb) {
					ci->ttb[TTB_FLOOD] = strtol(ttb, (char **) NULL, 10);
					if (errno == ERANGE || errno == EINVAL || ci->ttb[TTB_FLOOD] < 0) {
						if (debug) {
							alog("debug: errno is %d ERANGE %d EINVAL %d ttb %d", errno, ERANGE, EINVAL,
							ci->ttb[TTB_FLOOD]);
						}
						ci->ttb[TTB_FLOOD] = 0;
						notice_lang(ci->bi->nick, u, BOT_KICK_BAD_TTB, ttb);
						return MOD_CONT;
					}
				} else
					ci->ttb[TTB_FLOOD] = 0;

				if (!lines)
					ci->floodlines = 6;
				else
					ci->floodlines = atol(lines);
				if (ci->floodlines < 2)
					ci->floodlines = 6;

				if (!secs)
					ci->floodsecs = 10;
				else
					ci->floodsecs = atol(secs);
				if (ci->floodsecs < 1 || ci->floodsecs > BSKeepData)
					ci->floodsecs = 10;

				ci->botflags |= BS_KICK_FLOOD;
				if (ci->ttb[TTB_FLOOD])
					notice_lang(ci->bi->nick, u, BOT_KICK_FLOOD_ON_BAN, ci->floodlines, ci->floodsecs,
					ci->ttb[TTB_FLOOD]);
				else
					notice_lang(ci->bi->nick, u, BOT_KICK_FLOOD_ON, ci->floodlines, ci->floodsecs);
			} else {
				ci->botflags &= ~BS_KICK_FLOOD;
				notice_lang(ci->bi->nick, u, BOT_KICK_FLOOD_OFF);
			}
		} else if (!stricmp(option, "REPEAT")) {
			if (!stricmp(value, "ON")) {
				char *times = param1;

				if (ttb) {
					ci->ttb[TTB_REPEAT] = strtol(ttb, (char **) NULL, 10);
					if (errno == ERANGE || errno == EINVAL || ci->ttb[TTB_REPEAT] < 0) {
						if (debug) {
							alog("debug: errno is %d ERANGE %d EINVAL %d ttb %d", errno, ERANGE, EINVAL,
							ci->ttb[TTB_REPEAT]);
						}
						ci->ttb[TTB_REPEAT] = 0;
						notice_lang(ci->bi->nick, u, BOT_KICK_BAD_TTB, ttb);
						return MOD_CONT;
					}
				} else
					ci->ttb[TTB_REPEAT] = 0;

				if (!times)
					ci->repeattimes = 3;
				else
					ci->repeattimes = atol(times);
				if (ci->repeattimes < 2)
					ci->repeattimes = 3;

				ci->botflags |= BS_KICK_REPEAT;
				if (ci->ttb[TTB_REPEAT])
					notice_lang(ci->bi->nick, u, BOT_KICK_REPEAT_ON_BAN, ci->repeattimes, ci->ttb[TTB_REPEAT]);
				else
					notice_lang(ci->bi->nick, u, BOT_KICK_REPEAT_ON, ci->repeattimes);
			} else {
				ci->botflags &= ~BS_KICK_REPEAT;
				notice_lang(s_BotServ, u, BOT_KICK_REPEAT_OFF);
			}
		} else if (!stricmp(option, "REVERSES")) {
			if (!stricmp(value, "ON")) {
				if (ttb) {
					ci->ttb[TTB_REVERSES] = strtol(ttb, (char **) NULL, 10);
					if (errno == ERANGE || errno == EINVAL || ci->ttb[TTB_REVERSES] < 0) {
						if (debug) {
							alog("debug: errno is %d ERANGE %d EINVAL %d ttb %d", errno, ERANGE, EINVAL,
							ci->ttb[TTB_REVERSES]);
						}
						ci->ttb[TTB_REVERSES] = 0;
						notice_lang(ci->bi->nick, u, BOT_KICK_BAD_TTB, ttb);
						return MOD_CONT;
					}
				} else
					ci->ttb[TTB_REVERSES] = 0;
				ci->botflags |= BS_KICK_REVERSES;
				if (ci->ttb[TTB_REVERSES])
					notice_lang(ci->bi->nick, u, BOT_KICK_REVERSES_ON_BAN, ci->ttb[TTB_REVERSES]);
				else
					notice_lang(ci->bi->nick, u, BOT_KICK_REVERSES_ON);
			} else {
				ci->botflags &= ~BS_KICK_REVERSES;
				notice_lang(ci->bi->nick, u, BOT_KICK_REVERSES_OFF);
			}
		} else if (!stricmp(option, "UNDERLINES")) {
			if (!stricmp(value, "ON")) {
				if (ttb) {
					ci->ttb[TTB_UNDERLINES] = strtol(ttb, (char **) NULL, 10);
					if (errno == ERANGE || errno == EINVAL || ci->ttb[TTB_UNDERLINES] < 0) {
						if (debug) {
							alog("debug: errno is %d ERANGE %d EINVAL %d ttb %d", errno, ERANGE, EINVAL,
							ci->ttb[TTB_UNDERLINES]);
						}
						ci->ttb[TTB_UNDERLINES] = 0;
						notice_lang(ci->bi->nick, u, BOT_KICK_BAD_TTB, ttb);
						return MOD_CONT;
					}
				} else
					ci->ttb[TTB_UNDERLINES] = 0;
				ci->botflags |= BS_KICK_UNDERLINES;
				if (ci->ttb[TTB_UNDERLINES])
					notice_lang(ci->bi->nick, u, BOT_KICK_UNDERLINES_ON_BAN, ci->ttb[TTB_UNDERLINES]);
				else
					notice_lang(ci->bi->nick, u, BOT_KICK_UNDERLINES_ON);
			} else {
				ci->botflags &= ~BS_KICK_UNDERLINES;
				notice_lang(ci->bi->nick, u, BOT_KICK_UNDERLINES_OFF);
			}
		} else
			notice_help(ci->bi->nick, u, BOT_KICK_UNKNOWN, option);
	}
	return MOD_CONT;
}
#endif

/* EOF */

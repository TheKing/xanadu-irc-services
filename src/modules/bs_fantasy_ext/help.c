/**
 * Methods use the modules' help system. - Source
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
 * Last Updated   : 06/02/2008
 *
 **/

#include "help.h"

/**
 * Send a list of all available commands to the user.
 **/
int do_cmd_list(User *u, ChannelInfo *ci) {
#ifndef ENABLE_CMDLIST
	moduleNoticeLang(ci->bi->nick, u, LANG_CMD_DISABLED);
#else
	char buf[BUFSIZE], *end;
	const char *commastr = getstring(u->na, COMMA_SPACE);
	int need_comma = 0;

	moduleNoticeLang(ci->bi->nick, u, LANG_CMD_LIST_HEADER);
	notice(ci->bi->nick, u->nick, " ");

	/* General Commands */
	end = buf;
	*end = 0;

#ifdef ENABLE_CMDLIST
	end += snprintf(end, sizeof(buf) - (end - buf), "%scmdlist", BSFantasyCharacter);
	need_comma = 1;
#endif

#ifdef ENABLE_HELP
	end += snprintf(end, sizeof(buf) - (end - buf), "%s%shelp", need_comma ? commastr : "", BSFantasyCharacter);
	need_comma = 1;
#endif

#ifdef ENABLE_STAFF
	end += snprintf(end, sizeof(buf) - (end - buf), "%s%sstaff | %sircops, %sadmins", need_comma ? commastr : "",
			BSFantasyCharacter, BSFantasyCharacter, BSFantasyCharacter);
	need_comma = 1;
#endif

	end += snprintf(end, sizeof(buf) - (end - buf), "%s%sminfo | %smodinfo", need_comma ? commastr : "", BSFantasyCharacter, BSFantasyCharacter);
	end += snprintf(end, sizeof(buf) - (end - buf), "%s%ssversion", commastr, BSFantasyCharacter);
	end += snprintf(end, sizeof(buf) - (end - buf), "%s%sseen", commastr, BSFantasyCharacter);

	moduleNoticeLang(ci->bi->nick, u, LANG_CMD_LIST_GENERAL, buf);

	/* Channel access controls */
	end = buf;
	*end = 0;
	need_comma = 0;

#ifdef ENABLE_XOP
	end += snprintf(end, sizeof(buf) - (end - buf), "%ssop", BSFantasyCharacter);
	end += snprintf(end, sizeof(buf) - (end - buf), "%s%saop", commastr, BSFantasyCharacter);
	if (ircd->halfop)
		end += snprintf(end, sizeof(buf) - (end - buf), "%s%shop", commastr, BSFantasyCharacter);
	end += snprintf(end, sizeof(buf) - (end - buf), "%s%svop", commastr, BSFantasyCharacter);
	need_comma = 1;
#endif

#ifdef ENABLE_ACCESS
	end += snprintf(end, sizeof(buf) - (end - buf), "%s%saccess", need_comma ? commastr : "", BSFantasyCharacter);
	end += snprintf(end, sizeof(buf) - (end - buf), "%s%slevels", commastr, BSFantasyCharacter);
	need_comma = 1;
#endif

	moduleNoticeLang(ci->bi->nick, u, LANG_CMD_LIST_CHAN_AXX, buf);

	/* Channel setting commands */
	end = buf;
	*end = 0;
	need_comma = 0;

#ifdef ENABLE_AKICK
	end += snprintf(end, sizeof(buf) - (end - buf), "%sakick", BSFantasyCharacter);
	need_comma = 1;
#endif

#ifdef ENABLE_BADWORDS
	end += snprintf(end, sizeof(buf) - (end - buf), "%s%sbadwords", need_comma ? commastr : "", BSFantasyCharacter);
	need_comma = 1;
#endif

#ifdef ENABLE_SET
	end += snprintf(end, sizeof(buf) - (end - buf), "%s%sset", need_comma ? commastr : "", BSFantasyCharacter);
	need_comma = 1;
#endif

#ifdef ENABLE_BKICK
	end += snprintf(end, sizeof(buf) - (end - buf), "%s%sbkick", need_comma ? commastr : "", BSFantasyCharacter);
	need_comma = 1;
#endif

#ifdef ENABLE_INFO
	end += snprintf(end, sizeof(buf) - (end - buf), "%s%sinfo", need_comma ? commastr : "", BSFantasyCharacter);
	need_comma = 1;
#endif

	moduleNoticeLang(ci->bi->nick, u, LANG_CMD_LIST_CHG_SET, buf);

	/* Channel action commands */
	end = buf;
	*end = 0;
	need_comma = 0;

#ifdef ENABLE_CLEAR
	end += snprintf(end, sizeof(buf) - (end - buf), "%sclear", BSFantasyCharacter);
	need_comma = 1;
#endif

#ifdef ENABLE_TOPIC
	end += snprintf(end, sizeof(buf) - (end - buf), "%s%stopic | %sctopic", need_comma ? commastr : "", BSFantasyCharacter, BSFantasyCharacter);
	need_comma = 1;
#endif

#ifdef ENABLE_APPENDTOPIC
	end += snprintf(end, sizeof(buf) - (end - buf), "%s%sappendtopic | %stappend", need_comma ? commastr : "", BSFantasyCharacter, BSFantasyCharacter);
	need_comma = 1;
#endif

#ifdef ENABLE_INVITE
	end += snprintf(end, sizeof(buf) - (end - buf), "%s%sinvite", need_comma ? commastr : "", BSFantasyCharacter);
	need_comma = 1;
#endif

#ifdef ENABLE_UPDOWN
	end += snprintf(end, sizeof(buf) - (end - buf), "%s%sup | %sdown", need_comma ? commastr : "", BSFantasyCharacter, BSFantasyCharacter);
	need_comma = 1;
#endif

#ifdef ENABLE_BAN
	end += snprintf(end, sizeof(buf) - (end - buf), "%s%sban", need_comma ? commastr : "", BSFantasyCharacter);
	need_comma = 1;
#endif

#ifdef ENABLE_UNBAN
	end += snprintf(end, sizeof(buf) - (end - buf), "%s%sunban", need_comma ? commastr : "", BSFantasyCharacter);
	need_comma = 1;
#endif

#ifdef ENABLE_MUTEUNMUTE
	end += snprintf(end, sizeof(buf) - (end - buf), "%s%smute | %sunmute", need_comma ? commastr : "", BSFantasyCharacter, BSFantasyCharacter);
	need_comma = 1;
#endif

#ifdef ENABLE_TBAN
	end += snprintf(end, sizeof(buf) - (end - buf), "%s%stban", need_comma ? commastr : "", BSFantasyCharacter);
	need_comma = 1;
#endif

#ifdef ENABLE_TKICKBAN
	end += snprintf(end, sizeof(buf) - (end - buf), "%s%stkban", need_comma ? commastr : "", BSFantasyCharacter);
	need_comma = 1;
#endif

	end += snprintf(end, sizeof(buf) - (end - buf), "%s%skick", commastr, BSFantasyCharacter);
	end += snprintf(end, sizeof(buf) - (end - buf), "%s%skb\n", commastr, BSFantasyCharacter);

	/* Opping etc */
	need_comma = 0;
	if (ircd->owner) {
		end += snprintf(end, sizeof(buf) - (end - buf), "%sowner | %sdeowner", BSFantasyCharacter, BSFantasyCharacter);
		need_comma = 1;
	}
	if (ircd->protect) {
		end += snprintf(end, sizeof(buf) - (end - buf), "%s%sprotect | %sdeprotect", need_comma ? commastr : "", BSFantasyCharacter, BSFantasyCharacter);
		need_comma = 1;
	}
	end += snprintf(end, sizeof(buf) - (end - buf), "%s%sop | %sdeop", need_comma ? commastr : "", BSFantasyCharacter, BSFantasyCharacter);
	if (ircd->halfop) {
		end += snprintf(end, sizeof(buf) - (end - buf), "%s%shalfop | %sdehalfop", commastr, BSFantasyCharacter, BSFantasyCharacter);
	}
	end += snprintf(end, sizeof(buf) - (end - buf), "%s%svoice | %sdevoice", commastr, BSFantasyCharacter, BSFantasyCharacter);

#ifdef ENABLE_SYNC
	if (en_sync) {
		end += snprintf(end, sizeof(buf) - (end - buf), "%s%ssync", commastr, BSFantasyCharacter);
		need_comma = 1;
	}
#endif

	moduleNoticeLang(ci->bi->nick, u, LANG_CMD_LIST_ACTIONS, buf);


	/* IRCop commands */
	end = buf;
	*end = 0;
	need_comma = 0;

#ifdef ENABLE_KILL
	end += snprintf(end, sizeof(buf) - (end - buf), "%skill", BSFantasyCharacter);
	need_comma = 1;
#endif

#ifdef ENABLE_MODE
	end += snprintf(end, sizeof(buf) - (end - buf), "%s%smode", need_comma ? commastr : "", BSFantasyCharacter);
	need_comma = 1;
#endif

#ifdef ENABLE_AKILL
	end += snprintf(end, sizeof(buf) - (end - buf), "%s%sakill", need_comma ? commastr : "", BSFantasyCharacter);
	need_comma = 1;
#endif

#ifdef ENABLE_IGNORE
	end += snprintf(end, sizeof(buf) - (end - buf), "%s%signore", need_comma ? commastr : "", BSFantasyCharacter);
	need_comma = 1;
#endif

#ifdef ENABLE_SUSPEND
	end += snprintf(end, sizeof(buf) - (end - buf), "%s%ssuspend", need_comma ? commastr : "", BSFantasyCharacter);
	need_comma = 1;
#endif

#ifdef ENABLE_SHUN
	if (en_shun) {
		end += snprintf(end, sizeof(buf) - (end - buf), "%s%sshun", need_comma ? commastr : "", BSFantasyCharacter);
		need_comma = 1;
	}
#endif

#ifdef ENABLE_TSHUN
	if (en_shun) {
		end += snprintf(end, sizeof(buf) - (end - buf), "%s%stshun", need_comma ? commastr : "", BSFantasyCharacter);
		need_comma = 1;
	}
#endif

	moduleNoticeLang(ci->bi->nick, u, LANG_CMD_LIST_IRCOP, buf);

	notice(ci->bi->nick, u->nick, " ");
	moduleNoticeLang(ci->bi->nick, u, LANG_CMD_LIST_FOOTER);

#endif
	return MOD_CONT;
}


/**
 * Manages the !help command
 **/
int do_help(User *u, ChannelInfo *ci, char *cmd, char *param, char *opt) {
#ifndef ENABLE_HELP
	moduleNoticeLang(ci->bi->nick, u, LANG_CMD_DISABLED);
#else
	moduleNoticeLang(ci->bi->nick, u, LANG_HELP_HEADER);
	if (cmd == NULL) {
		/* help index */
		moduleNoticeLang(ci->bi->nick, u, LANG_HELP_LIST, BSFantasyCharacter, BSFantasyCharacter,
		BSFantasyCharacter, BSFantasyCharacter, BSFantasyCharacter, BSFantasyCharacter);
	} else {
		/* list commands */

		if (!stricmp(cmd, "general")) {
			moduleNoticeLang(ci->bi->nick, u, LANG_HELP_CMDS_GEN, BSFantasyCharacter, BSFantasyCharacter,
			BSFantasyCharacter, BSFantasyCharacter, BSFantasyCharacter, BSFantasyCharacter, BSFantasyCharacter,
			BSFantasyCharacter, BSFantasyCharacter);
			moduleNoticeLang(ci->bi->nick, u, LANG_HELP_MORE_INFO, BSFantasyCharacter);
		}

		else if (!stricmp(cmd, "chaccess")) {
			moduleNoticeLang(ci->bi->nick, u, LANG_HELP_CMDS_ACC, BSFantasyCharacter, BSFantasyCharacter,
			BSFantasyCharacter, BSFantasyCharacter, BSFantasyCharacter, BSFantasyCharacter);
			moduleNoticeLang(ci->bi->nick, u, LANG_HELP_MORE_INFO, BSFantasyCharacter);
		}

		else if (!stricmp(cmd, "config")) {
			moduleNoticeLang(ci->bi->nick, u, LANG_HELP_CMDS_SETTINGS, BSFantasyCharacter, BSFantasyCharacter,
			BSFantasyCharacter, BSFantasyCharacter, BSFantasyCharacter, BSFantasyCharacter);
			moduleNoticeLang(ci->bi->nick, u, LANG_HELP_MORE_INFO, BSFantasyCharacter);
		}

		else if (!stricmp(cmd, "act")) {
			moduleNoticeLang(ci->bi->nick, u, LANG_HELP_CMDS_ACTIONS, BSFantasyCharacter, BSFantasyCharacter,
			BSFantasyCharacter, BSFantasyCharacter, BSFantasyCharacter, BSFantasyCharacter, BSFantasyCharacter,
			BSFantasyCharacter, BSFantasyCharacter);
			notice(ci->bi->nick, u->nick, " ");
			moduleNoticeLang(ci->bi->nick, u, LANG_HELP_CMDS_PUTMODES, BSFantasyCharacter, BSFantasyCharacter,
			BSFantasyCharacter, BSFantasyCharacter, BSFantasyCharacter, BSFantasyCharacter, BSFantasyCharacter,
			BSFantasyCharacter, BSFantasyCharacter, BSFantasyCharacter, BSFantasyCharacter, BSFantasyCharacter,
			BSFantasyCharacter, BSFantasyCharacter, BSFantasyCharacter);
			moduleNoticeLang(ci->bi->nick, u, LANG_HELP_MORE_INFO, BSFantasyCharacter);
		}

		else if (!stricmp(cmd, "ircop")) {
			moduleNoticeLang(ci->bi->nick, u, LANG_HELP_CMDS_IRCOP, BSFantasyCharacter, BSFantasyCharacter,
			BSFantasyCharacter, BSFantasyCharacter);
			moduleNoticeLang(ci->bi->nick, u, LANG_HELP_MORE_INFO, BSFantasyCharacter);
		}

		/* list all commands */
		else if (!stricmp(cmd, "all")) {
			moduleNoticeLang(ci->bi->nick, u, LANG_HELP_CMDS_GEN, BSFantasyCharacter, BSFantasyCharacter,
			BSFantasyCharacter, BSFantasyCharacter, BSFantasyCharacter, BSFantasyCharacter, BSFantasyCharacter,
			BSFantasyCharacter, BSFantasyCharacter);
			notice(ci->bi->nick, u->nick, " ");
			moduleNoticeLang(ci->bi->nick, u, LANG_HELP_CMDS_ACC, BSFantasyCharacter, BSFantasyCharacter,
			BSFantasyCharacter, BSFantasyCharacter, BSFantasyCharacter, BSFantasyCharacter);
			notice(ci->bi->nick, u->nick, " ");
			moduleNoticeLang(ci->bi->nick, u, LANG_HELP_CMDS_SETTINGS, BSFantasyCharacter, BSFantasyCharacter,
			BSFantasyCharacter, BSFantasyCharacter, BSFantasyCharacter, BSFantasyCharacter);
			notice(ci->bi->nick, u->nick, " ");
			moduleNoticeLang(ci->bi->nick, u, LANG_HELP_CMDS_ACTIONS, BSFantasyCharacter, BSFantasyCharacter,
			BSFantasyCharacter, BSFantasyCharacter, BSFantasyCharacter, BSFantasyCharacter, BSFantasyCharacter,
			BSFantasyCharacter, BSFantasyCharacter);
			notice(ci->bi->nick, u->nick, " ");
			moduleNoticeLang(ci->bi->nick, u, LANG_HELP_CMDS_PUTMODES, BSFantasyCharacter, BSFantasyCharacter,
			BSFantasyCharacter, BSFantasyCharacter, BSFantasyCharacter, BSFantasyCharacter, BSFantasyCharacter,
			BSFantasyCharacter, BSFantasyCharacter, BSFantasyCharacter, BSFantasyCharacter, BSFantasyCharacter,
			BSFantasyCharacter, BSFantasyCharacter, BSFantasyCharacter);
			notice(ci->bi->nick, u->nick, " ");
			moduleNoticeLang(ci->bi->nick, u, LANG_HELP_CMDS_IRCOP, BSFantasyCharacter, BSFantasyCharacter,
			BSFantasyCharacter, BSFantasyCharacter);
			notice(ci->bi->nick, u->nick, " ");
			moduleNoticeLang(ci->bi->nick, u, LANG_HELP_MORE_INFO, BSFantasyCharacter);
		}

		/* help on commands... */
		else if (!stricmp(cmd, "clear"))
#ifdef ENABLE_CLEAR
			moduleNoticeLang(ci->bi->nick, u, LANG_CLEAR_SYNTAX, BSFantasyCharacter);
#else
			moduleNoticeLang(ci->bi->nick, u, LANG_CMD_DISABLED);
#endif
		else if (!stricmp(cmd, "sop"))
#ifdef ENABLE_XOP
			moduleNoticeLang(ci->bi->nick, u, LANG_SOP_SYNTAX_EXT, BSFantasyCharacter,
			BSFantasyCharacter, BSFantasyCharacter, BSFantasyCharacter);
#else
			moduleNoticeLang(ci->bi->nick, u, LANG_CMD_DISABLED);
#endif
		else if (!stricmp(cmd, "aop"))
#ifdef ENABLE_XOP
			moduleNoticeLang(ci->bi->nick, u, LANG_AOP_SYNTAX_EXT, BSFantasyCharacter,
			BSFantasyCharacter, BSFantasyCharacter, BSFantasyCharacter);
#else
			moduleNoticeLang(ci->bi->nick, u, LANG_CMD_DISABLED);
#endif
		else if ((!stricmp(cmd, "hop")) && (ircd->halfop))
#ifdef ENABLE_XOP
			moduleNoticeLang(ci->bi->nick, u, LANG_HOP_SYNTAX_EXT, BSFantasyCharacter,
			BSFantasyCharacter, BSFantasyCharacter, BSFantasyCharacter);
#else
			moduleNoticeLang(ci->bi->nick, u, LANG_CMD_DISABLED);
#endif
		else if ((!stricmp(cmd, "hop")) && (!ircd->halfop))
#ifdef ENABLE_XOP
			moduleNoticeLang(ci->bi->nick, u, LANG_HOPS_UNSUPPORTED);
#else
			moduleNoticeLang(ci->bi->nick, u, LANG_CMD_DISABLED);
#endif
		else if (!stricmp(cmd, "vop"))
#ifdef ENABLE_XOP
			moduleNoticeLang(ci->bi->nick, u, LANG_VOP_SYNTAX_EXT, BSFantasyCharacter,
			BSFantasyCharacter, BSFantasyCharacter, BSFantasyCharacter);
#else
			moduleNoticeLang(ci->bi->nick, u, LANG_CMD_DISABLED);
#endif
		else if (!stricmp(cmd, "access"))
#ifdef ENABLE_ACCESS
			if (opt && !stricmp(opt, "levels"))
				moduleNoticeLang(ci->bi->nick, u, LANG_ACCESS_LEVELS);
			else
				moduleNoticeLang(ci->bi->nick, u, LANG_ACCESS_SYNTAX_EXT, BSFantasyCharacter,
					BSFantasyCharacter, BSFantasyCharacter, BSFantasyCharacter);
#else
			moduleNoticeLang(ci->bi->nick, u, LANG_CMD_DISABLED);
#endif
		else if (!stricmp(cmd, "levels"))
#ifdef ENABLE_ACCESS
			if (opt && !stricmp(opt, "desc")) {
				int i;
				notice_help(ci->bi->nick, u, CHAN_HELP_LEVELS_DESC);
				if (!levelinfo_maxwidth) {
					for (i = 0; levelinfo[i].what >= 0; i++) {
						int len = strlen(levelinfo[i].name);
						if (len > levelinfo_maxwidth)
							levelinfo_maxwidth = len;
					}
				}
				for (i = 0; levelinfo[i].what >= 0; i++) {
					notice_help(ci->bi->nick, u, CHAN_HELP_LEVELS_DESC_FORMAT, levelinfo_maxwidth,
							levelinfo[i].name, getstring(u->na, levelinfo[i].desc));
				}
			} else
				moduleNoticeLang(ci->bi->nick, u, LANG_LEVELS_SYNTAX_EXT, BSFantasyCharacter,
					BSFantasyCharacter, BSFantasyCharacter, BSFantasyCharacter);
#else
			moduleNoticeLang(ci->bi->nick, u, LANG_CMD_DISABLED);
#endif
		else if ((!stricmp(cmd, "staff")) || (!stricmp(cmd, "ircops")))
#ifdef ENABLE_STAFF
			moduleNoticeLang(ci->bi->nick, u, LANG_STAFF, BSFantasyCharacter, BSFantasyCharacter);
#else
			moduleNoticeLang(ci->bi->nick, u, LANG_CMD_DISABLED);
#endif
		else if (!stricmp(cmd, "admins"))
#ifdef ENABLE_STAFF
			moduleNoticeLang(ci->bi->nick, u, LANG_ADMINS, BSFantasyCharacter, BSFantasyCharacter);
#else
			moduleNoticeLang(ci->bi->nick, u, LANG_CMD_DISABLED);
#endif
		else if (!stricmp(cmd, "akick"))
#ifdef ENABLE_AKICK
			moduleNoticeLang(ci->bi->nick, u, LANG_AKICK_SYNTAX_EXT, BSFantasyCharacter,
			BSFantasyCharacter, BSFantasyCharacter, BSFantasyCharacter);
#else
			moduleNoticeLang(ci->bi->nick, u, LANG_CMD_DISABLED);
#endif
		else if (!stricmp(cmd, "badwords"))
#ifdef ENABLE_BADWORDS
			moduleNoticeLang(ci->bi->nick, u, LANG_BADWORDS_SYNTAX_EXT, BSFantasyCharacter,
			BSFantasyCharacter, BSFantasyCharacter, BSFantasyCharacter);
#else
			moduleNoticeLang(ci->bi->nick, u, LANG_CMD_DISABLED);
#endif
		else if (!stricmp(cmd, "topic"))
#ifdef ENABLE_TOPIC
			moduleNoticeLang(ci->bi->nick, u, LANG_TOPIC_SYNTAX_EXT, BSFantasyCharacter, BSFantasyCharacter);
#else
			moduleNoticeLang(ci->bi->nick, u, LANG_CMD_DISABLED);
#endif
		else if ((!stricmp(cmd, "appendtopic")) || (!stricmp(cmd, "tappend")))
#ifdef ENABLE_APPENDTOPIC
			moduleNoticeLang(ci->bi->nick, u, LANG_APPENDTOPIC_SYNTAX_EXT, BSFantasyCharacter, BSFantasyCharacter);
#else
			moduleNoticeLang(ci->bi->nick, u, LANG_CMD_DISABLED);
#endif
		else if (!stricmp(cmd, "invite"))
#ifdef ENABLE_INVITE
			moduleNoticeLang(ci->bi->nick, u, LANG_INVITE_SYNTAX_EXT, BSFantasyCharacter);
#else
			moduleNoticeLang(ci->bi->nick, u, LANG_CMD_DISABLED);
#endif
		else if (!stricmp(cmd, "help"))
#ifdef ENABLE_HELP
			moduleNoticeLang(ci->bi->nick, u, LANG_HELP, BSFantasyCharacter);
#else
			moduleNoticeLang(ci->bi->nick, u, LANG_CMD_DISABLED);
#endif
		else if (!stricmp(cmd, "cmdlist"))
#ifdef ENABLE_CMDLIST
			moduleNoticeLang(ci->bi->nick, u, LANG_CMDLIST, BSFantasyCharacter);
#else
			moduleNoticeLang(ci->bi->nick, u, LANG_CMD_DISABLED);
#endif
		else if ((!stricmp(cmd, "minfo")) || (!stricmp(cmd, "modinfo")))
			moduleNoticeLang(ci->bi->nick, u, LANG_MINFO, BSFantasyCharacter, BSFantasyCharacter);

		else if (!stricmp(cmd, "sversion"))
			moduleNoticeLang(ci->bi->nick, u, LANG_SVERSION, BSFantasyCharacter);

		else if ((!stricmp(cmd, "ban")) || (!stricmp(cmd, "b")))
#ifdef ENABLE_BAN
			moduleNoticeLang(ci->bi->nick, u, LANG_BAN_SYNTAX_EXT, BSFantasyCharacter);
#else
			moduleNoticeLang(ci->bi->nick, u, LANG_CMD_DISABLED);
#endif
		else if ((!stricmp(cmd, "mute")) || (!stricmp(cmd, "m")))
#ifdef ENABLE_MUTEUNMUTE
			moduleNoticeLang(ci->bi->nick, u, LANG_MUTE_SYNTAX_EXT, BSFantasyCharacter);
#else
			moduleNoticeLang(ci->bi->nick, u, LANG_CMD_DISABLED);
#endif
		else if (!stricmp(cmd, "unmute"))
#ifdef ENABLE_MUTEUNMUTE
			moduleNoticeLang(ci->bi->nick, u, LANG_UNMUTE_SYNTAX_EXT, BSFantasyCharacter);
#else
			moduleNoticeLang(ci->bi->nick, u, LANG_CMD_DISABLED);
#endif
		else if ((!stricmp(cmd, "kb")) || (!stricmp(cmd, "kickban")))
#ifdef ENABLE_KICKBAN
			moduleNoticeLang(ci->bi->nick, u, LANG_KICKBAN_SYNTAX_EXT, BSFantasyCharacter, BSFantasyCharacter);
#else
			moduleNoticeLang(ci->bi->nick, u, LANG_CMD_DISABLED);
#endif

		else if (!stricmp(cmd, "set")) {
#ifdef ENABLE_SET
			if (!param) {
				moduleNoticeLang(ci->bi->nick, u, LANG_SET_SYNTAX_EXT, BSFantasyCharacter);
				moduleNoticeLang(ci->bi->nick, u, LANG_HELP_MORE_INFO, BSFantasyCharacter);
			} else {
				/* chanserv settings */
				if (!stricmp(param, "founder"))
					moduleNoticeLang(ci->bi->nick, u, LANG_SET_FOUNDER_SYNTAX_EXT, BSFantasyCharacter);
				else if (!stricmp(param, "successor"))
					moduleNoticeLang(ci->bi->nick, u, LANG_SET_SUCCESSOR_SYNTAX_EXT, BSFantasyCharacter);
				else if (!stricmp(param, "desc"))
					moduleNoticeLang(ci->bi->nick, u, LANG_SET_DESC_SYNTAX_EXT, BSFantasyCharacter);
				else if (!stricmp(param, "url"))
					moduleNoticeLang(ci->bi->nick, u, LANG_SET_URL_SYNTAX_EXT, BSFantasyCharacter);
				else if (!stricmp(param, "email"))
					moduleNoticeLang(ci->bi->nick, u, LANG_SET_EMAIL_SYNTAX_EXT, BSFantasyCharacter);
				else if (!stricmp(param, "entrymsg"))
					moduleNoticeLang(ci->bi->nick, u, LANG_SET_ENTRYMSG_SYNTAX_EXT, BSFantasyCharacter);
				else if (!stricmp(param, "bantype"))
					moduleNoticeLang(ci->bi->nick, u, LANG_SET_BANTYPE_SYNTAX_EXT, BSFantasyCharacter);
				else if (!stricmp(param, "mlock"))
					moduleNoticeLang(ci->bi->nick, u, LANG_SET_MLOCK_SYNTAX_EXT, BSFantasyCharacter);
				else if (!stricmp(param, "keeptopic"))
					moduleNoticeLang(ci->bi->nick, u, LANG_SET_KEEPTOPIC_SYNTAX_EXT, BSFantasyCharacter);
				else if (!stricmp(param, "topiclock"))
					moduleNoticeLang(ci->bi->nick, u, LANG_SET_TOPICLOCK_SYNTAX_EXT, BSFantasyCharacter);
				else if (!stricmp(param, "opnotice"))
					moduleNoticeLang(ci->bi->nick, u, LANG_SET_OPNOTICE_SYNTAX_EXT, BSFantasyCharacter);
				else if (!stricmp(param, "peace"))
					moduleNoticeLang(ci->bi->nick, u, LANG_SET_PEACE_SYNTAX_EXT, BSFantasyCharacter);
				else if (!stricmp(param, "private"))
					moduleNoticeLang(ci->bi->nick, u, LANG_SET_PRIVATE_SYNTAX_EXT, BSFantasyCharacter, s_ChanServ);
				else if (!stricmp(param, "restricted"))
					moduleNoticeLang(ci->bi->nick, u, LANG_SET_RESTRICTED_SYNTAX_EXT, BSFantasyCharacter);
				else if (!stricmp(param, "secure"))
					moduleNoticeLang(ci->bi->nick, u, LANG_SET_SECURE_SYNTAX_EXT, BSFantasyCharacter);
				else if (!stricmp(param, "secureops"))
					moduleNoticeLang(ci->bi->nick, u, LANG_SET_SECUREOPS_SYNTAX_EXT, BSFantasyCharacter);
				else if (!stricmp(param, "securefounder"))
					moduleNoticeLang(ci->bi->nick, u, LANG_SET_SECUREFOUNDER_SYNTAX_EXT, BSFantasyCharacter);
				else if (!stricmp(param, "signkick"))
					moduleNoticeLang(ci->bi->nick, u, LANG_SET_SIGNKICK_SYNTAX_EXT, BSFantasyCharacter, BSFantasyCharacter);
				else if (!stricmp(param, "xop"))
					moduleNoticeLang(ci->bi->nick, u, LANG_SET_XOP_SYNTAX_EXT, BSFantasyCharacter);
				else if (!stricmp(param, "noexpire"))
					moduleNoticeLang(ci->bi->nick, u, LANG_SET_NOEXPIRE_SYNTAX_EXT, BSFantasyCharacter);
				/* botserv settings */
				else if (!stricmp(param, "dontkickops"))
					moduleNoticeLang(ci->bi->nick, u, LANG_SET_DONTKICKOPS_SYNTAX_EXT, BSFantasyCharacter);
				else if (!stricmp(param, "dontkickvoices"))
					moduleNoticeLang(ci->bi->nick, u, LANG_SET_DONTKICKVOICES_SYNTAX_EXT, BSFantasyCharacter);
				else if (!stricmp(param, "greet"))
					moduleNoticeLang(ci->bi->nick, u, LANG_SET_GREET_SYNTAX_EXT, BSFantasyCharacter);
				else if (!stricmp(param, "fantasy"))
					moduleNoticeLang(ci->bi->nick, u, LANG_SET_FANTASY_SYNTAX_EXT, BSFantasyCharacter);
				else if (!stricmp(param, "symbiosis"))
					moduleNoticeLang(ci->bi->nick, u, LANG_SET_SYMBIOSIS_SYNTAX_EXT, BSFantasyCharacter);
				else if (!stricmp(param, "nobot"))
					moduleNoticeLang(ci->bi->nick, u, LANG_SET_NOBOT_SYNTAX_EXT, BSFantasyCharacter);
				else {
					moduleNoticeLang(ci->bi->nick, u, LANG_UNKNOWN_CMD, param);
				}
			}
#else
			moduleNoticeLang(ci->bi->nick, u, LANG_CMD_DISABLED);
#endif
		} else if (!stricmp(cmd, "kill"))
#ifdef ENABLE_KILL
			moduleNoticeLang(ci->bi->nick, u, LANG_KILL_SYNTAX_EXT, BSFantasyCharacter);
#else
			moduleNoticeLang(ci->bi->nick, u, LANG_CMD_DISABLED);
#endif
		else if (!stricmp(cmd, "mode"))
#ifdef ENABLE_MODE
			moduleNoticeLang(ci->bi->nick, u, LANG_MODE_SYNTAX_EXT, BSFantasyCharacter);
#else
			moduleNoticeLang(ci->bi->nick, u, LANG_CMD_DISABLED);
#endif
		else if (!stricmp(cmd, "akill"))
#ifdef ENABLE_AKILL
			moduleNoticeLang(ci->bi->nick, u, LANG_AKILL_SYNTAX_EXT, BSFantasyCharacter,
			BSFantasyCharacter, BSFantasyCharacter, BSFantasyCharacter);
#else
			moduleNoticeLang(ci->bi->nick, u, LANG_CMD_DISABLED);
#endif
		else if (!stricmp(cmd, "ignore"))
#ifdef ENABLE_IGNORE
			moduleNoticeLang(ci->bi->nick, u, LANG_IGNORE_SYNTAX_EXT, BSFantasyCharacter, BSFantasyCharacter);
#else
			moduleNoticeLang(ci->bi->nick, u, LANG_CMD_DISABLED);
#endif
		else if (!stricmp(cmd, "info"))
#ifdef ENABLE_INFO
			moduleNoticeLang(ci->bi->nick, u, LANG_INFO_SYNTAX_EXT, BSFantasyCharacter);
#else
			moduleNoticeLang(ci->bi->nick, u, LANG_CMD_DISABLED);
#endif

		else if (!stricmp(cmd, "bkick")) {
#ifdef ENABLE_BKICK
			if (!param) {
				moduleNoticeLang(ci->bi->nick, u, LANG_BKICK_SYNTAX_EXT, BSFantasyCharacter);
				moduleNoticeLang(ci->bi->nick, u, LANG_HELP_MORE_INFO, BSFantasyCharacter);
			} else {
				/* chanserv settings */
				if (!stricmp(param, "bolds"))
					moduleNoticeLang(ci->bi->nick, u, LANG_BKICK_BOLDS_SYNTAX_EXT, BSFantasyCharacter);
				else if (!stricmp(param, "badwords"))
					moduleNoticeLang(ci->bi->nick, u, LANG_BKICK_BADWORDS_SYNTAX_EXT, BSFantasyCharacter, BSFantasyCharacter);
				else if (!stricmp(param, "caps"))
					moduleNoticeLang(ci->bi->nick, u, LANG_BKICK_CAPS_SYNTAX_EXT, BSFantasyCharacter);
				else if (!stricmp(param, "colors"))
					moduleNoticeLang(ci->bi->nick, u, LANG_BKICK_COLORS_SYNTAX_EXT, BSFantasyCharacter);
				else if (!stricmp(param, "flood"))
					moduleNoticeLang(ci->bi->nick, u, LANG_BKICK_FLOOD_SYNTAX_EXT, BSFantasyCharacter);
				else if (!stricmp(param, "repeat"))
					moduleNoticeLang(ci->bi->nick, u, LANG_BKICK_REPEAT_SYNTAX_EXT, BSFantasyCharacter);
				else if (!stricmp(param, "reverses"))
					moduleNoticeLang(ci->bi->nick, u, LANG_BKICK_REVERSES_SYNTAX_EXT, BSFantasyCharacter);
				else if (!stricmp(param, "underlines"))
					moduleNoticeLang(ci->bi->nick, u, LANG_BKICK_UNDERLINES_SYNTAX_EXT, BSFantasyCharacter);
				else {
					moduleNoticeLang(ci->bi->nick, u, LANG_UNKNOWN_CMD, param);
				}
			}
#else
			moduleNoticeLang(ci->bi->nick, u, LANG_CMD_DISABLED);
#endif
		} else if (!stricmp(cmd, "up"))
#ifdef ENABLE_UPDOWN
			moduleNoticeLang(ci->bi->nick, u, LANG_UP_SYNTAX_EXT, BSFantasyCharacter);
#else
			moduleNoticeLang(ci->bi->nick, u, LANG_CMD_DISABLED);
#endif
		else if (!stricmp(cmd, "down"))
#ifdef ENABLE_UPDOWN
			moduleNoticeLang(ci->bi->nick, u, LANG_DOWN_SYNTAX_EXT, BSFantasyCharacter);
#else
			moduleNoticeLang(ci->bi->nick, u, LANG_CMD_DISABLED);
#endif
		else if ((!stricmp(cmd, "tb")) || (!stricmp(cmd, "tban")))
#ifdef ENABLE_TBAN
			moduleNoticeLang(ci->bi->nick, u, LANG_TBAN_SYNTAX_EXT, BSFantasyCharacter, BSFantasyCharacter, BSFantasyCharacter);
#else
			moduleNoticeLang(ci->bi->nick, u, LANG_CMD_DISABLED);
#endif
		else if ((!stricmp(cmd, "tkb")) || (!stricmp(cmd, "tkban")))
#ifdef ENABLE_TKICKBAN
			moduleNoticeLang(ci->bi->nick, u, LANG_TKBAN_SYNTAX_EXT, BSFantasyCharacter, BSFantasyCharacter, BSFantasyCharacter);
#else
			moduleNoticeLang(ci->bi->nick, u, LANG_CMD_DISABLED);
#endif
		else if (!stricmp(cmd, "suspend"))
#ifdef ENABLE_SUSPEND
			moduleNoticeLang(ci->bi->nick, u, LANG_SUSPEND_SYNTAX_EXT, BSFantasyCharacter);
#else
			moduleNoticeLang(ci->bi->nick, u, LANG_CMD_DISABLED);
#endif
		else if (!stricmp(cmd, "sync")) {
#ifdef ENABLE_SYNC
			if (en_sync)
				moduleNoticeLang(ci->bi->nick, u, LANG_SYNC_SYNTAX_EXT, BSFantasyCharacter);
			else
				moduleNoticeLang(ci->bi->nick, u, LANG_CMD_DISABLED);
#else
			moduleNoticeLang(ci->bi->nick, u, LANG_CMD_DISABLED);
#endif
		} else if (!stricmp(cmd, "shun")) {
#ifdef ENABLE_SYNC
			if (en_shun)
				moduleNoticeLang(ci->bi->nick, u, LANG_SHUN_SYNTAX_EXT, BSFantasyCharacter, BSFantasyCharacter);
			else
				moduleNoticeLang(ci->bi->nick, u, LANG_CMD_DISABLED);
#else
			moduleNoticeLang(ci->bi->nick, u, LANG_CMD_DISABLED);
#endif
		} else if (!stricmp(cmd, "tshun")) {
#ifdef ENABLE_SYNC
			if (en_shun)
				moduleNoticeLang(ci->bi->nick, u, LANG_TSHUN_SYNTAX_EXT, BSFantasyCharacter, BSFantasyCharacter);
			else
				moduleNoticeLang(ci->bi->nick, u, LANG_CMD_DISABLED);
#else
			moduleNoticeLang(ci->bi->nick, u, LANG_CMD_DISABLED);
#endif

		} else {
			moduleNoticeLang(ci->bi->nick, u, LANG_HELP_UNKNOWN_CMD);
		}
	}

	moduleNoticeLang(ci->bi->nick, u, LANG_HELP_FOOTER);
#endif

	return MOD_CONT;
}

/* EOF */

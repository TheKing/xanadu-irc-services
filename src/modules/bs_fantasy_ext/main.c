/**
 * Modules Main Functions for loading and matching commands. - Source
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
 * Last Updated   : 04/04/2008
 *
 **/

#include "module.h"
#include "main.h"

#include "language.c"

#include "ircd.c"
#include "misc.c"
#include "misc_cmds.c"
#include "misc_oper.c"
#include "help.c"
#include "ban.c"
#include "shun.c"

#ifdef ENABLE_CLEAR
  #include "clear.c"
#endif
#ifdef ENABLE_XOP
  #include "xop.c"
#endif
#ifdef ENABLE_ACCESS
  #include "access.c"
#endif
#ifdef ENABLE_AKICK
  #include "akick.c"
#endif
#ifdef ENABLE_BADWORDS
  #include "badwords.c"
#endif
#ifdef ENABLE_SET
  #include "set.c"
#endif
#ifdef ENABLE_BKICK
  #include "bkick.c"
#endif
#ifdef ENABLE_AKILL
  #include "akill.c"
#endif
#ifdef ENABLE_IGNORE
  #include "ignore.c"
#endif
#ifdef ENABLE_TBAN
  #include "tban.c"
#else
  #ifdef ENABLE_TKICKBAN
    #include "tban.c"
  #endif
#endif
#ifdef ENABLE_SUSPEND
  #include "suspend.c"
#endif
#ifdef ENABLE_MUTEUNMUTE
  #include "mute.c"
#endif

/**
 * Called when module is loaded.
 **/
int AnopeInit(void) {
	EvtHook *hook;

	moduleAddAuthor(AUTHOR);

	if (!moduleMinVersion(1,7,21,1341)) {
		alog("[\002bs_fantasy_ext\002] Your version of Anope isn't supported. Please update to a newer release.");
		return MOD_STOP;
	}

	hook = createEventHook(EVENT_BOT_FANTASY, do_fantasy);
	if (moduleAddEventHook(hook) != MOD_ERR_OK) {
		alog("[\002bs_fantasy_ext\002] Can't hook to EVENT_BOT_FANTASY event");
		return MOD_STOP;
	}

	hook = createEventHook(EVENT_BOT_FANTASY_NO_ACCESS, do_fantasy_denied);
	if (moduleAddEventHook(hook) != MOD_ERR_OK) {
		alog("[\002bs_fantasy_ext\002] Can't hook to EVENT_BOT_FANTASY_NO_ACCESS event");
		return MOD_STOP;
	}

	hook = createEventHook(EVENT_RELOAD, reload_config);
	if (moduleAddEventHook(hook) != MOD_ERR_OK) {
		alog("[\002bs_fantasy_ext\002] Can't hook to EVENT_RELOAD event");
		return MOD_STOP;
	}

	load_config();
	update_version();
	add_languages();
	create_dummy();

	if (check_modules() != MOD_CONT) {
		alog("[\002bs_fantasy_ext\002] Loading has been aborted...");
		return MOD_STOP;
	}

#ifdef SUPPORTED
	if (unsupported)
		alog("[\002bs_fantasy_ext\002] Module switching to unsupported mode!");
#elif
	alog("[\002bs_fantasy_ext\002] Module has been compiled in unsupported mode!");
#endif

	enabled = 1;
	check_core_modules();

	alog("\002bs_fantasy_ext\002 loaded... [Author: \002%s\002] [Version: \002%s\002]", AUTHOR, VERSION);

	if (debug) {

#ifdef ENABLE_CMDLIST
		alog("debug: [bs_fantasy_ext] Fantasy command \"cmdlist\" has been enabled.");
#endif

#ifdef ENABLE_HELP
		alog("debug: [bs_fantasy_ext] Fantasy command \"help\" has been enabled.");
#endif

#ifdef ENABLE_CLEAR
		alog("debug: [bs_fantasy_ext] Fantasy command \"clear\" has been enabled.");
#endif

#ifdef ENABLE_XOP
		if (ircd->halfop)
			alog("debug: [bs_fantasy_ext] Fantasy commands \"sop/aop/hop/vop\" have been enabled.");
		else
			alog("debug: [bs_fantasy_ext] Fantasy commands \"sop/aop/vop\" have been enabled.");
#endif

#ifdef ENABLE_ACCESS
		alog("debug: [bs_fantasy_ext] Fantasy commands \"access/levels\" have been enabled.");
#endif

#ifdef ENABLE_AKICK
		alog("debug: [bs_fantasy_ext] Fantasy command \"akick\" has been enabled.");
#endif

#ifdef ENABLE_BADWORDS
		alog("debug: [bs_fantasy_ext] Fantasy command \"badwords\" has been enabled.");
#endif

#ifdef ENABLE_SET
		alog("debug: [bs_fantasy_ext] Fantasy command \"set\" has been enabled.");
#endif

#ifdef ENABLE_TOPIC
		alog("debug: [bs_fantasy_ext] Fantasy command \"topic\" has been enabled.");
#endif

#ifdef ENABLE_APPENDTOPIC
		alog("debug: [bs_fantasy_ext] Fantasy command \"appendtopic\" has been enabled.");
#endif

#ifdef ENABLE_INVITE
		alog("debug: [bs_fantasy_ext] Fantasy command \"invite\" has been enabled.");
#endif

#ifdef ENABLE_STAFF
		alog("debug: [bs_fantasy_ext] Fantasy commands \"staff\" & \"admins\" have been enabled.");
#endif

#ifdef ENABLE_UPDOWN
		alog("debug: [bs_fantasy_ext] Fantasy commands \"up/down\" have been enabled.");
#endif

#ifdef ENABLE_BAN
		alog("debug: [bs_fantasy_ext] Fantasy command \"ban\" has been enabled.");
#endif

#ifdef ENABLE_UNBAN
		alog("debug: [bs_fantasy_ext] Fantasy command \"unban\" has been enabled.");
#endif

#ifdef ENABLE_MUTEUNMUTE
		alog("debug: [bs_fantasy_ext] Fantasy commands \"mute/unmute\" have been enabled.");
#endif

#ifdef ENABLE_KICKBAN
		alog("debug: [bs_fantasy_ext] Fantasy command \"kb\"has been enabled.");
#endif

#ifdef ENABLE_INFO
		alog("debug: [bs_fantasy_ext] Fantasy command \"info\" has been enabled.");
#endif

#ifdef ENABLE_KILL
		alog("debug: [bs_fantasy_ext] Fantasy command \"kill\" has been enabled.");
#endif

#ifdef ENABLE_MODE
		alog("debug: [bs_fantasy_ext] Fantasy command \"mode\" has been enabled.");
#endif

#ifdef ENABLE_AKILL
		alog("debug: [bs_fantasy_ext] Fantasy command \"akill\" has been enabled.");
#endif

#ifdef ENABLE_IGNORE
		alog("debug: [bs_fantasy_ext] Fantasy command \"ignore\" has been enabled.");
#endif

#ifdef ENABLE_TBAN
		alog("debug: [bs_fantasy_ext] Fantasy command \"tb\" has been enabled.");
#endif
#ifdef ENABLE_TKICKBAN
		alog("debug: [bs_fantasy_ext] Fantasy command \"tkb\" has been enabled.");
#endif
#ifdef ENABLE_SYNC
		alog("debug: [bs_fantasy_ext] Fantasy command \"sync\" has been enabled.");
#endif
#ifdef ENABLE_SHUN
		alog("debug: [bs_fantasy_ext] Fantasy command \"shun\" has been enabled.");
#endif
#ifdef ENABLE_TSHUN
		alog("debug: [bs_fantasy_ext] Fantasy command \"tshun\" has been enabled.");
#endif
		if (OverrideCoreCmds)
			alog("debug: [bs_fantasy_ext] Now Overriding Core Fantasy Commands.");
	}

	return MOD_CONT;
}


/**
 * Called when module in unloaded
 **/
void AnopeFini(void) {
	int i;
	for ( i = 0; i < excempt_nr; i++) {
		if (ListExempts[i])
			free(ListExempts[i]);
		else
			alog("[\002bs_fantasy_ext\002] Non fatal error occured during unloading...");
	}

	/* Reload the core modules if we unloaded them.*/
	OverrideCoreCmds = 0;
	check_core_modules();
	delete_dummy();

    alog("[\002bs_fantasy_ext\002]: Module Unloaded");
}


/**
 * Handles all fantasy commands.
 * Here we ll identify the command and call the right routines.
 **/
int do_fantasy(int ac, char **av) {
	User *u;
	ChannelInfo *ci;
	Channel *c;

	if (!enabled)
		return MOD_CONT;

	/* Some basic error checking... should never match */
	if (ac < 3)
		return MOD_CONT;

	if (!(ci = cs_findchan(av[2])))
		return MOD_CONT;
	if (!(u = finduser(av[1])))
		return MOD_CONT;
	if (!(c = findchan(ci->name)))
		return MOD_CONT;

	/* override core fantasy commands to add multi user support */
	if (OverrideCoreCmds) {
		if (do_core_fantasy(ac, u, ci, av[0], av[3]) == MOD_STOP)
			return MOD_STOP;
	}

#ifdef ENABLE_CLEAR
    if (stricmp(av[0], "clear") == 0) {
		if (ac == 3)
			moduleNoticeLang(ci->bi->nick, u, LANG_CLEAR_SYNTAX, BSFantasyCharacter);
		else if (my_check_access(u, ci, CA_CLEAR)) {
			if (stricmp("modes",av[3])== 0)
				return do_clear(u, c, 1);
			else if (stricmp("bans",av[3])== 0)
				return do_clear(u, c, 2);
			else if (stricmp("excepts",av[3])== 0)
				return do_clear(u, c, 3);
			else if (stricmp("invites",av[3])== 0)
				return do_clear(u, c, 4);
			else if (stricmp("ops",av[3])== 0)
				return do_clear(u, c, 5);
			else if (stricmp("hops",av[3])== 0)
				return do_clear(u, c, 6);
			else if (stricmp("voices",av[3])== 0)
				return do_clear(u, c, 7);
			else if (stricmp("users",av[3]) == 0)
				return do_clear(u, c, 8);
			else
				moduleNoticeLang(ci->bi->nick, u, LANG_CLEAR_SYNTAX, BSFantasyCharacter);
		} else
			notice_lang(ci->bi->nick, u, PERMISSION_DENIED);
	} else
#endif

#ifdef ENABLE_XOP
	if (stricmp(av[0], "sop") == 0) {
		if (ac == 3)
			moduleNoticeLang(ci->bi->nick, u, LANG_SOP_SYNTAX, BSFantasyCharacter);
		else {
			char *arg1 = myStrGetToken(av[3],' ',0);
			char *arg2 = myStrGetToken(av[3],' ',1);

			if (readonly)
				moduleNoticeLang(ci->bi->nick, u, LANG_CMD_NOT_AV);

			else if (arg1) {
				if (stricmp("add",arg1) == 0) {
					if (arg2) {
						if (my_check_access(u, ci, CA_ACCESS_CHANGE))
							do_xop(u, "SOP", ACCESS_SOP, xop_msgs[1], c, arg1, arg2);
						else
							notice_lang(ci->bi->nick, u, PERMISSION_DENIED);
					} else
						moduleNoticeLang(ci->bi->nick, u, LANG_SOP_SYNTAX, BSFantasyCharacter);

				} else if (stricmp("del",arg1) == 0) {
					if (arg2) {
						if (my_check_access(u, ci, CA_ACCESS_CHANGE))
							do_xop(u, "SOP", ACCESS_SOP, xop_msgs[1], c, arg1, arg2);
						else
							notice_lang(ci->bi->nick, u, PERMISSION_DENIED);
					} else
						moduleNoticeLang(ci->bi->nick, u, LANG_SOP_SYNTAX, BSFantasyCharacter);

				} else if (stricmp("list",arg1) == 0) {
					if (my_check_access(u, ci, CA_ACCESS_LIST))
						do_xop(u, "SOP", ACCESS_SOP, xop_msgs[1], c, arg1, arg2);
					else
						notice_lang(ci->bi->nick, u, PERMISSION_DENIED);

				} else if (stricmp("clear",arg1) == 0) {
					if (my_check_access(u, ci, CA_ACCESS_CHANGE))
						do_xop(u, "SOP", ACCESS_SOP, xop_msgs[1], c, arg1, arg2);
					else
						notice_lang(ci->bi->nick, u, PERMISSION_DENIED);

				} else
					moduleNoticeLang(ci->bi->nick, u, LANG_SOP_SYNTAX, BSFantasyCharacter);
			} else
				moduleNoticeLang(ci->bi->nick, u, LANG_SOP_SYNTAX, BSFantasyCharacter);

			if (arg1) free(arg1);
			if (arg2) free(arg2);
		}

	} else if (stricmp(av[0], "aop") == 0) {
		if (ac == 3)
			moduleNoticeLang(ci->bi->nick, u, LANG_AOP_SYNTAX, BSFantasyCharacter);
		else {
			char *arg1 = myStrGetToken(av[3],' ',0);
			char *arg2 = myStrGetToken(av[3],' ',1);

			if (readonly)
				moduleNoticeLang(ci->bi->nick, u, LANG_CMD_NOT_AV);

			else if (arg1) {
				if (stricmp("add",arg1) == 0) {
					if (arg2) {
						if (my_check_access(u, ci, CA_ACCESS_CHANGE))
							do_xop(u, "AOP", ACCESS_AOP, xop_msgs[0], c, arg1, arg2);
						else
							notice_lang(ci->bi->nick, u, PERMISSION_DENIED);
					} else
						moduleNoticeLang(ci->bi->nick, u, LANG_AOP_SYNTAX, BSFantasyCharacter);

				} else if (stricmp("del",arg1) == 0) {
					if (arg2) {
						if (my_check_access(u, ci, CA_ACCESS_CHANGE))
							do_xop(u, "AOP", ACCESS_AOP, xop_msgs[0], c, arg1, arg2);
						else
							notice_lang(ci->bi->nick, u, PERMISSION_DENIED);
					} else
						moduleNoticeLang(ci->bi->nick, u, LANG_AOP_SYNTAX, BSFantasyCharacter);

				} else if (stricmp("list",arg1) == 0) {
					if (my_check_access(u, ci, CA_ACCESS_LIST))
						do_xop(u, "AOP", ACCESS_AOP, xop_msgs[0], c, arg1, arg2);
					else
						notice_lang(ci->bi->nick, u, PERMISSION_DENIED);

				} else if (stricmp("clear",arg1) == 0) {
					if (my_check_access(u, ci, CA_ACCESS_CHANGE))
						do_xop(u, "AOP", ACCESS_AOP, xop_msgs[0], c, arg1, arg2);
					else
						notice_lang(ci->bi->nick, u, PERMISSION_DENIED);

				} else
					moduleNoticeLang(ci->bi->nick, u, LANG_AOP_SYNTAX, BSFantasyCharacter);
			} else
				moduleNoticeLang(ci->bi->nick, u, LANG_AOP_SYNTAX, BSFantasyCharacter);

			if (arg1) free(arg1);
			if (arg2) free(arg2);
		}

	} else if (ircd->halfop && (stricmp(av[0], "hop") == 0)) {
		if (ac == 3)
			moduleNoticeLang(ci->bi->nick, u, LANG_HOP_SYNTAX, BSFantasyCharacter);
		else {
			char *arg1 = myStrGetToken(av[3],' ',0);
			char *arg2 = myStrGetToken(av[3],' ',1);

			if (readonly)
				moduleNoticeLang(ci->bi->nick, u, LANG_CMD_NOT_AV);

			else if (arg1) {
				if (stricmp("add",arg1) == 0) {
					if (arg2) {
						if (my_check_access(u, ci, CA_ACCESS_CHANGE))
							do_xop(u, "HOP", ACCESS_HOP, xop_msgs[3], c, arg1, arg2);
						else
							notice_lang(ci->bi->nick, u, PERMISSION_DENIED);
					} else
						moduleNoticeLang(ci->bi->nick, u, LANG_HOP_SYNTAX, BSFantasyCharacter);

				} else if (stricmp("del",arg1) == 0) {
					if (arg2) {
						if (my_check_access(u, ci, CA_ACCESS_CHANGE))
							do_xop(u, "HOP", ACCESS_HOP, xop_msgs[3], c, arg1, arg2);
						else
							notice_lang(ci->bi->nick, u, PERMISSION_DENIED);
					} else
						moduleNoticeLang(ci->bi->nick, u, LANG_HOP_SYNTAX, BSFantasyCharacter);

				} else if (stricmp("list",arg1) == 0) {
					if (my_check_access(u, ci, CA_ACCESS_LIST))
						do_xop(u, "HOP", ACCESS_HOP, xop_msgs[3], c, arg1, arg2);
					else
						notice_lang(ci->bi->nick, u, PERMISSION_DENIED);

				} else if (stricmp("clear",arg1) == 0) {
					if (my_check_access(u, ci, CA_ACCESS_CHANGE))
						do_xop(u, "HOP", ACCESS_HOP, xop_msgs[3], c, arg1, arg2);
					else
						notice_lang(ci->bi->nick, u, PERMISSION_DENIED);

				} else
					moduleNoticeLang(ci->bi->nick, u, LANG_HOP_SYNTAX, BSFantasyCharacter);
			} else
				moduleNoticeLang(ci->bi->nick, u, LANG_HOP_SYNTAX, BSFantasyCharacter);

			if (arg1) free(arg1);
			if (arg2) free(arg2);
		}

	} else if (!(ircd->halfop) && (stricmp(av[0], "hop") == 0)) {
		moduleNoticeLang(c->ci->bi->nick, u, LANG_HOPS_UNSUPPORTED);

	} else if (stricmp(av[0], "vop") == 0) {
		if (ac == 3)
			moduleNoticeLang(ci->bi->nick, u, LANG_VOP_SYNTAX, BSFantasyCharacter);
		else {
			char *arg1 = myStrGetToken(av[3],' ',0);
			char *arg2 = myStrGetToken(av[3],' ',1);

			if (readonly)
				moduleNoticeLang(ci->bi->nick, u, LANG_CMD_NOT_AV);

			else if (arg1) {
				if (stricmp("add",arg1) == 0) {
					if (arg2) {
						if (my_check_access(u, ci, CA_ACCESS_CHANGE))
							do_xop(u, "VOP", ACCESS_VOP, xop_msgs[2], c, arg1, arg2);
						else
							notice_lang(ci->bi->nick, u, PERMISSION_DENIED);
					} else
						moduleNoticeLang(ci->bi->nick, u, LANG_VOP_SYNTAX, BSFantasyCharacter);

				} else if (stricmp("del",arg1) == 0) {
					if (arg2) {
						if (my_check_access(u, ci, CA_ACCESS_CHANGE))
							do_xop(u, "VOP", ACCESS_VOP, xop_msgs[2], c, arg1, arg2);
						else
							notice_lang(ci->bi->nick, u, PERMISSION_DENIED);
					} else
						moduleNoticeLang(ci->bi->nick, u, LANG_VOP_SYNTAX, BSFantasyCharacter);

				} else if (stricmp("list",arg1) == 0) {
					if (my_check_access(u, ci, CA_ACCESS_LIST))
						do_xop(u, "VOP", ACCESS_VOP, xop_msgs[2], c, arg1, arg2);
					else
						notice_lang(ci->bi->nick, u, PERMISSION_DENIED);

				} else if (stricmp("clear",arg1) == 0) {
					if (my_check_access(u, ci, CA_ACCESS_CHANGE))
						do_xop(u, "VOP", ACCESS_VOP, xop_msgs[2], c, arg1, arg2);
					else
						notice_lang(ci->bi->nick, u, PERMISSION_DENIED);

				} else
					moduleNoticeLang(ci->bi->nick, u, LANG_VOP_SYNTAX, BSFantasyCharacter);
			} else
				moduleNoticeLang(ci->bi->nick, u, LANG_VOP_SYNTAX, BSFantasyCharacter);

			if (arg1) free(arg1);
			if (arg2) free(arg2);
		}
	} else
#endif

#ifdef ENABLE_ACCESS
	if ((stricmp(av[0], "access") == 0) || (stricmp(av[0], "acc") == 0) || (stricmp(av[0], "axx") == 0)) {
		if (ac == 3)
			moduleNoticeLang(ci->bi->nick, u, LANG_ACCESS_SYNTAX, BSFantasyCharacter);
		else {
			char *arg1 = myStrGetToken(av[3],' ',0);
			char *arg2 = myStrGetToken(av[3],' ',1);
			char *arg3 = myStrGetToken(av[3],' ',2);

			if (readonly)
				moduleNoticeLang(ci->bi->nick, u, LANG_CMD_NOT_AV);

			else if (arg1) {
				if (stricmp("add",arg1) == 0) {
					if (arg2 && arg3) {
						if (my_check_access(u, ci, CA_ACCESS_CHANGE))
							do_access(u, c, arg1, arg2, arg3);
						else
							notice_lang(ci->bi->nick, u, PERMISSION_DENIED);
					} else
						moduleNoticeLang(ci->bi->nick, u, LANG_ACCESS_SYNTAX, BSFantasyCharacter);

				} else if (stricmp("del",arg1) == 0) {
					if (arg2) {
						if (my_check_access(u, ci, CA_ACCESS_CHANGE))
							do_access(u, c, arg1, arg2, arg3);
						else
							notice_lang(ci->bi->nick, u, PERMISSION_DENIED);
					} else
						moduleNoticeLang(ci->bi->nick, u, LANG_ACCESS_SYNTAX, BSFantasyCharacter);

				} else if (stricmp("list",arg1) == 0) {
					if (my_check_access(u, ci, CA_ACCESS_LIST))
						do_access(u, c, arg1, arg2, arg3);
					else
						notice_lang(ci->bi->nick, u, PERMISSION_DENIED);

				} else if (stricmp("clear",arg1) == 0) {
					if (my_check_access(u, ci, CA_ACCESS_CHANGE))
						do_access(u, c, arg1, arg2, arg3);
					else
						notice_lang(ci->bi->nick, u, PERMISSION_DENIED);

				} else
					moduleNoticeLang(ci->bi->nick, u, LANG_ACCESS_SYNTAX, BSFantasyCharacter);

			} else
				moduleNoticeLang(ci->bi->nick, u, LANG_ACCESS_SYNTAX, BSFantasyCharacter);

			if (arg1) free(arg1);
			if (arg2) free(arg2);
			if (arg3) free(arg3);
		}

	} else if ((stricmp(av[0], "level") == 0) || (stricmp(av[0], "levels") == 0)) {
		if (ac == 3)
			moduleNoticeLang(ci->bi->nick, u, LANG_LEVELS_SYNTAX, BSFantasyCharacter);
		else {
			char *arg1 = myStrGetToken(av[3],' ',0);
			char *arg2 = myStrGetToken(av[3],' ',1);
			char *arg3 = myStrGetToken(av[3],' ',2);

			char *error;
			short level;

			if (readonly)
				moduleNoticeLang(ci->bi->nick, u, LANG_CMD_NOT_AV);

			else if (arg1) {
				if (stricmp("set",arg1) == 0) {
					if (arg2) {
						if (arg3) {
							level = strtol(arg3, &error, 10);
							if (*error != '\0')
								moduleNoticeLang(ci->bi->nick, u, LANG_LEVELS_ERR_VAL);
							else {
								if (is_founder(u, ci) || (is_services_admin(u) && SAdminOverride))
									do_levels(u, c, arg1, arg2, level);
								else
									moduleNoticeLang(ci->bi->nick, u, LANG_ONLY_FOUNDERS);
							}
						} else
							moduleNoticeLang(ci->bi->nick, u, LANG_LEVELS_ERR_NO_VAL);
					} else
						moduleNoticeLang(ci->bi->nick, u, LANG_LEVELS_SYNTAX, BSFantasyCharacter);

				} else if ((stricmp("dis",arg1) == 0) || (stricmp("disable",arg1) == 0)) {
					if (arg2) {
						if (is_founder(u, ci) || (is_services_admin(u) && SAdminOverride))
							do_levels(u, c, arg1, arg2, 0);
						else
							moduleNoticeLang(ci->bi->nick, u, LANG_ONLY_FOUNDERS);
					} else
						moduleNoticeLang(ci->bi->nick, u, LANG_LEVELS_SYNTAX, BSFantasyCharacter);

				} else if (stricmp("list",arg1) == 0) {
					if (is_founder(u, ci) || (is_services_admin(u) && SAdminOverride))
						do_levels(u, c, arg1, arg2, 0);
					else
						moduleNoticeLang(ci->bi->nick, u, LANG_ONLY_FOUNDERS);

				} else if (stricmp("reset",arg1) == 0) {
					if (is_founder(u, ci) || (is_services_admin(u) && SAdminOverride))
						do_levels(u, c, arg1, arg2, 0);
					else
						moduleNoticeLang(ci->bi->nick, u, LANG_ONLY_FOUNDERS);

				} else
					moduleNoticeLang(ci->bi->nick, u, LANG_LEVELS_SYNTAX, BSFantasyCharacter);

			} else
				moduleNoticeLang(ci->bi->nick, u, LANG_LEVELS_SYNTAX, BSFantasyCharacter);

			if (arg1) free(arg1);
			if (arg2) free(arg2);
			if (arg3) free(arg3);
		}
	} else
#endif

#ifdef ENABLE_STAFF
	if ((stricmp(av[0], "ircops") == 0) || (stricmp(av[0], "staff") == 0))
		list_global_opers(u);
	else if (stricmp(av[0], "admins") == 0)
		list_admins(u);
	else
#endif

#ifdef ENABLE_UPDOWN
	if ((stricmp(av[0], "up") == 0) || (stricmp(av[0], "down") == 0)) {
		if (ac == 3) {
			if (c->mode & CMODE_m)
				do_up_down(u, c, av[0], 1);
			else
				do_up_down(u, c, av[0], 0);

		} else {
			char *arg1 = myStrGetToken(av[3],' ',0);
			char *arg2 = myStrGetToken(av[3],' ',1);
			User *target = finduser(arg1);

			if (target) {
				if (!is_founder(u, ci) && !is_services_root(u))
					notice_lang(ci->bi->nick, u, PERMISSION_DENIED);
				else {
					if (!arg2)
						do_up_down(target, c, av[0], 0);
					else if (!stricmp(arg2, "v"))
						do_up_down(target, c, av[0], 1);
					else if (!stricmp(arg2, "h"))
						do_up_down(target, c, av[0], 2);
					else if (!stricmp(arg2, "o"))
						do_up_down(target, c, av[0], 3);
					else if (!stricmp(arg2, "a"))
						do_up_down(target, c, av[0], 4);
					else if (!stricmp(arg2, "q"))
						do_up_down(target, c, av[0], 5);
					else
						moduleNoticeLang(ci->bi->nick, u, LANG_UPDOWN_SYNTAX, BSFantasyCharacter, BSFantasyCharacter);
				}
			} else if (!stricmp(arg1, "v"))
				if (my_check_access(u, ci, CA_VOICEME))
					do_up_down(u, c, av[0], 1);
				else
					notice_lang(ci->bi->nick, u, PERMISSION_DENIED);
			else if (!stricmp(arg1, "h"))
				if (my_check_access(u, ci, CA_HALFOPME))
					do_up_down(u, c, av[0], 2);
				else
					notice_lang(ci->bi->nick, u, PERMISSION_DENIED);
			else if (!stricmp(arg1, "o"))
				if (my_check_access(u, ci, CA_OPDEOPME))
					do_up_down(u, c, av[0], 3);
				else
					notice_lang(ci->bi->nick, u, PERMISSION_DENIED);
			else if (!stricmp(arg1, "a"))
				if (my_check_access(u, ci, CA_PROTECTME))
					do_up_down(u, c, av[0], 4);
				else
					notice_lang(ci->bi->nick, u, PERMISSION_DENIED);
			else if (!stricmp(arg1, "q"))
				if (is_founder(u, ci) || (is_services_admin(u) && SAdminOverride))
					do_up_down(u, c, av[0], 5);
				else
					notice_lang(ci->bi->nick, u, PERMISSION_DENIED);
			else
				moduleNoticeLang(ci->bi->nick, u, LANG_UPDOWN_SYNTAX, BSFantasyCharacter, BSFantasyCharacter);

			if (arg1) free(arg1);
			if (arg2) free(arg2);
		}
	} else
#endif

#ifdef ENABLE_AKICK
	if (stricmp(av[0], "akick") == 0) {
		if (ac == 3)
			moduleNoticeLang(ci->bi->nick, u, LANG_AKICK_SYNTAX, BSFantasyCharacter);
		else {
			char *arg1 = myStrGetToken(av[3],' ',0);
			char *arg2 = myStrGetToken(av[3],' ',1);
			char *arg3 = myStrGetTokenRemainder(av[3],' ',2);

			if (readonly)
				moduleNoticeLang(ci->bi->nick, u, LANG_CMD_NOT_AV);

			else if (arg1) {
				if (stricmp("add",arg1) == 0) {
					if (arg2) {
						if (my_check_access(u, ci, CA_AKICK)) {
							if (arg3)
								do_akick(u, c, arg1, arg2, arg3);
							else
								do_akick(u, c, arg1, arg2, "[Added via !akick]");
						} else
							notice_lang(ci->bi->nick, u, PERMISSION_DENIED);
					} else
						moduleNoticeLang(ci->bi->nick, u, LANG_AKICK_SYNTAX, BSFantasyCharacter);

				} else if ((stricmp("stick",arg1) == 0) || (stricmp("unstick",arg1) == 0)) {
					if (arg2) {
						if (my_check_access(u, ci, CA_AKICK)) {
							do_akick(u, c, arg1, arg2, NULL);
						} else
							notice_lang(ci->bi->nick, u, PERMISSION_DENIED);
					} else
						moduleNoticeLang(ci->bi->nick, u, LANG_AKICK_SYNTAX, BSFantasyCharacter);
				} else if (stricmp("del",arg1) == 0) {
					if (arg2) {
						if (my_check_access(u, ci, CA_AKICK)) {
							do_akick(u, c, arg1, arg2, NULL);
						} else
							notice_lang(ci->bi->nick, u, PERMISSION_DENIED);
					} else
						moduleNoticeLang(ci->bi->nick, u, LANG_AKICK_SYNTAX, BSFantasyCharacter);

				} else if ((stricmp("list",arg1) == 0) || (stricmp("view",arg1) == 0)) {
					if (my_check_access(u, ci, CA_AKICK)) {
						do_akick(u, c, arg1, arg2, NULL);
					} else
						notice_lang(ci->bi->nick, u, PERMISSION_DENIED);

				} else if ((stricmp("enforce",arg1) == 0) || (stricmp("clear",arg1) == 0)) {
					if (my_check_access(u, ci, CA_AKICK)) {
						do_akick(u, c, arg1, NULL, NULL);
					} else
						notice_lang(ci->bi->nick, u, PERMISSION_DENIED);

				} else
					moduleNoticeLang(ci->bi->nick, u, LANG_AKICK_SYNTAX, BSFantasyCharacter);

			} else
				moduleNoticeLang(ci->bi->nick, u, LANG_AKICK_SYNTAX, BSFantasyCharacter);

			if (arg1) free(arg1);
			if (arg2) free(arg2);
			if (arg3) free(arg3);
		}
	} else
#endif

#ifdef ENABLE_BADWORDS
	if (stricmp(av[0], "badwords") == 0) {
		if (ac == 3)
			moduleNoticeLang(ci->bi->nick, u, LANG_BADWORDS_SYNTAX, BSFantasyCharacter);
		else {
			char *arg1 = myStrGetToken(av[3],' ',0);
			char *arg2 = myStrGetToken(av[3],' ',1);
			char *arg3 = myStrGetToken(av[3],' ',2);

			if (readonly)
				moduleNoticeLang(ci->bi->nick, u, LANG_CMD_NOT_AV);

			else if (arg1) {
				if (stricmp("add",arg1) == 0) {
					if (arg2 && arg3) {
						if ((!stricmp(arg3,"start")) || (!stricmp(arg3,"end")) || (!stricmp(arg3,"single"))
							|| (!stricmp(arg3,"any"))) {
							if (my_check_access(u, ci, CA_BADWORDS)) {
								do_badwords(u, c, arg1, arg2, arg3);
							} else
								notice_lang(ci->bi->nick, u, PERMISSION_DENIED);
						} else {
							moduleNoticeLang(ci->bi->nick, u, LANG_BADWORDS_SYNTAX, BSFantasyCharacter);
							moduleNoticeLang(ci->bi->nick, u, LANG_BADWORDS_ERR_STYLE);
						}
					} else
						moduleNoticeLang(ci->bi->nick, u, LANG_BADWORDS_SYNTAX, BSFantasyCharacter);

				} else if (stricmp("del",arg1) == 0) {
					if (arg2) {
						if (my_check_access(u, ci, CA_BADWORDS)) {
							do_badwords(u, c, arg1, arg2, NULL);
						} else
							notice_lang(ci->bi->nick, u, PERMISSION_DENIED);
					} else
						moduleNoticeLang(ci->bi->nick, u, LANG_BADWORDS_SYNTAX, BSFantasyCharacter);

				} else if (stricmp("list",arg1) == 0) {
					if (my_check_access(u, ci, CA_BADWORDS)) {
						do_badwords(u, c, arg1, arg2, NULL);
					} else
						notice_lang(ci->bi->nick, u, PERMISSION_DENIED);

				} else if (stricmp("clear",arg1) == 0) {
					if (my_check_access(u, ci, CA_BADWORDS)) {
						do_badwords(u, c, arg1, NULL, NULL);
					} else
						notice_lang(ci->bi->nick, u, PERMISSION_DENIED);

				} else
					moduleNoticeLang(ci->bi->nick, u, LANG_BADWORDS_SYNTAX, BSFantasyCharacter);

			} else
				moduleNoticeLang(ci->bi->nick, u, LANG_BADWORDS_SYNTAX, BSFantasyCharacter);

			if (arg1) free(arg1);
			if (arg2) free(arg2);
			if (arg3) free(arg3);
		}
	} else
#endif

#ifdef ENABLE_SET
	if (stricmp(av[0], "set") == 0) {
		if (ac == 3)
			moduleNoticeLang(ci->bi->nick, u, LANG_SET_SYNTAX, BSFantasyCharacter);

		else {
			char *arg1 = myStrGetToken(av[3],' ',0);
			char *arg2 = myStrGetTokenRemainder(av[3],' ',1);

			if (readonly)
				moduleNoticeLang(ci->bi->nick, u, LANG_CMD_NOT_AV);

			else if ((!my_check_access(u, ci, CA_SET)) && (stricmp(arg1, "NOEXPIRE") != 0)
			&& (stricmp(arg1, "NOBOT") != 0)) {
				 notice_lang(ci->bi->nick, u, ACCESS_DENIED);

			/* chanserv set commands */
			} else if (stricmp(arg1, "FOUNDER") == 0) {
				if (arg2) {
					if ((ci->flags & CI_SECUREFOUNDER ? !is_real_founder(u, ci) : !is_founder(u, ci)) && !is_services_root(u)) {
						notice_lang(ci->bi->nick, u, PERMISSION_DENIED);
					} else {
						if (!SAdminOverride)
							notice_lang(ci->bi->nick, u, PERMISSION_DENIED);
						else
							do_set_founder(u, ci, arg2);
					}
				} else
					moduleNoticeLang(ci->bi->nick, u, LANG_SET_FOUNDER_SYNTAX, BSFantasyCharacter);

			} else if (stricmp(arg1, "SUCCESSOR") == 0) {
				if (arg2) {
					if ((ci->flags & CI_SECUREFOUNDER ? !is_real_founder(u, ci) : !is_founder(u, ci)) && !is_services_root(u)) {
						notice_lang(ci->bi->nick, u, PERMISSION_DENIED);
					} else {
						if (!SAdminOverride)
							notice_lang(ci->bi->nick, u, PERMISSION_DENIED);
						else
							do_set_successor(u, ci, arg2);
					}
				} else
					moduleNoticeLang(ci->bi->nick, u, LANG_SET_SUCCESSOR_SYNTAX, BSFantasyCharacter);

			} else if (stricmp(arg1, "DESC") == 0) {
				if (arg2) {
					do_set_desc(u, ci, arg2);
				} else
					moduleNoticeLang(ci->bi->nick, u, LANG_SET_DESC_SYNTAX, BSFantasyCharacter);

			} else if (stricmp(arg1, "URL") == 0) {
				do_set_url(u, ci, arg2);

			} else if (stricmp(arg1, "EMAIL") == 0) {
				do_set_email(u, ci, arg2);

			} else if (stricmp(arg1, "ENTRYMSG") == 0) {
				do_set_entrymsg(u, ci, arg2);

			} else if (stricmp(arg1, "TOPIC") == 0) {
				set_topic(u, c, arg2);

			} else if (stricmp(arg1, "BANTYPE") == 0) {
				if (arg2) {
					do_set_bantype(u, ci, arg2);
				} else
					moduleNoticeLang(ci->bi->nick, u, LANG_SET_BANTYPE_SYNTAX, BSFantasyCharacter);

			} else if (stricmp(arg1, "MLOCK") == 0) {
				if (arg2) {
					do_set_mlock(u, ci, arg2);
				} else
					moduleNoticeLang(ci->bi->nick, u, LANG_SET_MLOCK_SYNTAX, BSFantasyCharacter);

			} else if (stricmp(arg1, "KEEPTOPIC") == 0) {
				if (arg2 && ((stricmp(arg2, "ON") == 0) || (stricmp(arg2, "OFF") == 0))) {
					do_set_keeptopic(u, ci, arg2);
				} else
					moduleNoticeLang(ci->bi->nick, u, LANG_SET_KEEPTOPIC_SYNTAX, BSFantasyCharacter);

			} else if (stricmp(arg1, "TOPICLOCK") == 0) {
				if (arg2 && ((stricmp(arg2, "ON") == 0) || (stricmp(arg2, "OFF") == 0))) {
					do_set_topiclock(u, ci, arg2);
				} else
					moduleNoticeLang(ci->bi->nick, u, LANG_SET_TOPICLOCK_SYNTAX, BSFantasyCharacter);

			} else if (stricmp(arg1, "PRIVATE") == 0) {
				if (arg2 && ((stricmp(arg2, "ON") == 0) || (stricmp(arg2, "OFF") == 0))) {
					do_set_private(u, ci, arg2);
				} else
					moduleNoticeLang(ci->bi->nick, u, LANG_SET_PRIVATE_SYNTAX, BSFantasyCharacter);

			} else if (stricmp(arg1, "SECUREOPS") == 0) {
				if (arg2 && ((stricmp(arg2, "ON") == 0) || (stricmp(arg2, "OFF") == 0))) {
					do_set_secureops(u, ci, arg2);
				} else
					moduleNoticeLang(ci->bi->nick, u, LANG_SET_SECUREOPS_SYNTAX, BSFantasyCharacter);

			} else if (stricmp(arg1, "SECUREFOUNDER") == 0) {
				if (arg2 && ((stricmp(arg2, "ON") == 0) || (stricmp(arg2, "OFF") == 0))) {
					if ((ci->flags & CI_SECUREFOUNDER ? !is_real_founder(u, ci) : !is_founder(u, ci)) && !is_services_root(u)) {
						notice_lang(ci->bi->nick, u, PERMISSION_DENIED);
					} else {
						if (!SAdminOverride)
							notice_lang(ci->bi->nick, u, PERMISSION_DENIED);
						else
							do_set_securefounder(u, ci, arg2);
					}
				} else
					moduleNoticeLang(ci->bi->nick, u, LANG_SET_SECUREFOUNDER_SYNTAX, BSFantasyCharacter);

			} else if (stricmp(arg1, "RESTRICTED") == 0) {
				if (arg2 && ((stricmp(arg2, "ON") == 0) || (stricmp(arg2, "OFF") == 0))) {
					do_set_restricted(u, ci, arg2);
				} else
					moduleNoticeLang(ci->bi->nick, u, LANG_SET_RESTRICTED_SYNTAX, BSFantasyCharacter);

			} else if (stricmp(arg1, "SECURE") == 0) {
				if (arg2 && ((stricmp(arg2, "ON") == 0) || (stricmp(arg2, "OFF") == 0))) {
					do_set_secure(u, ci, arg2);
				} else
					moduleNoticeLang(ci->bi->nick, u, LANG_SET_SECURE_SYNTAX, BSFantasyCharacter);

			} else if (stricmp(arg1, "SIGNKICK") == 0) {
				if (arg2 && ((stricmp(arg2, "ON") == 0) || (stricmp(arg2, "OFF") == 0))) {
					do_set_signkick(u, ci, arg2);
				} else
					moduleNoticeLang(ci->bi->nick, u, LANG_SET_SIGNKICK_SYNTAX, BSFantasyCharacter);

			} else if (stricmp(arg1, "OPNOTICE") == 0) {
				if (arg2 && ((stricmp(arg2, "ON") == 0) || (stricmp(arg2, "OFF") == 0))) {
					do_set_opnotice(u, ci, arg2);
				} else
					moduleNoticeLang(ci->bi->nick, u, LANG_SET_OPNOTICE_SYNTAX, BSFantasyCharacter);

			} else if (stricmp(arg1, "XOP") == 0) {
				if (!(findModule("cs_xop"))) {
					notice_lang(ci->bi->nick, u, CHAN_XOP_NOT_AVAILABLE);
				} else {
					if (arg2 && ((stricmp(arg2, "ON") == 0) || (stricmp(arg2, "OFF") == 0))) {
						do_set_xop(u, ci, arg2);
					} else
						moduleNoticeLang(ci->bi->nick, u, LANG_SET_XOP_SYNTAX, BSFantasyCharacter);
				}

			} else if (stricmp(arg1, "PEACE") == 0) {
				if (arg2 && ((stricmp(arg2, "ON") == 0) || (stricmp(arg2, "OFF") == 0))) {
					do_set_peace(u, ci, arg2);
				} else
					moduleNoticeLang(ci->bi->nick, u, LANG_SET_PEACE_SYNTAX, BSFantasyCharacter);

			} else if (stricmp(arg1, "NOEXPIRE") == 0) {
				if (arg2 && ((stricmp(arg2, "ON") == 0) || (stricmp(arg2, "OFF") == 0))) {
					if (is_services_admin(u))
						do_set_noexpire(u, ci, arg2);
					else
						notice_lang(ci->bi->nick, u, PERMISSION_DENIED);
				} else
					moduleNoticeLang(ci->bi->nick, u, LANG_SET_NOEXPIRE_SYNTAX, BSFantasyCharacter);
			}

			/* botserv set commands */
			else if (stricmp(arg1, "DONTKICKOPS") == 0) {
				if (arg2 && ((stricmp(arg2, "ON") == 0) || (stricmp(arg2, "OFF") == 0))) {
					do_set(u, ci, arg1, arg2);
				} else
					moduleNoticeLang(ci->bi->nick, u, LANG_SET_DONTKICKOPS_SYNTAX, BSFantasyCharacter);
			} else if (stricmp(arg1, "DONTKICKVOICES") == 0) {
				if (arg2 && ((stricmp(arg2, "ON") == 0) || (stricmp(arg2, "OFF") == 0))) {
					do_set(u, ci, arg1, arg2);
				} else
					moduleNoticeLang(ci->bi->nick, u, LANG_SET_DONTKICKVOICES_SYNTAX, BSFantasyCharacter);
			} else if (stricmp(arg1, "GREET") == 0) {
				if (arg2 && ((stricmp(arg2, "ON") == 0) || (stricmp(arg2, "OFF") == 0))) {
					do_set(u, ci, arg1, arg2);
				} else
					moduleNoticeLang(ci->bi->nick, u, LANG_SET_GREET_SYNTAX, BSFantasyCharacter);
			} else if (stricmp(arg1, "FANTASY") == 0) {
				if (arg2 && ((stricmp(arg2, "ON") == 0) || (stricmp(arg2, "OFF") == 0))) {
					do_set(u, ci, arg1, arg2);
				} else
					moduleNoticeLang(ci->bi->nick, u, LANG_SET_FANTASY_SYNTAX, BSFantasyCharacter);
			} else if (stricmp(arg1, "SYMBIOSIS") == 0) {
				if (arg2 && ((stricmp(arg2, "ON") == 0) || (stricmp(arg2, "OFF") == 0))) {
					do_set(u, ci, arg1, arg2);
				} else
					moduleNoticeLang(ci->bi->nick, u, LANG_SET_SYMBIOSIS_SYNTAX, BSFantasyCharacter);

			} else if (stricmp(arg1, "NOBOT") == 0) {
				if (arg2 && ((stricmp(arg2, "ON") == 0) || (stricmp(arg2, "OFF") == 0))) {
					if (is_services_admin(u))
						do_set(u, ci, arg1, arg2);
					else
						notice_lang(ci->bi->nick, u, PERMISSION_DENIED);
				} else
					moduleNoticeLang(ci->bi->nick, u, LANG_SET_NOBOT_SYNTAX, BSFantasyCharacter);

			} else {
				notice_lang(ci->bi->nick, u, CHAN_SET_UNKNOWN_OPTION, arg1);
				moduleNoticeLang(ci->bi->nick, u, LANG_SET_SYNTAX, BSFantasyCharacter);
			}

			if (arg1) free(arg1);
			if (arg2) free(arg2);
		}
	} else
#endif

#ifdef ENABLE_TOPIC
	if (stricmp(av[0], "topic") == 0) {
		if (ac == 3)
			xanadu_cmd_notice(ci->bi->nick, ci->name, "Channel Topic: %s", (((c->topic))?c->topic:""));

		else {
			if (readonly)
				moduleNoticeLang(ci->bi->nick, u, LANG_CMD_NOT_AV);

			else {
				char *arg1 = myStrGetTokenRemainder(av[3],' ',0);

				if ((ci->flags & CI_TOPICLOCK) && my_check_access(u, ci, CA_TOPIC)) {
					set_topic(u, c, arg1);
				} else if (!(ci->flags & CI_TOPICLOCK) && my_check_access(u, ci, CA_OPDEOPME)) {
					set_topic(u, c, arg1);
				} else
					notice_lang(ci->bi->nick, u, PERMISSION_DENIED);

				if (arg1) free(arg1);
			}
		}
	} else

	if (stricmp(av[0], "ctopic") == 0) {
		if (readonly)
			moduleNoticeLang(ci->bi->nick, u, LANG_CMD_NOT_AV);

		else {
			if ((ci->flags & CI_TOPICLOCK) && my_check_access(u, ci, CA_TOPIC)) {
				set_topic(u, c, NULL);
			} else if (!(ci->flags & CI_TOPICLOCK) && my_check_access(u, ci, CA_OPDEOPME)) {
				set_topic(u, c, NULL);
			} else
				notice_lang(ci->bi->nick, u, PERMISSION_DENIED);
		}
	} else
#endif

#ifdef ENABLE_APPENDTOPIC
	if ((stricmp(av[0], "appendtopic") == 0) ||(stricmp(av[0], "tappend") == 0)){
		if (ac == 3)
			moduleNoticeLang(ci->bi->nick, u, LANG_APPENDTOPIC_SYNTAX, BSFantasyCharacter, BSFantasyCharacter);

		else {
			if (readonly)
				moduleNoticeLang(ci->bi->nick, u, LANG_CMD_NOT_AV);

			else {
				char *arg1 = myStrGetTokenRemainder(av[3],' ',0);

				if (my_check_access(u, ci, CA_TOPIC)) {
					append_to_topic(u, c, arg1);
				} else
					notice_lang(ci->bi->nick, u, PERMISSION_DENIED);

				if (arg1) free(arg1);
			}
		}
	} else
#endif

#ifdef ENABLE_INVITE
	if (stricmp(av[0], "invite") == 0) {
		if (ac == 3)
			moduleNoticeLang(ci->bi->nick, u, LANG_INVITE_SYNTAX, BSFantasyCharacter);

		else {
			char *arg1 = myStrGetToken(av[3],' ',0);

			if (my_check_access(u, ci, CA_INVITE)) {
				do_invite(u, c, arg1);
			} else
				notice_lang(ci->bi->nick, u, PERMISSION_DENIED);

			if (arg1) free(arg1);
		}
	} else
#endif

#ifdef ENABLE_BAN
	if ((stricmp(av[0], "ban") == 0) || (stricmp(av[0], "b") == 0)) {
		if (ac == 3) {
			if (my_check_access(u, ci, CA_BANME))
				do_ban(u, c, NULL);
			else
				notice_lang(ci->bi->nick, u, PERMISSION_DENIED);

		} else {
			char *arg1 = myStrGetToken(av[3],' ',0);

			if (my_check_access(u, ci, CA_BAN)) {
				do_ban(u, c, arg1);
			} else
				notice_lang(ci->bi->nick, u, PERMISSION_DENIED);

			if (arg1) free(arg1);
		}
	} else
#endif

#ifdef ENABLE_UNBAN
	if (stricmp(av[0], "unban") == 0) {
		if (ac == 3) {
			if (my_check_access(u, ci, CA_UNBAN))
				do_unban(u, c, NULL);
			else
				notice_lang(ci->bi->nick, u, PERMISSION_DENIED);

		} else {
			char *arg1 = myStrGetToken(av[3],' ',0);

			if (my_check_access(u, ci, CA_UNBAN)) {
				do_unban(u, c, arg1);
			} else
				notice_lang(ci->bi->nick, u, PERMISSION_DENIED);

			if (arg1) free(arg1);
		}
	} else
#endif

#ifdef ENABLE_MUTEUNMUTE
	if ((stricmp(av[0], "mute") == 0) || (stricmp(av[0], "m") == 0)) {
		if (ac == 3) {
			if (my_check_access(u, ci, CA_BANME))
				do_mute(u, c, NULL);
			else
				notice_lang(ci->bi->nick, u, PERMISSION_DENIED);

		} else {
			char *arg1 = myStrGetToken(av[3],' ',0);

			if (my_check_access(u, ci, CA_BAN)) {
				do_mute(u, c, arg1);
			} else
				notice_lang(ci->bi->nick, u, PERMISSION_DENIED);

			if (arg1) free(arg1);
		}

	} else if (stricmp(av[0], "unmute") == 0) {
		if (ac == 3) {
			if (my_check_access(u, ci, CA_UNBAN))
				do_unmute(u, c, NULL);
			else
				notice_lang(ci->bi->nick, u, PERMISSION_DENIED);

		} else {
			char *arg1 = myStrGetToken(av[3],' ',0);

			if (my_check_access(u, ci, CA_UNBAN)) {
				do_unmute(u, c, arg1);
			} else
				notice_lang(ci->bi->nick, u, PERMISSION_DENIED);

			if (arg1) free(arg1);
		}
	} else
#endif

#ifdef ENABLE_KICKBAN
	if ((stricmp(av[0], "kb") == 0) || (stricmp(av[0], "kickban") == 0)) {
		if (OverrideCoreCmds) {
			if (ac == 3) {
				if (my_check_access(u, ci, CA_BANME)) {
					moduleNoticeLang(ci->bi->nick, u, LANG_KICKBAN_SYNTAX, BSFantasyCharacter, BSFantasyCharacter);
					do_core_kickban(u, c, NULL, "Look I can fly... !");
				} else
					notice_lang(ci->bi->nick, u, PERMISSION_DENIED);

			} else {
				char *arg1 = myStrGetToken(av[3],' ',0);
				char *arg2 = myStrGetTokenRemainder(av[3],' ',1);

				if (my_check_access(u, ci, CA_BAN)) {
					do_core_kickban(u, c, arg1, arg2);
				} else
					notice_lang(ci->bi->nick, u, PERMISSION_DENIED);

				if (arg1) free(arg1);
				if (arg2) free(arg2);
			}
		}
	} else
#endif


	if (stricmp(av[0], "cmdlist") == 0) {
		do_cmd_list(u, ci);
	} else

	if ((stricmp(av[0], "minfo") == 0) || (stricmp(av[0], "modinfo") == 0)) {
		show_modinfo(u, ci);

	} else if (stricmp(av[0], "sversion") == 0) {
		show_version(u, ci);
	} else

	if (stricmp(av[0], "help") == 0) {
		if (ac == 3)
			do_help(u, ci, NULL, NULL, NULL);

		else {
			char *arg1 = myStrGetToken(av[3],' ',0);
			char *arg2 = myStrGetToken(av[3],' ',1);
			char *arg3 = myStrGetToken(av[3],' ',2);

			do_help(u, ci, arg1, arg2, arg3);

			if (arg1) free(arg1);
			if (arg2) free(arg2);
			if (arg3) free(arg3);
		}
	} else

#ifdef ENABLE_KILL
	if (stricmp(av[0], "kill") == 0) {
		if (!check_opercmds(u))
			notice_lang(ci->bi->nick, u, ACCESS_DENIED);

		else {
			if (ac == 3)
				my_do_kill(u, c, NULL, NULL);

			else {
				if (!is_services_oper(u))
					notice_lang(ci->bi->nick, u, PERMISSION_DENIED);

				else {
					char *arg1 = myStrGetToken(av[3],' ',0);
					char *arg2 = myStrGetTokenRemainder(av[3],' ',1);

					my_do_kill(u, c, arg1, arg2);

					if (arg1) free(arg1);
					if (arg2) free(arg2);
				}
			}
		}
	} else
#endif

#ifdef ENABLE_MODE
	if (stricmp(av[0], "mode") == 0) {
		if (!check_opercmds(u))
			notice_lang(ci->bi->nick, u, ACCESS_DENIED);

		else {
			if (ac == 3)
				moduleNoticeLang(ci->bi->nick, u, LANG_MODE_SYNTAX, BSFantasyCharacter);

			else if (!is_services_oper(u))
				notice_lang(ci->bi->nick, u, PERMISSION_DENIED);

			else {
				char *arg1 = myStrGetTokenRemainder(av[3],' ',0);

				do_mode(u, c, arg1);

				if (arg1) free(arg1);
			}
		}
	} else
#endif

#ifdef ENABLE_AKILL
	if (stricmp(av[0], "akill") == 0) {
		if (!check_opercmds(u))
			notice_lang(ci->bi->nick, u, ACCESS_DENIED);

		else {
			if (readonly)
				moduleNoticeLang(ci->bi->nick, u, LANG_CMD_NOT_AV);

			else {
				if (ac == 3)
					moduleNoticeLang(ci->bi->nick, u, LANG_AKILL_SYNTAX, BSFantasyCharacter);

				else {
					char *arg1 = myStrGetToken(av[3],' ',0);		/* command */
					char *arg2 = myStrGetToken(av[3],' ',1);

					if (!is_services_oper(u))
						notice_lang(ci->bi->nick, u, PERMISSION_DENIED);

					else if (stricmp(arg1, "ADD") == 0) {
						char *expiry, *mask, *reason;

						if (arg2 && *arg2 == '+') {
							expiry = arg2;
							mask = myStrGetToken(av[3],' ',2);
							reason = myStrGetTokenRemainder(av[3], ' ', 3);
						} else {
							expiry = NULL;
							mask = arg2;
							reason = myStrGetTokenRemainder(av[3], ' ', 2);
						}

						if (!mask || !reason)
							moduleNoticeLang(ci->bi->nick, u, LANG_AKILL_SYNTAX, BSFantasyCharacter);
						else
							do_akill(u, c, arg1, expiry, mask, reason);

						if ((mask) && (arg2 && *arg2 == '+')) free(mask);
						if (reason) free(reason);

					} else if (stricmp(arg1, "DEL") == 0) {
						if (!arg2)
							moduleNoticeLang(ci->bi->nick, u, LANG_AKILL_SYNTAX, BSFantasyCharacter);

						else
							do_akill(u, c, arg1, NULL, arg2, NULL);

					} else if ((stricmp(arg1, "LIST") == 0) || (stricmp(arg1, "VIEW") == 0)) {
						do_akill(u, c, arg1, NULL, arg2, NULL);

					} else if (stricmp(arg1, "CLEAR") == 0) {
						do_akill(u, c, arg1, NULL, NULL, NULL);
					} else
						moduleNoticeLang(ci->bi->nick, u, LANG_AKILL_SYNTAX, BSFantasyCharacter);

					if (arg1) free(arg1);
					if (arg2) free(arg2);
				}
			}
		}
	} else
#endif

#ifdef ENABLE_IGNORE
	if (stricmp(av[0], "ignore") == 0) {
		if (!check_opercmds(u))
			notice_lang(ci->bi->nick, u, ACCESS_DENIED);

		else {
			if (ac == 3)
				moduleNoticeLang(ci->bi->nick, u, LANG_IGNORE_SYNTAX, BSFantasyCharacter);

			else {
				char *arg1 = myStrGetToken(av[3],' ',0);
				char *arg2 = myStrGetToken(av[3],' ',1);
				char *arg3 = myStrGetToken(av[3],' ',2);

				if (!is_services_admin(u))
					notice_lang(ci->bi->nick, u, PERMISSION_DENIED);

				else if (stricmp(arg1, "add") == 0) {
					if (!arg2 || !arg3)
						moduleNoticeLang(ci->bi->nick, u, LANG_IGNORE_SYNTAX, BSFantasyCharacter);
					else
						do_ignoreuser(u, c, arg1, arg3, arg2);
				} else if (stricmp(arg1, "del") == 0) {
					if (!arg2)
						moduleNoticeLang(ci->bi->nick, u, LANG_IGNORE_SYNTAX, BSFantasyCharacter);
					else
						do_ignoreuser(u, c, arg1, arg2, NULL);
				} else if ((stricmp(arg1, "list") == 0) || (stricmp(arg1, "clear") == 0)) {
					do_ignoreuser(u, c, arg1, NULL, NULL);
				} else
					moduleNoticeLang(ci->bi->nick, u, LANG_IGNORE_SYNTAX, BSFantasyCharacter);

				if (arg1) free(arg1);
				if (arg2) free(arg2);
				if (arg3) free(arg3);
			}
		}
	} else if (stricmp(av[0], "unignore") == 0) {
		if (!check_opercmds(u))
			notice_lang(ci->bi->nick, u, ACCESS_DENIED);

		else {
			if (ac == 3)
				moduleNoticeLang(ci->bi->nick, u, LANG_IGNORE_SYNTAX, BSFantasyCharacter);

			else {
				char *arg1 = myStrGetToken(av[3],' ',0);

				if (!is_services_admin(u))
					notice_lang(ci->bi->nick, u, PERMISSION_DENIED);

				else {
					if (!arg1)
						moduleNoticeLang(ci->bi->nick, u, LANG_IGNORE_SYNTAX, BSFantasyCharacter);
					else
						do_ignoreuser(u, c, "del", arg1, NULL);
				}

				if (arg1) free(arg1);
			}
		}
	} else
#endif

#ifdef ENABLE_INFO
	if (stricmp(av[0], "info") == 0) {
		if (ac == 3)
			do_info(u, c, NULL);

		else {
			char *arg1 = myStrGetToken(av[3],' ',0);

			if (!arg1)
				do_info(u, c, NULL);
			else if (stricmp(arg1, "all") == 0)
				do_info(u, c, arg1);
			else
				moduleNoticeLang(ci->bi->nick, u, LANG_INFO_SYNTAX, BSFantasyCharacter);

			if (arg1) free(arg1);
		}
	} else
#endif

#ifdef ENABLE_BKICK
	if (stricmp(av[0], "bkick") == 0) {
		if (ac == 3)
			moduleNoticeLang(ci->bi->nick, u, LANG_BKICK_SYNTAX, BSFantasyCharacter);

		else {
			char *arg1 = myStrGetToken(av[3],' ',0);
			char *arg2 = myStrGetToken(av[3],' ',1);
			char *arg3 = myStrGetToken(av[3],' ',2);
			char *arg4 = myStrGetToken(av[3],' ',3);
			char *arg5 = myStrGetToken(av[3],' ',4);

			if (readonly)
				moduleNoticeLang(ci->bi->nick, u, LANG_CMD_NOT_AV);

			else if (!my_check_access(u, ci, CA_SET))
				 notice_lang(ci->bi->nick, u, ACCESS_DENIED);

			else if (stricmp(arg1, "BOLDS") == 0) {
				if (arg2 && ((stricmp(arg2, "ON") == 0) || (stricmp(arg2, "OFF") == 0)))
					do_set_kick(u, ci, arg1, arg2, arg3, NULL, NULL);
				else
					moduleNoticeLang(ci->bi->nick, u, LANG_BKICK_BOLDS_SYNTAX, BSFantasyCharacter);

			} else if (stricmp(arg1, "BADWORDS") == 0) {
				if (arg2 && ((stricmp(arg2, "ON") == 0) || (stricmp(arg2, "OFF") == 0)))
					do_set_kick(u, ci, arg1, arg2, arg3, NULL, NULL);
				else
					moduleNoticeLang(ci->bi->nick, u, LANG_BKICK_BADWORDS_SYNTAX, BSFantasyCharacter);

			} else if (stricmp(arg1, "COLORS") == 0) {
				if (arg2 && ((stricmp(arg2, "ON") == 0) || (stricmp(arg2, "OFF") == 0)))
					do_set_kick(u, ci, arg1, arg2, arg3, NULL, NULL);
				else
					moduleNoticeLang(ci->bi->nick, u, LANG_BKICK_COLORS_SYNTAX, BSFantasyCharacter);

			} else if (stricmp(arg1, "REVERSES") == 0) {
				if (arg2 && ((stricmp(arg2, "ON") == 0) || (stricmp(arg2, "OFF") == 0)))
					do_set_kick(u, ci, arg1, arg2, arg3, NULL, NULL);
				else
					moduleNoticeLang(ci->bi->nick, u, LANG_BKICK_REVERSES_SYNTAX, BSFantasyCharacter);


			} else if (stricmp(arg1, "UNDERLINES") == 0) {
				if (arg2 && ((stricmp(arg2, "ON") == 0) || (stricmp(arg2, "OFF") == 0)))
					do_set_kick(u, ci, arg1, arg2, arg3, NULL, NULL);
				else
					moduleNoticeLang(ci->bi->nick, u, LANG_BKICK_UNDERLINES_SYNTAX, BSFantasyCharacter);

			} else if (stricmp(arg1, "CAPS") == 0) {
				if (arg2 && ((stricmp(arg2, "ON") == 0) || (stricmp(arg2, "OFF") == 0)))
					do_set_kick(u, ci, arg1, arg2, arg3, arg4, arg5);
				else
					moduleNoticeLang(ci->bi->nick, u, LANG_BKICK_UNDERLINES_SYNTAX, BSFantasyCharacter);

			} else if (stricmp(arg1, "FLOOD") == 0) {
				if (arg2 && ((stricmp(arg2, "ON") == 0) || (stricmp(arg2, "OFF") == 0)))
					do_set_kick(u, ci, arg1, arg2, arg3, arg4, arg5);
				else
					moduleNoticeLang(ci->bi->nick, u, LANG_BKICK_UNDERLINES_SYNTAX, BSFantasyCharacter);

			} else if (stricmp(arg1, "REPEAT") == 0) {
				if (arg2 && ((stricmp(arg2, "ON") == 0) || (stricmp(arg2, "OFF") == 0)))
					do_set_kick(u, ci, arg1, arg2, arg3, arg4, NULL);
				else
					moduleNoticeLang(ci->bi->nick, u, LANG_BKICK_UNDERLINES_SYNTAX, BSFantasyCharacter);

			} else {
				moduleNoticeLang(ci->bi->nick, u, LANG_BKICK_SYNTAX, BSFantasyCharacter);
			}

			if (arg1) free(arg1);
			if (arg2) free(arg2);
			if (arg3) free(arg3);
			if (arg4) free(arg4);
			if (arg5) free(arg5);
		}
	} else
#endif

#ifdef ENABLE_TBAN
	if (stricmp(av[0], "tb") == 0 || stricmp(av[0], "tban") == 0) {
		if (ac == 3) {
			if (my_check_access(u, ci, CA_BANME))
				do_tban(u, c, NULL, 3600);
			else
				notice_lang(ci->bi->nick, u, PERMISSION_DENIED);

		} else {
			int time;
			char *arg1 = myStrGetToken(av[3],' ',0);
			char *arg2 = myStrGetToken(av[3],' ',1);

			if (my_check_access(u, ci, CA_BAN)) {
				if (!arg2)
					do_tban(u, c, arg1, 3600);
				else if ((time = dotime(arg2)) == -1)
					moduleNoticeLang(ci->bi->nick, u, LANG_INVALID_TIME, arg2);
				else
					do_tban(u, c, arg1, time);
			} else
				notice_lang(ci->bi->nick, u, PERMISSION_DENIED);

			if (arg1) free(arg1);
			if (arg2) free(arg2);
		}
	} else
#endif

#ifdef ENABLE_TKICKBAN
	if (stricmp(av[0], "tkb") == 0 || stricmp(av[0], "tkban") == 0) {
		if (ac == 3) {
			if (my_check_access(u, ci, CA_BANME)) {
				moduleNoticeLang(ci->bi->nick, u, LANG_TKBAN_SYNTAX, BSFantasyCharacter, BSFantasyCharacter);
				do_tkban(u, c, NULL, 3600, "Look I can fly... !");
			} else
				notice_lang(ci->bi->nick, u, PERMISSION_DENIED);

		} else {
			int time;
			char *arg3, *arg1 = myStrGetToken(av[3],' ',0);
			char *arg2 = myStrGetToken(av[3],' ',1);

			if ((time = dotime(arg2)) == -1) {
				arg3 = myStrGetTokenRemainder(av[3],' ',1);
				time = 3600;
			} else
				arg3 = myStrGetTokenRemainder(av[3],' ',2);

			if (my_check_access(u, ci, CA_BAN)) {
				do_tkban(u, c, arg1, time, arg3);
			} else
				notice_lang(ci->bi->nick, u, PERMISSION_DENIED);

			if (arg1) free(arg1);
			if (arg2) free(arg2);
			if (arg3) free(arg3);
		}
	} else
#endif

#ifdef ENABLE_SUSPEND
	if (stricmp(av[0], "suspend") == 0) {
		if (!check_opercmds(u))
			notice_lang(ci->bi->nick, u, ACCESS_DENIED);

		else {
			if (ac == 3)
				moduleNoticeLang(ci->bi->nick, u, LANG_SUSPEND_SYNTAX, BSFantasyCharacter);

			else {
				char *arg1 = myStrGetTokenRemainder(av[3],' ',0);

				if (!is_services_oper(u))
					notice_lang(ci->bi->nick, u, PERMISSION_DENIED);
				else
					do_suspend(u, ci, arg1);

				if (arg1) free(arg1);
			}
		}
	} else
#endif

#ifdef ENABLE_SYNC
	if (stricmp(av[0], "sync") == 0) {
		if (en_sync) {
			if (ac == 3) {
				if (!is_founder(u, ci) && !is_services_root(u))
					notice_lang(ci->bi->nick, u, PERMISSION_DENIED);
				else
					do_sync(u, c);
			} else
				moduleNoticeLang(ci->bi->nick, u, LANG_SYNC_SYNTAX, BSFantasyCharacter);
		} else
			moduleNoticeLang(ci->bi->nick, u, LANG_CMD_DISABLED);
	} else
#endif

#ifdef ENABLE_SHUN
	if (stricmp(av[0], "shun") == 0) {
		if (!check_opercmds(u))
			notice_lang(ci->bi->nick, u, ACCESS_DENIED);

		else {
			if (en_shun) {
				if (ac == 3)
					moduleNoticeLang(ci->bi->nick, u, LANG_SHUN_SYNTAX, BSFantasyCharacter);

				else {
					if (!is_services_oper(u))
						notice_lang(ci->bi->nick, u, PERMISSION_DENIED);

					else {
						/* All sanity checks are handled there.. */
						do_shun(u, c, av[3]);
					}
				}
			} else
				moduleNoticeLang(ci->bi->nick, u, LANG_CMD_DISABLED);
		}
	} else
#endif

#ifdef ENABLE_TSHUN
	if (stricmp(av[0], "tshun") == 0) {
		if (!check_opercmds(u))
			notice_lang(ci->bi->nick, u, ACCESS_DENIED);

		else {
			if (en_shun) {
				if (ac == 3)
					moduleNoticeLang(ci->bi->nick, u, LANG_TSHUN_SYNTAX, BSFantasyCharacter);

				else {
					if (!is_services_oper(u))
						notice_lang(ci->bi->nick, u, PERMISSION_DENIED);

					else {
						/* All sanity checks are handled there.. */
						do_tshun(u, c, av[3]);
					}
				}
			} else
				moduleNoticeLang(ci->bi->nick, u, LANG_CMD_DISABLED);
		}
	} else
#endif

		/* if it didn't match any of the strings */
		return MOD_CONT;

	/* if it matched, it ends here */
	return MOD_CONT;
}


/**
 * Handles all denied fantasy commands.
 * People without chanserv access should still have axx to some fantasy commands.
 * Here we ll identify the command and call the right routines.
 **/
int do_fantasy_denied(int ac, char **av) {
	User *u;
	ChannelInfo *ci;
	Channel *c;

	if (!enabled)
		return MOD_CONT;

	/* Some basic error checking... should never match */
	if (ac < 3)
		return MOD_CONT;

	if (!(ci = cs_findchan(av[2])))
		return MOD_CONT;
	if (!(u = finduser(av[1])))
		return MOD_CONT;
	if (!(c = findchan(ci->name)))
		return MOD_CONT;

	if (is_services_admin(u)) {
		do_fantasy(ac, av);
		return MOD_CONT;
	}

#ifdef ENABLE_STAFF
	if ((stricmp(av[0], "ircops") == 0) || (stricmp(av[0], "staff") == 0))
		list_global_opers(u);
	else if (stricmp(av[0], "admins") == 0)
		list_admins(u);
	else
#endif

	if (stricmp(av[0], "cmdlist") == 0) {
		do_cmd_list(u, ci);
	} else

	if ((stricmp(av[0], "minfo") == 0) || (stricmp(av[0], "modinfo") == 0)) {
		show_modinfo(u, ci);

	} else if (stricmp(av[0], "sversion") == 0) {
		show_version(u, ci);
	} else

	if (stricmp(av[0], "help") == 0) {
		if (ac == 3)
			do_help(u, ci, NULL, NULL, NULL);

		else {
			char *arg1 = myStrGetToken(av[3],' ',0);
			char *arg2 = myStrGetToken(av[3],' ',1);
			char *arg3 = myStrGetToken(av[3],' ',2);

			do_help(u, ci, arg1, arg2, arg3);

			if (arg1) free(arg1);
			if (arg2) free(arg2);
			if (arg3) free(arg3);
		}
	} else

#ifdef ENABLE_KILL
	if (stricmp(av[0], "kill") == 0) {
		if (!check_opercmds(u))
			notice_lang(ci->bi->nick, u, ACCESS_DENIED);

		else {
			if (ac == 3)
				my_do_kill(u, c, NULL, NULL);

			else {
				char *arg1 = myStrGetToken(av[3],' ',0);
				char *arg2 = myStrGetTokenRemainder(av[3],' ',1);

				my_do_kill(u, c, arg1, arg2);

				if (arg1) free(arg1);
				if (arg2) free(arg2);
			}
		}
	} else
#endif

#ifdef ENABLE_MODE
	if (stricmp(av[0], "mode") == 0) {
		if (!check_opercmds(u))
			notice_lang(ci->bi->nick, u, ACCESS_DENIED);

		else {
			if (ac == 3)
				moduleNoticeLang(ci->bi->nick, u, LANG_MODE_SYNTAX, BSFantasyCharacter);

			else if (!is_services_oper(u))
				notice_lang(ci->bi->nick, u, PERMISSION_DENIED);

			else {
				char *arg1 = myStrGetTokenRemainder(av[3],' ',0);

				do_mode(u, c, arg1);

				if (arg1) free(arg1);
			}
		}
	} else
#endif

#ifdef ENABLE_AKILL
	if (stricmp(av[0], "akill") == 0) {
		if (!check_opercmds(u))
			notice_lang(ci->bi->nick, u, ACCESS_DENIED);

		else {
			if (readonly)
				moduleNoticeLang(ci->bi->nick, u, LANG_CMD_NOT_AV);

			else {
				if (ac == 3)
					moduleNoticeLang(ci->bi->nick, u, LANG_AKILL_SYNTAX, BSFantasyCharacter);

				else if (!is_services_oper(u))
					notice_lang(ci->bi->nick, u, PERMISSION_DENIED);

				else {
					char *arg1 = myStrGetToken(av[3],' ',0);
					char *arg2 = myStrGetToken(av[3],' ',1);

					if (stricmp(arg1, "ADD") == 0) {
						char *expiry, *mask, *reason;

						if (arg2 && *arg2 == '+') {
							expiry = arg2;
							mask = myStrGetToken(av[3],' ',2);
							reason = myStrGetTokenRemainder(av[3], ' ', 3);
						} else {
							expiry = NULL;
							mask = arg2;
							reason = myStrGetTokenRemainder(av[3], ' ', 2);
						}

						if (!mask || !reason)
							moduleNoticeLang(ci->bi->nick, u, LANG_AKILL_SYNTAX, BSFantasyCharacter);
						else
							do_akill(u, c, arg1, expiry, mask, reason);

						if ((mask) && (arg2 && *arg2 == '+')) free(mask);
						if (reason) free(reason);

					} else if (stricmp(arg1, "DEL") == 0) {
						if (!arg2)
							moduleNoticeLang(ci->bi->nick, u, LANG_AKILL_SYNTAX, BSFantasyCharacter);

						else
							do_akill(u, c, arg1, NULL, arg2, NULL);

					} else if ((stricmp(arg1, "LIST") == 0) || (stricmp(arg1, "VIEW") == 0)) {
						do_akill(u, c, arg1, NULL, arg2, NULL);

					} else if (stricmp(arg1, "CLEAR") == 0) {
						do_akill(u, c, arg1, NULL, NULL, NULL);
					} else
						moduleNoticeLang(ci->bi->nick, u, LANG_AKILL_SYNTAX, BSFantasyCharacter);

					if (arg1) free(arg1);
					if (arg2) free(arg2);
				}
			}
		}
	} else
#endif

#ifdef ENABLE_IGNORE
	if (stricmp(av[0], "ignore") == 0) {
		if (!check_opercmds(u))
			notice_lang(ci->bi->nick, u, ACCESS_DENIED);

		else {
			if (ac == 3)
				moduleNoticeLang(ci->bi->nick, u, LANG_IGNORE_SYNTAX, BSFantasyCharacter);

			else {
				char *arg1 = myStrGetToken(av[3],' ',0);
				char *arg2 = myStrGetToken(av[3],' ',1);
				char *arg3 = myStrGetToken(av[3],' ',2);

				if (!is_services_admin(u))
					notice_lang(ci->bi->nick, u, PERMISSION_DENIED);

				else if (stricmp(arg1, "add") == 0) {
					do_ignoreuser(u, c, arg1, arg3, arg2);
				} else if (stricmp(arg1, "del") == 0) {
					do_ignoreuser(u, c, arg1, arg2, NULL);
				} else if ((stricmp(arg1, "list") == 0) || (stricmp(arg1, "clear") == 0)) {
					do_ignoreuser(u, c, arg1, NULL, NULL);
				} else
					moduleNoticeLang(ci->bi->nick, u, LANG_IGNORE_SYNTAX, BSFantasyCharacter);

				if (arg1) free(arg1);
				if (arg2) free(arg2);
				if (arg3) free(arg3);
			}
		}
	} else if (stricmp(av[0], "unignore") == 0) {
		if (!check_opercmds(u))
			notice_lang(ci->bi->nick, u, ACCESS_DENIED);

		else {
			if (ac == 3)
				moduleNoticeLang(ci->bi->nick, u, LANG_IGNORE_SYNTAX, BSFantasyCharacter);

			else {
				char *arg1 = myStrGetToken(av[3],' ',0);

				if (!is_services_admin(u))
					notice_lang(ci->bi->nick, u, PERMISSION_DENIED);

				else {
					if (!arg1)
						moduleNoticeLang(ci->bi->nick, u, LANG_IGNORE_SYNTAX, BSFantasyCharacter);
					else
						do_ignoreuser(u, c, "del", arg1, NULL);
				}

				if (arg1) free(arg1);
			}
		}
	} else
#endif

#ifdef ENABLE_INFO
	if (stricmp(av[0], "info") == 0) {
		if (ac == 3)
			do_info(u, c, NULL);

		else {
			char *arg1 = myStrGetToken(av[3],' ',0);

			if (!arg1)
				do_info(u, c, NULL);
			else if (stricmp(arg1, "all") == 0)
				do_info(u, c, arg1);
			else
				moduleNoticeLang(ci->bi->nick, u, LANG_INFO_SYNTAX, BSFantasyCharacter);

			if (arg1) free(arg1);
		}
	} else
#endif

#ifdef ENABLE_SUSPEND
	if (stricmp(av[0], "suspend") == 0) {
		if (!check_opercmds(u))
			notice_lang(ci->bi->nick, u, ACCESS_DENIED);

		else {
			if (ac == 3)
				moduleNoticeLang(ci->bi->nick, u, LANG_SUSPEND_SYNTAX, BSFantasyCharacter);

			else {
				char *arg1 = myStrGetTokenRemainder(av[3],' ',0);

				if (!is_services_oper(u))
					notice_lang(ci->bi->nick, u, PERMISSION_DENIED);
				else
					do_suspend(u, ci, arg1);

				if (arg1) free(arg1);
			}
		}
	} else
#endif

#ifdef ENABLE_TOPIC
	if (stricmp(av[0], "topic") == 0) {
		if (ac == 3)
			xanadu_cmd_notice(ci->bi->nick, ci->name, "Channel Topic: %s", c->topic);
		else
			notice_lang(ci->bi->nick, u, ACCESS_DENIED);
	} else
#endif

#ifdef ENABLE_SHUN
	if (stricmp(av[0], "shun") == 0) {
		if (!check_opercmds(u))
			notice_lang(ci->bi->nick, u, ACCESS_DENIED);

		else {
			if (en_shun) {
				if (ac == 3)
					moduleNoticeLang(ci->bi->nick, u, LANG_SHUN_SYNTAX, BSFantasyCharacter);

				else {
					if (!is_services_oper(u))
						notice_lang(ci->bi->nick, u, PERMISSION_DENIED);

					else {
						/* All sanity checks are handled there.. */
						do_shun(u, c, av[3]);
					}
				}
			} else
				moduleNoticeLang(ci->bi->nick, u, LANG_CMD_DISABLED);
		}
	} else
#endif

#ifdef ENABLE_TSHUN
	if (stricmp(av[0], "tshun") == 0) {
		if (!check_opercmds(u))
			notice_lang(ci->bi->nick, u, ACCESS_DENIED);

		else {
			if (en_shun) {
				if (ac == 3)
					moduleNoticeLang(ci->bi->nick, u, LANG_TSHUN_SYNTAX, BSFantasyCharacter);

				else {
					if (!is_services_oper(u))
						notice_lang(ci->bi->nick, u, PERMISSION_DENIED);

					else {
						/* All sanity checks are handled there.. */
						do_tshun(u, c, av[3]);
					}
				}
			} else
				moduleNoticeLang(ci->bi->nick, u, LANG_CMD_DISABLED);
		}
	} else
#endif

		/* if it didn't match any of the strings */
		return MOD_CONT;

	/* if it matched, it ends here */
	return MOD_CONT;
}

/* EOF */

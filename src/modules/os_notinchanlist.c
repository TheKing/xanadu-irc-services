/**
 * -----------------------------------------------------------------------------
 * Name    : os_notinchanlist
 * Author  : Viper  <Viper@Absurd-IRC.net>
 * Date    : 05/08/2006  (Last update: 01/01/2008)
 * Version : 2.3
 * -----------------------------------------------------------------------------
 * Limitations : JOIN, SHUN and TSHUN options are IRCd dependant.
 * Requires    : Anope-1.7.20
 * Tested      : Anope-1.7.19 (SVN) + UnrealIRCd 3.2.6
 * -----------------------------------------------------------------------------
 * This module adds the NOTINCHANLIST command to operserv. I allows Services Admins
 * to list all users not in any channel and to take action against them.
 * For more info < /operserv help notinchanlist > once the module is loaded.
 *
 * The JOIN argument is only available on following IRCd's:
 *   - InspIRCd
 *   - Plexus 3.0 [or later]
 *   - PTLink
 *   - UltimateIRCd 2.8.2 [or later] [Excluding 3.x]
 *   - Unreal 3.1.1 [or later]
 *   - Unreal 3.2 [or later]
 *   - ViagraIRCd 1.3.x [or later]
 *
 * The SHUN and TSHUN arguments are only available on following IRCd's:
 *   - Unreal 3.2 [or later]
 *
 * This module allows services admins to list all clients that are
 * not in any channels and take actions: notice, kill, akill and
 * join (if supported).
 *
 * This version of this module is a rewrite of the original os_notinchanlist
 * module by SGR and some of the code has been kept when rewriting
 * the module for 1.7 so a lot of the credit goes to him.
 *
 * -----------------------------------------------------------------------------
 *
 * Changes:
 *
 *  2.3  Updated module to work with anope 1.7.20. Dropped support for older versions.
 *       Fixed some compiler errors & warnings.
 *       Reworked the help (Thx to Fudge)
 *
 *  2.2  Some minor cleaning up done.
 *       Few minor edits to the help.
 *       Added TSHUN parameter which uses unreals' tempshun functionality.
 *       Added FAKILL parameter, identical akill but always enforces, even if
 *           AkillOnAdd is not set.
 *
 *  2.1S Added Shun for UnrealIRCd
 *
 *  2.1b Fixed an error in english lang.
 *
 *  2.1  Fixed a few minor bugs
 *       Added config directives to services.conf for public release
 *       Added dutch translations
 *
 *  2.0  First release by me
 *       Rewrite to a 1.7 style module
 *       No longer supporting 1.6 branch
 *
 *
 * -----------------------------------------------------------------------------
 **/

/**
 * Configuration directives that should be copy-pasted to services.conf

# OSNotInChanAkillTime [OPTIONAL]
# Module: os_notinchanlist
#
# This determines how long users will be Akilled if the AKILL argument of the notinchanlist
# is specified. Please note this is subject to ExpireTimeout.
# If this is not specified a default value of 30 minutes will be used instead.
#OSNotInChanAkillTime 2700s

# OSNotInChanKillReason [OPTIONAL]
# Module: os_notinchanlist
#
# The reason that is given when killing users after notinchanlist is executed
# with the KILL argument.
# If this is not specified a default message will be used instead.
#OSNotInChanKillReason "Client not in any channels, possibly a malicious bot."

# OSNotInChanAkillReason [OPTIONAL]
# Module: os_notinchanlist
#
# The reason that is given when akilling users after notinchanlist is executed
# with the AKILL argument.
# If this is not specified a default message will be used instead.
#OSNotInChanAkillReason "Temporary idle (not in any channel) AKILL."

# OSNotInChanIdleChan [OPTIONAL]
# Module: os_notinchanlist
#
# This specifies to which channel users will be joined when notinchanlist is
# executed with the JOIN argument. (Not needed if ircd is not supported.)
# If this is not specified a default channel (#idle) will be used instead.
#OSNotInChanIdleChan "#idle"

 *
 **/

/*------------------------------Configuration Block----------------------------*/


// This module has no compile time configuration directives...


/*-------------------------End of Configuration Block--------------------------*/

#include "module.h"
#include <stdio.h>

#define AUTHOR "Viper"
#define VERSION "2.3"


/* Language defines */
#define LANG_NUM_STRINGS 					23

#define LANG_NOTINCHAN_DESC					0
#define LANG_NOTINCHAN_SYNTAX				1
#define LANG_NOTINCHAN_SYNTAX_EXT			2
#define LANG_NOTINCHANLIST_NOTICE			3
#define LANG_NOTINCHANLIST_KILL				4
#define LANG_NOTINCHANLIST_JOIN				5
#define LANG_JOIN_NOT_SUPPORTED				6
#define LANG_QUIET_NO_ACTION				7
#define LANG_ACTION_SECOND					8
#define LANG_SEARCHING						9
#define LANG_NOTINCHANLIST_MATCH			10
#define LANG_NOTINCHANLIST_NOTICING			11
#define LANG_NOTINCHANLIST_KILLING			12
#define LANG_NOTINCHANLIST_AKILLING			13
#define LANG_NOTINCHANLIST_JOINING			14
#define LANG_SEARCHING_COMPLETE				15
#define LANG_SEARCHING_RESULT				16
#define LANG_NOTINCHANLIST_SHUN				17
#define LANG_SHUN_NOT_SUPPORTED				18
#define LANG_NOTINCHANLIST_SHUNING			19
#define LANG_NOTINCHANLIST_TSHUN			20
#define LANG_TSHUN_NOT_SUPPORTED			21
#define LANG_NOTINCHANLIST_TSHUNING			22


/* variables */
int niclAkillTime;
char *niclKillReason, *niclAkillReason, *niclIdleChan;


/* constants */
char *DefKillReason = "Client not in any channels, possibly a mailcious bot.";
char *DefAkillReason = "Temporary idle (not in any channel) AKILL.";
char *DefJoinChan = "#idle";


/* Functions */
void do_help_list(User *u);
int do_help(User *u);
int do_find_nochan_users(User * u);
int parse_action(char *action);
void load_config(void);
int reload_config(int argc, char **argv);
void add_languages(void);


/**
 * Create the command, and tell anope about it.
 * @param argc Argument count
 * @param argv Argument list
 * @return MOD_CONT to allow the module, MOD_STOP to stop it
 **/
int AnopeInit(int argc, char **argv) {
	Command *c;
	EvtHook *hook;
	int status;

	if (!moduleMinVersion(1,7,19,1291)) {
		alog("[\002os_notinchanlist\002] Your version of Anope isn't supported. Please update to a newer release.");
		return MOD_STOP;
	}

	c = createCommand("NOTINCHANLIST", do_find_nochan_users, is_services_admin, -1, -1, -1, -1, -1);
	status = moduleAddCommand(OPERSERV,c,MOD_HEAD);
	moduleAddHelp(c,do_help);
	moduleSetOperHelp(do_help_list);

	alog("[\002os_notinchanlist\002] Loading module... [Status: %d]", status);
	if (status != MOD_ERR_OK) {
		alog("[\002os_notinchanlist\002] Aborting loading...");
		return MOD_STOP;
	}

	hook = createEventHook(EVENT_RELOAD, reload_config);
	if (moduleAddEventHook(hook) != MOD_ERR_OK) {
		alog("[\002os_notinchanlist\002] Can't hook to EVENT_RELOAD event");
		return MOD_STOP;
	}

	load_config();
	add_languages();

	moduleAddAuthor(AUTHOR);
	moduleAddVersion(VERSION);

	alog("[\002os_notinchanlist\002] Command added. For more info: /%s HELP NOTINCHANLIST", s_OperServ);

	return MOD_CONT;
}


/**
 * Unload the module
 **/
void AnopeFini(void) {
	alog("[\002os_notinchanlist\002] Unloading module...");

	/* free vars */
	if (niclKillReason) free(niclKillReason);
	if (niclAkillReason) free(niclAkillReason);
	if (niclIdleChan) free(niclIdleChan);
}


/* ------------------------------------------------------------------------------- */


/**
 * Add the NOTINCHANLIST command to the OperServ HELP listing
 **/
void do_help_list(User *u) {
	if (is_services_admin(u)) {
		moduleNoticeLang(s_OperServ, u, LANG_NOTINCHAN_DESC);
	}
}


/**
 * Show the extended help on the NOTINCHANLIST command
 **/
int do_help(User *u) {
	if (is_services_admin(u))
		moduleNoticeLang(s_OperServ, u, LANG_NOTINCHAN_SYNTAX_EXT, s_OperServ, niclIdleChan, s_NickServ);

	return MOD_CONT;
}


/* ------------------------------------------------------------------------------- */

/**
 * Main function to process the command...
 **/
int do_find_nochan_users(User * u) {
	char mask[BUFSIZE], akillmask[BUFSIZE];
	int j, notid = 0, ok = 1, action = 0, quiet = 0, count = 0, total = 0;
	User *next, *u2;
	char *cur_buffer, *options1, *options2, *options3;

	cur_buffer = moduleGetLastBuffer();
	options1 = myStrGetToken(cur_buffer, ' ', 0);
	options2 = myStrGetToken(cur_buffer, ' ', 1);
	options3 = myStrGetTokenRemainder(cur_buffer, ' ', 2);

	if (options1) {
		if (stricmp(options1,"quiet") == 0) {
			moduleNoticeLang(s_OperServ, u, LANG_NOTINCHAN_SYNTAX);
			moduleNoticeLang(s_OperServ, u, LANG_QUIET_NO_ACTION);
			ok = 0;
		} else if (stricmp(options1,"notid") == 0) {
			notid = 1;
		} else {
			action = parse_action(options1);
			if (!action) {
				moduleNoticeLang(s_OperServ, u, LANG_NOTINCHAN_SYNTAX);
				ok = 0;
			}
		}
	}

	if (options2 && ok) {
		if ((stricmp(options2,"kill") == 0) || (stricmp(options2,"akill") == 0) || (stricmp(options2,"notice") == 0)|| (stricmp(options2,"join") == 0)) {
			moduleNoticeLang(s_OperServ, u, LANG_NOTINCHAN_SYNTAX);
			moduleNoticeLang(s_OperServ, u, LANG_ACTION_SECOND);
			ok = 0;
		} else if (stricmp(options2,"notid") == 0)
			notid = 1;
		else if (stricmp(options2,"quiet") == 0 && action == 0) {
			moduleNoticeLang(s_OperServ, u, LANG_NOTINCHAN_SYNTAX);
			moduleNoticeLang(s_OperServ, u, LANG_QUIET_NO_ACTION);
			ok = 0;
		} else if (stricmp(options2,"quiet") == 0)
			quiet = 1;
		else {
			moduleNoticeLang(s_OperServ, u, LANG_NOTINCHAN_SYNTAX);
			ok = 0;
		}
	}


	if (options3 && ok) {
		if (stricmp(options3,"notid") == 0) {
			notid = 1;
		} else {
			moduleNoticeLang(s_OperServ, u, LANG_NOTINCHAN_SYNTAX);
			ok = 0;
		}
	}

	if (ok) {
		moduleNoticeLang(s_OperServ, u,LANG_SEARCHING);

		for (j = 0; j < 1024; j++) {
			for (u2 = userlist[j]; u2; u2 = next) {
				next = u2->next;
				if (finduser((u2->nick))) {
					total++;

					if (notid)
						if (nick_identified(u2))
							 continue;

					if (!is_oper(u2) && !u2->chans) {
						snprintf(mask, sizeof(mask), "%s!%s@%s", u2->nick, u2->username, common_get_vhost(u2));
						count++;

						if (action == 0) {
							moduleNoticeLang(s_OperServ, u, LANG_NOTINCHANLIST_MATCH, u2->nick, mask);

						} else {
							if (!quiet)
								moduleNoticeLang(s_OperServ, u, LANG_NOTINCHANLIST_MATCH, u2->nick, mask);

							if (action == 1) {
								if (!quiet)
									moduleNoticeLang(s_OperServ, u, LANG_NOTINCHANLIST_NOTICING, u2->nick);

								moduleNoticeLang(s_OperServ, u2, LANG_NOTINCHANLIST_NOTICE);
							} else if (action == 2) {
								moduleNoticeLang(s_OperServ, u2, LANG_NOTINCHANLIST_KILL);
								if (!quiet)
									moduleNoticeLang(s_OperServ, u, LANG_NOTINCHANLIST_KILLING, u2->nick);

								kill_user(s_OperServ, u2->nick, niclKillReason);

							} else if (action == 3) {
								sprintf(akillmask, "*@%s", u2->host);
								if (!quiet)
									moduleNoticeLang(s_OperServ, u, LANG_NOTINCHANLIST_AKILLING, u2->nick);

								add_akill(u, akillmask, u->nick, time(NULL) + niclAkillTime, niclAkillReason);

							} else if (action == 4) {
								if ((stricmp(IRCDModule,"inspircd") == 0) || (stricmp(IRCDModule,"plexus3") == 0)
									|| (stricmp(IRCDModule,"ptlink") == 0) || (stricmp(IRCDModule,"ultimate2") == 0)
									|| (stricmp(IRCDModule,"unreal31") == 0) || (stricmp(IRCDModule,"unreal32") == 0)
									|| (stricmp(IRCDModule,"viagra") == 0)) {

									moduleNoticeLang(s_OperServ, u2, LANG_NOTINCHANLIST_JOIN);

									if (!quiet)
										moduleNoticeLang(s_OperServ, u, LANG_NOTINCHANLIST_JOINING, u2->nick);

									xanadu_cmd_svsjoin(s_OperServ, u2->nick, niclIdleChan, NULL);
								} else
									moduleNoticeLang(s_OperServ, u, LANG_JOIN_NOT_SUPPORTED);

							} else if (action == 5) {
								if (stricmp(IRCDModule,"unreal32") == 0) {
									moduleNoticeLang(s_OperServ, u2, LANG_NOTINCHANLIST_SHUN);

									if (!quiet)
										moduleNoticeLang(s_OperServ, u, LANG_NOTINCHANLIST_SHUNING, u2->nick);

									/* SHUN u2->nick niclAkillTime :niclKillReason */
									send_cmd(NULL, "TKL + s %s %s %s %ld %ld :%s", u2->nick, u2->host, s_OperServ,
										(long int) time(NULL) + niclAkillTime, time(NULL), niclKillReason);
								} else
									moduleNoticeLang(s_OperServ, u, LANG_SHUN_NOT_SUPPORTED);

							} else if (action == 6) {
								if (stricmp(IRCDModule,"unreal32") == 0) {
									moduleNoticeLang(s_OperServ, u2, LANG_NOTINCHANLIST_TSHUN);

									if (!quiet)
										moduleNoticeLang(s_OperServ, u, LANG_NOTINCHANLIST_TSHUNING, u2->nick);

									/* SHUN u2->nick niclAkillTime :niclKillReason */
									send_cmd(s_OperServ, "TEMPSHUN %s %s", u2->nick, niclKillReason);
								} else
									moduleNoticeLang(s_OperServ, u, LANG_TSHUN_NOT_SUPPORTED);

							} else if (action == 7) {
								int temp;
								sprintf(akillmask, "*@%s", u2->host);
								if (!quiet)
									moduleNoticeLang(s_OperServ, u, LANG_NOTINCHANLIST_AKILLING, u2->nick);

								/* Stupid hack to make sure the akill is enforced... */
								temp = AkillOnAdd;
								AkillOnAdd = 1;
								add_akill(u, akillmask, u->nick, time(NULL) + niclAkillTime, niclAkillReason);
								AkillOnAdd = temp;
							}
						}
					}
				}
			}
		}

		moduleNoticeLang(s_OperServ, u, LANG_SEARCHING_COMPLETE);
		moduleNoticeLang(s_OperServ, u, LANG_SEARCHING_RESULT, total, count);
	}

	if (options1) free(options1);
	if (options2) free(options2);
	if (options3) free(options3);

	return MOD_CONT;
}

int parse_action(char *action) {
	int act = 0;

	if (stricmp(action,"notice") == 0)
		act = 1;
	else if (stricmp(action,"kill") == 0)
		act = 2;
	else if (stricmp(action,"akill") == 0)
		act = 3;
	else if (stricmp(action,"join") == 0)
		act = 4;
	else if (stricmp(action,"shun") == 0)
		act = 5;
	else if (stricmp(action,"tshun") == 0)
		act = 6;
	else if (stricmp(action,"fakill") == 0)
		act = 7;

	return act;
}


/* ------------------------------------------------------------------------------- */


/**
 * (Re)Load the configuration directives
 **/
void load_config(void) {
	int i;
	char *tkillreason = NULL, *takillreason = NULL, *idlechan = NULL;

	Directive confvalues[][1] = {
		{{ "OSNotInChanAkillTime", { { PARAM_TIME, PARAM_RELOAD, &niclAkillTime } } }},
		{{ "OSNotInChanKillReason", { { PARAM_STRING, PARAM_RELOAD, &tkillreason } } }},
		{{ "OSNotInChanAkillReason", { { PARAM_STRING, PARAM_RELOAD, &takillreason } } }},
		{{ "OSNotInChanIdleChan", { { PARAM_STRING, PARAM_RELOAD, &idlechan } } }}
	};

	niclAkillTime = 0;
	if (niclKillReason) free(niclKillReason);
	niclKillReason = NULL;
	if (niclAkillReason) free(niclAkillReason);
	niclAkillReason = NULL;
	if (niclIdleChan) free(niclIdleChan);
	niclIdleChan = NULL;

	for (i = 0; i < 4; i++)
		moduleGetConfigDirective(confvalues[i]);

	if (!niclAkillTime)
		niclAkillTime = 30 * 60;  /* default time = 30 mins */
	else if (niclAkillTime < 60) {
		niclAkillTime = 30 * 60;
		alog("OSNotInChanAkillTime needs to be at least 60. Using default values...");
	}

	if (tkillreason)
		niclKillReason = sstrdup(tkillreason);
	else
		niclKillReason = sstrdup(DefKillReason);

	if (takillreason)
		niclAkillReason = sstrdup(takillreason);
	else
		niclAkillReason = sstrdup(DefAkillReason);

	if (idlechan) {
		if (*idlechan != '#') {
			alog("[\002os_notinchanlist\002] OSNotInChanIdleChan is invalid. Using default values...");
			niclIdleChan = sstrdup(DefJoinChan);
		} else
			niclIdleChan = sstrdup(idlechan);
	} else
		niclIdleChan = sstrdup(DefJoinChan);


	if (debug)
		alog("debug: [os_notinchanlist] Config vars: AkillTime=%d KillReason='%s' AkillReason='%s' IdleChan='%s'", niclAkillTime, niclKillReason, niclAkillReason, niclIdleChan);

	/* free everything we used... */
	free(tkillreason);
	free(takillreason);
	free(idlechan);
}


/**
 * Upon /os reload call the routines for reloading the configuration directives
 **/
int reload_config(int argc, char **argv) {
	if (argc >= 1) {
		if (!stricmp(argv[0], EVENT_START)) {
			alog("[\002os_notinchanlist\002]: Reloading configuration directives...");
			load_config();
		}
	}
	return MOD_CONT;
}


/* ------------------------------------------------------------------------------- */


/**
 * Add language strings to the module's language db.
 **/
void add_languages(void) {
	char *langtable_en_us[] = {
		/* LANG_NOTINCHAN_DESC */
		"    NOTINCHANLIST  List and act against users not in any channels",
		/* LANG_NOTINCHAN_SYNTAX */
		" Syntax: NOTINCHANLIST [NOTICE | KILL | FAKILL | AKILL | JOIN | SHUN | TSHUN] [QUIET] [NOTID]",
		/* LANG_NOTINCHAN_SYNTAX_EXT */
		" Syntax: NOTINCHANLIST [NOTICE | KILL | FAKILL | AKILL | JOIN | SHUN | TSHUN] [QUIET] [NOTID]\n"
		" \n"
		" This command allows services admins to output a list of all clients\n"
		" that are not in channels. In addition to this, various actions can\n"
		" be performed on the users that are discovered. ALL opered\n"
		" clients are exempt from the scanning.\n"
		" \n"
		" If the NOTID option is used, users that are not in a channel, but are\n"
		" IDENTIFIED with %s will be omitted from the scan.\n"
		" \n"
		" The QUIET option can only be used if NOTICE, KILL, AKILL, JOIN, SHUN \n"
		" or TSHUN has been specified."
		" The QUIET argument will only take the action specified and will not\n"
		" report its findings.\n"
		" \n"
		" If the KILL option is used, any users not in a channel will be \n"
		" forced to disconnect from the network.\n"
		" \n"
		" If the AKILL option is used, i will AKILL any users not in a channel.\n"
		" FAKILL is similar to AKILL, however it bans detected users even if\n"
		" AKILLonAdd is not defined.\n"
		" \n"
		" If the NOTICE option is used, I will notice any users not in a channel. \n"
 		" \n"
		" If the JOIN option is used, any user not in a channel will be joined\n"
		" to the channel %s. \n"
		" \n"
		" If the SHUN option is used, the users found will be shunned for OSNotInChanAkillTime.\n"
		" \n"
		" If the TSHUN option is used, the users will be temprorarily shunned. The effect\n"
		" of this only lasts untill the user reconnects.\n"
		" \n"
		" NOTE1: The JOIN,SHUN and TSHUN arguments may not be supported by this IRCd! \n"
		" \n"
		" NOTE2: Opers are exempt from the scan, however, if an oper has an\n"
		" un-opered clone on the network and that clone is not in any channels\n"
		" they run the risk of being AKILLed or SHUNed.",
		/* LANG_NOTINCHANLIST_NOTICE */
		"Please Join a channel; clients not in a channel are often confused \n"
		"with malicious bots. All clients not in a channel may find they \n"
		"are forcefully disconnected or banned from the network.",
		/* LANG_NOTINCHANLIST_KILL */
		"You are not in any channels. This makes you appear as if \n"
		"you are a mailicious connection. When you next re-connect, \n"
		"please ensure you join a channel.",
		/* LANG_NOTINCHANLIST_JOIN */
		"You are not in any channels. This makes you appear as if \n"
		"you are a mailicious connection. You are now being forced \n"
		"into our IDLE channel.",
		/* LANG_JOIN_NOT_SUPPORTED*/
		"The JOIN argument is not supported by this IRCd!",
		/* LANG_QUIET_NO_ACTION */
		"Error: QUIET spefified with no action.",
		/* LANG_ACTION_SECOND */
		"Error: KILL, AKILL, NOTICE or JOIN specified as 2nd parameter",
		/* LANG_SEARCHING */
		"Scanning for users not in any channels...",
		/* LANG_NOTINCHANLIST_MATCH */
		"Match: %-20s - %s",
		/* LANG_NOTINCHANLIST_NOTICING */
		"NOTICING: %s",
		/* LANG_NOTINCHANLIST_KILLING */
		"KILLING: %s",
		/* LANG_NOTINCHANLIST_AKILLING */
		"AKILLING: %s",
		/* LANG_NOTINCHANLIST_JOINING */
		"JOINING: %s",
		/* LANG_SEARCHING_COMPLETE */
		"Scan complete.",
		/* LANG_SEARCHING_RESULT */
		"Out of %d users, %d possible malicious clients were detected.",
		/* LANG_NOTINCHANLIST_SHUN */
		"You are not in any channels. This makes you appear as if \n"
		"you are a mailicious connection. You are now being shunned...\n"
		"Please contact the Network Staff for more information.",
		/* LANG_SHUN_NOT_SUPPORTED */
		"The SHUN argument is not supported on this IRCd!",
		/* LANG_NOTINCHANLIST_SHUNING */
		"SHUNNING: %s",
		/* LANG_NOTINCHANLIST_TSHUN */
		"You are not in any channels. This makes you appear as if you are \n"
		"a mailicious connection. You are now being temporarily shunned...\n"
		"This will last untill you reconnect to the network.\n"
		"Please contact the Network Staff for more information.",
		/* LANG_TSHUN_NOT_SUPPORTED */
		"The TSHUN argument is not supported on this IRCd!",
		/* LANG_NOTINCHANLIST_TSHUNING */
		"Temporarily SHUNNING: %s","",
	};

	char *langtable_nl[] = {
		/* LANG_NOTINCHAN_DESC */
		"    NOTINCHANLIST  Geef gebruikers die niet in een kanaal zijn.",
		/* LANG_NOTINCHAN_SYNTAX */
		" Gebruik: NOTINCHANLIST [NOTICE | KILL | FAKILL | AKILL | JOIN | SHUN | TSHUN] [QUIET] [NOTID]",
		/* LANG_NOTINCHAN_SYNTAX_EXT */
		" Gebruik: NOTINCHANLIST [NOTICE | KILL | FAKILL | AKILL | JOIN | SHUN | TSHUN] [QUIET] [NOTID]\n"
		" \n"
		" Dit commando geeft de services admins een lijst met alle clients\n"
		" die zich momenteel in geen enkel kanaal bevinden. Daarenboven kunnen\n"
		" verschillende acties ondernomen worden op de gevonden gebruikers.ALLE ge-operde\n"
		" clients zijn vrijgesteld van deze scan.\n"
		" \n"
		" Als de NOTID optie is opgegeven, zullen gebruikers die zich niet in een kanaal \n"
		" bevinden, maar wel IDENTIFED zijn bij %s van de scan gevrijwaard worden.\n"
		" \n"
		" QUIET kan alleen maar gespecifieerd worden als NOTICE, KILL, AKILL, JOIN\n"
		" SHUN of TSHUN is opgegeven.."
		" Het QUIET argument geeft %s instructies de gevonden clients niet te reporteren,\n"
		" maar alleen de opgegeven actie te ondernemen.\n"
		" \n"
		" Als de KILL optie gebruikt wordt, worden alle gebruikers die zicht niet in een\n"
		" kanaal bevinden van het netwerk werwijderd.\n"
		" \n"
		" Als de AKILL optie wordt gebruikt, zullen alle gebruikers die zich niet in een\n"
		" kanaam bevinden ge-AKILLed worden.\n"
		" FAKILL is bijna identiek aan AKILL, met als enige verschil dat de gedetecteerde\n"
		" gebruikers van het netwerk worden verbannen zelfs als AKILLonAdd niet is gedefinieerd.\n"
		" \n"
		" Als de NOTICE optie wordt gebruikt, worden alle gebruikers die zich in geen enkel \n"
		" kanaal bevinden over de kwestie gewaarschuwd.\n"
 		" \n"
		" Als het JOIN argument wordt gebruikt, zullen alle gebruikers die zich in geen enkel\n"
		" kanaal bevinden naar in kanaal %s gejoind worden.\n"
		" \n"
		" Als de SHUN optie wordt gebruikt zullen de gevonden gebruikers ge-shunned worden \n"
		" voor OSNotInChanAkillTime tijd.\n"
		" \n"
		" Als de TSHUN optie wordt gebruikt zullen de gebruikers tijdelijk ge-shunned  worden.\n"
		" Het effect hiervan duurt tot slechts tot de volgende keer dat ge gebruiker verbind.\n"
		" \n"
		" NOTE1: De JOIN, SHUN en TSHUN argumenten zijn mogelijk niet ondersteund door dit IRCd! \n"
		" \n"
		" NOTE2: Opers zijn vrijgesteld van deze scan, hoewel als er een oper een niet ge-operde\n"
		" clone heeft op het netwerk en deze zich niet in een kanaal bevind, dan lopen zij\n"
		" het risiko ge-AKILLed of ge-SHUNed te worden.",
		/* LANG_NOTINCHANLIST_NOTICE */
		"Gelieve een kanaal te joinen; gebruikers die zich niet in een kanaal bevinden worden \n"
		"gemakkelijk verward met kwaadaardige bots. Alle gebruikers die zich niet in een kanaal \n"
		"bevinden kunnen mogelijk verwijderd worden van het netwerk en evetueel geband worden.",
		/* LANG_NOTINCHANLIST_KILL */
		"U bevind zich momenteel niet in een kanaal. Dit zorgt ervoor dat u op een kwaadaardige \n"
		"gebruiker lijkt. Als u de volgende keer herverbind, gelieve er zeker van te zijn dat u \n"
		"zich in een kanaal bevind.",
		/* LANG_NOTINCHANLIST_JOIN */
		"U bevind zich momenteel niet in een kanaal. Dit zorgt ervoor dat u op een kwaadaardige \n"
		"gebruiker lijkt. U wordt nu verplicht om ons IDLE kanaal te joinen",
		/* LANG_JOIN_NOT_SUPPORTED*/
		"Het JOIN argument is niet ondersteund door dit IRCd!",
		/* LANG_QUIET_NO_ACTION */
		"Fout: QUIET opgegeven zonder een actie.",
		/* LANG_ACTION_SECOND */
		"Fout: KILL, AKILL, NOTICE, JOIN of SHUN opgegeven als 2de parameter",
		/* LANG_SEARCHING */
		"Zoeken naar gebruikers die zich niet in een kanaal bevinden...",
		/* LANG_NOTINCHANLIST_MATCH */
		"Gevonden: %-20s - %s",
		/* LANG_NOTINCHANLIST_NOTICING */
		"NOTICING: %s",
		/* LANG_NOTINCHANLIST_KILLING */
		"KILLING: %s",
		/* LANG_NOTINCHANLIST_AKILLING */
		"AKILLING: %s",
		/* LANG_NOTINCHANLIST_JOINING */
		"JOINING: %s",
		/* LANG_SEARCHING_COMPLETE */
		"Zoeken voltooid.",
		/* LANG_SEARCHING_RESULT */
		"Op de %d gebruikers werden er %d  mogelijk kwaadaardige gebruikers gedetecteerd.",
		/* LANG_NOTINCHANLIST_SHUN */
		"U bevind zich momenteel niet in een kanaal. Dit zorgt ervoor dat u op een kwaadaardige \n"
		"gebruiker lijkt. U wordt nu ge'shunned (dwz dat u niet meer kan praten)...\n"
		"Gelieve contact op te nemen met het Netwerk Beheer voor meer informatie.",
		/* LANG_SHUN_NOT_SUPPORTED */
		"Het SHUN argument wordt niet ondersteund op dit IRCd!",
		/* LANG_NOTINCHANLIST_SHUNING */
		"SHUNNING: %s",
		/* LANG_NOTINCHANLIST_TSHUN */
		"U bevind zich momenteel niet in een kanaal. Dit zorgt ervoor dat u op een kwaadaardige \n"
		"gebruiker lijkt. U wordt nu tijdelijk ge'shunned (dwz dat u niet meer kan praten enz)...\n"
		"Het effect hiervan duurt tot de volgende keer dat u met het netwerk verbind.\n"
		"Gelieve contact op te nemen met het Netwerk Beheer voor meer informatie.",
		/* LANG_TSHUN_NOT_SUPPORTED */
		"Het TSHUN argument wordt niet ondersteund op dit IRCd!",
		/* LANG_NOTINCHANLIST_TSHUNING */
		"Temporarily SHUNNING: %s","",

	};

	moduleInsertLanguage(LANG_EN_US, LANG_NUM_STRINGS, langtable_en_us);
	moduleInsertLanguage(LANG_NL, LANG_NUM_STRINGS, langtable_nl);
}

/* EOF */

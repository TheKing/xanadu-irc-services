/**
 * ----------------------------------------------------------------
 * Name    : m_connect_flood
 * Author  : Viper <Viper@Absurd-IRC.net>
 * Date    : 29/05/2006  (Last updated: 22/06/2007 )
 * Version : 3.3
 * ----------------------------------------------------------------
 * Tested  : Anope-1.7.19 + UnrealIRCd 3.2.6
 * ----------------------------------------------------------------
 * This module watches any new client connections. If the connect
 * amount reaches the maximum value in  the given time, it will
 * activate the trigger and services will go into defcon mode.
 *
 * Made a few changes to Certus's module to suit my needs...
 * Made it public since more ppl seem to be asking for this.
 *
 * Note: Configurations are now best made through config file.
 * I left the old /os connflood in place, but each time the
 * services.conf is reloaded and directives are present, changes
 * that have been made through operserv will be undone.
 *
 * ----------------------------------------------------------------
 * Changelog:
 *
 *   3.3  - Added win32 support.
 *
 *   3.2  - Fixed bug causing AutoDefconLevel to be set incorrectly
 *          if not defined in services.conf
 *        - added sanity checks..
 *
 *   3.1  - Fixed crashbug if ConnectionLimit is not present in services.conf
 *
 *	 3.0  - Defcon level is set in services.conf now
 *	      - Connection limits are now set in services.conf
 *	      - The module now defaults to a limit of 100 connections in 10 secs
 *
 * ----------------------------------------------------------------
 * Add this to services.conf !!
 *

# AutoDefconLevel <level> [OPTIONAL]
# Module m_connect_flood
#
# Set the defcon level u want services to go to when the connection limit is breached.
# If not defined, Services will go to defconn 3.
#
#AutoDefconLevel 3

# ConnectionLimit "<connecions> <period>" [OPTIONAL]
# Module m_connect_flood
#
# Set the defcon level u want services to go to when the connection limit is breached.
# If not defined, Services will go to defconn 3.
# The Connection limit has to be set with "/msg operserv CONNFLOOD <connections> <seconds>".
# Note: Anope WILL crash if the ""s are not present!
#
#ConnectionLimit "100 10"

 *
 **/


/*------------------------- Source - Don't change below --------------------------*/

#include "module.h"

#define AUTHOR "Viper"
#define VERSION "3.3"

static int DEF_DEFCON_LEVEL = 3;
static int DEF_CONNECTIONS = 100;
static int DEF_TIME = 10;


void my_load_config(void);
int hsReloadConf(int argc, char **argv);

int cf_nick(int argc, char **arg);
int cf_server(int argc, char **arg);
int cf_set(User *u);
int cf_count = 0;
int cf_lasttime = 0;
int cf_valid= 0;
int cf_lastserver = 0;
int cf_defconlevel, cf_connections, cf_time;

#ifdef _WIN32
  extern __declspec(dllimport);
#else
  extern time_t DefContimer;
  extern void runDefCon(void);
#endif

void AnopeInit(void) {
	EvtHook *newnick = NULL, *newserver = NULL;
	EvtHook *hook;
	Command *c;

	alog("Loading m_connect_flood.so");

	my_load_config();

	c = createCommand("CONNFLOOD",cf_set,is_services_admin,-1,-1,-1,-1,-1);
	moduleAddCommand(OPERSERV,c,MOD_UNIQUE);

	hook = createEventHook(EVENT_RELOAD, hsReloadConf);
	moduleAddEventHook(hook);

	newnick = createEventHook(EVENT_NEWNICK, cf_nick);
	moduleAddEventHook(newnick);

	newserver = createEventHook(EVENT_SERVER_CONNECT, cf_server);
	moduleAddEventHook(newserver);

	moduleAddAuthor(AUTHOR);
	moduleAddVersion(VERSION);
}

void AnopeFini(void) {
	alog("Unloading m_connect_flood.so");
}

void my_load_config(void) {
	int i, temp = 0;
	char *limits = NULL;

	Directive confvalues[][1] = {
		{{"AutoDefconLevel", {{PARAM_POSINT, PARAM_RELOAD, &temp}}}},
		{{"ConnectionLimit", {{PARAM_STRING, PARAM_RELOAD, &limits}}}}
	};

	for (i = 0; i < 2; i++)
		moduleGetConfigDirective(confvalues[i]);

	if (temp != 0) {
		if (temp >= 1 && temp <= 5)
			cf_defconlevel = temp;
		else {
			cf_defconlevel = DEF_DEFCON_LEVEL;
			alog("[m_connect_flood] AutoDefconLevel not configured correctly. Using default value. ");
		}
	} else {
		cf_defconlevel = DEF_DEFCON_LEVEL;
		alog("[m_connect_flood] AutoDefconLevel not set in services.conf. Using default value. ");
	}

	if (limits) {
		char *arg1 = myStrGetToken(limits,' ',0);
		char *arg2 = myStrGetToken(limits,' ',1);

		int conn, time;
		if (!arg1 || !arg2) {
			cf_connections = DEF_CONNECTIONS;
			cf_time = DEF_TIME;

			alog("[m_connect_flood] ConnectionLimit not configured correctly. Using default values.");
			alog("[m_connect_flood] Returning to default values: defcon %d after %d connections in %d secs", cf_defconlevel, cf_connections, cf_time);
		} else {
			conn = atoi(arg1);
			time = atoi(arg2);

			if ((conn <= 0) || (time <= 0) || (conn > 300) || (time > 300)) {
				cf_connections = DEF_CONNECTIONS;
				cf_time = DEF_TIME;

				alog("[m_connect_flood] The values must be between 0 and 300.");
				alog("[m_connect_flood] Returning to default values: defcon %d after %d connections in %d secs", cf_defconlevel, cf_connections, cf_time);
			} else {
				cf_connections = conn;
				cf_time = time;
			}
		}
		if (arg1) free(arg1);
		if (arg2) free(arg2);
	} else {
		cf_connections = DEF_CONNECTIONS;
		cf_time = DEF_TIME;
		alog("[m_connect_flood] ConnectionLimit not set in services.conf. Using default values. ");
	}

	if (debug)
		alog("[m_connect_flood] Services will go into defcon %d after %d connections in %d secs", cf_defconlevel, cf_connections, cf_time);

	if (limits)
		free(limits);
}


int hsReloadConf(int argc, char **argv) {
	if (argc >= 1) {
		if (!stricmp(argv[0], EVENT_START)) {
			alog("[m_connect_flood] Reloading configuration directives...");
			my_load_config();
		}
	}

	return MOD_CONT;
}

int cf_nick(int argc, char **arg) {
	int cf_currenttime = time(NULL);

	cf_count++;

	/* let's pass conections for 5 seconds after a server joined,
	 * so we avoid malfunctions after a netsplit
	 */

	if (((cf_currenttime - cf_lastserver) < 5 ) && (cf_lastserver != 0))
		return MOD_CONT;

	/* if (now - lastchecktime >= timevalue for connects) reset
	 * connect count and set lastchecktime to now [because we check
	 * for connections:SECONDS ]
	 */

	if ((cf_currenttime - cf_lasttime) >= cf_time) {
		cf_count = 0;
		cf_lasttime = time(NULL);
	} else if ((cf_count >= cf_connections) && (cf_valid == 1)) {
		alog("CAUTION! Connect-flood limit reached, possible bot-flood. Going into DefCon %d", cf_defconlevel);
		wallops(s_OperServ, "CAUTION! Connect-flood limit reached, possible bot-flood. Going into DefCon %d", cf_defconlevel);
		DefConLevel = cf_defconlevel;
		DefContimer = time(NULL);
		runDefCon();
	}
	return MOD_CONT;
}

int cf_set(User *u) {
	char *cf_arg1 = strtok(NULL, " ");
	char *cf_arg2 = strtok(NULL, " ");
	int cf_bak1 = 0;
	int cf_bak2 = 0;

	if (!cf_arg1 || !cf_arg2)  {
		notice(s_OperServ, u->nick, "Syntax: /msg operserv CONNFLOOD <connections> <seconds>");
		return MOD_CONT;
	}

	cf_bak1 = atoi(cf_arg1);
	cf_bak2 = atoi(cf_arg2);

	if ((cf_bak1 <= 0) || (cf_bak2 <= 0) || (cf_bak1 > 300) || (cf_bak2 > 300)) {
		notice(s_OperServ, u->nick, "The values must be between 0 and 300");
		return MOD_CONT;
	} else {
		cf_valid = 1;
		cf_connections = cf_bak1;
		cf_time = cf_bak2;
		notice(s_OperServ, u->nick, "New connflood values set (Services will now go into DefCon %d after %d connects in %d seconds).",
			cf_defconlevel, cf_connections, cf_time);
		alog("[m_connect_flood] New Connflood: Services will now go into defcon %d after %d connections in %d secs", cf_defconlevel, cf_connections, cf_time);
	}
	return MOD_CONT;
}

/* set the time of the last SERVER event */
int cf_server(int argc, char **arg) {
	cf_lastserver = time(NULL);
	return MOD_CONT;
}

/* EOF */

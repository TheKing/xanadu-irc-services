#include "module.h"
#define AUTHOR "n00bie"
#define VERSION "$Id: os_forceid.c v1.0.1 12-01-2007 n00bie $"

/* -----------------------------------------------------------------------------------
 * Name		: os_forceid.c
 * Author	: n00bie
 * Version	: 1.0.1
 * Date		: 5th October, 2006
 * Updated	: 12th January, 2007
 * -----------------------------------------------------------------------------------
 * Original Ideas by SGR
 * -----------------------------------------------------------------------------------
 * Changes from SGR module:
 *
 * 1) Remove strtok();
 * 2) Changed Services Admin to Services Operator
 * 3) Check whether nick is part of the network services
 * 4) Check whether nick is registered or not
 * 5) Check whether nick is already identified to services
 * 6) Added modes on ID (if enabled on services.conf)
 * 7) Added NS nick tracking (if enabled on services.conf)
 * 8) Enable vHost for nick (if the user have vHost)
 * -----------------------------------------------------------------------------------
 * Description: Forcefully identify a nick to services.
 * -----------------------------------------------------------------------------------
 * Tested: Unreal3.2.5, Anope-1.7.15, 1.7.18
 * -----------------------------------------------------------------------------------
 * Providing command:
 *
 * /msg OperServ FORCEID nick
 * -----------------------------------------------------------------------------------
 * This program is free software; you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation; either version 1, or (at your option) any later version.
 * -----------------------------------------------------------------------------------
 * Changelog:
 *	v1.0.0 - First Public Release and changes from the old SGR module
 *	v1.0.1 - Minor Code Update.
 *			 Compiled for 1.7.18 windows users.
 * -----------------------------------------------------------------------------------
 * This module have no configurable option.
 */

int do_forceid(User *u);
int myOperServOperHelp(User *u);
void myOperServHelp(User*u);
int AnopeInit(int argc, char **argv)
{
	Command *c;
	int status = 0;
	c = createCommand("FORCEID", do_forceid, is_services_oper, -1, -1, -1, -1, -1);
	moduleAddHelp(c, myOperServOperHelp);
	moduleSetOperHelp(myOperServHelp);
	status = moduleAddCommand(OPERSERV, c, MOD_HEAD);
	if (status != MOD_ERR_OK) {
		return MOD_STOP;
	} else {
		alog("%s: Successfully loaded module.", s_OperServ);
		alog("%s: Providing command '\2/msg OperServ HELP FORCEID\2'", s_OperServ);
	}
	moduleAddAuthor(AUTHOR);
	moduleAddVersion(VERSION);
	return MOD_CONT;
}

void AnopeFini(void)
{
	alog("%s: os_forceid: module unloaded.", s_OperServ);
}

void myOperServHelp(User *u)
{
	notice(s_OperServ, u->nick, "    FORCEID     Forcefully identify a nick to services");
}

int myOperServOperHelp(User *u)
{
	if (is_services_oper(u)) {
		notice(s_OperServ, u->nick, "Syntax: \2FORCEID \037nick\037\2");
		notice(s_OperServ, u->nick, " ");
		notice(s_OperServ, u->nick, "The FORCEID command is used to identify a user to services.");
		notice(s_OperServ, u->nick, "This is helpful in the case of an automatic nick script in");
		notice(s_OperServ, u->nick, "which services keeps changing the nick to %s.", NSGuestNickPrefix);
		notice(s_OperServ, u->nick, " ");
		notice(s_OperServ, u->nick, "NOTE: Do not use this command unless necessary!");
	}
	return MOD_CONT;
}

int do_forceid(User *u)
{
	User *u2;
	NickAlias *na;
	char *buf = moduleGetLastBuffer();
	char *nick = myStrGetToken(buf, ' ', 0);
	char *vHost;
	char *vIdent = NULL;
	
	if (!nick) {
		notice(s_OperServ, u->nick, "Syntax: \2FORCEID \037nick\037\2");
	} else if (nickIsServices(nick, 1)) {
		notice(s_OperServ, u->nick, "Nick \2%s\2 is part of this Network Services!", nick);
	} else if (!(u2 = finduser(nick))) {
		notice_lang(s_OperServ, u, NICK_X_NOT_IN_USE, nick);
	} else if (!(na = u2->na)) {
		notice_lang(s_OperServ, u, NICK_X_NOT_REGISTERED, nick);
	} else if (na->status & NS_VERBOTEN) {
		notice_lang(s_OperServ, u, NICK_X_FORBIDDEN, na->nick);
	} else if (nick_identified(u2)) {
		notice(s_OperServ, u->nick, "\2%s\2 is already identified to services.", nick);
	} else {
		if (u2->na) {
			na = u2->na;
			na->status |= NS_IDENTIFIED;
			na->last_seen = time(NULL);
			change_user_mode(u2, "+r", "");
			notice_lang(s_NickServ, u2, NICK_IDENTIFY_SUCCEEDED);
			if (LogChannel) {
				alog("%s: \2%s\2 used FORCEID command to forcefully identify \2%s\2 to services.", s_OperServ, 
					u->nick, nick);
				alog("%s: \2%s\2 is now successfully identified.", s_OperServ, nick);
			}
			if (NSModeOnID) {
				do_setmodes(u2);
			}
			if (NSNickTracking) {
				nsStartNickTracking(u2);
			}
		}
		if ((na = findnick(nick))) {
			if (na->status & NS_IDENTIFIED) {
				vHost = getvHost(nick);
				vIdent = getvIdent(nick);
				if (vHost == NULL) {
					free(nick);
					return MOD_CONT;
				} else {
					if (vIdent) {
						notice(s_HostServ, u2->nick, "Your vHost of \2%s@%s\2 is now activated.", vIdent, vHost);
					} else {
						notice(s_HostServ, u2->nick, "Your vHost of \2%s\2 is now activated.", vHost);
					}
					anope_cmd_vhost_on(nick, vIdent, vHost);
				}
			}
		}
	}
	if (nick)
		free(nick);
	return MOD_CONT;
}

/* EOF */








/**
 * Methods to modify and access the ChanServ access list. - Headers
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
 * Last Updated   : 02/01/2008
 *
 **/

/* Functions */
int do_access(User *u, Channel *c, char *cmd, char *nick, char *s);
static int access_del(User *u, ChannelInfo *ci, ChanAccess *access, int *perm, int uacc);
static int access_del_callback(User *u, int num, va_list args);
static int access_list(User *u, int index, ChannelInfo *ci, int *sent_header);
static int access_list_callback(User *u, int num, va_list args);
int do_levels(User *u, Channel *c, char *cmd, char *what, short level);


/* EOF */

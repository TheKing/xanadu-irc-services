/**
 * Methods to modify and access the ChanServ XOP lists. - Headers
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
 * Last Updated   : 17/06/2007
 *
 **/

/* Functions */
int do_xop(User * u, char *xname, int xlev, int *xmsgs, Channel *c, char *cmd, char *nick);
static int xop_del(User * u, ChannelInfo * ci, ChanAccess * access, int *perm, int uacc, int xlev);
static int xop_del_callback(User *u, int num, va_list args);
static int xop_list(User *u, int index, ChannelInfo *ci, int *sent_header, int xlev, int xmsg);
static int xop_list_callback(User *u, int num, va_list args);


/* EOF */

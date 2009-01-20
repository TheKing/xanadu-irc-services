/**
 * Methods to modify and access the Services ingore list. - Headers
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
 * Last Updated   : 22/07/2006
 *
 **/

/* Functions */
int do_ignoreuser(User * u, Channel *c, char *cmd, char *nick, char *time);
static int do_ignorelist(User * u, ChannelInfo *ci);
static void delete_ignore(const char *nick);
static int do_clearignore(User * u, ChannelInfo *ci);


/* EOF */

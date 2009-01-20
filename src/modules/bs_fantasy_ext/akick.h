/**
 * Methods to modify and access the ChanServ akick list. - Headers
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
int do_akick(User * u, Channel *c, char *cmd, char *mask, char *reason);
static int akick_del(User * u, AutoKick * akick);
static int akick_del_callback(User * u, int num, va_list args);
static int akick_list(User * u, int index, ChannelInfo * ci, int *sent_header);
static int akick_view(User * u, int index, ChannelInfo * ci, int *sent_header);
static int akick_list_callback(User * u, int num, va_list args);
static int akick_view_callback(User * u, int num, va_list args);


/* EOF */

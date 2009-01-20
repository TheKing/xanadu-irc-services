/**
 * Methods to modify and access the OperServ akill list. - Headers
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
int do_akill(User * u, Channel *c, char *cmd, char *expiry, char *mask, char *reason);
static int akill_list(int number, Akill * ak, User * u, int *sent_header, ChannelInfo *ci);
static int akill_view(int number, Akill * ak, User * u, int *sent_header, ChannelInfo *ci);
static int akill_list_callback(SList * slist, int number, void *item, va_list args);
static int akill_view_callback(SList * slist, int number, void *item, va_list args);


/* EOF */

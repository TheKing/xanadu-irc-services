/**
 * Main routines for the fantasy ban and unban commands - Headers
 *
 ***********
 * Module Name    : bs_fantasy_ext
 * Author         : Viper <Viper@Absurd-IRC.net>
 * Creation Date  : 14/09/2007
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
 * Last Updated   : 06/01/2008
 *
 **/

#ifdef ENABLE_BAN
int do_ban(User * u, Channel *c, char *params);
#endif

#ifdef ENABLE_UNBAN
int do_unban(User * u, Channel *c, char *params);
#endif

 /* EOF */

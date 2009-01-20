/**
 * Functionality used to temporary ban a user from a channel - Headers
 *
 ***********
 * Module Name    : bs_fantasy_ext
 * Author         : Viper <Viper@Absurd-IRC.net>
 * Creation Date  : 22/02/2007
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
 * Last Updated   : 26/02/2007
 *
 **/

 /* Functions */
#ifdef ENABLE_TBAN
int do_tban(User * u, Channel *c, char *target, int time);
#endif

#ifdef ENABLE_TKICKBAN
int do_tkban(User * u, Channel *c, char *target, int time, char *reason);
#endif

 /* EOF */

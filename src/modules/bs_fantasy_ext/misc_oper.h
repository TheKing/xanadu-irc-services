/**
 * Misc IRCoper commands the module adds - Headers
 *
 ***********
 * Module Name    : bs_fantasy_ext
 * Author         : Viper <Viper@Absurd-IRC.net>
 * Creation Date  : 21/07/2006
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
 * Last Updated   : 22/07/2006
 *
 **/

/* Functions */
#ifdef ENABLE_KILL
int my_do_kill(User *u, Channel *c, char *target, char *reason);
#endif

#ifdef ENABLE_MODE
int do_mode(User * u, Channel *c, char *modes);
#endif


/* EOF */

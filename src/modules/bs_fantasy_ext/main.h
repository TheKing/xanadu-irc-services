/**
 * Modules Main Functions for loading and matching commands. - Headers
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
 * Last Updated   : 20/04/2008
 *
 **/

#include "config.h"

#define AUTHOR "Viper"
#define VERSION "1.1.16"

/* Functions */
int do_fantasy(int ac, char **av);
int do_fantasy_denied(int ac, char **av);

/* Global Variables */
int enabled, SHUNExpiry;
int excempt_nr, EnOperCmds, SAdminOverride, OverrideCoreCmds, RestrictKB, en_sync, EnUnbanIP, en_shun;
char **ListExempts;

/* Constants */
#ifdef SUPPORTED
int supported = 1;
#else
int supported = 0;
#endif
int unsupported = 0;
char *PrevUnbanIP = "PrevUnbanIP";
char *NrUnbanIP = "NrUnbanIP";
int DefSHUNExpiry = 172800;			/* 48hrs (60.60.48) */

/* EOF */

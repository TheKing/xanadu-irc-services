/**
 * Methods to modify the ChanServ Channel settings. - Headers
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
#ifdef ENABLE_SET
static int do_set(User * u, ChannelInfo * ci, char *option, char *value);
static int do_set_founder(User * u, ChannelInfo * ci, char *param);
static int do_set_successor(User * u, ChannelInfo * ci, char *param);
static int do_set_desc(User * u, ChannelInfo * ci, char *param);
static int do_set_url(User * u, ChannelInfo * ci, char *param);
static int do_set_email(User * u, ChannelInfo * ci, char *param);
static int do_set_entrymsg(User * u, ChannelInfo * ci, char *param);
static int do_set_bantype(User * u, ChannelInfo * ci, char *param);
static int do_set_mlock(User * u, ChannelInfo * ci, char *param);
static int do_set_keeptopic(User * u, ChannelInfo * ci, char *param);
static int do_set_topiclock(User * u, ChannelInfo * ci, char *param);
static int do_set_private(User * u, ChannelInfo * ci, char *param);
static int do_set_secureops(User * u, ChannelInfo * ci, char *param);
static int do_set_securefounder(User * u, ChannelInfo * ci, char *param);
static int do_set_restricted(User * u, ChannelInfo * ci, char *param);
static int do_set_secure(User * u, ChannelInfo * ci, char *param);
static int do_set_signkick(User * u, ChannelInfo * ci, char *param);
static int do_set_opnotice(User * u, ChannelInfo * ci, char *param);
static int do_set_xop(User * u, ChannelInfo * ci, char *param);
static int do_set_peace(User * u, ChannelInfo * ci, char *param);
static int do_set_noexpire(User * u, ChannelInfo * ci, char *param);
#endif



/* EOF */

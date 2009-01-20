/**
 * Misc methodes used by the modules - Headers
 * Misc commands the module adds - Headers
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
 * Last Updated   : 04/04/2008
 *
 **/

/* Check if the variable is NULL or is \0.. we shouldn't free() in either case.
 * Many thanks to Trystan for providing this */
#define BadPtr(x) (!(x) || (*(x) == '\0'))

/* Variables */
User *dummy;

/* Functions */
static int check_opercmds(User *u);
static int my_check_access(User *u, ChannelInfo *ci, int what);

static int do_core_fantasy(int ac, User *u, ChannelInfo *ci, char *cmd, char *params);
static int do_up_down(User *u, Channel *c, char *cmd, int target);

void load_config(void);
int reload_config(int argc, char **argv);

void check_core_modules(void);
int check_modules(void);
void create_dummy(void);
void delete_dummy(void);

static void show_version(User *u, ChannelInfo *ci);
static void show_modinfo(User *u, ChannelInfo *ci);
static char* get_flags();
void update_version(void);

void addTempBan(Channel * c, time_t timeout, char *banmask);
int delTempBan(int argc, char **argv);
char * makeexpiry(int time);

int delBan(ChannelInfo *ci, char *target);

char* get_str(char *param);
int my_match_wild_nocase(char *pat, char *str);

#ifdef ENABLE_STAFF
int list_global_opers(User * u);
int list_admins(User * u);
#endif

#ifdef ENABLE_INFO
int do_info(User * u, Channel *c, char *param);
#endif

/* EOF */

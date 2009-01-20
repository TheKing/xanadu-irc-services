/**
 * Module's Compile time Configuration Options
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
 * Last Updated   : 06/02/2008
 *
 **/


/**
 * CPU Usage Reduction mode.      <<!TESTING PHASE!>>
 *
 * This spcifies how CPU intensive this module will be.
 * Unless used on a very large network, the default value (0) should be fine.
 * This directive influences the CPU cycles used when processing the !kb, !ban,
 * !tban and !mute commands. Keep in mind that when enabling CPU Usage Reduction the behaviour
 * of these commands will be slightly altered.
 * This option was primarily added because some resource intensive code was added to improve
 * upon above commands. Some larger networks though way want to prefer not using this.
 *
 * Mode			0
 *    This is the default setting in which the module will use all of its special more CPU
 *    intensive functions. In this mode !kb, !tkb, !ban, !tban and !mute will perform several
 *    checks to verify no user with a higher access level or special protection is affected
 *    and will automatically remove all modes from affected users.
 *
 * Mode			5
 *    In this mode !ban, !tban and !mute will no longer deop affected users when
 *    the command is issued with a mask.
 *
 * Mode			10
 *    In this mode !ban, !tban and !mute will no longer check whether the command will affect
 *    users with a higher access level or protected users when the command is issued with a mask.
 *
 * Mode			20
 *    In this mode !kb and !tkb will no longer check whether the command will affect users with
 *    a higher access level or protected users when the command is issued with a mask.
 *    It will however still refuse to kick the affected users if they have a higher access level.
 *
 * When undefined, maximum CPU usage reduction mode is enabled, but this is HIGHLY DISCOURAGED
 * because of its unwanted side effects like allowing "!kb <mask>" to kick users with higher access.
 **/
#define CPU_USAGE_REDUCTION		0


/**
 * Undefining this will make you lose all support for bs_fantasy_ext!!!
 *
 * As of version 1.1.13 (RC2) bs_fantasy_ext checks whether unsupported modules are enabled/loaded in anope.
 * If this is the case, bs_fantasy_ext will attempt to unload them or unload itself if it s a config directive.
 * These measures have been added to protect users against themselves and prevent the author of this module
 * from being confronted with situations that would not occor during normal use (for example when using RAW).
 *
 * If you undef this directive, these checks will be disabled, however you WILL LOSE ALL SUPPORT!!!
 *
 * To undefine this replace "#define" by "#undef" or simply comment the line out.
 **/
#define SUPPORTED


/**
 * This determines the delimeter that will be used to seperate the existing topic and the part
 * added to it by !appendtopic.
 **/
#define AppendToTopicDel		"||"


/**
 * This determines how many IP based bans !unban may remove for a single identified user.
 * The count is reset when services restart or the users' IP changes.
 *
 * It should be safe to increase this if services have a high uptime.
 *
 * cf the README for more information.
 **/
int MaxUnbanIP = 3;


/**
 * To disable a fantasy trigger Undefine (#undef) these or simply comment the line out.
 * Define it to enable the trigger. This was added on request and considering the anope core provides
 * the ability to anable/disable certain commands by not loading some core modules it would
 * be stupid if those commands were still accessible through the fantasy commands.
 * Note that not all commands can be disabled here !
 *
 * This needs to be set at compile time and changing these will require you to recompile the module.
 **/

/* Enable the !cmdlist trigger */
#define ENABLE_CMDLIST

/* Enable the !help trigger */
#define ENABLE_HELP

/* Enable the !clear trigger */
#define ENABLE_CLEAR

/* Enable the !xop trigger */
#define ENABLE_XOP

/* Enable the !access & !levels trigger */
#define ENABLE_ACCESS

/* Enable the !akick trigger */
#define ENABLE_AKICK

/* Enable the !badwords trigger */
#define ENABLE_BADWORDS

/* Enable the !set trigger  that gives access to both the chanserv and botserv set options*/
#define ENABLE_SET

/* Enable the !topic trigger */
#define ENABLE_TOPIC

/* Enable the !appendtopic trigger */
#define ENABLE_APPENDTOPIC

/* Enable the !invite trigger */
#define ENABLE_INVITE

/* Enable the !staff (!ircops) and !admin trigger */
#define ENABLE_STAFF

/* Enable the !up and !down triggers */
#define ENABLE_UPDOWN

/* Enable the !ban trigger */
#define ENABLE_BAN

/* Enable the !mute & !unmute trigger */
#define ENABLE_MUTEUNMUTE

/* Enable the !kb trigger implementation by this module to override the one in the core */
#define ENABLE_KICKBAN

/* Enable the !info trigger */
#define ENABLE_INFO

/* Enable the !kill trigger */
#define ENABLE_KILL

/* Enable the !mode trigger */
#define ENABLE_MODE

/* Enable the !akill trigger */
#define ENABLE_AKILL

/* Enable the !ignore trigger */
#define ENABLE_IGNORE

/* Enable the !bkick trigger to change botserv kick settings */
#define ENABLE_BKICK

/* Enable the !tb(an) trigger */
#define ENABLE_TBAN

/* Enable the !tkb trigger */
#define ENABLE_TKICKBAN

/* Enable the !suspend trigger */
#define ENABLE_SUSPEND

/* Enable the !unban command (Also requires OverrideCoreCmds to be set) *
 * Part of the unban funtionality is also disabled unless EnUnbanIP is set.*/
#define ENABLE_UNBAN

/* Enable the !sync trigger (Also requires cs_sync to be loaded.) */
#define ENABLE_SYNC

/* Enable the !shun trigger (Also requires os_shun to be loaded) */
#define ENABLE_SHUN

/* Enable the !tshun trigger (Also requires os_tshun to be loaded) */
#define ENABLE_TSHUN

/* EOF */

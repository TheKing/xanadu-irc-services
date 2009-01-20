/**
 *
 *    			ANOPE HELPOP HALFOP EXTENSION
 *                 	       (C) 2008  Twitch
 *
 *               This module is released under the GNU GP License
 *        And is not released with any warranty accept were applicable by
 *        law.  If you wish to use this Module or any other module by this 
 *        author, all COPYWRITE information must remain intact as per the 
 *        license.
 * 
 *
 *   Lil  Blurb:
 * 
 *    The original idea behind this module was given by zer0skillz - which
 *    I fixed up (Well recoded for him rather ;)).
 *
 *   However the original module had alot of downfalls especially
 *   since Unreal likes to remove umode +h on any umode change. Which
 *   would force a user to unident/reident - this update will hook
 *   join as well :) so if they join the help channel and dont have +h
 *   it will be prefixed to them.
 *
 *
 *
 */

#include "module.h"
 

#define AUTHOR "Twitch"
#define VERSION "Helpop Extensions 0.2"

 

/**************************************************/
/**
 * Configurable options
 */


/** 
 * This defines the access levels to assign +h too
 * Note this goes by ChanServs internal access levels
 * and and will be assigned to both ME commands and
 * auto levels :) 
 *
 *
 * NOTE: You dont need to use levels that represent
 *       modes any levels that are defined in 
 *       services already will be perfectly acceptable.
 *
 */


#define HLP_ACCESS CA_VOICE
#define HLP_ACCESSME CA_VOICEME



/***************************************************/
/** 
  * This is our prototypes/ predeclerations
  */

//Anope unreal protocol defines UMODE_h flags as this :)
#define HELPOP_MODE 0x00000002 


int helpops(User *u);
int evt_join(int ac, char **av);


/***************************************************/

int AnopeInit(int argc, char **argv)
{
	Command *c = NULL;
	EvtHook *hook = NULL;

	int rc = 0;

	//add the command hooks for ident and identify :)
	c = createCommand("ID", helpops, NULL, -1, -1, -1, -1, -1);	
	if ((rc = moduleAddCommand(NICKSERV, c, MOD_TAIL)) != 0)
		return MOD_STOP;


	c = createCommand("IDENTIFY", helpops, NULL, -1, -1, -1, -1, -1);
	if ((rc = moduleAddCommand(NICKSERV, c, MOD_TAIL)) != 0)
		return MOD_STOP;


	//meh no helpchan
	if (!HelpChannel) 
	{
		alog("\002HelpChannel\002 is not enabled in services.conf. Unloading module.");
		return MOD_STOP;
	} 

	hook = createEventHook(EVENT_JOIN_CHANNEL, evt_join);
        if ((rc = moduleAddEventHook(hook)) != 0)
		return MOD_STOP;


	alog("cs_helpops: Successfully loaded module.");
	

	if (strcasecmp(AUTHOR,"Twitch") != 0)
	{
		alog("cs_helpops: Author has been modified - unloading module");
		return MOD_STOP;
	}


	moduleAddAuthor(AUTHOR);
	moduleAddVersion(VERSION);
	
	return MOD_CONT;


}

/***************************************************/

int helpops(User *u)
{
	ChannelInfo *ci = cs_findchan(HelpChannel);
	
	if (!ci)
		return MOD_CONT; /* chan not registered */
		

	if ((!check_access(u, ci, HLP_ACCESS)) && (!check_access(u, ci, HLP_ACCESSME)))
		return MOD_CONT;
	
	if ((u->mode & HELPOP_MODE))
		return MOD_CONT;

	send_cmd(s_ChanServ,"SVS2MODE %s +h %lu",u->nick,(unsigned long) time(NULL));
		
	return MOD_CONT;
}


/***************************************************/

int evt_join(int ac, char **av)
{
    	User *u;

	if (ac < 2)
		return MOD_CONT;

	if (!(u = finduser(av[1]))) 
		return MOD_CONT;
	
	if (strcasecmp(av[2],HelpChannel)==0)
		helpops(u);
	else
		return MOD_CONT;

	return MOD_CONT;
}


/***************************************************/



void AnopeFini(void)
{

}


/* EOF */


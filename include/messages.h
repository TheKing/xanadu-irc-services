/* Declarations of IRC message structures, variables, and functions.
 *
 * (C) 2003-2007 Anope Team
 * Contact us at info@anope.org
 *
 * Please read COPYING and README for furhter details.
 *
 * Based on the original code of Epona by Lara.
 * Based on the original code of Services by Andy Church. 
 * 
 * $Id: messages.h 1265 2007-08-26 15:33:06Z geniusdex $ 
 *
 */

/*************************************************************************/
#include "modules.h"

extern Message messages[];
extern void moduleAddMsgs(void);
extern Message *find_message(const char *name);


/*************************************************************************/

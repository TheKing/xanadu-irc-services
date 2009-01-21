/* English Language File
 *
 * (C) 2009 Xanadu IRC Services Team
 *
 * Based on the original code of Anope by the Anope Team.
 * Based on the original code of Epona by Lara.
 * Based on the original code of Services by Andy Church.
 *
 * $Id: en_us.l 1.0 01-21-2009 $
 */

CHANSERV_HELP

       REGISTER                        Register a channel.
       IDENTIFY                        Identify to a channel.
       SET                             Set channel settings.
       AKICK                           Manipulate the aKick list.
       XOP                             Manipulate the XOP list.
       ACCESS                          Manipulate the access list.
       ACCESSLEV                       Manipulate the access levels.
       FLAGS                           Manipulate the flags system.
       SYNC                            Sync a channel with the user xop/flags/access list.
       UNBAN                           Unbans you or a hostmask from a channel 
       DROP                            De-registers a channel.
       INFO                            Shows information about a channel.
       LOGOUT                          Logs you out of an identified channel
       INVITE                          Invite you or a given nick/hostmask to a channel
       TOPIC                           Manipulate the channel topic.
       BAN                             Bans the selected nick/hostmask on a channel
       OWNER                           Gives you or another user temporary channel owner status on a channel
       DEOWNER                         Removes your or another user's channel owner status in a channel
       PROTECT                         Gives you or another user temporary channel administrator access
       DEPROTECT                       Removes your or another user's channel administrator status on a channel
       OP                              Gives you or another user temporary channel operator status in a channel
       DEOP                            Removes your or another user's channel operator status in a channel
       HALFOP                          Gives you or another user temporary channel half-operator status in a channel
       DEHALFOP                        Removes your or another user's channel half-operator status in a channel
       VOICE                           Gives you or another user temporary channel voice status in a channel
       DEVOICE                         Removes your or another user's channel voice status in a channel
       FORBID                          Forbid's a channel from being used.
       SUSPEND                         Forbid's a channel from being used, but keeps channel settings preserved
             
CHANSERV_HELP_REGISTER

                 Syntax: /msg ChanServ register #channel password description [xop|flags|access]

                 This allows you to register the channel, if you meet 
                 the following requirements: 

                 1) You have channel operator status.
                 2) The channel is not registered currently.
 
                 xop|flags|access is for you to choose what kind of
                 permissions system you want your channel to use.

                 This can be changed in /msg ChanServ help set chansys.

CHANSERV_HELP_IDENTIFY

                 Syntax: /msg ChanServ identify #channel password

                 This allows you to identify to a channel, reciving
                 founder status on the channel.

                 If /msg ChanServ set SECUREFOUNDER is on and enabled on 
                 the channel, you will not be able to change founder 
                 status to yourself.

CHANSERV_HELP_SET

               CHANSYS                     Choose what type of channel permissions system you want
               URL                         Set a URL for the channel
               DESC                        Set a description for the channel
               PASSWORD                    Set the channel password
               SECUREFOUNDER               Prevent other identified users to the channel changing founders
               







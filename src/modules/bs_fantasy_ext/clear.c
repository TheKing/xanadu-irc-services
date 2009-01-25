/**
 * The ChanServ CLEAR command. - Source
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
 * Last Updated   : 09/08/2006
 *
 **/

 /**
  * This code should be even better then the implementation in the core.
  * w00000t !!!....
  **/

#ifdef ENABLE_CLEAR
#include "clear.h"

int do_clear(User *u, Channel *c, int type) {
/* 1= Modes
 * 2= Bans
 * 3= Excepts
 * 4= Invites
 * 5= Ops
 * 6= Hops
 * 7= Voices
 * 8= Users */
	char tmp[BUFSIZE];

	/* Clear Channel Modes */
	if (type == 1) {
		char *argv[2];

		notice(c->ci->bi->nick, c->name, "CLEAR MODES command from %s", u->nick);

		if (c->mode) {
			/* Clear modes the bulk of the modes */
			xanadu_cmd_mode(c->ci->bi->nick, c->name, "%s", ircd->modestoremove);
			argv[0] = sstrdup(ircd->modestoremove);
			chan_set_modes(c->ci->bi->nick, c, 1, argv, 0);
			free(argv[0]);

			if (c->key) {
				xanadu_cmd_mode(c->ci->bi->nick, c->name, "-k %s", c->key);
				argv[0] = sstrdup("-k");
				argv[1] = c->key;
				chan_set_modes(c->ci->bi->nick, c, 2, argv, 0);
				free(argv[0]);
			}
			if (ircd->Lmode && c->redirect) {
				xanadu_cmd_mode(c->ci->bi->nick, c->name, "-L %s", c->redirect);
				argv[0] = sstrdup("-L");
				argv[1] = c->redirect;
				chan_set_modes(c->ci->bi->nick, c, 2, argv, 0);
				free(argv[0]);
			}
			if (ircd->fmode && c->flood) {
				if (flood_mode_char_remove) {
					xanadu_cmd_mode(c->ci->bi->nick, c->name, "%s %s", flood_mode_char_remove, c->flood);
					argv[0] = sstrdup(flood_mode_char_remove);
					argv[1] = c->flood;
					chan_set_modes(c->ci->bi->nick, c, 2, argv, 0);
					free(argv[0]);
				}
			}
			check_modes(c);
		}

	/* Clear Channel Bans */
	} else if (type == 2) {
		char *av[2];
		int i, count = c->bancount;
		char **bans = scalloc(sizeof(char *) * count, 1);

		/*count = c->bancount;*/
		notice(c->ci->bi->nick, c->name, "CLEAR BANS command from %s", u->nick);

		/* Save original ban info */
		for (i = 0; i < count; i++)
			bans[i] = sstrdup(c->bans[i]);

		if (ircd->svsmode_ucmode) {
			xanadu_cmd_svsmode_chan(c->name, "-b", NULL);

			for (i = 0; i < count; i++) {
				av[0] = sstrdup("-b");
				av[1] = bans[i];
				chan_set_modes(c->ci->bi->nick, c, 2, av, 0);
				free(av[0]);
				free(av[1]);
			}
		} else {
			for (i = 0; i < count; i++) {
				av[0] = sstrdup("-b");
				av[1] = bans[i];
				xanadu_cmd_mode(c->ci->bi->nick, c->name, "%s %s", av[0], av[1]);
				chan_set_modes(c->ci->bi->nick, c, 2, av, 0);
				free(av[0]);
				free(av[1]);
			}
		}

		free(bans);

	/* Clear Channel Excepts */
    } else if (ircd->except && type == 3) {
		char *av[2];
		int i, count = c->exceptcount;
		char **excepts = scalloc(sizeof(char *) * count, 1);

		notice(c->ci->bi->nick, c->name, "CLEAR EXCEPTS command from %s", u->nick);

		/* Save original except info */
		for (i = 0; i < count; i++)
			excepts[i] = sstrdup(c->excepts[i]);

		if (ircd->svsmode_ucmode) {
			xanadu_cmd_svsmode_chan(c->name, "-e", NULL);

			for (i = 0; i < count; i++) {
				av[0] = sstrdup("-e");
				av[1] = excepts[i];
				chan_set_modes(c->ci->bi->nick, c, 2, av, 0);
				free(av[1]);
				free(av[0]);
			}
		} else {
			for (i = 0; i < count; i++) {
				av[0] = sstrdup("-e");
				av[1] = excepts[i];
				xanadu_cmd_mode(c->ci->bi->nick, c->name, "%s %s", av[0], av[1]);
				chan_set_modes(c->ci->bi->nick, c, 2, av, 0);
				free(av[1]);
				free(av[0]);
			}
		}

		free(excepts);
	} else if (!(ircd->except) && type == 3) {
		moduleNoticeLang(c->ci->bi->nick, u, LANG_EXCEPTS_UNSUPPORTED);

	/* Clear Channel Invites */
    } else if (ircd->invitemode && type == 4) {
		char *av[2];
		int i, count = c->invitecount;
		char **invites = scalloc(sizeof(char *) * count, 1);

		notice(c->ci->bi->nick, c->name, "CLEAR INVITES command from %s", u->nick);

		/* Save original invite  info */
		for (i = 0; i < count; i++)
			invites[i] = sstrdup(c->invite[i]);

		if (ircd->svsmode_ucmode) {
			xanadu_cmd_svsmode_chan(c->name, "-I", NULL);
			for (i = 0; i < count; i++) {
				av[0] = sstrdup("-I");
				av[1] = invites[i];
				chan_set_modes(c->ci->bi->nick, c, 2, av, 0);
				free(av[1]);
				free(av[0]);
			}
		} else {
			for (i = 0; i < count; i++) {
				av[0] = sstrdup("-I");
				av[1] = invites[i];
				xanadu_cmd_mode(c->ci->bi->nick, c->name, "%s %s", av[0], av[1]);
				chan_set_modes(c->ci->bi->nick, c, 2, av, 0);
				free(av[1]);
				free(av[0]);
			}
		}

		free(invites);
	} else if (!(ircd->invitemode) && type == 4) {
		moduleNoticeLang(c->ci->bi->nick, u, LANG_INVITEMODE_UNSUPPORTED);

	/* Clear Channel OPs */
    } else if (type == 5) {
		char *av[4];
		struct c_userlist *cu, *next;
		char buf[BUFSIZE];
		int ac;

		notice(c->ci->bi->nick, c->name, "CLEAR OPS command from %s", u->nick);
		av[0] = c->name;

		if (ircd->svsmode_ucmode) {
			xanadu_cmd_svsmode_chan(c->name, "-o", NULL);
			if (ircd->owner) {
				xanadu_cmd_svsmode_chan(c->name, ircd->ownerunset, NULL);
			}
			if (ircd->protect || ircd->admin) {
				xanadu_cmd_svsmode_chan(c->name, ircd->adminunset, NULL);
			}
			for (cu = c->users; cu; cu = next) {
				next = cu->next;
				if (!chan_has_user_status(c, cu->user, CUS_OP)) {
					if (!chan_has_user_status(c, cu->user, CUS_PROTECT)) {
						if (!chan_has_user_status(c, cu->user, CUS_OWNER)) {
							continue;
						} else {
							snprintf(tmp, BUFSIZE, "%so", ircd->ownerunset);
						}
					} else {
						snprintf(tmp, BUFSIZE, "%so", ircd->adminunset);
					}
				} else {
					snprintf(tmp, BUFSIZE, "-o");
				}

				if (ircdcap->tsmode) {
					snprintf(buf, BUFSIZE - 1, "%ld", (long int) time(NULL));
					av[1] = buf;
					av[2] = tmp;
					av[3] = cu->user->nick;
					ac = 4;
				} else {
					av[1] = tmp;
					av[2] = cu->user->nick;
					ac = 3;
				}

				do_cmode(c->ci->bi->nick, ac, av);
			}
		} else {
			for (cu = c->users; cu; cu = next) {
				next = cu->next;
				if (!chan_has_user_status(c, cu->user, CUS_OP)) {
					if (!chan_has_user_status(c, cu->user, CUS_PROTECT)) {
						if (!chan_has_user_status(c, cu->user, CUS_OWNER)) {
							continue;
						} else {
							snprintf(tmp, BUFSIZE, "%so", ircd->ownerunset);
						}
					} else {
						snprintf(tmp, BUFSIZE, "%so", ircd->adminunset);
					}
				} else {
					snprintf(tmp, BUFSIZE, "-o");
				}

				if (ircdcap->tsmode) {
					snprintf(buf, BUFSIZE - 1, "%ld", (long int) time(NULL));
					av[1] = buf;
					av[2] = tmp;
					av[3] = cu->user->nick;
					ac = 4;
				} else {
					av[1] = tmp;
					av[2] = cu->user->nick;
					ac = 3;
				}

				xanadu_cmd_mode(c->ci->bi->nick, c->name, "%s :%s", tmp, cu->user->nick);
				do_cmode(c->ci->bi->nick, ac, av);
			}
		}

	/* Clear Channel HOPs */
    } else if (ircd->halfop && type == 6) {
		char *av[4];
		struct c_userlist *cu, *next;
		char buf[BUFSIZE];
		int ac;

		notice(c->ci->bi->nick, c->name, "CLEAR HOPS command from %s", u->nick);

		av[0] = c->name;
		if (ircdcap->tsmode)
			av[2] = sstrdup("-h");
		else
			av[1] = sstrdup("-h");

		if (ircd->svsmode_ucmode) {
			xanadu_cmd_svsmode_chan(c->name, "-h", NULL);

			for (cu = c->users; cu; cu = next) {
				next = cu->next;
				if (!chan_has_user_status(c, cu->user, CUS_HALFOP))
					continue;

				if (ircdcap->tsmode) {
					snprintf(buf, BUFSIZE - 1, "%ld", (long int) time(NULL));
					av[1] = buf;
					av[3] = cu->user->nick;
					ac = 4;
				} else {
					av[2] = cu->user->nick;
					ac = 3;
				}

				do_cmode(c->ci->bi->nick, ac, av);
			}
		} else {
			for (cu = c->users; cu; cu = next) {
				next = cu->next;
				if (!chan_has_user_status(c, cu->user, CUS_HALFOP))
					continue;

				if (ircdcap->tsmode) {
					snprintf(buf, BUFSIZE - 1, "%ld", (long int) time(NULL));
					av[1] = buf;
					av[3] = cu->user->nick;
					ac = 4;
				} else {
					av[2] = cu->user->nick;
					ac = 3;
				}

				xanadu_cmd_mode(c->ci->bi->nick, c->name, "-h :%s", cu->user->nick);
				do_cmode(c->ci->bi->nick, ac, av);
			}
		}

		if (ircdcap->tsmode)
			free(av[2]);
		else
			free(av[1]);

	} else if (!(ircd->halfop) && type == 6) {
		moduleNoticeLang(c->ci->bi->nick, u, LANG_HOPS_UNSUPPORTED);

	/* Clear Channel Voices */
    } else if (type == 7) {
		char *av[4];
		struct c_userlist *cu, *next;
		char buf[BUFSIZE];
		int ac;

		notice(c->ci->bi->nick, c->name, "CLEAR VOICES command from %s", u->nick);

		av[0] = c->name;
		if (ircdcap->tsmode)
			av[2] = sstrdup("-v");
		else
			av[1] = sstrdup("-v");

		if (ircd->svsmode_ucmode) {
			xanadu_cmd_svsmode_chan(av[0], "-v", NULL);

			for (cu = c->users; cu; cu = next) {
				next = cu->next;
				if (!chan_has_user_status(c, cu->user, CUS_VOICE))
					continue;

				if (ircdcap->tsmode) {
					snprintf(buf, BUFSIZE - 1, "%ld", (long int) time(NULL));
					av[1] = buf;
					av[3] = cu->user->nick;
					ac = 4;
				} else {
					av[2] = cu->user->nick;
					ac = 3;
				}

				do_cmode(c->ci->bi->nick, ac, av);
			}
		} else {
			for (cu = c->users; cu; cu = next) {
				next = cu->next;
				if (!chan_has_user_status(c, cu->user, CUS_VOICE))
					continue;

				if (ircdcap->tsmode) {
					snprintf(buf, BUFSIZE - 1, "%ld", (long int) time(NULL));
					av[1] = buf;
					av[3] = cu->user->nick;
					ac = 4;
				} else {
					av[2] = cu->user->nick;
					ac = 3;
				}

				xanadu_cmd_mode(c->ci->bi->nick, c->name, "-v :%s", cu->user->nick);
				do_cmode(c->ci->bi->nick, ac, av);
			}
		}

		if (ircdcap->tsmode)
			free(av[2]);
		else
			free(av[1]);

	/* Clear Channel Users */
    } else if (type == 8) {
		char *av[3];
		struct c_userlist *cu, *next;
		char buf[256];

		snprintf(buf, sizeof(buf), "CLEAR USERS command from %s", u->nick);
		notice(c->ci->bi->nick, c->name, buf);

		av[0] = c->name;
		av[2] = buf;
		for (cu = c->users; cu; cu = next) {
			next = cu->next;
			av[1] = cu->user->nick;
			xanadu_cmd_kick(c->ci->bi->nick, av[0], av[1], av[2]);
			do_kick(c->ci->bi->nick, 3, av);
		}
    }

    /* Should NEVER happen, therefor if it does, stop processing */
    else {
    	alog("[bs_fantasy_ext] An error has occured while processing CLEAR !!!");
    	return MOD_STOP;
	}

    return MOD_CONT;
}
#endif

/* EOF */

/**
 * Methods to modify the ChanServ Channel settings. - Source
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
 * Last Updated   : 05/04/2007
 *
 **/

#ifdef ENABLE_SET
#include "set.h"

/* Botserv set commands */
static int do_set(User * u, ChannelInfo * ci, char *option, char *value) {
	int is_servadmin = is_services_admin(u);

	if (readonly)
		notice_lang(s_BotServ, u, BOT_SET_DISABLED);
	else if (!option || !value)
		syntax_error(s_BotServ, u, "SET", BOT_SET_SYNTAX);
	else if (ci->flags & CI_VERBOTEN)
		notice_lang(ci->bi->nick, u, CHAN_X_FORBIDDEN, ci->name);
	else if (!is_servadmin && !check_access(u, ci, CA_SET))
		notice_lang(ci->bi->nick, u, ACCESS_DENIED);
	else {
		if (!stricmp(option, "DONTKICKOPS")) {
			if (!stricmp(value, "ON")) {
				ci->botflags |= BS_DONTKICKOPS;
				notice_lang(ci->bi->nick, u, BOT_SET_DONTKICKOPS_ON, ci->name);
			} else if (!stricmp(value, "OFF")) {
				ci->botflags &= ~BS_DONTKICKOPS;
				notice_lang(ci->bi->nick, u, BOT_SET_DONTKICKOPS_OFF, ci->name);
			} else {
				syntax_error(ci->bi->nick, u, "SET DONTKICKOPS", BOT_SET_DONTKICKOPS_SYNTAX);
			}
		} else if (!stricmp(option, "DONTKICKVOICES")) {
			if (!stricmp(value, "ON")) {
				ci->botflags |= BS_DONTKICKVOICES;
				notice_lang(ci->bi->nick, u, BOT_SET_DONTKICKVOICES_ON, ci->name);
			} else if (!stricmp(value, "OFF")) {
				ci->botflags &= ~BS_DONTKICKVOICES;
				notice_lang(ci->bi->nick, u, BOT_SET_DONTKICKVOICES_OFF, ci->name);
			} else {
			syntax_error(ci->bi->nick, u, "SET DONTKICKVOICES", BOT_SET_DONTKICKVOICES_SYNTAX);
			}
		} else if (!stricmp(option, "FANTASY")) {
			if (!stricmp(value, "ON")) {
				ci->botflags |= BS_FANTASY;
				notice_lang(ci->bi->nick, u, BOT_SET_FANTASY_ON, ci->name);
			} else if (!stricmp(value, "OFF")) {
				ci->botflags &= ~BS_FANTASY;
				notice_lang(ci->bi->nick, u, BOT_SET_FANTASY_OFF, ci->name);
			} else {
				syntax_error(ci->bi->nick, u, "SET FANTASY", BOT_SET_FANTASY_SYNTAX);
			}
		} else if (!stricmp(option, "GREET")) {
			if (!stricmp(value, "ON")) {
				ci->botflags |= BS_GREET;
				notice_lang(ci->bi->nick, u, BOT_SET_GREET_ON, ci->name);
			} else if (!stricmp(value, "OFF")) {
				ci->botflags &= ~BS_GREET;
				notice_lang(ci->bi->nick, u, BOT_SET_GREET_OFF, ci->name);
			} else {
				syntax_error(ci->bi->nick, u, "SET GREET", BOT_SET_GREET_SYNTAX);
			}
		} else if (is_servadmin && !stricmp(option, "NOBOT")) {
			if (!stricmp(value, "ON")) {
				ci->botflags |= BS_NOBOT;
				if (ci->bi)
					unassign(u, ci);
				notice_lang(ci->bi->nick, u, BOT_SET_NOBOT_ON, ci->name);
			} else if (!stricmp(value, "OFF")) {
				ci->botflags &= ~BS_NOBOT;
				notice_lang(ci->bi->nick, u, BOT_SET_NOBOT_OFF, ci->name);
			} else {
				syntax_error(ci->bi->nick, u, "SET NOBOT", BOT_SET_NOBOT_SYNTAX);
			}
		} else if (!stricmp(option, "SYMBIOSIS")) {
			if (!stricmp(value, "ON")) {
				ci->botflags |= BS_SYMBIOSIS;
				notice_lang(ci->bi->nick, u, BOT_SET_SYMBIOSIS_ON, ci->name);
			} else if (!stricmp(value, "OFF")) {
				ci->botflags &= ~BS_SYMBIOSIS;
				notice_lang(ci->bi->nick, u, BOT_SET_SYMBIOSIS_OFF, ci->name);
			} else {
				syntax_error(ci->bi->nick, u, "SET SYMBIOSIS", BOT_SET_SYMBIOSIS_SYNTAX);
			}
		} else {
			notice_help(ci->bi->nick, u, BOT_SET_UNKNOWN, option);
		}
	}
	return MOD_CONT;
}


/* Chanserv Set commands */
static int do_set_founder(User * u, ChannelInfo * ci, char *param) {
	NickAlias *na = findnick(param);
	NickCore *nc, *nc0 = ci->founder;

	if (!na) {
		notice_lang(ci->bi->nick, u, NICK_X_NOT_REGISTERED, param);
		return MOD_CONT;
	} else if (na->status & NS_VERBOTEN) {
		notice_lang(ci->bi->nick, u, NICK_X_FORBIDDEN, param);
		return MOD_CONT;
	}

	nc = na->nc;
	if (nc->channelmax > 0 && nc->channelcount >= nc->channelmax && !is_services_admin(u)) {
		notice_lang(ci->bi->nick, u, CHAN_SET_FOUNDER_TOO_MANY_CHANS, param);
		return MOD_CONT;
	}

	alog("%s: Changing founder of %s from %s to %s by %s!%s@%s", ci->bi->nick, ci->name, ci->founder->display,
		nc->display, u->nick, u->username, u->host);

	/* Founder and successor must not be the same group */
	if (nc == ci->successor)
		ci->successor = NULL;

	nc0->channelcount--;
	ci->founder = nc;
	nc->channelcount++;

	notice_lang(ci->bi->nick, u, CHAN_FOUNDER_CHANGED, ci->name, param);
	return MOD_CONT;
}

static int do_set_successor(User * u, ChannelInfo * ci, char *param) {
	NickAlias *na;
	NickCore *nc;

	if (param) {
		na = findnick(param);

		if (!na) {
			notice_lang(ci->bi->nick, u, NICK_X_NOT_REGISTERED, param);
			return MOD_CONT;
		}
		if (na->status & NS_VERBOTEN) {
			notice_lang(ci->bi->nick, u, NICK_X_FORBIDDEN, param);
			return MOD_CONT;
		}
		if (na->nc == ci->founder) {
			notice_lang(ci->bi->nick, u, CHAN_SUCCESSOR_IS_FOUNDER, param, ci->name);
			return MOD_CONT;
		}
		nc = na->nc;

	} else {
		nc = NULL;
	}

	alog("%s: Changing successor of %s from %s to %s by %s!%s@%s", ci->bi->nick, ci->name,
		(ci->successor ? ci->successor->display : "none"),
		(nc ? nc->display : "none"), u->nick, u->username, u->host);

	ci->successor = nc;

	if (nc)
		notice_lang(ci->bi->nick, u, CHAN_SUCCESSOR_CHANGED, ci->name, param);
	else
		notice_lang(ci->bi->nick, u, CHAN_SUCCESSOR_UNSET, ci->name);
	return MOD_CONT;
}

static int do_set_desc(User * u, ChannelInfo * ci, char *param) {
	if (ci->desc)
		free(ci->desc);
	ci->desc = sstrdup(param);
	notice_lang(ci->bi->nick, u, CHAN_DESC_CHANGED, ci->name, param);
	return MOD_CONT;
}

static int do_set_url(User * u, ChannelInfo * ci, char *param) {
	if (ci->url)
		free(ci->url);
	if (param) {
		ci->url = sstrdup(param);
		notice_lang(ci->bi->nick, u, CHAN_URL_CHANGED, ci->name, param);
	} else {
		ci->url = NULL;
		notice_lang(ci->bi->nick, u, CHAN_URL_UNSET, ci->name);
	}
	return MOD_CONT;
}

static int do_set_email(User * u, ChannelInfo * ci, char *param) {
	if (ci->email)
		free(ci->email);
	if (param) {
		ci->email = sstrdup(param);
		notice_lang(ci->bi->nick, u, CHAN_EMAIL_CHANGED, ci->name, param);
	} else {
		ci->email = NULL;
		notice_lang(ci->bi->nick, u, CHAN_EMAIL_UNSET, ci->name);
	}
	return MOD_CONT;
}

static int do_set_entrymsg(User * u, ChannelInfo * ci, char *param) {
	if (ci->entry_message)
		free(ci->entry_message);
	if (param) {
		ci->entry_message = sstrdup(param);
		notice_lang(ci->bi->nick, u, CHAN_ENTRY_MSG_CHANGED, ci->name, param);
	} else {
		ci->entry_message = NULL;
		notice_lang(ci->bi->nick, u, CHAN_ENTRY_MSG_UNSET, ci->name);
	}
	return MOD_CONT;
}

static int do_set_mlock(User * u, ChannelInfo * ci, char *param) {
	int add = -1;               /* 1 if adding, 0 if deleting, -1 if neither */
	unsigned char mode;
	CBMode *cbm;
	char *modes, *mbegin, *args;
	int i = 0;

	if (checkDefCon(DEFCON_NO_MLOCK_CHANGE)) {
		notice_lang(ci->bi->nick, u, OPER_DEFCON_DENIED);
		return MOD_CONT;
	}

	/* Reinitialize everything */
	if (ircd->chanreg) {
		ci->mlock_on = ircd->regmode;
	} else {
		ci->mlock_on = 0;
	}
	ci->mlock_off = ci->mlock_limit = 0;
	ci->mlock_key = NULL;
	if (ircd->fmode) {
		ci->mlock_flood = NULL;
	}
	if (ircd->Lmode) {
		ci->mlock_redirect = NULL;
	}

	modes = myStrGetToken(param,' ',0);
	mbegin = modes;
	args = myStrGetTokenRemainder(param, ' ', 1);

	while ((mode = *modes++)) {
		switch (mode) {
			case '+':
				add = 1;
				continue;
			case '-':
				add = 0;
				continue;
			default:
				if (add < 0)
					continue;
		}

		if ((int) mode < 128 && (cbm = &cbmodes[(int) mode])->flag != 0) {
			if ((cbm->flags & CBM_NO_MLOCK) || ((cbm->flags & CBM_NO_USER_MLOCK) && !is_oper(u))) {
				notice_lang(ci->bi->nick, u, CHAN_SET_MLOCK_IMPOSSIBLE_CHAR, mode);
			} else if (add) {
				ci->mlock_on |= cbm->flag;
				ci->mlock_off &= ~cbm->flag;
				if (cbm->cssetvalue)
					cbm->cssetvalue(ci, myStrGetToken(args, ' ', i));
			} else {
				ci->mlock_off |= cbm->flag;
				if (ci->mlock_on & cbm->flag) {
					ci->mlock_on &= ~cbm->flag;
					if (cbm->cssetvalue)
						cbm->cssetvalue(ci, NULL);
				}
			}
		} else {
			notice_lang(ci->bi->nick, u, CHAN_SET_MLOCK_UNKNOWN_CHAR, mode);
		}
		i++;
	}                           /* while (*param) */

	if (ircd->Lmode) {
		/* We can't mlock +L if +l is not mlocked as well. */
		if ((ci->mlock_on & ircd->chan_lmode) && !(ci->mlock_on & anope_get_limit_mode())) {
			ci->mlock_on &= ~ircd->chan_lmode;
			free(ci->mlock_redirect);
			notice_lang(ci->bi->nick, u, CHAN_SET_MLOCK_L_REQUIRED);
		}
	}

	/* Some ircd we can't set NOKNOCK without INVITE */
	/* So check if we need there is a NOKNOCK MODE and that we need INVITEONLY */
	if (ircd->noknock && ircd->knock_needs_i) {
		if ((ci->mlock_on & ircd->noknock) && !(ci->mlock_on & anope_get_invite_mode())) {
			ci->mlock_on &= ~ircd->noknock;
			notice_lang(ci->bi->nick, u, CHAN_SET_MLOCK_K_REQUIRED);
		}
	}

	/* Since we always enforce mode r there is no way to have no
	* mode lock at all.
	*/
	if (get_mlock_modes(ci, 0)) {
		notice_lang(ci->bi->nick, u, CHAN_MLOCK_CHANGED, ci->name, get_mlock_modes(ci, 0));
	}

	/* Implement the new lock. */
	if (ci->c)
		check_modes(ci->c);

	/* Free the modes and their arguments */
	/* Thanks to Trystan for pointing out an issue free'ing modes */
	if (mbegin) free(mbegin);
	if (args) free(args);

	return MOD_CONT;
}

static int do_set_bantype(User * u, ChannelInfo * ci, char *param) {
	char *endptr;

	int16 bantype = strtol(param, &endptr, 10);

	if (*endptr != 0 || bantype < 0 || bantype > 3) {
		notice_lang(ci->bi->nick, u, CHAN_SET_BANTYPE_INVALID, param);
	} else {
		ci->bantype = bantype;
		notice_lang(ci->bi->nick, u, CHAN_SET_BANTYPE_CHANGED, ci->name, ci->bantype);
	}
	return MOD_CONT;
}

static int do_set_keeptopic(User * u, ChannelInfo * ci, char *param) {
	if (stricmp(param, "ON") == 0) {
		ci->flags |= CI_KEEPTOPIC;
		notice_lang(ci->bi->nick, u, CHAN_SET_KEEPTOPIC_ON, ci->name);
	} else if (stricmp(param, "OFF") == 0) {
		ci->flags &= ~CI_KEEPTOPIC;
		notice_lang(ci->bi->nick, u, CHAN_SET_KEEPTOPIC_OFF, ci->name);
	} else {
		moduleNoticeLang(ci->bi->nick, u, LANG_SET_KEEPTOPIC_SYNTAX, BSFantasyCharacter);
	}
	return MOD_CONT;
}

static int do_set_topiclock(User * u, ChannelInfo * ci, char *param) {
	if (stricmp(param, "ON") == 0) {
		ci->flags |= CI_TOPICLOCK;
		notice_lang(ci->bi->nick, u, CHAN_SET_TOPICLOCK_ON, ci->name);
	} else if (stricmp(param, "OFF") == 0) {
		ci->flags &= ~CI_TOPICLOCK;
		notice_lang(ci->bi->nick, u, CHAN_SET_TOPICLOCK_OFF, ci->name);
	} else {
		moduleNoticeLang(ci->bi->nick, u, LANG_SET_TOPICLOCK_SYNTAX, BSFantasyCharacter);
	}
	return MOD_CONT;
}

static int do_set_private(User * u, ChannelInfo * ci, char *param) {
	if (stricmp(param, "ON") == 0) {
		ci->flags |= CI_PRIVATE;
		notice_lang(ci->bi->nick, u, CHAN_SET_PRIVATE_ON, ci->name);
	} else if (stricmp(param, "OFF") == 0) {
		ci->flags &= ~CI_PRIVATE;
		notice_lang(ci->bi->nick, u, CHAN_SET_PRIVATE_OFF, ci->name);
	} else {
		moduleNoticeLang(ci->bi->nick, u, LANG_SET_PRIVATE_SYNTAX, BSFantasyCharacter);
	}
	return MOD_CONT;
}

static int do_set_secureops(User * u, ChannelInfo * ci, char *param) {
	if (stricmp(param, "ON") == 0) {
		ci->flags |= CI_SECUREOPS;
		notice_lang(ci->bi->nick, u, CHAN_SET_SECUREOPS_ON, ci->name);
	} else if (stricmp(param, "OFF") == 0) {
		ci->flags &= ~CI_SECUREOPS;
		notice_lang(ci->bi->nick, u, CHAN_SET_SECUREOPS_OFF, ci->name);
	} else {
		moduleNoticeLang(ci->bi->nick, u, LANG_SET_SECUREOPS_SYNTAX, BSFantasyCharacter);
	}
	return MOD_CONT;
}

static int do_set_securefounder(User * u, ChannelInfo * ci, char *param) {
	if (stricmp(param, "ON") == 0) {
		ci->flags |= CI_SECUREFOUNDER;
		notice_lang(ci->bi->nick, u, CHAN_SET_SECUREFOUNDER_ON, ci->name);
	} else if (stricmp(param, "OFF") == 0) {
		ci->flags &= ~CI_SECUREFOUNDER;
		notice_lang(ci->bi->nick, u, CHAN_SET_SECUREFOUNDER_OFF, ci->name);
	} else {
		moduleNoticeLang(ci->bi->nick, u, LANG_SET_SECUREFOUNDER_SYNTAX, BSFantasyCharacter);
	}
	return MOD_CONT;
}

static int do_set_restricted(User * u, ChannelInfo * ci, char *param) {
	if (stricmp(param, "ON") == 0) {
		ci->flags |= CI_RESTRICTED;
		if (ci->levels[CA_NOJOIN] < 0)
			ci->levels[CA_NOJOIN] = 0;
		notice_lang(ci->bi->nick, u, CHAN_SET_RESTRICTED_ON, ci->name);
	} else if (stricmp(param, "OFF") == 0) {
		ci->flags &= ~CI_RESTRICTED;
		if (ci->levels[CA_NOJOIN] >= 0)
			ci->levels[CA_NOJOIN] = -2;
		notice_lang(ci->bi->nick, u, CHAN_SET_RESTRICTED_OFF, ci->name);
	} else {
		moduleNoticeLang(ci->bi->nick, u, LANG_SET_RESTRICTED_SYNTAX, BSFantasyCharacter);
	}
	return MOD_CONT;
}

static int do_set_secure(User * u, ChannelInfo * ci, char *param) {
	if (stricmp(param, "ON") == 0) {
		ci->flags |= CI_SECURE;
		notice_lang(ci->bi->nick, u, CHAN_SET_SECURE_ON, ci->name);
	} else if (stricmp(param, "OFF") == 0) {
		ci->flags &= ~CI_SECURE;
		notice_lang(ci->bi->nick, u, CHAN_SET_SECURE_OFF, ci->name);
	} else {
		moduleNoticeLang(ci->bi->nick, u, LANG_SET_SECURE_SYNTAX, BSFantasyCharacter);
	}
	return MOD_CONT;
}

static int do_set_signkick(User * u, ChannelInfo * ci, char *param) {
	if (stricmp(param, "ON") == 0) {
		ci->flags |= CI_SIGNKICK;
		ci->flags &= ~CI_SIGNKICK_LEVEL;
		notice_lang(ci->bi->nick, u, CHAN_SET_SIGNKICK_ON, ci->name);
	} else if (stricmp(param, "LEVEL") == 0) {
		ci->flags |= CI_SIGNKICK_LEVEL;
		ci->flags &= ~CI_SIGNKICK;
		notice_lang(ci->bi->nick, u, CHAN_SET_SIGNKICK_LEVEL, ci->name);
	} else if (stricmp(param, "OFF") == 0) {
		ci->flags &= ~(CI_SIGNKICK | CI_SIGNKICK_LEVEL);
		notice_lang(ci->bi->nick, u, CHAN_SET_SIGNKICK_OFF, ci->name);
	} else {
		moduleNoticeLang(ci->bi->nick, u, LANG_SET_SIGNKICK_SYNTAX, BSFantasyCharacter);
	}
	return MOD_CONT;
}

static int do_set_opnotice(User * u, ChannelInfo * ci, char *param) {
	if (stricmp(param, "ON") == 0) {
		ci->flags |= CI_OPNOTICE;
		notice_lang(ci->bi->nick, u, CHAN_SET_OPNOTICE_ON, ci->name);
	} else if (stricmp(param, "OFF") == 0) {
		ci->flags &= ~CI_OPNOTICE;
		notice_lang(ci->bi->nick, u, CHAN_SET_OPNOTICE_OFF, ci->name);
	} else {
		moduleNoticeLang(ci->bi->nick, u, LANG_SET_OPNOTICE_SYNTAX, BSFantasyCharacter);
	}
	return MOD_CONT;
}

#define CHECKLEV(lev) ((ci->levels[(lev)] != ACCESS_INVALID) && (access->level >= ci->levels[(lev)]))

static int do_set_xop(User * u, ChannelInfo * ci, char *param) {
	if (stricmp(param, "ON") == 0) {
		if (!(ci->flags & CI_XOP)) {
			int i;
			ChanAccess *access;

			for (access = ci->access, i = 0; i < ci->accesscount; access++, i++) {
				if (!access->in_use)
					continue;
				/* This will probably cause wrong levels to be set, but hey,
				* it's better than losing it altogether.
				*/
				if (CHECKLEV(CA_AKICK) || CHECKLEV(CA_SET)) {
					access->level = ACCESS_SOP;
				} else if (CHECKLEV(CA_AUTOOP) || CHECKLEV(CA_OPDEOP) || CHECKLEV(CA_OPDEOPME)) {
					access->level = ACCESS_AOP;
				} else if (ircd->halfop) {
					if (CHECKLEV(CA_AUTOHALFOP) || CHECKLEV(CA_HALFOP) || CHECKLEV(CA_HALFOPME)) {
						access->level = ACCESS_HOP;
					}
				} else if (CHECKLEV(CA_AUTOVOICE) || CHECKLEV(CA_VOICE) || CHECKLEV(CA_VOICEME)) {
					access->level = ACCESS_VOP;
				} else {
					access->in_use = 0;
					access->nc = NULL;
				}
			}

			reset_levels(ci);
			ci->flags |= CI_XOP;
		}

		alog("%s: %s!%s@%s enabled XOP for %s", ci->bi->nick, u->nick, u->username, u->host, ci->name);
		notice_lang(ci->bi->nick, u, CHAN_SET_XOP_ON, ci->name);
	} else if (stricmp(param, "OFF") == 0) {
		ci->flags &= ~CI_XOP;

		alog("%s: %s!%s@%s disabled XOP for %s", ci->bi->nick, u->nick, u->username, u->host, ci->name);
		notice_lang(ci->bi->nick, u, CHAN_SET_XOP_OFF, ci->name);
	} else {
		moduleNoticeLang(ci->bi->nick, u, LANG_SET_XOP_SYNTAX, BSFantasyCharacter);
	}
	return MOD_CONT;
}

#undef CHECKLEV

static int do_set_peace(User * u, ChannelInfo * ci, char *param) {
	if (stricmp(param, "ON") == 0) {
		ci->flags |= CI_PEACE;
		notice_lang(ci->bi->nick, u, CHAN_SET_PEACE_ON, ci->name);
	} else if (stricmp(param, "OFF") == 0) {
		ci->flags &= ~CI_PEACE;
		notice_lang(ci->bi->nick, u, CHAN_SET_PEACE_OFF, ci->name);
	} else {
		moduleNoticeLang(ci->bi->nick, u, LANG_SET_PEACE_SYNTAX, BSFantasyCharacter);
	}
	return MOD_CONT;
}

static int do_set_noexpire(User * u, ChannelInfo * ci, char *param) {
	if (!is_services_admin(u)) {
		notice_lang(ci->bi->nick, u, PERMISSION_DENIED);
		return MOD_CONT;
	}
	if (stricmp(param, "ON") == 0) {
		ci->flags |= CI_NO_EXPIRE;
		notice_lang(ci->bi->nick, u, CHAN_SET_NOEXPIRE_ON, ci->name);
	} else if (stricmp(param, "OFF") == 0) {
		ci->flags &= ~CI_NO_EXPIRE;
		notice_lang(ci->bi->nick, u, CHAN_SET_NOEXPIRE_OFF, ci->name);
	} else {
		moduleNoticeLang(ci->bi->nick, u, LANG_SET_NOEXPIRE_SYNTAX, BSFantasyCharacter);
	}
	return MOD_CONT;
}
#endif

/* EOF */

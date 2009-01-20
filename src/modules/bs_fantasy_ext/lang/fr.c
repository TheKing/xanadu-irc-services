/**
 * French Language file.
 *
 ***********
 * Module Name    : bs_fantasy_ext
 * Author         : Viper <Viper@Absurd-IRC.net>
 * Creation Date  : 17/06/2007
 *
 * More info on http://modules.anope.org and http://forum.anope.org
 ***********
 *
 * French language file provided by Seb Inconnu.
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
 * Last Updated   : 03/04/2008
 *
 **/

void add_french(void);

void add_french(void) {
	char *langtable_fr[] = {
		/* LANG_HELP */
		"SYNTAX: %shelp\n"
		"Voici l'aide pour les commandes fantasy botserv.",
		/* LANG_CLEAR_SYNTAX */
		"SYNTAX: %sclear [modes|bans|excepts|invites|ops|hops|voices|users]",
		/* LANG_EXCEPTS_UNSUPPORTED */
		"Les Chans Excepts ne sont pas support�s sur ce r�seau.",
		/* LANG_INVITEMODE_UNSUPPORTED */
		"Les Chans Invites ne sont pas support�s sur ce r�seau.",
		/* LANG_HOPS_UNSUPPORTED */
		"Le Grade Halfops n'est pas support� par ce r�seau.",
		/* LANG_SOP_SYNTAX */
		"SYNTAX: %ssop {add|del|list|clear} [pseudo|entry-list]",
		/* LANG_SOP_SYNTAX_EXT */
		"SYNTAX: \002%ssop\002 add [pseudo]\n"
		"        \002%ssop\002 del [pseudo|num de list|list]\n"
		"        \002%ssop\002 list [mask|list]\n"
		"        \002%ssop\002 clear"
		" \n"
		"Maintains the \002SOP\002 (SuperOP) \002list\002 for the channel.\n"
		"The SOP list gives users all rights given by the AOP list, and adds\n"
		"those needed to use the AutoKick and the BadWords lists,\n"
		"to send and read channel memos, and so on.\n"
		" \n"
		"This command may have been disabled for your channel, and\n"
		"in that case you need to use the access list. See HELP ACCESS\n"
		"for information about the access list, and HELP SET XOP to know how\n"
		"to toggle between the access list and xOP list systems.",
		/* LANG_AOP_SYNTAX */
		"SYNTAX: %saop {add|del|list|clear} [pseudo|entry-list]",
		/* LANG_AOP_SYNTAX_EXT */
		"SYNTAX: \002%saop\002 add [pseudo]\n"
		"        \002%saop\002 del [pseudo|num de list|list]\n"
		"        \002%saop\002 list [mask|list]\n"
		"        \002%saop\002 clear"
		" \n"
		"Maintains the \002AOP\002 (AutoOP) \002list\002 for the channel.\n"
		"The AOP list gives users the right to be auto-opped on your channel,\n"
		"to unban or invite themselves if needed, to have their\n"
		"greet message showed on join, and so on.\n"
		" \n"
		"This command may have been disabled for your channel, and\n"
		"in that case you need to use the access list. See HELP ACCESS\n"
		"for information about the access list, and HELP SET XOP to know how\n"
		"to toggle between the access list and xOP list systems.",
		/* LANG_HOP_SYNTAX */
		"SYNTAX: %shop {add|del|list|clear} [pseudo|entry-list]",
		/* LANG_HOP_SYNTAX_EXT */
		"SYNTAX: \002%shop\002 add [pseudo]\n"
		"        \002%shop\002 del [pseudo|num de list|list]\n"
		"        \002%shop\002 list [mask|list]\n"
		"        \002%shop\002 clear"
		" \n"
		"Maintains the \002HOP\002 (HalfOP) \002list\002 for the channel. The HOP\n"
		"list gives users the right to be auto-halfopped on your channel\n"
		" \n"
		"This command may have been disabled for your channel, and\n"
		"in that case you need to use the access list. See HELP ACCESS\n"
		"for information about the access list, and HELP SET XOP to know how\n"
		"to toggle between the access list and xOP list systems.",
		/* LANG_VOP_SYNTAX */
		"SYNTAX: %svop {add|del|list|clear} [pseudo|entry-list]",
		/* LANG_VOP_SYNTAX_EXT */
		"SYNTAX: \002%svop\002 add [pseudo]\n"
		"        \002%svop\002 del [pseudo|num de list|list]\n"
		"        \002%svop\002 list [mask|list]\n"
		"        \002%svop\002 clear"
		" \n"
		"Maintains the \002VOP\002 (VOicePeople) \002list\002 for a channel.\n"
		"The VOP list allows users to be auto-voiced and to voice\n"
		"themselves if they aren't.\n"
		" \n"
		"This command may have been disabled for your channel, and\n"
		"in that case you need to use the access list. See HELP ACCESS\n"
		"for information about the access list, and HELP SET XOP to know how\n"
		"to toggle between the access list and xOP list systems.",
		/* LANG_ACCESS_SYNTAX */
		"SYNTAX: %saccess {add|del|list|clear} [pseudo [level]|entry-list]",
		/* LANG_ACCESS_SYNTAX_EXT */
		"SYNTAX: \002%saccess\002 add [pseudo] [level]\n"
		"        \002%saccess\002 del [pseudo|num de list|list]\n"
		"        \002%saccess\002 list [mask|list]"
		"        \002%saccess\002 clear"
		" \n"
		"Maintains the \002access list\002 for a channel.  The access\n"
		"list specifies which users are allowed chanop status or\n"
		"access to commands on the channel.\n"
		"Different user levels allow for access to different subsets of\n"
		"privileges; cf. HELP ACCESS LEVELS for more specific information.\n"
		"Any nick not on the access list has a user level of 0.",
		/* LANG_LEVELS_SYNTAX */
		"SYNTAX: %slevels {set|dis[able]|list|reset} [item [level]]",
		/* LANG_LEVELS_SYNTAX_EXT */
		"SYNTAX: \002%slevels\002 set [type] [level]\n"
		"        \002%slevels\002 {dis|disable} [type]\n"
		"        \002%slevels\002 list\n"
		"        \002%slevels\002 reset"
		" \n"
		"The \002LEVELS\002 command allows fine control over the meaning of\n"
		"the numeric access levels used for the channel.  With this command,\n"
		"you can define the access level required for most administrative functions.\n"
		"(The SET FOUNDER and SET PASSWORD commands, as well as this command, \n"
		"are always restricted to the channel founder.) \n"
		" \n"
		"\002LEVELS SET\002 allows the access level for a function or group of functions to be changed.\n"
		"\002LEVELS DISABLE\002 (or \002DIS\002 for short) disables an automatic\n"
		"feature or disallows access to a function by anyone other than the channel founder.\n"
		"\002LEVELS LIST\002 shows the current levels for each function or group of functions.\n"
		"\002LEVELS RESET\002 resets the levels to the default levels of a newly-created channel.\n"
		"(See HELP ACCESS LEVELS for more info.)\n"
		" \n"
		"For a list of the features and functions whose levels can be\n"
		"set, see HELP LEVELS DESC.",
		/* LANG_LEVELS_ERR_NO_VAL */
		"Erreur: Pas de valeur donn�e au param�tre SET.",
		/* LANG_LEVELS_ERR_VAL */
		"Erreur: Valeur donn�e non-num�rique.",
		/* LANG_ONLY_FOUNDERS */
		"Acc�s refus� : Seuls les Owners peuvent utiliser cette commande.",
		/* LANG_GOLIST_OPER_ONLY */
		"IRCop",
		/* LANG_GOLIST_OPER_AND_SO */
		"IRCop + Services",
		/* LANG_GOLIST_OPER_AND_SA */
		"IRCop + Services Administrateur",
		/* LANG_GOLIST_OPER_AND_SRA */
		"IRCOperateur + Services Root Administrateur",
		/* LANG_GOLIST_NONE */
		"Pas d'IRCops disponiblent pour l'instant",
		/* LANG_GOLIST_HEADER */
		"-----------------------------------------------------------------------\n"
		"Les IRCop suivant sont en ligne:\n"
		"-----------------------------------------------------------------------",
		/* LANG_GOLIST_FOOTER */
		"-----------------------------------------------------------------------\n"
		"Total d'IRCops disponiblent : %d \n"
		"-----------------------------------------------------------------------\n"
		" -> Toujours essayer de contacter un IRCop sur un chan avant de le PV\n"
		" -> Eviter de PV un IRCop sans avoir sa permission avant.\n"
		" -> faire un /whois sur IRCop avant de vouloir lui parler.\n"
		" -> Lire (et respecter) un message d'away pr�sent dans le Whois.\n"
		"-----------------------------------------------------------------------",
		/* LANG_AKICK_SYNTAX */
		"SYNTAX: %sakick {add|stick|unstick|del|list|view|enforce|clear} [pseudo-or-usermask] [raison]",
		/* LANG_AKICK_SYNTAX_EXT */
		"SYNTAX: \002%sakick\002 add [mask | user] [raison]\n"
		"        \002%sakick\002 {stick|unstick|del} mask\n"
		"        \002%sakick\002 {list|view} [mask]\n"
		"        \002%sakick\002 {enforce|clear}",
		/* LANG_BADWORDS_SYNTAX */
		"SYNTAX: %sbadwords {add|del|list|clear} [mot|entry-list] [single|start|end|any]",
		/* LANG_BADWORDS_SYNTAX_EXT */
		"SYNTAX: \002%sbadwords\002 add [mot] {single|start|end|any}\n"
		"SYNTAX: \002%sbadwords\002 del [mot|num de list|list]\n"
		"SYNTAX: \002%sbadwords\002 list [mask|list]\n"
		"SYNTAX: \002%sbadwords\002 clear",
		/* LANG_BADWORDS_ERR_STYLE */
		"Erreur: Style invalide. Utiliser uniquement START, SINGLE, ANY ou END.",
		/* LANG_CMD_NOT_AV */
		"Cette commande n'est pas disponible. Contacter le staff du r�seau pour plus d'informations.",
		/* LANG_SET_SYNTAX */
		"SYNTAX: %sset [option] [parametres]\n"
		"Available options: founder, successor, desc, email, url, entrymsg, bantype, mlock,\n"
		"        keeptopic, topiclock, opnotice, peace, private, restricted, secure, secureops,\n"
		"        securefounder, signkick, xop, noexpire (limited to SA)\n"
		"        dontkickops, dontkickvoices, greet, fantasy, symbiosis, nobot (limited to SA)",
		/* LANG_SET_SYNTAX_EXT */
		"SYNTAX: \002%sset\002 [option] [parameters]\n"
		"Available options:\n"
		"        founder       -  D�signe le nouvel Owner du Chan\n"
		"        successor     -  D�signe le successeur de l'Owner pour un Chan\n"
		"        desc          -  R�gle la description du Chan\n"
		"        url           -  Associe une URL au Chan\n"
		"        email         -  Associe un E-mail au Chan\n"
		"        entrymsg      -  R�gle le message envoy� aux utilisateurs quand ils entrent dans un Chan\n"
		"        bantype       -  Regle le type de ban que les services appliquent\n"
		"        mlock         -  Verrouille les modes de chan choisis\n"
		"        keeptopic     -  Maintient le Topic quand le Chan n'est pas utilis�\n"
		"        topiclock     -  Le Topic peut seulement �tre modifi� avec TOPIC\n"
		"        opnotice      -  Envoie une notice quand la commande OP/DEOP est utilis�e\n"
		"        peace         -  R�gule l'usage des commandes de sanction\n"
		"        private       -  Cache le chan dans la commande LIST\n"
		"        restricted    -  Restreint l'acc�s du Chan\n"
		"        secure        -  Active les options de s�curit� des services\n"
		"        secureops     -  Contr�le strict du statut d'op\n"
		"        securefounder -  Contr�le strict du statut d'owner\n"
		"        signkick      -  Signature des kicks qui ne sont pas faits par la commande KICK\n"
		"        xop           -  Met le syst�me d'acc�s en mode XOP\n"
		"        noexpire      -  R�gle l'expiration du Chan. (limit� au Service Admin)\n"
		" \n"
		"        dontkickops   -  Prot�ge les ops des kicks par les services.\n"
		"        dontkickvoices - Prot�ge les voices des kicks par les services.\n"
		"        greet         -  Autorise les messages greet quand on rejoint un Chan.\n"
		"        fantasy       -  Autorise les commandes fantasy sur le Chan. (Lire l'aide avant !)\n"
		"        symbiosis     -  Autorise le bot � agir comme un vrai bot.\n"
		"        nobot         -  Emp�che l'assignation d'un bot sur le Chan. (limit� au Service Admin)",
		/* LANG_SET_FOUNDER_SYNTAX */
		"SYNTAX: %sset founder [pseudo]",
		/* LANG_SET_FOUNDER_SYNTAX_EXT */
		"SYNTAX: \002%sset founder\002 [pseudo]\n"
		"Change le propri�taire du Chan. Le nouveau propri�taire doit �tre un pseudo enregistr�.",
		/* LANG_SET_SUCCESSOR_SYNTAX */
		"SYNTAX: %sset successor [pseudo]",
		/* LANG_SET_SUCCESSOR_SYNTAX_EXT */
		"SYNTAX: \002%sset successor\002 [pseudo]\n"
		"Change le successeur du propri�taire. Si le pseudo du propri�taire expire ou est dropp�\n"
		"Tant que le Chan est enregistr� le successeur peut devenir le propri�taire du Chan.\n"
		"Le pseudo du Successeur doit �tre enregistr�.",
		/* LANG_SET_DESC_SYNTAX */
		"SYNTAX: %sset desc [description]",
		/* LANG_SET_DESC_SYNTAX_EXT */
		"SYNTAX: \002%sset desc\002 [description]\n"
		"R�gle la description du Chan, elle se voit sur la commande LIST et INFO.",
		/* LANG_SET_URL_SYNTAX */
		"SYNTAX: %sset url [url]",
		/* LANG_SET_URL_SYNTAX_EXT */
		"SYNTAX: \002%sset url\002 [url]\n"
		"Associe L'URL donn�e au Chan.\n"
		"Cette URL sera visible sur la commande INFO du Chan.\n"
		"Si aucune URL n'est precis�e, cela effacera toute URL associ�e au Chan.",
		/* LANG_SET_EMAIL_SYNTAX */
		"SYNTAX: %sset email [email]",
		/* LANG_SET_EMAIL_SYNTAX_EXT */
		"SYNTAX: \002%sset email\002 [email]\n"
		"Associe L'E-mail donn� au Chan.\n"
		"Cet E-mail sera visible sur la commande INFO du Chan.\n"
		"Si aucun E-mail n'est precis�, cela effacera tout E-mail associ� au Chan.",
		/* LANG_SET_ENTRYMSG_SYNTAX */
		"SYNTAX: %sset entrymsg [Message d'entr�]\n",
		/* LANG_SET_ENTRYMSG_SYNTAX_EXT */
		"SYNTAX: \002%sset entrymsg\002 [Message d'entr�]\n"
		"R�gle le message qui sera envoy� par /notice aux utilisateurs qui rejoindront le chan.\n"
		"Si aucun message n'est precis�, il n'y aura pas de notice envoy�e.",
		/* LANG_SET_BANTYPE_SYNTAX */
		"SYNTAX: %sset bantype [0|1|2|3]",
		/* LANG_SET_BANTYPE_SYNTAX_EXT */
		"SYNTAX: \002%sset bantype\002 [0|1|2|3]\n"
		"R�gle le type de ban utilis� par les services quand ils auront besoin de bannir un utilisateur du Chan.\n"
		"\n"
		"Le type de ban sera un nombre entre 0 et 3 qui signifie :\n"
		"0: ban de cette forme *!user@host\n"
		"1: ban de cette forme *!*user@host\n"
		"2: ban de cette forme *!*@host\n"
		"3: ban de cette forme *!*user@*.domain",
		/* LANG_SET_MLOCK_SYNTAX */
		"SYNTAX: %sset mlock [modes]",
		/* LANG_SET_MLOCK_SYNTAX_EXT */
		"SYNTAX: \002%sset mlock\002 [modes]\n"
		"R�gle le mode-lock pour le Chan.\n"
		"Ceci vous permet de d�finir les modes de Chan qui seront maintenus.\n",
		/* LANG_SET_KEEPTOPIC_SYNTAX */
		"SYNTAX: %sset keeptopic [on|off]",
		/* LANG_SET_KEEPTOPIC_SYNTAX_EXT */
		"SYNTAX: \002%sset keeptopic\002 [on|off]\n"
		"Met ou Enl�ve le maintien du Topic d'un Chan. Quand l'option KEEPTOPIC est activ�e,\n"
		"le topic du Chan sera gard� apr�s que le dernier utilisateur quitte le Chan,\n"
		"et sera restaur� � la prochaine cr�ation du Chan.",
		/* LANG_SET_TOPICLOCK_SYNTAX */
		"SYNTAX: %sset topiclock [on|off]",
		/* LANG_SET_TOPICLOCK_SYNTAX_EXT */
		"SYNTAX: \002%sset topiclock\002 [on|off]\n"
		"Met ou enl�ve le verrouillage du topic pour un Chan. Quand l'option TOPICLOCK est activ�e,\n"
		"les services ne permettront pas la modification du Topic sur le Chan.",
		/* LANG_SET_OPNOTICE_SYNTAX */
		"SYNTAX: %sset opnotice [on|off]",
		/* LANG_SET_OPNOTICE_SYNTAX_EXT */
		"SYNTAX: \002%sset opnotice\002 [on|off]\n"
		"Met ou enl�ve la notice d'Op pour un Chan. Quand l'option OPNOTICE est activ�e,\n"
		"les services enverront une notice au Chan pour pr�venir de l utilisation de la commande OP/DEOP.",
		/* LANG_SET_PEACE_SYNTAX */
		"SYNTAX: %sset peace [on|off]",
		/* LANG_SET_PEACE_SYNTAX_EXT */
		"SYNTAX: \002%sset peace\002 [on|off]\n"
		"Met ou enl�ve l'option peace pour le Chan. Quand l'option PEACE est activ�e,\n"
		"un utilisateur ne pourra pas kicker, bannir ou enlever le statut d'un utilisateur qui aura\n"
		"un niveau sup�rieur ou �gal au sien.",
		/* LANG_SET_PRIVATE_SYNTAX */
		"SYNTAX: %sset private [on|off]",
		/* LANG_SET_PRIVATE_SYNTAX_EXT */
		"SYNTAX: \002%sset private\002 [on|off]\n"
		"Met ou enl�ve l'option private pour le Chan. Quand l'option PRIVATE est activ�e,\n"
		" la commande /%s LIST ne montrera le Chan dans aucune liste.",
		/* LANG_SET_RESTRICTED_SYNTAX */
		"SYNTAX: %sset restricted [on|off]",
		/* LANG_SET_RESTRICTED_SYNTAX_EXT */
		"SYNTAX: \002%sset restricted\002 [on|off]\n"
		"Met ou enl�ve l'option de restriction pour un Chan. Quand l'option RESTRICTED est activ�e,\n"
		"les utilisateurs qui n'ont pas d'acc�s Op�rateur\n"
		"(utilisateurs avec acc�s n�gatif et, si \002secure ops\002 est actif, utilisateurs sans acc�s)\n"
		"seront immediatement kick�s et bannis du Chan.",
		/* LANG_SET_SECURE_SYNTAX */
		"SYNTAX: %sset secure [on|off]",
		/* LANG_SET_SECURE_SYNTAX_EXT */
		"SYNTAX: \002%sset secure\002 [on|off]\n"
		"Met ou enl�ve l'option de s�curit� ChanServ pour un Chan. Quand l'option SECURE est activ�e,\n"
		"Seuls les utilisateurs qui ont un pseudo enregistr� et identifi� aux services, \n"
		"auront un acc�s au Chan sous contr�le de la liste d'acc�s.",
		/* LANG_SET_SECUREOPS_SYNTAX */
		"SYNTAX: %sset secureops [on|off]",
		/* LANG_SET_SECUREOPS_SYNTAX_EXT */
		"SYNTAX: \002%sset secureops\002 [on|off]\n"
		"Met ou enl�ve l'option secure ops option pour un Chan. Quand l'option SECUREOPS est activ�e,\n"
		"les utilisateurs qui ne sont pas dans la liste d'acc�s,\n"
		"ne pourront pas avoir le statut d'Op.",
		/* LANG_SET_SECUREFOUNDER_SYNTAX */
		"SYNTAX: %sset securefounder [on|off]",
		/* LANG_SET_SECUREFOUNDER_SYNTAX_EXT */
		"SYNTAX: \002%sset securefounder\002 [on|off]\n"
		"Met ou enl�ve l'option secure founder pour un Chan. Quand l'option SECURE FOUNDER est activ�e,\n"
		"seul le propri�taire et son successeur pourront dropper le Chan, changer son password,\n"
		"pas les propri�taires identifi�s aupr�s des services.",
		/* LANG_SET_SIGNKICK_SYNTAX */
		"SYNTAX: %sset signkick [on|off]",
		/* LANG_SET_SIGNKICK_SYNTAX_EXT */
		"SYNTAX: \002%sset signkick\002 [on|off]\n"
		"Met ou enl�ve la signature des kicks pour un Chan. Quand l'option SIGNKICK est activ�e, les kicks mis\n"
		"par les services afficheront le pseudo et la raison de celui qui utilise la commande de kick.\n"
		"Si vous utilisez les LEVELS, ceux qui auront un level sup�rieur ou �gal au level SIGNKICK\n"
		"n'auront pas de kick sign� sur le Chan. Voir '%shelp levels' pour plus d'information.",
		/* LANG_SET_XOP_SYNTAX */
		"SYNTAX: %sset xop [on|off]",
		/* LANG_SET_XOP_SYNTAX_EXT */
		"SYNTAX: \002%sset xop\002 [on|off]\n"
		"Met ou enl�ve le syst�me xOP pour un Chan. Quand l'option XOP est activ�e, vous devez utiliser les commandes\n"
		"AOP/SOP/VOP pour donner les acc�s aux utilisateurs, autrement utiliser la commande ACCESS.\n",
		/* LANG_SET_NOEXPIRE_SYNTAX */
		"SYNTAX: %sset noexpire [on|off]",
		/* LANG_SET_NOEXPIRE_SYNTAX_EXT */
		"SYNTAX: \002%sset noexpire\002 [on|off]\n"
		"R�gle le param�tre d'expiration du Chan. Quand l'option NOEXPIRE est activ�e le Chan n'expirera pas.\n"
		" \n"
		"Cette commande est r�serv�e aux Administrateurs de Services.",
		/* LANG_TOPIC_SYNTAX */
		"SYNTAX: %stopic [topic]\n"
		"SYNTAX: %sctopic [topic]",
		/* LANG_TOPIC_SYNTAX_EXT */
		"SYNTAX: \002%stopic\002 [topic]\n"
		"SYNTAX: \002%sctopic\002\n"
		"Sets the channel topic. If no topic is given, the current topic is displayed in the channel.\n"
		"The CTOPIC command is provided to clear the topic.\n"
		"Cette commande s'utilise avec \002SET TOPICLOCK\002",
		/* LANG_INVITE_SYNTAX */
		"SYNTAX: %sinvite [pseudo]",
		/* LANG_INVITE_SYNTAX_EXT */
		"SYNTAX: \002%sinvite\002 [pseudo]\n"
		"Invite 'Pseudo' sur le Chan si celui ci est pr�sent sur le r�seau",
		/* LANG_INVITE_NO_USER */
		"Cet utilisateur n'est pas connect�.",
		/* LANG_INVITE_IS_ON */
		"Cet utilisateur est d�j� sur le Chan !",
		/* LANG_INVITE_YOURSELF */
		"Vous ne pouvez pas vous inviter vous m�me !",
		/* LANG_CMD_LIST_HEADER */
		"Commandes Fantasy disponiblent :",
		/* LANG_CMD_LIST_FOOTER */
		"Toutes les fonctions de services ne sont pas d�taill�es ici...",
		/* LANG_CMD_LIST_GENERAL */
		"Commandes g�n�ral: %s",
		/* LANG_CMD_LIST_CHAN_AXX */
		"Commandes de l'acc�s au Chan: %s",
		/* LANG_CMD_LIST_CHG_SET */
		"Commandes de r�glage de Chan: %s",
		/* LANG_CMD_LIST_ACTIONS */
		"Commandes d'actions sur le Chan: %s",
		/* LANG_CMD_LIST_IRCOP */
		"Commandes IRC Operateur: %s",
		/* LANG_UNKNOWN_CMD */
		"Commandes inconnues: \002%s\002",
		/* LANG_HELP_HEADER */
		"----- Aide pour les commandes Fantasy -----",
		/* LANG_HELP_FOOTER */
		"----- Fin de la liste des commandes -----",
		/* LANG_APPENDTOPIC_SYNTAX */
		"SYNTAX: %sappendtopic ou %stappend [texte � ajouter au topic]",
		/* LANG_APPEND_SYNTAX_EXT */
		"SYNTAX: \002%sappendtopic\002 ou \002%stappend\002 [texte � ajouter au topic]\n"
		"Ajoute le texte donn� au topic actuel du Chan.\n"
		"Cette commande s'utilise avec \002SET TOPICLOCK\002",
		/* LANG_STAFF */
		"SYNTAX: \002%sstaff\002 ou \002%sircops\002\n"
		"Cette commande donne la liste des IRC Operateurs disponibles.",
		/* LANG_CMDLIST */
		"SYNTAX: \002%scmdlist\002\n"
		"Montre la liste des commandes Fantasy",
		/* LANG_MINFO */
		"SYNTAX: \002%sminfo\002 (ou %smodinfo)\n"
		"Montre les informations du module des commandes Fantasy.",
		/* LANG_HELP_UNKNOWN_CMD */
		"Pas d'aide disponible pour cette commande.",
		/* LANG_BAN_SYNTAX */
		"SYNTAX: %sban [pseudo|mask]",
		/* LANG_BAN_SYNTAX_EXT */
		"SYNTAX: \002%sban\002 [pseudo|mask]\n"
		"Banni le pseudo ou le mask donn� du Chan.",
		/* LANG_MUTE_SYNTAX */
		"SYNTAX: %smute [pseudo|mask] (d�pend de l'IRCD)",
		/* LANG_MUTE_SYNTAX_EXT */
		"SYNTAX: \002%smute\002 [pseudo|mask]\n"
		"Banni ou rend muet le pseudo ou le mask donn� pour le Chan. (d�pend de L'IRCD)",
		/* LANG_UNMUTE_SYNTAX */
		"SYNTAX: %sunmute [pseudo|mask] (d�pend de l'IRCD)",
		/* LANG_UNMUTE_SYNTAX_EXT */
		"SYNTAX: \002%sunmute\002 [pseudo|mask]\n"
		"Debanni ou enl�ve Le mode muet sur le pseudo ou le mask donn� pour le Chan. (d�pend de l'IRCD)",
		/* LANG_SVERSION */
		"SYNTAX: \002%ssversion\002\n"
		"Montre les informations sur la version et le build des Services Anope.",
		/* LANG_KILL_SYNTAX */
		"SYNTAX: %skill [pseudo] [raison]",
		/* LANG_KILL_SYNTAX_EXT */
		"SYNTAX: \002%skill\002 [pseudo] [raison]\n"
		"Cette commande force l'utilisateur � se deconnecter du r�seau (identique � /kill).\n"
		"IRC Op�rateurs seulement. Cette commande peut �tre d�sactiv�e",
		/* LANG_MODE_SYNTAX */
		"SYNTAX: %smode [mode(s)]",
		/* LANG_MODE_SYNTAX_EXT */
		"SYNTAX: \002%smode\002 [mode(s)]\n"
		"Cette commande modifie les modes de Chan.\n"
		"IRC Op�rateurs seulement. Cette commande peut �tre d�sactiv�e",
		/* LANG_AKILL_SYNTAX */
		"SYNTAX: %sakill AKILL {add|del|list|view|clear} [[+expiry] {mask|entry-list} [raison]]",
		/* LANG_AKILL_SYNTAX_EXT */
		"SYNTAX: \002%sakill\002 add [+expiry] {mask} raison\n"
		"SYNTAX: \002%sakill\002 del {mask|num de list|list}\n"
		"SYNTAX: \002%sakill\002 [list|view] [mask|list]\n"
		"SYNTAX: \002%sakill\002 clear\n"
		"IRC Op�rateurs seulement. Cette commande peut �tre d�sactiv�e",
		/* LANG_IGNORE_SYNTAX */
		"Syntax: %signore {add|del|list|clear} [time] [pseudo]",
		/* LANG_IGNORE_SYNTAX_EXT */
		"SYNTAX: \002%signore\002 {add|del} [expiry] [pseudo]\n"
		"SYNTAX: \002%signore\002 {list|clear}\n"
		"Autorise les Services Admins � rendre un pseudo ignor� par les services pour\n"
		"un certain temps ou jusqu au red�marrage. Le temps par d�faut\n"
		"� un format en secondes. Les combinaisons d'unit� de temps ne sont pas permises.\n"
		"Pour que les services ignorent ind�finiment, utiliser 0 pour le temps.",
		/* LANG_INFO_SYNTAX */
		"Syntax: %sinfo [all]",
		/* LANG_INFO_SYNTAX_EXT */
		"SYNTAX: \002%sinfo\002 [all]\n"
		"Donne la liste des informations pour le Chan.\n"
		"L'option 'all' est limit�e au propri�taire et aux Services Admins.",
		/* LANG_CMD_DISABLED */
		"Cette commande a �t� d�sactiv�e, Merci de contacter Le staff du r�seau pour plus d'informations.",
		/* LANG_HELP_CMDS_GEN */
		" Commandes G�n�rales :\n"
		"   %shelp      -   Aide de botserv pour les commandes\n"
		"   %scmdlist   -   Montre la liste de toutes les commandes FANTASY\n"
		"   %sircops    -   Montre la liste de tous les IRCops disponibles\n"
		"   %sstaff     -   Montre la liste de tous les IRCops disponibles\n"
		"   %sminfo     -   Montre les informations � propos du module\n"
		"   %ssversion  -   Montre les informations sur Anope en g�n�ral\n"
		"   %sinfo      -   Montre les informations � propos du Chan\n"
		" \n"
		"Note: Ces commandes peuvent �tre indisponibles, d�sactiv�es ou � acc�s restreint\n",
		/* LANG_HELP_CMDS_ACC */
		"Commandes de Contr�le d'Acc�s:\n\n"
		"   %ssop       -   Permet de g�rer la liste des SOP (SuperOP)\n"
		"   %saop       -   Permet de g�rer la liste des AOP (AutoOP)\n"
		"   %shop       -   Permet de g�rer la liste des HOP (HalfOP)(peut ne pas �tre g�r� par le r�seau)\n"
		"   %svop       -   Permet de g�rer la liste des VOP (VOicePeople)\n"
		"   %saccess    -   Permet de g�rer la liste des AXX (quand XOP est d�sactiv�)\n"
		"   %slevels    -   Permet de g�rer les levels du Chan\n"
		" \n"
		"Note: Ces commandes peuvent �tre indisponibles, d�sactiv�es ou � acc�s restreint\n",
		/* LANG_HELP_CMDS_SETTINGS */
		"Commands de R�glage du chan: \n"
		"   %sakick     -   G�re la liste de akick \n"
		"   %sbadwords  -   G�re la liste des mots interdits \n"
		"   %sset       -   Change les r�glages du Chan. (cf. '%shelp set')\n"
		"   %sbkick     -   Change le r�glage des kicks Botserv.(cf. '%shelp bkick')\n"
		" \n"
		"Note: Ces commandes peuvent �tre indisponibles, d�sactiv�es ou � acc�s restreint\n",
		/* LANG_HELP_CMDS_ACTIONS */
		"Commandes d'Action du Chan:\n"
		"   %sclear     -   Efface l'option sp�cifi�e sur le chan.\n"
		"   %stopic     -   R�gle le topic du chan\n"
		"   %stappend   -   Ajoute le texte au topic\n"
		"   %sinvite    -   Invite un utilisateur sur le chan\n"
		"   %sup        -   Attribue les grades auxquels l'utilisateur a acc�s\n"
		"   %sdown      -   Enl�ve tous les grades d'un utilisateur\n"
		"   %skick      -   Kick un utilisateur sur le Chan\n"
		"   %skb        -   Kick et ban un ou des utilisateur(s) sur le Chan\n"
		"   %stkb       -   Kick et temporaire ban un ou des utilisateur(s) sur le Chan\n"
		" \n"
		"Note: Ces commandes peuvent �tre indisponibles, d�sactiv�es ou � acc�s restreint\n",
		/* LANG_HELP_CMDS_PUTMODES */
		"Commands de Mode du Chan:\n"
		"   %sban       -   Bannit l'utilisateur sur le Chan sans le kicker.\n"
		"   %stban      -   Bannit l'utilisateur temporaire sur le Chan sans le kicker.\n"
		"   %sunban     -   D�bannit l'utilisateur sur le Chan\n"
		"   %smute      -   Bannit l'utilisateur ou le rend muet (d�pend de L'IRCD)\n"
		"   %sunmute    -   D�bannit l'utilisateur ou l�ve le mode muet (d�pend de L'IRCD)\n"
		"   %sop        -   +o: Op vous-m�me ou un autre utilisateur\n"
		"   %sdeop      -   -o: Deop vous-m�me ou un autre utilisateur\n"
		"   %svoice     -   +v: Voice vous-m�me ou un autre utilisateur\n"
		"   %sdevoice   -   -v: Devoice vous-m�me ou un autre utilisateur\n"
		"   %sprotect   -   +a: Protect vous-m�me ou un autre utilisateur (Peut ne pas �tre disponible)\n"
		"   %sdeprotect -   -a: Deprotect vous-m�me ou un autre utilisateur ((Peut ne pas �tre disponible)\n"
		"   %sowner     -   +q: Attribue le grade owner (Peut ne pas �tre disponible)\n"
		"   %sdeowner   -   -q: Enl�ve votre grade d'owner (Peut ne pas �tre disponible)\n"
		"   %shalfop    -   +h: Halfop vous-m�me ou un autre utilisateur (Peut ne pas �tre disponible)\n"
		"   %sdehalfop  -   -h: Dehalfop vous-m�me ou un autre utilisateur (Peut ne pas �tre disponible)\n"
		" \n"
		"Note: Ces commandes peuvent �tre indisponibles, d�sactiv�es ou � acc�s restreint\n",
		/* LANG_HELP_CMDS_IRCOP */
		"Commandes IRC Operateur :\n"
		"   %sakill     -   Force un ban sur le mask pour tout le serveur.\n"
		"   %skill      -   Deconnecte l'utilisateur\n"
		"   %smode      -   R�gle les modes de Chan\n"
		"   %signore    -   R�gle les services pour ignorer un utilisateur\n"
		" \n"
		"Note: Ces commandes peuvent �tre indisponibles, d�sactiv�es ou � acc�s restreint\n",
		/* LANG_HELP_MORE_INFO */
		"Pour avoir de l'aide sur une commande %shelp <commande>",
		/* LANG_HELP_LIST */
		"Pour avoir une liste des commandes possibles pour ce que vous voulez faire, tapez: \n"
		" \n"
		"  %shelp general  - Montre les commandes g�n�rales disponibles. Aucun acc�s requis.\n"
		"  %shelp chaccess - Montre les commandes pour modifier la liste d'acc�s ou des levels d'un chan.\n"
		"  %shelp config   - Configure les r�glages d'un chan.\n"
		"  %shelp act      - Liste des commandes d'action possibles par les services.\n"
		"  %shelp ircop    - Commandes vablables seulement pour les IRC Operateurs.\n"
		" \n"
		"Pour avoir la liste de \002toutes\002 les commandes disponibles, tape '%shelp all'",
		/* LANG_SET_DONTKICKOPS_SYNTAX */
		"SYNTAX: %sset dontkickops {on | off}",
		/* LANG_SET_DONTKICKOPS_SYNTAX_EXT */
		"SYNTAX: \002%sset dontkickops\002 {on | off}\n"
		"Active ou d�sactive la protection des ops sur un chan. Quand l'option DONTKICKOPS est activ�e,\n"
		"cette option emp�che le bot de kicker les ops m�me s'ils ne sont pas concern�s\n"
		"par le level NOKICK.",
		/* LANG_SET_DONTKICKVOICES_SYNTAX */
		"SYNTAX: %sset dontkickvoices {on | off}",
		/* LANG_SET_DONTKICKVOICES_SYNTAX_EXT */
		"SYNTAX: \002%sset dontkickvoices\002 {on | off}\n"
		"Active ou d�sactive la protection des voices sur un chan. Quand l'option DONTKICKVOICES est activ�e,\n"
		"cette option emp�che le bot de kicker les voices m�me s'ils ne sont pas concern�s \n"
		"par le level NOKICK.",
		/* LANG_SET_GREET_SYNTAX */
		"SYNTAX: %sset greet [on|off]",
		/* LANG_SET_GREET_SYNTAX_EXT */
		"SYNTAX: \002%sset greet\002 {on | off}\n"
		"Active ou d�sactive le  Message d'Accueil sur un chan. Quand l'option GREET est activ�e,\n"
		"elle fait afficher au bot le message d'accueil de chaque utilisateur qui a un acc�s suffisant au chan.",
		/* LANG_SET_FANTASY_SYNTAX */
		"SYNTAX: %sset fantasy [on|off]",
		/* LANG_SET_FANTASY_SYNTAX_EXT */
		"SYNTAX: \002%sset fantasy\002 {on | off}\n"
		"Active ou d�sactive le mode FANTASY sur un chan. Quand l'option FANTASY est activ�e, cette option\n"
		"permet aux utilisateurs d'utiliser les commandes !op, !deop, !voice, !devoice, !kick, !kb, !unban.\n"
		"Noter que les utilisateurs qui veulent utliliser les commandes FANTASY doivent avoir \n"
		"suffisamment d'acc�s pour les levels FANTASIA et le level qui d�pend de la commande\n"
		" \n"
		"IMPORTANT: D�sactiver cette option d�sactivera les commandes FANTASY",
		/* LANG_SET_SYMBIOSIS_SYNTAX */
		"SYNTAX: %sset symbiosis [on|off]",
		/* LANG_SET_SYMBIOSIS_SYNTAX_EXT */
		"SYNTAX: \002%sset symbiosis\002 {on | off}\n"
		"Active ou d�sactive la symbiose sur un chan. Quand l'option SYMBIOSIS est activ�e,\n"
		"cette option fait faire au bot tout ce qui est normalement effectu� par BotServ sur les chans,\n"
		"tels que les MODEs,les KICKs, et m�me le message d'entr�e. \n"
		"Note: Quand l'option est d�sactiv�e, la plupart des commandes fantasy seront possibles avec le bot.",
		/* LANG_SET_NOBOT_SYNTAX */
		"SYNTAX: %sset nobot [on|off]",
		/* LANG_SET_NOBOT_SYNTAX_EXT */
		"SYNTAX: \002%sset nobot\002 {on | off}\n"
		"Cette option fait qu'un bot ne peut pas �tre assign� sur le Chan. Si un bot est \n"
		"quand m�me assign�, il sera d�sassign� automatiquement si cette option est activ�e.",
		/* LANG_BKICK_SYNTAX */
		"SYNTAX: %sbkick [option] {on | off} [ttb [params]]\n"
		"Options disponibles: bolds, badwords, caps, colors, flood, repeat, reverses, underlines.",
		/* LANG_BKICK_SYNTAX_EXT */
		"SYNTAX: \002%sbkick\002 [option] {on | off} [ttb [params]]\n"
		"Available options:\n"
		"        bolds       -  R�gle le bot pour kicker les Gras.\n"
		"        badwords    -  R�gle le bot pour kicker les Mots Interdits.\n"
		"        caps        -  R�gle le bot pour kicker les Majuscules.\n"
		"        colors      -  R�gle le bot pour kicker les Couleurs.\n"
		"        flood       -  R�gle le bot pour kicker le flood.\n"
		"        repeat      -  R�gle le bot pour kicker les R�p�titions.\n"
		"        reverses    -  R�gle le bot pour kicker les invers�s.\n"
		"        underlines  -  R�gle le bot pour kicker les Soulign�s.\n"
		" \n"
		"ttb est le nombre de fois qu'un utilisateur peut �tre kick� \n"
		"avant d'�tre banni. Laissez ttb vierge pour d�sactiver",
		/* LANG_BKICK_BOLDS_SYNTAX */
		"SYNTAX: %sbkick bolds {on | off} [ttb]",
		/* LANG_BKICK_BOLDS_SYNTAX_EXT */
		"SYNTAX: \002%sbkick bolds\002 {on | off} [ttb]\n"
		"Quand l'option est selectionn�e, le bot pourra kicker les personnes \n"
		"qui �crivent en gras. les services enveront une notice quand la commande OP DEOP sera utilis�e.\n"
		"ttb est le nombre de fois qu'un utilisateur peut �tre kicker avant d'�tre banni.\n"
		"Laissez ttb vierge pour d�sactiver le syst�me de ban automatique.",
		/* LANG_BKICK_BADWORDS_SYNTAX */
		"SYNTAX: %sbkick badwords {on | off} [ttb]",
		/* LANG_BKICK_BADWORDS_SYNTAX_EXT */
		"SYNTAX: \002%sbkick badwords\002 {on | off} [ttb]\n"
		"Quand l'option est selectionn�e, le bot pourra kicker les personnes qui utilisent \n"
		"des mots interdits. Vous pouvez definir la liste de mots interdits en tapant %sbadwords command.\n"
		"ttb est le nombre de fois qu'un utilisateur peut �tre kick� avant d'�tre banni.\n"
		"Laissez ttb vierge pour d�sactiver le syst�me de ban automatique.",
		/* LANG_BKICK_CAPS_SYNTAX */
		"SYNTAX: %sbkick caps {on | off} [ttb [min [percent]]]",
		/* LANG_BKICK_CAPS_SYNTAX_EXT */
		"SYNTAX: \002%sbkick caps\002 {on | off} [ttb [min [percent]]]\n"
		"Quand l'option est selectionn�e, le bot pourra kicker les personnes qui \n"
		"�crivent en majuscule. Le bot kicke si il y a minimum \002min\002 majuscule caps \n"
		"et qu'elles constituent au moins \002percent%\002 de la ligne de texte enti�re \n"
        "(si ces param�tres ne sont pas donn�s, c'est par d�faut 10 caract�res et 25%).\n"
		"ttb est le nombre de fois qu'un utilisateur peut �tre kick� avant d'�tre banni.\n"
		"Laissez ttb vierge pour d�sactiver le syst�me de ban automatique.",
		/* LANG_BKICK_COLORS_SYNTAX */
		"SYNTAX: %sbkick colors {on | off} [ttb]",
		/* LANG_BKICK_COLORS_SYNTAX_EXT */
		"SYNTAX: \002%sbkick colors\002 {on | off} [ttb]\n"
		"Quand l'option est selectionn�e, le bot pourra kicker les personnes qui ecrivent en couleur.\n"
		"ttb est le nombre de fois qu'un utilisateur peut �tre kick� avant d'�tre banni.\n"
		"Laissez ttb vierge pour d�sactiver le syst�me de ban automatique.",
		/* LANG_BKICK_FLOOD_SYNTAX */
		"SYNTAX: %sbkick flood {on | off} [ttb [ln [secs]]]",
		/* LANG_BKICK_FLOOD_SYNTAX_EXT */
		"SYNTAX: \002%sbkick flood\002 {on | off} [ttb [ln [secs]]]\n"
		"Quand l'option est selectionn�e, le bot pourra kicker les personnes \n"
		"qui flood sur le chan en ecrivant \002ln\002 lignes en \002secs\002 secondes. \n"
		"(si vous ne param�trez pas, c'est 6 lignes en 10 secondes par d�faut).\n"
		"ttb est le nombre de fois qu'un utilisateur peut �tre kick� avant d'�tre banni.\n"
		"Laissez ttb vierge pour d�sactiver le syst�me de ban automatique.",
		/* LANG_BKICK_REPEAT_SYNTAX */
		"SYNTAX: %sbkick repeat {on | off} [ttb [num]]",
		/* LANG_BKICK_REPEAT_SYNTAX_EXT */
		"SYNTAX: \002%sbkick repeat\002 {on | off} [ttb [num]]\n"
		"Quand l'option est selectionn�e, le bot pourra kicker les personnes \n"
		"qui se r�p�tent \002num\002 fois (si num n'est pas design�, c'est 3 par d�faut)."
		"ttb est le nombre de fois qu'un utilisateur peut �tre kick� avant d'�tre banni.\n"
		" Laissez ttb vierge pour d�sactiver le syst�me de ban.",
		/* LANG_BKICK_REVERSES_SYNTAX */
		"SYNTAX: %sbkick reverses {on | off} [ttb]",
		/* LANG_BKICK_REVERSES_SYNTAX_EXT */
		"SYNTAX: \002%sbkick reverses\002 {on | off} [ttb]\n"
		"Quand l'option est selectionn�e, le bot pourra kicker les personnes qui ecrivent en invers�.\n"
		"ttb est le nombre de fois qu'un utilisateur peut �tre kick� avant d'�tre banni.\n"
		"Laissez ttb vierge pour d�sactiver le syst�me de ban.",
		/* LANG_BKICK_UNDERLINES_SYNTAX */
		"SYNTAX: %sbkick underlines {on | off} [ttb]",
		/* LANG_BKICK_UNDERLINES_SYNTAX_EXT */
		"SYNTAX: \002%sbkick underlines\002 {on | off} [ttb]\n"
		"Quand l'option est selectionn�e, le bot pourra kicker les personnes qui ecrivent en soulign�.  \n"
		"ttb est le nombre de fois qu'un utilisateur peut �tre kick� avant d'�tre banni.\n"
		"Laissez ttb vierge pour d�sactiver le syst�me de ban.",
		/* LANG_BKICK_UNKNOWN_OPTION */
		"Option BKICK inconnue: %s.",
		/* LANG_ADLIST_SA */
		"Services Administrateur",
		/* LANG_ADLIST_SRA */
		"Services Root Administrateur",
		/* LANG_ADLIST_NONE */
		"Aucun administrateur disponible actuellement",
		/* LANG_ADLIST_HEADER */
		"-----------------------------------------------------------------------\n"
		"Les Administrateurs suivant sont en ligne: \n"
		"-----------------------------------------------------------------------",
		/* LANG_ADLIST_FOOTER */
		"-----------------------------------------------------------------------\n"
		"Total d'Admins disponibles : %d \n"
		"-----------------------------------------------------------------------\n"
		" -> Toujours essayer de contacter un Admin sur un chan avant de le PV \n"
		" -> Eviter de PV un Admin sans avoir sa permission avant. \n"
		" -> faire un /whois sur Admin avant de vouloir lui parler. \n"
		" -> Lire (et respecter) un message d'away present dans le Whois. \n"
		"-----------------------------------------------------------------------",
		/* LANG_KICKBAN_SYNTAX */
		"SYNTAX: %skickban or %skb [pseudo | [pseudo!]ident@host] [raison]",
		/* LANG_KICKBAN_SYNTAX_EXT */
		"SYNTAX: \002%skickban\002 or \002%skb\002 [pseudo | [pseudo!]ident@host] [raison]\n"
		"Ceci kickera et bannira la personne d�sign�e ou toutes les personnes ayant \n"
		"le m�me mask sur le Chan. Si un mask est donn� et que quelqu'un ne devrait   \n"
		"pas �tre kick�' � cause des r�glages du chan, la commande est anul�e. \n"
		"Personne n'est kick� ou banni.",
		/* LANG_UP_SYNTAX */
		"SYNTAX: %sup [pseudo]",
		/* LANG_UP_SYNTAX_EXT */
		"SYNTAX: \002%sup\002 [pseudo]\n"
		"Donne � l'utilisateur les modes auquel il a droit. Ceci ne donnera pas de grade si\n"
		"l'utilisateur n'a pas d'acc�s.\n"
		" \n"
		"Note: Un pseudo peut seulement etre specifi� par le owner du Chan ou un SRA.",
		/* LANG_DOWN_SYNTAX */
		"SYNTAX: %sdown [pseudo]",
		/* LANG_DOWN_SYNTAX_EXT */
		"SYNTAX: \002%sdown\002 [pseudo]\n"
		"Enleve tout les modes au pseudo. Si le Chan est mod�r�, cela enlevera les voices,\n"
		"jusqu'a ce qu'un utilisateur soit de nouveaux design� par l'owner.\n"
		" \n"
		"Note: Un pseudo peut seulement etre specifi� par le owner du Chan ou un SRA.",
		/* LANG_UPDOWN_SYNTAX */
		"SYNTAX: %sup|%sdown [pseudo]",
		/* LANG_TBAN_SYNTAX	*/
		"SYNTAX: %stb or %stban [pseudo | [pseudo!]ident@host] [time]",
		/* LANG_TBAN_SYNTAX_EXT */
		"SYNTAX: \002%stb\002 or \002%stban\002 [pseudo | [pseudo!]ident@host] [time]\n"
		"Ceci bannira le pseudo ou le mask specifi� dans le Chan pour\n"
		"la dur�e pr�cis�e. si le temp n'est pas donn�, il sera d'une heure par defaut.\n"
		"le temp devra etre donn� dans le format suivant <nombre><(s)/m/h/d>.\n"
		"Exemple: \002%stb Viper 10m\002 ceci me bannira pour 10 minutes.\n"
		" \n"
		"Note: Si les services sont redemarr�s avant l'expiration du ban,\n"
		"il ne sera pas automatiquement enlev�",
		/* LANG_TBAN_RESPONSE */
		"Le ban temporaire %s de %s, expirera dans %s.",
		/* LANG_TKBAN_SYNTAX */
		"SYNTAX: %stkb [pseudo | [pseudo!]ident@host] [temp] [raison]",
		/* LANG_TKBAN_SYNTAX_EXT */
		"SYNTAX: \002%stkb\002 or \002%stkban\002 [pseudo | [pseudo!]ident@host] [temp] [raison]\n"
		"Ceci Kickera et bannira l'utilisateur sp�cifi� ou tout ceux qui correspondront\n"
		"au mask donn� dans le Chan pour le temps donn�. \n"
		"Si un mask est donn� et qu'un utilisateur qui correspond aux\n"
		"reglages de Chan ne doit pas etre banni, La commande sera annul� avant que le premier kick sois fait.\n"
		"Si le temps n'est pas precis� ou donn� dans un mauvais format, il sera d'une heure par defaut.\n"
		"le temp devra etre donn� dans le format suivant <nombre><(s)/m/h/d>.\n"
		"Exemple: \002%stb Viper 10m\002 ceci me kickera et bannira pour 10 minutes.\n"
		" \n"
		"Note: Si les services sont redemarr�s avant l'expiration du ban\n"
		"il ne sera pas automatiquement enlev�",
		/* LANG_INVALID_TIME */
		"%s et un format de temp invalide.",
		/* LANG_REQ_NICK_OR_MASK */
		"Cette commande requierre un pseudo ou un mask user@host valide.",
		/* LANG_SUSPEND_SYNTAX */
		"SYNTAX: %ssuspend [reason]",
		/* LANG_SUSPEND_SYNTAX_EXT */
		"SYNTAX: \002%ssuspend\002 [reason]\n"
		"Disallows anyone from using the channel but preserves the channel\n"
		"data and settings untill the channel in unsuspended.",
		/* LANG_SUSPEND_NO_REASON */
		"A reason is mandatory to SUSPEND a channel on this network.",
		/* LANG_KB_KICK_DENIED */
		"Permission to kick %s is denied.",
		/* LANG_CLEAR_SYNTAX_EXT */
		"SYNTAX: \002%sclear\002 [param]\n"
		" \n"
		"Following parameters are accepted:\n"
		"MODES   - Resets all modes on the channel (i.e. clears modes i,k,l,m,n,p,s,t).\n"
		"BANS    - Clears all bans on the channel.\n"
		"EXCEPTS - Clears all excepts on the channel.\n"
		"INVITES - Clears all invites on the channel.\n"
		"OPS     - Removes channel-operator status (mode +o) from all channel operators.\n"
		"HOPS    - Removes channel-halfoperator status (mode +h) from all channel halfoperators.\n"
		"VOICES  - Removes \"voice\" status (mode +v) from anyone with that mode set.\n"
		"USERS   - Removes (kicks) all users from the channel.\n"
		" \n"
		"Note: Not all parameters may be supported, depending on the IRCd.",
		/* LANG_ACCESS_LEVELS */
		"\002User access levels\002:\n"
		" \n"
		"By default, the following access levels are defined:\n"
		" *** \037Founder\037 *** \n"
		" Full access to administrative functions and opping upon entering the channel.\n"
		" Note that only one person may have founder status (it cannot be given using the\n"
		" ACCESS command {This is not true if cs_fop is loaded!}).\n"
		" *** \037     10 \037 *** \n"
		" Access to AKICK command and automatic opping.\n"
		" *** \037      5 \037 *** \n"
		" Automatic opping upon entering the channel.\n"
		" *** \037      4 \037 ***\n"
		" Automatic halfopping (+h) upon entering the channel. (If supported by IRCd.)\n"
		" *** \037      3 \037 ***\n"
		" Automatic voicing (+v) upon entering the channel.\n"
		" *** \037      0 \037 *** \n"
		" No special privileges; Can be opped by other ops (unless \002secure-ops\002 is set).\n"
		" *** \037     <0 \037 ***\n"
		" May not be opped.\n"
		" \n"
		"These levels may be changed, or new ones added, using the\n"
		"\002LEVELS\002 command. For more information, cf. HELP LEVELS.",
		/* LANG_ADMINS */
		"SYNTAX: \002%sadmins\002\n"
		"This will return list of all available Services and Network Administrators..",
		/* LANG_SYNC_SYNTAX */
		" SYNTAX: %ssync",
		/* LANG_SYNC_SYNTAX_EXT */
		" SYNTAX: \002%ssync\002\n"
		" \n"
		" This command will give all users currently in the channel the level\n"
		" they are granted by the channels' access list. Users who have a level\n"
		" greater then the one they are supposed to have will be demoted.\n"
		" \n"
		" The use ofthis command is restricted to the Channel Founder.",
		/* LANG_SYNC_DONE */
		" Synchronized userlist with accesslist.",
		/* LANG_SHUN_SYNTAX */
		" SYNTAX: %sshun [add | del] [nick | user@host] [+expiry] [reason]",
		/* LANG_SHUN_SYNTAX_EXT */
		" SYNTAX: \002%sshun\002 [add] [nick | user@host] [+expiry] [reason]\n"
		"         \002%sshun\002 del [nick | user@host]\n"
		" \n"
		" This command adds or deletes a shun on the given nick or username@usermask\n"
		" If 'ADD' or 'DEL' is not specified, ADD is assumed.\n"
		" The time given is considered to be in days, unless another interval is specified.\n"
		" Possible alternatives: secondes (s), minutes, (m), hours, (h) and days (d).\n"
		" Note that these may not be combined! Only the first part will be read.\n"
		" If no reason is specified, a default reason will be used.\n"
		" If the reason starts with a '+', an expiry \037must\037 be given!\n"
		" \n"
		" Note that SHUN DEL is only guaranteed to work for shuns added by the SHUN command\n"
		" and only if the user is still online. In any other case, use name@mask.\n"
		" When shunning the target 'add' or 'del', 'ADD' must be given.",
		/* LANG_TSHUN_SYNTAX */
		" SYNTAX: %stshun [add | del] [nick] [reason]",
		/* LANG_TSHUN_SYNTAX_EXT */
		" SYNTAX: \002%stshun\002 [add] [nick] [reason]\n"
		"         \002%stshun\002 del [nick]\n"
		" \n"
		" This command adds or deletes a temporary shun on the given user.\n"
		" If add or del is not specified, add is assumed.\n"
		" This command cannot be used on IRC operators.\n"
		" Note that this shun will be automatically removed when the user disconnects\n"
		" and that it will only affect one session.",
		/* LANG_SHUN_ADDED */
		" SHUN added for %s@%s.",
		/* LANG_SHUN_DELETION */
		" SHUN on %s@%s has been deleted, if it existed.",
		/* LANG_TSHUN_ADDED */
		" Temporary SHUN placed on user %s.",
		/* LANG_TSHUN_DELETION */
		" Removed temporary SHUN on user %s.",
		/* LANG_TSHUN_TARGET_NEXIST */
		" User %s does not currently exist.",
		/* LANG_TSHUN_TARGET_OPER */
		" Command cannot be used on IRC operators.",
	};

	moduleInsertLanguage(LANG_FR, LANG_NUM_STRINGS, langtable_fr);
}

/* EOF */

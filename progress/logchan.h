/** HEADER FILE FOR LOGCHAN.C - Liber **/

/*
***********************************************************************************************
***********************************************************************************************
***********************************************************************************************
*/

/* JOINS PARTS KICKS RELOAD RESTART SHUTDOWN AWAY RELOAD*/
int log_joins(int argc, char **argv); // EVENT_JOIN
int log_parts(int argc, char **argv); // EVENT_PART
int log_kicks(int argc, char **argv); // EVENT_KICK
int log_topic(int argc, char **argv); // EVENT_TOPIC_UPDATED
int log_restart(int argc, char **argv); // EVENT_RESTART
int log_shutdown(int argc, char **argv); // EVENT_SHUTDOWN
int log_away(char *source, int argc, char **argv); // EVENT_AWAY
int handle_reload(int argc, char **argv); // EVENT_RELOAD

/* BOTSERV */
int log_botadd(int argc, char **argv); // EVENT_BOT_CREATE
int log_botdel(int argc, char **argv); // EVENT_BOT_DEL
int log_botAssign(int argc, char **argv); // EVENT_BOT_ASSIGN
int log_botUnassign(int argc, char **argv); // EVENT_BOT_UNASSIGN
int log_botBan(int argc, char **argv); // EVENT_BOT_BAN

/* UMODE Logging */
int logchan_umode(char *source, int ac, char **av);
void my_set_umode(User * user, int ac, char **av);

/* Miscellaneous */
void LogChanInit(void); // Enable LogChannel
void LoadConfig(void); // Load our configuration directive(s)

int AddBot(void); // Add the services client
int DelBot(const char *reason, ...); // Remove the Services Client
int BindEvents(void); // Bind to all events

/*
***********************************************************************************************
***********************************************************************************************
***********************************************************************************************
*/


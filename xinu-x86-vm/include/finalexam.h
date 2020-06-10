/* finalexam.h -  */

#define STARVE_DEFAULT 0
#define STARVE_Q1_CONTEXT_SWITCH 1
#define STARVE_Q2_2_SECONDS 2

#define SLEEP_TIME_FIGHT 2000
#define SLEEP_TIME_PRINT 500
#define SETUP_SLEEP_TIME 300

extern uint8 g_starvation_setting;
extern uint8 g_starvation_seconds;
extern uint8 g_starvation_increment;

/* finalexam.c - footsoldier, turtle, continuous_fight, print_process */

#include <xinu.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SLEEP_TIME_FIGHT 2000
#define SLEEP_TIME_PRINT 500
#define SETUP_SLEEP_TIME 300

int16 g_line_current = 0;
int16 g_line_next = 0;

// global setting for starvation.
uint8 g_starvation_setting = 0;
uint8 g_starvation_seconds = 10;
uint8 g_starvation_increment = 1;

int8 g_total_clan_membership = 0;
int8 g_total_clan_sleeping = 0;

/*------------------------------------------------------------------------
 *  footsoldier  -  a robot ninja. (resource hog)
 *------------------------------------------------------------------------
 */
syscall footsoldier(char *my_name)
{
	// increment membership.
	++g_total_clan_membership;

	sleepms(SETUP_SLEEP_TIME);

	continuous_fight(my_name);

	return OK;
}

/*------------------------------------------------------------------------
 *  turtle  -  A Teenage Mutent Ninja Turtle (starvation process)
 *------------------------------------------------------------------------
 */
syscall turtle(char *my_name)
{
	sleepms(SETUP_SLEEP_TIME);
	intmask mask; /* saved interrupt mask		*/

	mask = disable();
	kprintf("\n");
	kprintf("	              ,;;;!!;;\n");
	kprintf("        ,;<!!!!!!!!!!!;\n");
	kprintf("     `'<!!!!!!!!!!(``'!!\n");
	kprintf("           ,;;;;;;, `\\. `\\         .,c$$$$$$$$$$$$$ec,.\n");
	kprintf("      ,;;!!!!!!!!!!!>; `. ,;!>> .e$$$$$$$$\"\".  \"?$$$$$$$e.\n");
	kprintf(" <:<!!!!!!!!'` ..,,,.`` ,!!!' ;,(?\"\"\"\"\"\"\";!!''<; `?$$$$$$PF ,;,\n");
	kprintf("  `'!!!!;;;;;;;;<!'''`  !!! ;,`'``''!!!;!!!!`..`!;  ,,,  .<!''`).\n");
	kprintf("     ```'''''``         `!  `!!!!><;;;!!!!! J$$b,`!>;!!:!!`,d?b`!>\n");
	kprintf("                          `'-;,(<!!!!!!!!!> $F   )...:!.  d\"  3 !>\n");
	kprintf("                              ```````''<!!!- \"=-='     .  `--=\",!>\n");
	kprintf("                         .ze$$$$$$$$$er  .,cd$$$$$$$$$$$$$$$$bc.'\n");
	kprintf("                      .e$$$$$$$$$$$$$$,$$$$$$$$$$$$$$$$$$$$$$$$$$.\n");
	kprintf("                     z$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$c .\n");
	kprintf("                    J$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$c\n");
	kprintf("                    $$$$$$$$$$$$$$P\"`?$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$b\n");
	kprintf("                    $$$$$$$$$$$$$$ dbc `\"\"?$$$$$$$$$$$$$$$$$$$$$$?$$$$$$$c\n");
	kprintf("                    ?$$$$$$$$$$$$$$$$$$c.      \"\"\"\"????????\"\"\"\" c$$$$$$$$P\n");
	kprintf("         .,,.        \"$$$$$$$$$$$$$$$$$$$$c.   ._              J$$$$$$$$$\n");
	kprintf(" .,,cc$$$$$$$$$bec,.  `?$$$$$$$$$$$$$$$$$$$$$c.```%%%%%%%%,%%%%%%,   c$$$$$$$$P\"\n");
	kprintf("$$$$$$$$$$$$$$$$$$$$$$c  \"\"?$$$$$$$$$$$$$$$$$$$$$bc,,.`` .,,c$$$$$$$P\"\",cb\n");
	kprintf("$$$$$$$$$$$$$$$$$$$$$$$b bc,.\"\"??$$$$$$$$$$$$$$FF\"\"?????\"\",J$$$$$P\" ,zd$$$\n");
	kprintf("$$$$$$$$$$$$$$$$$$$$$$$$ ?$???%%   `\"\"??$$$$$$$$$$$$bcucd$$$P\"\"\"  ==$$$$$$$\n");
	kprintf("$$$$$$$$$$$$$$$$$$$$$$$P\" ,;;;<!!!!!>;;,. `\"\"\"\"??????\"\"  ,;;;;;;;;;, `\"?$$\n");
	kprintf("$$$$$$$$$$$$$$$$$$$P\"\",;!!!!!!!!!!!!!!!!!!!!!!!;;;;;;!!!!!!!!!!!!!!!!!;  \"\n");
	kprintf("$$$$$$$$$$$$$$$$$\"\",;!!!!!!'``.,,,,,.```''!!!!!!!!!!!!!!!!!!!!'''''!!!!!>\n");
	kprintf("$$$$$$$$$$$$$$$\" ;!!!!!'`.z$$$$$$$$$$$$$ec,. ```'''''''``` .,,ccecec,`'!!!\n");
	kprintf("$$$$$$$$$$$$$\" ;!!!!' .c$$$$$$$$$$$$$$$$$$$$$$$c  :: .c$$$$$$$$$$$$$$$. <!\n");
	kprintf("$$$$$$$$$$$\" ;!!!!' .d$$$$$$$$$$$$$$$$$$$$$$$$$$b ' z$$$$$$$$$$$$$$$$$$c <\n");
	kprintf("$$$$$$$$$F  <!!!'.c$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$b  $$$$$$$$$$$$$$$$$$$$r\n");
	kprintf("$$$$$$$P\" <!!!' c$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$, \"$$$$$$$$$$$$$$$$$$$$\n");
	kprintf("$$$$$P\" ;!!!' z$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  $$$$$$$$$$$$$$$$$$$$\n");
	kprintf("\n");

	int16 i;
	for (i = 0; i < 33; ++i)
	{
		kprintf("%c", 11); // vertical tab
	}

	restore(mask);

	continuous_fight(my_name);

	return OK;
}

/*------------------------------------------------------------------------
 *  continuous_fight  -  continue to use resources and sleep every 
 *    so often
 *------------------------------------------------------------------------
 */
void continuous_fight(char *my_name)
{
	intmask mask; /* saved interrupt mask		*/

	while (TRUE)
	{
		mask = disable();

		if (g_total_clan_membership == 1 || g_total_clan_membership - 1 > g_total_clan_sleeping)
		{
			++g_total_clan_sleeping;
			restore(mask);
			sleepms(SLEEP_TIME_FIGHT + ((rand() % 10) * 1000));
			--g_total_clan_sleeping;
		}
		else
		{
			restore(mask);
		}
	}
}

/*------------------------------------------------------------------------
 *  print_jabber  -  a helper function that works for everyone
 *------------------------------------------------------------------------
 */
syscall print_process()
{
	struct procent *prptr; /* pointer to process	*/
	int32 i;			   /* index into proctabl	*/
	int16 count = 0;

	/* names for process states	*/
	char *pstate[] = {
		"free ", "curr ", "ready", "recv ", "sleep", "susp ",
		"wait ", "rtime "};

	/* Print header for items from the process table */

	kprintf("%c", 13); // carriage return

	kprintf("%3s %-16s %4s %10s state  QK\n",
			"Pid",
			"Name",
			"Prio",
			"Last Touch");

	kprintf("%3s %-16s %4s %10s ----- ----\n",
			"---",
			"----------------",
			"----",
			"----------");

	while (TRUE)
	{
		kprintf("%c", 10); // line feed
		while (count > 0)
		{
			--count;
			kprintf("%c", 11); // vertical tab
		}

		++count;

		for (i = 3; i < NPROC; ++i)
		{
			prptr = &proctab[i];
			if (prptr->prstate == PR_FREE)
			{
				// if the state is free then there is nothing to see here.
				continue;
			}
			if (strncmp(prptr->prname, "TMNT", 4) == 0)
			{
				// don't print the fun output stuff for this assignment
				continue;
			}
			if (strncmp(prptr->prname, "tmnt", 4) == 0)
			{
				// don't print the fun output stuff for this assignment
				continue;
			}
			kprintf("%3d %-16s %4d %10d %s %d\n",
					i,
					prptr->prname,
					prptr->prprio,
					prptr->last_touched,
					pstate[(int)prptr->prstate],
					queuetab[i].qkey);
			++count;
		}

		sleepms(SLEEP_TIME_PRINT);
	}
	return OK;
}

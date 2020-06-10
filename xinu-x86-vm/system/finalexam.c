/* finalexam.c - 
 *		footsoldier
 *		turtle
 *		continuous_fight
 *		print_process
 *		starvation_check
 * */

#include <xinu.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*------------------------------------------------------------------------
 *  global setting for starvation.
 *------------------------------------------------------------------------
 */
uint8 g_starvation_setting = 0;			/* which starvation fix will be used.	*/
uint8 g_starvation_seconds = 10;		/* how long will starvation go for.		*/
uint8 g_starvation_increment = 1;		/* how much should priority increase.	*/
uint8 g_is_starvation_checking = FALSE; /* keep time checking for starvation.	*/

/*------------------------------------------------------------------------
 *  global values for only this file.
 *------------------------------------------------------------------------
 */
int8 g_total_clan_membership = 0; /* keep track of # of resource hogs		*/
int8 g_total_clan_sleeping = 0;	  /* keep track of sleeping resource hogs	*/

/*------------------------------------------------------------------------
 *  footsoldier  -  a robot ninja. (resource hog)
 *------------------------------------------------------------------------
 */
syscall footsoldier(char *my_name)
{
	// increment membership.
	++g_total_clan_membership;

	// let the process sleep so that other process can start up.
	sleepms(SETUP_SLEEP_TIME);

	// start being a resource hog!
	continuous_fight(my_name);

	return OK;
}

/*------------------------------------------------------------------------
 *  turtle  -  A Teenage Mutant Ninja Turtle (starvation process)
 *------------------------------------------------------------------------
 */
syscall turtle(char *my_name)
{
	// let the process sleep so that other process can start up.
	sleepms(SETUP_SLEEP_TIME);

	intmask mask; /* saved interrupt mask		*/

	// going to print out a lot of info, so lets disable interupts
	mask = disable();

	// print a nice image of a Teenage Mutant Ninja Turtle.
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

	// move mouse pointer back to the top of the image.
	int16 i;
	for (i = 0; i < 33; ++i)
	{
		kprintf("%c", 11); // vertical tab
	}

	// done printing so restore interupts!
	restore(mask);

	// join the fight and be a resource hog!
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
	int32 half_sleep_time_fight = SLEEP_TIME_FIGHT / 2;

	// continue to use resources
	while (TRUE)
	{
		// disable interupts to make sure things are calculated correctly
		mask = disable();

		// if there is only one member in the clan
		// or there is more than one clan member hogging resources
		// sleep!
		if (g_total_clan_membership == 1 || g_total_clan_membership - 1 > g_total_clan_sleeping)
		{
			// update the number of clan members who are sleeping.
			++g_total_clan_sleeping;

			// turn back on interupts.
			restore(mask);

			// sleep for 100% to 150% of the default sleep milliseconds.
			sleepms(SLEEP_TIME_FIGHT + (rand() % half_sleep_time_fight));

			// this process is done sleeping.
			--g_total_clan_sleeping;
		}
		else
		{
			// turn back on interupts.
			restore(mask);
		}
	}
}

/*------------------------------------------------------------------------
 *  print_process  -  A process that prints out the process info.
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

	kprintf("%3s %-16s %4s %10s %5s %4s \n",
			"Pid",
			"Name",
			"Prio",
			"Last Touch",
			"state",
			" QK ");

	kprintf("%3s %-16s %4s %10s %5s %4s \n",
			"---",
			"----------------",
			"----",
			"----------",
			"-----",
			"----");

	// keep printing the table data over and over.
	while (TRUE)
	{
		// place the curser to the top of the table data area.
		while (count > 0)
		{
			--count;
			kprintf("%c", 11); // vertical tab
		}

		// go through the process table and look for prosses to present
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

			// output a row of information for this process.
			kprintf("%3d %-16s %4d %10d %5s %4d\n",
					i,
					prptr->prname,
					prptr->prprio,
					prptr->last_touched,
					pstate[(int)prptr->prstate],
					queuetab[i].qkey);

			// count the number of lines we are adding.
			++count;
		}

		// sleep this process so that other processes may run
		// no need to be a resource hog!
		sleepms(SLEEP_TIME_PRINT);
	}

	return OK;
}

/*------------------------------------------------------------------------
 *  starvation_check  -  A process checks for starvation.
 *------------------------------------------------------------------------
 */
syscall starvation_check(void)
{
	// always check for starvation.
	while (g_is_starvation_checking)
	{
		// sleep for 2 seconds (2000 milliseconds).
		sleepms(SLEEP_TIME_CHECK);

		// Check if any processes are starving.
		resched_starvation_check(g_starvation_seconds, g_starvation_increment);
	}

	return OK;
}

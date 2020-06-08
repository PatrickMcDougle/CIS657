/* finalexam.c - rocksteady, bebop, turtle */

#include <xinu.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SLEEP_TIME_FIGHT 2000
#define SLEEP_TIME_PRINT 200
#define SETUP_SLEEP_TIME 300

int16 g_line_current = 0;
int16 g_line_next = 0;

/*------------------------------------------------------------------------
 *  rocksteady  -  a Rhino Mutant for the footclan
 *------------------------------------------------------------------------
 */
syscall rocksteady(char *my_name)
{
	int16 my_line = g_line_next++;

	char *my_frases[] = {
		"Ooh, I'm gonna enjoy this!                    ",
		"Hahaha! One smashed up chump, comin' RIGHT up!"};
	int16 frases_length = 2;

	continuous_jabber(my_line, my_name, my_frases, frases_length);

	return OK;
}

/*------------------------------------------------------------------------
 *  bebop  -  a Warthog Mutant for the footclan
 *------------------------------------------------------------------------
 */
syscall bebop(char *my_name)
{
	int16 my_line = g_line_next++;

	char *my_frases[] = {
		"Now we gotcha right where we want ya!",
		"We got a score to settle!            "};
	int16 frases_length = 2;

	continuous_jabber(my_line, my_name, my_frases, frases_length);

	return OK;
}

/*------------------------------------------------------------------------
 *  shredder  -  The leader of the footclan
 *------------------------------------------------------------------------
 */
syscall shredder(char *my_name)
{
	int16 my_line = g_line_next++;

	char *my_frases[] = {
		"Destroy them all! Ha ha. Aw it feels so good to be so bad.",
		"Far from it. Tonight, I dine on turtle soup!              "};
	int16 frases_length = 2;

	continuous_jabber(my_line, my_name, my_frases, frases_length);

	return OK;
}

/*------------------------------------------------------------------------
 *  krang  -  a being from Demension X
 *------------------------------------------------------------------------
 */
syscall krang(char *my_name)
{
	int16 my_line = g_line_next++;

	char *my_frases[] = {
		"If I had hands I would cover my ears, if I had them!               ",
		"Sounds like the perfect job for a couple of yo-yos. I made a funny.",
		"What have you to say now, mini mutants?                            "};
	int16 frases_length = 3;

	continuous_jabber(my_line, my_name, my_frases, frases_length);

	return OK;
}

/*------------------------------------------------------------------------
 *  footsoldier  -  a robot ninja.
 *------------------------------------------------------------------------
 */
syscall footsoldier(char *my_name)
{
	int16 my_line = g_line_next++;

	char *my_frases[] = {
		"HI YAH!",
		"Kiai!!!"};
	int16 frases_length = 2;

	continuous_jabber(my_line, my_name, my_frases, frases_length);

	return OK;
}

/*------------------------------------------------------------------------
 *  turtle  -  a Turtle Mutant that is starving.
 *------------------------------------------------------------------------
 */
syscall turtle(char *my_name)
{
	int16 my_line = g_line_next++;

	char *my_frases[] = {
		"Cowabunga!  ",
		"Pizza power!",
		"Radical!    "};
	int16 frases_length = 2;

	sleepms(100);

	continuous_jabber(my_line, my_name, my_frases, frases_length);

	return OK;
}

void continuous_jabber(int16 my_line, char *my_name, char *my_frases[], int16 frases_length)
{
	int16 i;
	char *my_speak = NULL;
	intmask mask; /* saved interrupt mask		*/

	while (TRUE)
	{
		mask = disable();

		i = rand() % (frases_length + frases_length + frases_length + frases_length);
		if (i < frases_length)
		{
			my_speak = my_frases[i];
		}
		else
		{
			my_speak = NULL;
		}
		print_jabber(my_line, my_name, my_speak, getprio(getpid()));

		restore(mask);

		sleepms(SLEEP_TIME_FIGHT);
	}
}

/*------------------------------------------------------------------------
 *  print_jabber  -  a helper function that works for everyone
 *------------------------------------------------------------------------
 */
void print_jabber(int16 my_line, char *my_name, char *my_speak, pri16 priority)
{
	kprintf("%c", 13); // carriage return
	while (g_line_current < my_line)
	{
		kprintf("%c", 10); // line feed
		++g_line_current;
	}
	while (g_line_current > my_line)
	{
		kprintf("%c", 11); // vertical tab
		--g_line_current;
	}
	if (my_speak != NULL)
	{
		kprintf("%14s [%d] %s", my_name, priority, my_speak);
	}
	else
	{
		kprintf("%14s [%d] ", my_name, priority);
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

		// for (i = 0; i < NQENT; ++i)
		// {
		// 	kprintf(" Q [%d|%d|%d]\n", queuetab[i].qprev, queuetab[i].qkey, queuetab[i].qnext);
		// }

		sleepms(SLEEP_TIME_PRINT);
	}
	return OK;
}

int8 g_total_clan_membership = 0;
int8 g_total_clan_sleeping = 0;

/*------------------------------------------------------------------------
 *  footsoldier2  -  a robot ninja.
 *------------------------------------------------------------------------
 */
syscall footsoldier2(char *my_name)
{
	// increment membership.
	++g_total_clan_membership;

	sleepms(SETUP_SLEEP_TIME);

	continuous_fight(my_name);

	return OK;
}

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
			sleepms(SLEEP_TIME_FIGHT + ((rand() % 10) * 100));
			--g_total_clan_sleeping;
		}
		else
		{
			restore(mask);
		}
	}
}

syscall turtle2(char *my_name)
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
	kprintf(" .,,cc$$$$$$$$$bec,.  `?$$$$$$$$$$$$$$$$$$$$$c.```%%%%,%%%,   c$$$$$$$$P\"\n");
	kprintf("$$$$$$$$$$$$$$$$$$$$$$c  \"\"?$$$$$$$$$$$$$$$$$$$$$bc,,.`` .,,c$$$$$$$P\"\",cb\n");
	kprintf("$$$$$$$$$$$$$$$$$$$$$$$b bc,.\"\"??$$$$$$$$$$$$$$FF\"\"?????\"\",J$$$$$P\" ,zd$$$\n");
	kprintf("$$$$$$$$$$$$$$$$$$$$$$$$ ?$???%   `\"\"??$$$$$$$$$$$$bcucd$$$P\"\"\"  ==$$$$$$$\n");
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
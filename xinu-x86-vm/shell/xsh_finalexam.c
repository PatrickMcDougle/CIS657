/* xsh_finalexam.c - xsh_tmnt, xsh_starve */

#include <xinu.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*------------------------------------------------------------------------
 * xsh_tmnt - Q1: command line to show off starvation fix.
 *------------------------------------------------------------------------
 */
shellcmd xsh_tmnt(int nargs, char *args[])
{
	int32 i;
	pri16 priority = 50;
	char ch;	 /* next character of argument	*/
	char *chptr; /* walks along argument string	*/
	char *turtle_names[] = {
		"Leonardo",
		"Donatello",
		"Michelangelo",
		"Raphael"};

	char *the_name;
	uint8 number_of_footclan = 4;
	uint8 number_of_turtles = 1;
	uint8 turtle_name_start = 0;

	if (nargs == 2 && strncmp(args[1], "--help", 7) == 0)
	{
		printf("Use:\n");
		printf("\t%s [foot [turtles]]\n", args[0]);
		printf("Description:\n");
		printf("\tStarts the presentation of processes and starvation.\n");
		printf("Options:\n");
		printf("  foot   \tThe number of footclan solders (resource hogs) to start.\n");
		printf("  turtles\tThe number of turtles (starving processes) to start.\n");
		printf("  --help \tdisplay this help and exit\n");
		return SHELL_OK;
	}

	if (nargs >= 2)
	{
		chptr = args[1];
		ch = *chptr++;
		number_of_footclan = 0;

		while (ch != NULLCH)
		{
			if ((ch < '0') || (ch > '9'))
			{
				fprintf(stderr, "%s: non-digit in footclan soldiers (resource hogs).\n", args[0]);
				return SHELL_ERROR;
			}
			number_of_footclan = 10 * number_of_footclan + (ch - '0');
			ch = *chptr++;
		}
	}

	if (nargs >= 3)
	{
		chptr = args[2];
		ch = *chptr++;
		number_of_turtles = 0;

		if (ch != NULLCH)
		{
			if ((ch < '0') || (ch > '9'))
			{
				fprintf(stderr, "%s: non-digit in turtles (starving processes).\n", args[0]);
				return SHELL_ERROR;
			}
			if ((ch < '1') || (ch > '4'))
			{
				fprintf(stderr, "%s: digit out of range for turtles (starving processes).\n", args[0]);
				return SHELL_ERROR;
			}
			number_of_turtles = (ch - '0');
			ch = *chptr++;
		}
	}

	// Start up resource hogs
	resume(create(footsoldier, 1024, priority, "Rocksteady", 1, "Rocksteady"));
	resume(create(footsoldier, 1024, priority - 5, "Bebop", 1, "Bebop"));

	for (i = 1; i <= number_of_footclan; ++i)
	{
		the_name = getmem(sizeof(char) * PNMLEN);
		sprintf(the_name, "FootSoldier %d", i);
		resume(create(footsoldier, 1024, priority - 10, the_name, 1, the_name));
	}

	// Start up starving processes
	turtle_name_start = rand() % 4;
	for (i = 1; i <= number_of_turtles; ++i)
	{
		the_name = getmem(sizeof(char) * PNMLEN);
		sprintf(the_name, "%-*s", PNMLEN, turtle_names[(turtle_name_start + i) % 4]);
		resume(create(turtle, 1024, priority - 25, the_name, 1, the_name));
	}

	// Start printout table.
	resume(create(print_process, 1024, 400, "TMNT", 0));

	return SHELL_OK;
}

/*------------------------------------------------------------------------
 * xsh_starve - Set which starvation helper to use or not.
 *------------------------------------------------------------------------
 */
shellcmd xsh_starve(int nargs, char *args[])
{
	char ch;	 /* next character of argument	*/
	char *chptr; /* walks along argument string	*/

	if (nargs == 2 && strncmp(args[1], "--help", 7) == 0)
	{

		printf("Use:\n");
		printf("\t%s [set [sec [inc]]]\n", args[0]);
		printf("Description:\n");
		printf("\tSets the system to handle process starvation.\n");
		printf("Options:\n");
		printf("  set\tSpecific how to handle starvation:\n");
		printf("    0:\tDefault value. Lets process starve.\n");
		printf("    1:\tOn Context Switch when # of seconds of starving start incrementing\n");
		printf("      \tstarving process's priority.\n");
		printf("    2:\tOn every seconds increment starving process's priority.\n");
		printf("  sec\tSets the number of seconds needed before resolving starvation.\n");
		printf("  inc\tSets the number of increments to increase starving process's priority.\n");
		printf("  --help\tdisplay this help and exit\n");
		return SHELL_OK;
	}

	g_starvation_setting = 0;
	g_starvation_seconds = 10;
	g_starvation_increment = 1;

	if (nargs >= 2)
	{
		chptr = args[1];
		ch = *chptr++;

		if (ch != NULLCH)
		{
			if ((ch < '0') || (ch > '9'))
			{
				fprintf(stderr, "%s: non-digit in starvation argument.\n", args[0]);
				return SHELL_ERROR;
			}
			if ((ch < '0') || (ch > '2'))
			{
				fprintf(stderr, "%s: digit out of range in starvation argument.\n", args[0]);
				return SHELL_ERROR;
			}
			g_starvation_setting = (ch - '0');
		}
	}
	if (nargs >= 3)
	{
		chptr = args[2];
		ch = *chptr++;
		g_starvation_seconds = 0;

		while (ch != NULLCH)
		{
			if ((ch < '0') || (ch > '9'))
			{
				fprintf(stderr, "%s: non-digit in starvation argument.\n", args[0]);
				return SHELL_ERROR;
			}
			g_starvation_seconds = 10 * g_starvation_seconds + (ch - '0');
			ch = *chptr++;
		}
	}
	if (nargs >= 4)
	{
		chptr = args[3];
		ch = *chptr++;
		g_starvation_increment = 0;

		while (ch != NULLCH)
		{
			if ((ch < '0') || (ch > '9'))
			{
				fprintf(stderr, "%s: non-digit in starvation argument.\n", args[0]);
				return SHELL_ERROR;
			}
			g_starvation_increment = 10 * g_starvation_increment + (ch - '0');
			ch = *chptr++;
		}
	}

	kprintf("Starvation Settings\n");
	kprintf("  Type: ");
	switch (g_starvation_setting)
	{
	case STARVE_DEFAULT:
		kprintf("Default\n");
		break;
	case STARVE_Q1_CONTEXT_SWITCH:
		kprintf("Context Switch\n");
		break;
	case STARVE_Q2_2_SECONDS:
		kprintf("Seconds\n");
		break;

	default:
		kprintf("???\n");
		break;
	}

	kprintf("  Sec : %d\n", g_starvation_seconds);
	kprintf("  Inc : %d\n", g_starvation_increment);

	return SHELL_OK;
}
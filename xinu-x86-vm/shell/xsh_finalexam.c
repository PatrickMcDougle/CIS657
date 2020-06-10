/* xsh_finalexam.c - 
 *		xsh_tmnt
 *		xsh_starve
 * */

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
	int32 i;				 /* default incrementor			*/
	int32 process_name_size; /* max process memory size		*/
	pri16 priority = 50;	 /* starting priority for foot	*/
	char ch;				 /* next character of argument	*/
	char *chptr;			 /* walks along argument string	*/
	char *turtle_names[] = {
		"Leonardo",
		"Donatello",
		"Michelangelo",
		"Raphael"};

	char *the_name;
	uint8 number_of_footclan = 4; /* default number of footclan	*/
	uint8 number_of_turtles = 1;  /* default number of turtles	*/
	uint8 turtle_name_start = 0;  /* default turtle name		*/

	// default help menu for this command
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

	// if there is a 2nd argument, then setup the number of footclan.
	if (nargs >= 2)
	{
		chptr = args[1];
		ch = *chptr++;
		number_of_footclan = 0; /* clear out value to read in new value	*/

		// keep reading characters utill we get to the end.
		while (ch != NULLCH)
		{
			// only except digits.
			if ((ch < '0') || (ch > '9'))
			{
				fprintf(stderr, "%s: non-digit in footclan soldiers (resource hogs).\n", args[0]);
				return SHELL_ERROR;
			}

			// convert characters to digits.
			number_of_footclan = 10 * number_of_footclan + (ch - '0');

			// next character please.
			ch = *chptr++;
		}
	}

	// if there is a 3rd argument, then setup the number of turtles.
	if (nargs >= 3)
	{
		chptr = args[2];
		ch = *chptr++;
		number_of_turtles = 0; /* clear out value to read in new value	*/

		// only need to read one character.
		if (ch != NULLCH)
		{
			// only except digits.
			if ((ch < '0') || (ch > '9'))
			{
				fprintf(stderr, "%s: non-digit in turtles (starving processes).\n", args[0]);
				return SHELL_ERROR;
			}

			// need 1 to 4 characters.  Only valid values.
			if ((ch < '1') || (ch > '4'))
			{
				fprintf(stderr, "%s: digit out of range for turtles (starving processes).\n", args[0]);
				return SHELL_ERROR;
			}

			// convert characters to digits.
			number_of_turtles = (ch - '0');

			// next character please.
			ch = *chptr++;
		}
	}

	// set the proces name size used for everyone.
	process_name_size = sizeof(char) * PNMLEN;

	// Start up two default Bosses (resource hogs)
	resume(create(footsoldier, 1024, priority, "Rocksteady", 1, "Rocksteady"));
	resume(create(footsoldier, 1024, priority - 5, "Bebop", 1, "Bebop"));

	// starup the other footclan soldiers (resource hogs)
	for (i = 1; i <= number_of_footclan; ++i)
	{
		the_name = getmem(process_name_size);
		sprintf(the_name, "FootSoldier %d", i);
		resume(create(footsoldier, 1024, priority - 10, the_name, 1, the_name));
	}

	// Start up turtles (starving processes)
	turtle_name_start = rand() % 4;
	for (i = 1; i <= number_of_turtles; ++i)
	{
		the_name = getmem(process_name_size);
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

	// default help menu for this command
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

	// if there is a 2nd argument than get the setting value.
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

	// if there is a 3rd argument than get the seconds value.
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

	// if there is a 4th argument than get the increment value.
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

	// print out the settings
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

	// if starvation setting is set to seconds...
	if (g_starvation_setting == STARVE_Q2_2_SECONDS)
	{
		// we are checking for starvation.
		g_is_starvation_checking = TRUE;

		// start a timer to check for starvation.
		resume(create(starvation_check, 1024, 400, "TMNT-2sec", 0));
	}
	else
	{
		// we are done checking for starvation.
		g_is_starvation_checking = FALSE;
	}

	kprintf("  Sec : %d\n", g_starvation_seconds);
	kprintf("  Inc : %d\n", g_starvation_increment);

	return SHELL_OK;
}
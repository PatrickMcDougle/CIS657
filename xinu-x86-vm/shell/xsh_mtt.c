/* xsh_mtt.c - xsh_mtt1, xsh_mtt2, xsh_mtt3 */

#include <xinu.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// function declarations:
void run_forever(pri16);
void run_forever_but_sleep_first(pri16, int32);
void run_after_wait(pri16, sid32);
bool8 is_positive_integer(const char *);

/*------------------------------------------------------------------------
 * xsh_mtt1 - Creates a new process with a user specified priority 
 *   (or default) that starts running in an empty infinite loop after
 *   announcing its PID.
 *------------------------------------------------------------------------
 */
shellcmd xsh_mtt1(int nargs, char *args[])
{
	pri16 priority = INITPRIO;

	// Output info for '--help' argument
	if (nargs == 2)
	{
		if (strncmp(args[1], "--help", 7) == 0)
		{
			printf("Usage: %s <priority>\n\n", args[0]);
			printf("Description:\n");
			printf("\tStarts a new process that runs forever with the given priority.\n");
			printf("Options:\n");
			printf("\t<priority>\tThe priority value to use when creating the forever\n");
			printf("\t\t\tprocess.  Valid values are in the range: 1 to 127.\n");
			printf("\t\t\tDefault value is 20.\n");
			printf("\t--help    \tDisplay this help and exit\n");
			return OK;
		}

		if (is_positive_integer(args[1]) == TRUE)
		{
			// going to assume the input is an integer.
			priority = atoi(args[1]);

			// printf("Priority updated: [%d]\n", priority);

			// check to make sure the priority is valid.
			if (priority < 1 || priority > 127)
			{
				fprintf(stderr, "%s: Incorrect argument - value needs to be in the range of 1 to 127 inclusive.\n", args[0]);
				fprintf(stderr, "Try '%s --help' for more information\n", args[0]);
				return SYSERR;
			}
		}
		else
		{
			fprintf(stderr, "%s: Incorrect argument - value is not a postive number\n", args[0]);
			fprintf(stderr, "Try '%s --help' for more information\n", args[0]);
			return SYSERR;
		}
	}

	// Check argument count
	else if (nargs > 2)
	{
		fprintf(stderr, "%s: Incorrect number of arguments\n", args[0]);
		fprintf(stderr, "Try '%s --help' for more information\n", args[0]);
		return SYSERR;
	}

	pid32 pid_run_forever = create(run_forever, 1024, priority, "patrick-c", 1, priority);

	resume(pid_run_forever);

	return 0;
}

/*------------------------------------------------------------------------
 * xsh_mtt2 - This command creates a new process with a user specified
 *   priority (or default) that starts running in an empty infinite loop
 *   after announcing its PID after calling sleep () or sleepms () to
 *   sleep for 10 seconds.
 *------------------------------------------------------------------------
 */
shellcmd xsh_mtt2(int nargs, char *args[])
{
	pri16 priority = INITPRIO;

	// Output info for '--help' argument
	if (nargs == 2)
	{
		if (strncmp(args[1], "--help", 7) == 0)
		{
			printf("Usage: %s <priority>\n\n", args[0]);
			printf("Description:\n");
			printf("\tStarts a new process that runs forever with the given priority.\n");
			printf("Options:\n");
			printf("\t<priority>\tThe priority value to use when creating the forever\n");
			printf("\t\t\tprocess.  Valid values are in the range: 1 to 127.\n");
			printf("\t\t\tDefault value is 20.\n");
			printf("\t--help    \tDisplay this help and exit\n");
			return OK;
		}

		if (is_positive_integer(args[1]) == TRUE)
		{
			// going to assume the input is an integer.
			priority = atoi(args[1]);

			// printf("Priority updated: [%d]\n", priority);

			// check to make sure the priority is valid.
			if (priority < 1 || priority > 127)
			{
				fprintf(stderr, "%s: Incorrect argument - value needs to be in the range of 1 to 127 inclusive.\n", args[0]);
				fprintf(stderr, "Try '%s --help' for more information\n", args[0]);
				return SYSERR;
			}
		}
		else
		{
			fprintf(stderr, "%s: Incorrect argument - value is not a postive number\n", args[0]);
			fprintf(stderr, "Try '%s --help' for more information\n", args[0]);
			return SYSERR;
		}
	}

	// Check argument count
	else if (nargs > 2)
	{
		fprintf(stderr, "%s: Incorrect number of arguments\n", args[0]);
		fprintf(stderr, "Try '%s --help' for more information\n", args[0]);
		return SYSERR;
	}

	pid32 pid_run_forever = create(run_forever_but_sleep_first, 1024, priority, "patrick-s", 2, priority, 10);

	resume(pid_run_forever);

	return 0;
}

/*------------------------------------------------------------------------
 * xsh_mtt3 - This shell command called “psready” will list the content
 *   of the ready list only.  Subset of info of the "ps" command
 *------------------------------------------------------------------------
 */
shellcmd xsh_mtt3(int nargs, char *args[])
{
	// THIS CODE WAS COPIED OUT OF xsh_ps.c with minor tweeks.
	// //// //// /// ////// /// // //////// //// ///// ///////

	struct procent *prptr; /* pointer to process		*/
	int32 i;			   /* index into proctabl		*/
	char *pstate[] = {	   /* names for process states	*/
					  "free ", "curr ", "ready", "recv ", "sleep", "susp ",
					  "wait ", "rtime "};

	/* For argument '--help', emit help about the 'ps' command	*/

	if (nargs == 2 && strncmp(args[1], "--help", 7) == 0)
	{
		printf("Use: %s\n\n", args[0]);
		printf("Description:\n");
		printf("\tDisplays information about running processes\n");
		printf("Options:\n");
		printf("\t--help\t display this help and exit\n");
		return 0;
	}

	/* Check for valid number of arguments */

	if (nargs > 1)
	{
		fprintf(stderr, "%s: too many arguments\n", args[0]);
		fprintf(stderr, "Try '%s --help' for more information\n",
				args[0]);
		return 1;
	}

	/* Print header for items from the process table */

	printf("%3s %-16s %5s %4s %4s %10s %-10s %10s\n",
		   "Pid", "Name", "State", "Prio", "Ppid", "Stack Base",
		   "Stack Ptr", "Stack Size");

	printf("%3s %-16s %5s %4s %4s %10s %-10s %10s\n",
		   "---", "----------------", "-----", "----", "----",
		   "----------", "----------", "----------");

	/* Output information for each process */

	for (i = 0; i < NPROC; i++)
	{
		prptr = &proctab[i];
		if (prptr->prstate != PR_READY)
		{
			/* skip everything except for ready state items	*/
			continue;
		}
		printf("%3d %-16s %s %4d %4d 0x%08X 0x%08X %10d\n",
			   i, prptr->prname, pstate[(int)prptr->prstate],
			   prptr->prprio, prptr->prparent, prptr->prstkbase,
			   prptr->prstkptr, prptr->prstklen);
	}
	return 0;
}

/*------------------------------------------------------------------------
 * xsh_mtt4 - This shell command called “wait” will create a new process
 *   that starts waiting on a semaphore that was created with the initial
 *   count of zero after announcing its PID.
 *------------------------------------------------------------------------
 */
shellcmd xsh_mtt4(int nargs, char *args[])
{
	pri16 priority = INITPRIO;

	// Output info for '--help' argument
	if (nargs == 2)
	{
		if (strncmp(args[1], "--help", 7) == 0)
		{
			printf("Usage: %s <priority>\n\n", args[0]);
			printf("Description:\n");
			printf("\tStarts a new process that runs forever with the given priority.\n");
			printf("Options:\n");
			printf("\t<priority>\tThe priority value to use when creating the forever\n");
			printf("\t\t\tprocess.  Valid values are in the range: 1 to 127.\n");
			printf("\t\t\tDefault value is 20.\n");
			printf("\t--help    \tDisplay this help and exit\n");
			return OK;
		}

		if (is_positive_integer(args[1]) == TRUE)
		{
			// going to assume the input is an integer.
			priority = atoi(args[1]);

			// printf("Priority updated: [%d]\n", priority);

			// check to make sure the priority is valid.
			if (priority < 1 || priority > 127)
			{
				fprintf(stderr, "%s: Incorrect argument - value needs to be in the range of 1 to 127 inclusive.\n", args[0]);
				fprintf(stderr, "Try '%s --help' for more information\n", args[0]);
				return SYSERR;
			}
		}
		else
		{
			fprintf(stderr, "%s: Incorrect argument - value is not a postive number\n", args[0]);
			fprintf(stderr, "Try '%s --help' for more information\n", args[0]);
			return SYSERR;
		}
	}

	// Check argument count
	else if (nargs > 2)
	{
		fprintf(stderr, "%s: Incorrect number of arguments\n", args[0]);
		fprintf(stderr, "Try '%s --help' for more information\n", args[0]);
		return SYSERR;
	}
	
	sid32 semephore = semcreate(-1);

	pid32 pid_run_forever = create(run_after_wait, 1024, priority, "patrick-w", 2, priority, semephore);

	resume(pid_run_forever);

	return 0;
}

/*------------------------------------------------------------------------
 * xsh_mtt5 - This shell command called “signaln” will signal the given
 *   number of semaphores.
 *------------------------------------------------------------------------
 */
shellcmd xsh_mtt5(int nargs, char *args[])
{
	int32 i; /* walks through args array	*/

	if (nargs > 1)
	{
		printf("%s", args[1]);

		for (i = 2; i < nargs; i++)
		{
			printf(" %s", args[i]);
		}
	}
	printf("\n");

	return 0;
}

/*------------------------------------------------------------------------
 * xsh_mtt6 - This shell command called "resumen" will take process IDS
 *   as arguments and resume them all at once.
 *------------------------------------------------------------------------
 */
shellcmd xsh_mtt6(int nargs, char *args[])
{
	int32 i; /* walks through args array	*/

	if (nargs > 1)
	{
		printf("%s", args[1]);

		for (i = 2; i < nargs; i++)
		{
			printf(" %s", args[i]);
		}
	}
	printf("\n");

	return 0;
}

////////////////////////////////////////////////////////////////////
// FOLLOWING CODE IS A COPY OF MY LAB3 VERSION OF THE CODE.
// This is not the group's version of the code.
// Also some code changes due to test and finding "bugs"
////////////////////////////////////////////////////////////////////

/* ---------- ---------- ---------- ---------- ---------- ----------
 * run_forever - shell command to do whatever you want it to do.
 * ---------- ---------- ---------- ---------- ---------- ----------
 */
void run_forever(
	pri16 priority /* Priority value for this process */
)
{
	printf("create priority & pid: [%d,%d]\n", priority, getpid());

	while (TRUE)
	{
		// do nothing at this time.
	}
}

/* ---------- ---------- ---------- ---------- ---------- ----------
 * is_positive_integer - checks the string to make sure it is a 
 *   positive integer.
 * ---------- ---------- ---------- ---------- ---------- ----------
 */
bool8 is_positive_integer(const char *pStr)
{
	if (pStr == NULL || *pStr == '\0')
	{
		return FALSE;
	}

	char c = *pStr;

	// checking to see if the string starts with a +
	if (c == '+')
	{
		// positive values are ok.
		// move to the next character.
		++pStr;
	}

	while (*pStr)
	{
		c = *pStr;

		// ASCII: '0' ... '9' values.  Anything outside that is not valid.
		if (c < '0' || c > '9')
		{
			return FALSE;
		}

		++pStr;
	}

	// this string is a valid number.
	return TRUE;
}

////////////////////////////////////////////////////////////////////

void run_forever_but_sleep_first(
	pri16 priority,		/* Priority value for this process */
	int32 sleep_seconds /* how long to sleep in seconds. */
)
{
	sleep(sleep_seconds);

	printf("sleep priority & pid: [%d,%d]\n", priority, getpid());

	while (TRUE)
	{
		// do nothing at this time.
	}
}

/* ---------- ---------- ---------- ---------- ---------- ----------
 * run_after_wait - Function/Process that prints priority and 
 *   process ID, then waits for the semaphore to clear up before
 *   moving on.
 * ---------- ---------- ---------- ---------- ---------- ----------
 */
void run_after_wait(
	pri16 priority, /* Priority value for this process */
	sid32 semaphore)
{
	printf("wait priority & pid: [%d,%d]\n", priority, getpid());

	wait(semaphore);

	printf("done waiting!\n");

	// do nothing at this time.
}

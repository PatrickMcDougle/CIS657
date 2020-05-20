/* xsh_mtt.c - xsh_mtt1, xsh_mtt2, xsh_mtt3 */

#include <xinu.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// function declarations:
void run_forever(pri16);
void run_forever_but_sleep_first(pri16, int32);
syscall run_after_wait(pri16, sid32);
bool8 is_positive_integer(const char *);

// global variables:
sid32 g_semaphoreID;
// pid32 g_first_wait_process_id = -1;

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
			kprintf("Usage: %s <priority>\n\n", args[0]);
			kprintf("Description:\n");
			kprintf("\tStarts a new process that runs forever with the given priority.\n");
			kprintf("Options:\n");
			kprintf("\t<priority>\tThe priority value to use when creating the process\n");
			kprintf("\t\t\tprocess.  Valid values are in the range: 1 to 127.\n");
			kprintf("\t\t\tDefault value is 20.\n");
			kprintf("\t--help    \tDisplay this help and exit\n");
			return OK;
		}

		if (is_positive_integer(args[1]) == TRUE)
		{
			// going to assume the input is an integer.
			priority = atoi(args[1]);

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
			kprintf("Usage: %s <priority>\n\n", args[0]);
			kprintf("Description:\n");
			kprintf("\tStarts a new process that runs forever with the given priority.\n");
			kprintf("Options:\n");
			kprintf("\t<priority>\tThe priority value to use when creating the process\n");
			kprintf("\t\t\tprocess.  Valid values are in the range: 1 to 127.\n");
			kprintf("\t\t\tDefault value is 20.\n");
			kprintf("\t--help    \tDisplay this help and exit\n");
			return OK;
		}

		if (is_positive_integer(args[1]) == TRUE)
		{
			// going to assume the input is an integer.
			priority = atoi(args[1]);

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
		kprintf("Use: %s\n\n", args[0]);
		kprintf("Description:\n");
		kprintf("\tDisplays information about READY only running processes\n");
		kprintf("Options:\n");
		kprintf("\t--help\t display this help and exit\n");
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

	kprintf("%3s %-16s %5s %4s %4s %10s %-10s %10s\n",
			"Pid", "Name", "State", "Prio", "Ppid", "Stack Base",
			"Stack Ptr", "Stack Size");

	kprintf("%3s %-16s %5s %4s %4s %10s %-10s %10s\n",
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
		kprintf("%3d %-16s %s %4d %4d 0x%08X 0x%08X %10d\n",
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
	struct procent *prptr; /* pointer to proc. table entry */

	// Output info for '--help' argument
	if (nargs == 2)
	{
		if (strncmp(args[1], "--help", 7) == 0)
		{
			kprintf("Usage: %s <priority>\n\n", args[0]);
			kprintf("Description:\n");
			kprintf("\tStarts a new process that waits to finish running with the given priority.\n");
			kprintf("Options:\n");
			kprintf("\t<priority>\tThe priority value to use when creating the process\n");
			kprintf("\t\t\tprocess.  Valid values are in the range: 1 to 127.\n");
			kprintf("\t\t\tDefault value is 20.\n");
			kprintf("\t--help    \tDisplay this help and exit\n");
			return OK;
		}

		if (is_positive_integer(args[1]) == TRUE)
		{
			// going to assume the input is an integer.
			priority = atoi(args[1]);

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

	sid32 semaphoreID = semcreate(0);

	pid32 pid_run_forever = create(run_after_wait, 1024, priority, "patrick-w", 2, priority, semaphoreID);

	// if (g_first_wait_process_id == -1)
	// {
	// 	g_first_wait_process_id = pid_run_forever;
	// }

	prptr = &proctab[pid_run_forever];
	prptr->prsem = semaphoreID;

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
	struct procent *prptr; /* pointer to proc. table entry */
	int16 unwait_number_of_processes = 1;
	int16 i;

	// Output info for '--help' argument
	if (nargs == 2)
	{
		if (strncmp(args[1], "--help", 7) == 0)
		{
			kprintf("Usage: %s <priority>\n\n", args[0]);
			kprintf("Description:\n");
			kprintf("\tStarts a new process that runs forever with the given priority.\n");
			kprintf("Options:\n");
			kprintf("\t<priority>\tThe priority value to use when creating the forever\n");
			kprintf("\t\t\tprocess.  Valid values are in the range: 1 to 127.\n");
			kprintf("\t\t\tDefault value is 20.\n");
			kprintf("\t--help    \tDisplay this help and exit\n");
			return OK;
		}

		if (is_positive_integer(args[1]) == TRUE)
		{
			// going to assume the input is an integer.
			unwait_number_of_processes = atoi(args[1]);

			// check to make sure the unwait_number_of_processes is valid.
			if (unwait_number_of_processes < 1 || unwait_number_of_processes > 127)
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
	else
	{
		fprintf(stderr, "%s: Incorrect number of arguments\n", args[0]);
		fprintf(stderr, "Try '%s --help' for more information\n", args[0]);
		return SYSERR;
	}

	kprintf(" unwaiting:: %d", unwait_number_of_processes);

	// prptr = &proctab[g_first_wait_process_id];

	while (unwait_number_of_processes > 0)
	{

		for (i = 0; i < NPROC; i++)
		{
			prptr = &proctab[i];

			if (prptr->prstate == PR_WAIT)
			{
				signal(prptr->prsem);
				--unwait_number_of_processes;
				break;
			}
		}
		if (i == NPROC)
		{
			break;
		}
	}

	return 0;
}

/*------------------------------------------------------------------------
 * xsh_mtt6 - This shell command called "resumen" will take process IDS
 *   as arguments and resume them all at once.
 *------------------------------------------------------------------------
 */
shellcmd xsh_mtt6(int nargs, char *args[])
{
	int16 resume_process_id;
	int16 i;

	intmask mask; /* saved interrupt mask		*/

	// Output info for '--help' argument
	if (nargs >= 2)
	{
		if (strncmp(args[1], "--help", 7) == 0)
		{
			kprintf("Usage: %s <processID>+\n\n", args[0]);
			kprintf("Description:\n");
			kprintf("\tPuts all the processes that were passed in into resume state.\n");
			kprintf("Options:\n");
			kprintf("\t<priority>+\tThe priority value to use when creating the forever\n");
			kprintf("\t\t\tprocess.  Valid values are in the range: 1 to 127.\n");
			kprintf("\t\t\tDefault value is 20.\n");
			kprintf("\t--help    \tDisplay this help and exit\n");
			return OK;
		}

		mask = disable();
		for (i = 1; i < nargs; ++i)
		{
			if (is_positive_integer(args[i]) == TRUE)
			{
				// going to assume the input is an integer.
				resume_process_id = atoi(args[i]);
				// check to make sure the unwait_number_of_processes is valid.
				if (resume_process_id < 1 || resume_process_id > 127)
				{
					fprintf(stderr, "%s: Incorrect argument - value needs to be in the range of 1 to 127 inclusive.\n", args[0]);
					fprintf(stderr, "Try '%s --help' for more information\n", args[0]);
					return SYSERR;
				}

				resume(resume_process_id);
			}
			else
			{
				fprintf(stderr, "%s: Incorrect argument - value is not a postive number\n", args[0]);
				fprintf(stderr, "Try '%s --help' for more information\n", args[0]);
				return SYSERR;
			}
		}
		restore(mask);
	}

	// Check argument count
	else
	{
		fprintf(stderr, "%s: Incorrect number of arguments\n", args[0]);
		fprintf(stderr, "Try '%s --help' for more information\n", args[0]);
		return SYSERR;
	}

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
	kprintf("create priority & pid: [%d,%d]\n", priority, getpid());

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

	kprintf("sleep priority & pid: [%d,%d]\n", priority, getpid());

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
syscall run_after_wait(
	pri16 priority, /* Priority value for this process */
	sid32 semaphoreID)
{
	kprintf("wait priority & pid: [%d,%d]\n", priority, getpid());

	wait(semaphoreID);

	// kprintf("done waiting!\n");

	// do nothing at this time.

	return OK;
}

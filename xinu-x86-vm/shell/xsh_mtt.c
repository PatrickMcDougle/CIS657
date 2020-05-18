/* xsh_mtt.c - xsh_mtt1, xsh_mtt2, xsh_mtt3 */

#include <xinu.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * xsh_mtt1 - Creates a new process with a user specified priority 
 *   (or default) that starts running in an empty infinite loop after
 *   announcing its PID.
 *------------------------------------------------------------------------
 */
shellcmd xsh_mtt1(int nargs, char *args[])
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
 * xsh_mtt2 - This command creates a new process with a user specified
 *   priority (or default) that starts running in an empty infinite loop
 *   after announcing its PID after calling sleep () or sleepms () to
 *   sleep for 10 seconds.
 *------------------------------------------------------------------------
 */
shellcmd xsh_mtt2(int nargs, char *args[])
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
 * xsh_mtt3 - This shell command called “psready” will list the content
 *   of the ready list only.  Subset of info of the "ps" command
 *------------------------------------------------------------------------
 */
shellcmd xsh_mtt3(int nargs, char *args[])
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
 * xsh_mtt4 - This shell command called “wait” will create a new process
 *   that starts waiting on a semaphore that was created with the initial
 *   count of zero after announcing its PID.
 *------------------------------------------------------------------------
 */
shellcmd xsh_mtt4(int nargs, char *args[])
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

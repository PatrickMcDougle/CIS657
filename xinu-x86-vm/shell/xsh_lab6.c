/* xsh_lab6.c - xsh_kill */

#include <xinu.h>
#include <string.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * xsh_sendk - Sends a message to a specific process.
 *------------------------------------------------------------------------
 */
shellcmd xsh_sendk(int nargs, char *args[])
{
	pid32 pid;	 /* ID of process to kill			*/
	char ch;	 /* next character of argument		*/
	char *chptr; /* walks along argument string	*/
	int16 i;
	umsg32 msg;

	/* Output info for '--help' argument */

	if (nargs == 2 && strncmp(args[1], "--help", 7) == 0)
	{
		kprintf("Usage: %s PID <MSG>\n\n", args[0]);
		kprintf("Description:\n");
		kprintf("\tSends a message to a specific process.\n");
		kprintf("Options:\n");
		kprintf("\tPID   \tthe ID of a process to send the message too.\n");
		kprintf("\t<MSG> \tthe message or messages to send to the process.\n");
		kprintf("\t--help\tdisplay this help and exit\n");
		return SHELL_OK;
	}

	/* Check argument count */

	if (nargs < 3)
	{
		fprintf(stderr, "%s: Not enough arguments\n", args[0]);
		fprintf(stderr, "Try '%s --help' for more information\n", args[0]);
		return SHELL_ERROR;
	}

	// read in the first argument which should be the process ID
	// value for where the messages should go.
	chptr = args[1];
	ch = *chptr++;
	pid = 0;
	while (ch != NULLCH)
	{
		if ((ch < '0') || (ch > '9'))
		{
			fprintf(stderr, "%s: non-digit in process ID\n", args[0]);
			return SHELL_ERROR;
		}
		pid = 10 * pid + (ch - '0');
		ch = *chptr++;
	}
	if (pid == 0)
	{
		fprintf(stderr, "%s: cannot send a message to the null process\n", args[0]);
		return SHELL_ERROR;
	}

	// For each argument after the first two, process them as message numbers
	// that are 32 bit unsigned integers.
	for (i = 2; i < nargs; ++i)
	{
		chptr = args[i];
		ch = *chptr++;
		msg = 0;

		while (ch != NULLCH)
		{
			if ((ch < '0') || (ch > '9'))
			{
				fprintf(stderr, "%s: non-digit in process message\n", args[0]);
				return SHELL_ERROR;
			}
			msg = 10 * msg + (ch - '0');
			ch = *chptr++;
		}

		// kprintf(" [%d] %d:%u {%s}\n", i, pid, msg, args[i]);
		sendk(pid, msg);
	}

	return SHELL_OK;
}

/*------------------------------------------------------------------------
 * xsh_receivek - receive all messages that were sent to this process.
 *------------------------------------------------------------------------
 */
shellcmd xsh_receivek(int nargs, char *args[])
{
	/* Output info for '--help' argument */

	if (nargs == 2 && strncmp(args[1], "--help", 7) == 0)
	{
		kprintf("Usage: %s\n\n", args[0]);
		kprintf("Description:\n");
		kprintf("\tReads and prints out messages sent to it.\n");
		kprintf("Options:\n");
		kprintf("\t--help\tdisplay this help and exit\n");
		return SHELL_OK;
	}

	/* Check argument count */

	if (nargs != 1)
	{
		fprintf(stderr, "Try '%s --help' for more information\n", args[0]);
		return SHELL_ERROR;
	}

	// start a process that runs in the background waiting for messages to show up.
	resume(create(receiver, 1024, 20, "receiver", 0));

	return SHELL_OK;
}

/*------------------------------------------------------------------------
 * xsh_printmsgbuff - print the messagebuffer.
 *------------------------------------------------------------------------
 */
shellcmd xsh_printmsgbuff(int nargs, char *args[])
{
	int16 i;
	struct message *msgPtr;

	/* Output info for '--help' argument */

	if (nargs == 2 && strncmp(args[1], "--help", 7) == 0)
	{
		kprintf("Usage: %s\n\n", args[0]);
		kprintf("Description:\n");
		kprintf("\tPrint the contents of the message buffer..\n");
		kprintf("Options:\n");
		kprintf("\t--help\tdisplay this help and exit\n");
		return SHELL_OK;
	}

	/* Check argument count */

	if (nargs != 1)
	{
		fprintf(stderr, "Try '%s --help' for more information\n", args[0]);
		return SHELL_ERROR;
	}

	// just print out all the valid messages in the buffer.
	for (i = 0; i < MSG_BUF_SIZE; ++i)
	{
		msgPtr = &msgtab[i];
		if (msgtab[i].msgState == MSG_VALID)
		{
			kprintf(" [%d] %d|%d:%u\n", i, msgPtr->processId, msgPtr->msgState, msgPtr->msg);
		}
	}

	return SHELL_OK;
}

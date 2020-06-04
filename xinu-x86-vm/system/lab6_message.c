/* lab6_message.c - sendk, receivek */

#include <xinu.h>

struct message msgtab[MSG_BUF_SIZE];

/*------------------------------------------------------------------------
 *  sendk  -  pass a message to a process and start recipient if waiting
 *------------------------------------------------------------------------
 */
syscall sendk(
	pid32 pid, /* ID of recipient process	*/
	umsg32 msg /* contents of message		*/
)
{
	intmask mask;					  /* saved interrupt mask										*/
	struct procent *prptr;			  /* ptr to process' table entry								*/
	struct message *msgPtr = NULL;	  /* ptr to the last message entry for a process in the table	*/
	struct message *msgPtrNew = NULL; /* ptr to the new free message entry in the table				*/
	int16 i = 0;					  /* i is for increment 										*/

	mask = disable();
	if (isbadpid(pid))
	{
		kprintf("Process ID is not valid!  You can only send messages to currently running process.\n");
		restore(mask); /* restore interrupts */
		return SYSERR;
	}

	prptr = &proctab[pid];
	if ((prptr->prstate == PR_FREE))
	{
		kprintf("Process is in FREE state so it will not get your message!\n");
		restore(mask); /* restore interrupts */
		return SYSERR;
	}

	// Find the first available free space in the table.
	while (i < MSG_BUF_SIZE && msgPtrNew == NULL)
	{
		if (msgtab[i].msgState == MSG_FREE)
		{
			msgPtrNew = &msgtab[i];
		}
		++i;
	}

	if (i >= MSG_BUF_SIZE || msgPtrNew == NULL)
	{
		kprintf("Message Buffer Table is FULL.  Message is dropped and must be resent");
		restore(mask); /* restore interrupts */
		return SYSERR;
	}

	// load up the new message into the message table.
	msgPtrNew->msg = msg;
	msgPtrNew->msgNext = NULL;
	msgPtrNew->msgState = MSG_VALID;
	msgPtrNew->processId = pid;

	// Add the message to the correct process queue.
	if (prptr->prhasmsg == TRUE)
	{
		// There are already messages waiting for this process.
		// Find the last message for this process.
		// Add the message to the back of the queue.
		msgPtr = prptr->msgbuff;
		while (msgPtr->msgNext != NULL)
		{
			msgPtr = msgPtr->msgNext;
		}
		msgPtr->msgNext = msgPtrNew;
	}
	else
	{
		// This is the first message in the process queue.
		prptr->msgbuff = msgPtrNew;
	}

	prptr->prhasmsg = TRUE; /* indicate message(s) is waiting	*/

	/* If recipient waiting or in timed-wait make it ready	*/

	if (prptr->prstate == PR_RECV)
	{
		ready(pid, RESCHED_YES);
	}
	else if (prptr->prstate == PR_RECTIM)
	{
		unsleep(pid);
		ready(pid, RESCHED_YES);
	}

	restore(mask); /* restore interrupts */
	return OK;
}

/*------------------------------------------------------------------------
 *  receivek  -  wait for a message and return the message to the caller
 *------------------------------------------------------------------------
 */
umsg32 receivek(void)
{
	intmask mask;		   /* saved interrupt mask			*/
	struct procent *prptr; /* ptr to process' table entry	*/
	umsg32 msg;			   /* message to return				*/

	mask = disable();
	prptr = &proctab[currpid];
	if (prptr->prhasmsg == FALSE)
	{
		/* block until message arrives	*/
		prptr->prstate = PR_RECV;
		resched();
	}

	/* retrieve message from the message table buffer	*/
	msg = prptr->msgbuff->msg;
	prptr->msgbuff->msgState = MSG_FREE;
	prptr->msgbuff = prptr->msgbuff->msgNext;

	/* reset message flag								*/
	prptr->prhasmsg = (prptr->msgbuff == NULL) ? FALSE : TRUE;

	restore(mask);
	return msg;
}

/*------------------------------------------------------------------------
 *  receiver  -  An Infinite while loop to read messages sent to this process.
 *------------------------------------------------------------------------
 */
syscall receiver(void)
{
	while (TRUE)
	{
		kprintf("Message: %d\n", receivek());
	}

	return OK;
}
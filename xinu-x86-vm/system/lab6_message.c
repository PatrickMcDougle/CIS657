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
	intmask mask;		   /* saved interrupt mask			*/
	struct procent *prptr; /* ptr to process' table entry	*/

	mask = disable();
	if (isbadpid(pid))
	{
		restore(mask);
		return SYSERR;
	}

	prptr = &proctab[pid];
	if ((prptr->prstate == PR_FREE))
	{
		restore(mask);
		return SYSERR;
	}

	struct message *msgPtr = NULL;
	struct message *msgPtrNew = NULL;
	int16 i = 0;

	while (i < MSG_BUF_SIZE && msgPtrNew == NULL)
	{
		if (msgtab[i].msgState == MSG_FREE)
		{
			msgPtrNew = &msgtab[i];
		}
		++i;
	}

	msgPtrNew->msg = msg;
	msgPtrNew->msgNext = NULL;
	msgPtrNew->msgState = MSG_VALID;
	msgPtrNew->processId = pid;

	if (prptr->prhasmsg == TRUE)
	{
		msgPtr = prptr->msgbuff;
		while (msgPtr->msgNext != NULL)
		{
			msgPtr = msgPtr->msgNext;
		}
		msgPtr->msgNext = msgPtrNew;
	}
	else
	{
		prptr->msgbuff = msgPtrNew;
	}

	prptr->prhasmsg = TRUE; /* indicate message is waiting	*/

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

	/* retrieve message			*/
	msg = prptr->msgbuff->msg;
	prptr->msgbuff->msgState = MSG_FREE;
	prptr->msgbuff = prptr->msgbuff->msgNext;

	/* reset message flag		*/
	prptr->prhasmsg = (prptr->msgbuff == NULL) ? FALSE : TRUE;

	restore(mask);
	return msg;
}

syscall receiver(void)
{
	while (TRUE)
	{
		kprintf("Message: %d\n", receivek());
	}

	return OK;
}
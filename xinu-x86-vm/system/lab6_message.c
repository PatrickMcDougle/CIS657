/* lab6_message.c - sendk, receivek */
#include <xinu.h>

struct message msgtab[MSG_BUF_SIZE];

/*------------------------------------------------------------------------
 *  send  -  pass a message to a process and start recipient if waiting
 *------------------------------------------------------------------------
 */
syscall sendk(
    pid32 pid, /* ID of recipient process	*/
    umsg32 msg /* contents of message		*/
)
{
    intmask mask;          /* saved interrupt mask			*/
    struct procent *prptr; /* ptr to process' table entry	*/

    mask = disable();
    if (isbadpid(pid))
    {
        restore(mask);
        return SYSERR;
    }

    prptr = &proctab[pid];
    if ((prptr->prstate == PR_FREE) || prptr->prhasmsg)
    {
        restore(mask);
        return SYSERR;
    }
    prptr->prmsg = msg;     /* deliver message				*/
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

umsg32 receive(void)
{
    intmask mask;          /* saved interrupt mask			*/
    struct procent *prptr; /* ptr to process' table entry	*/
    umsg32 msg;            /* message to return				*/

    mask = disable();
    prptr = &proctab[currpid];
    if (prptr->prhasmsg == FALSE)
    {
        prptr->prstate = PR_RECV;
        resched(); /* block until message arrives			*/
    }
    msg = prptr->prmsg;      /* retrieve message			*/
    prptr->prhasmsg = FALSE; /* reset message flag			*/
    restore(mask);
    return msg;
}
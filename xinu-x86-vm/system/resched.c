/* resched.c - resched */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  resched  -  Reschedule processor to highest priority eligible process
 *------------------------------------------------------------------------
 */
void resched(void) /* assumes interrupts are disabled	*/
{
	struct procent *ptold; /* ptr to table entry for old process	*/
	struct procent *ptnew; /* ptr to table entry for new process	*/

	/* If rescheduling is deferred, record attempt and return */

	if (Defer.ndefers > 0)
	{
		Defer.attempt = TRUE;
		return;
	}

	/* Point to process table entry for the current (old) process */

	ptold = &proctab[currpid];

	// update currint process last touch value.
	ptold->last_touched++;

	if (ptold->prstate == PR_CURR)
	{ /* process remains running */
		if (ptold->prprio > firstkey(readylist))
		{
			return;
		}

		/* Old process will no longer remain current */

		ptold->prstate = PR_READY;
		insert(currpid, readylist, ptold->prprio);
	}

	// check if any last touched values are lower than 100 from current process.
	int32 i = 0;
	for (i = 3; i < NPROC; i++)
	{
		ptnew = &proctab[i];
		if (ptnew->prstate != PR_FREE && ptold->prprio > ptnew->prprio && ptold->last_touched > (ptnew->last_touched + 10))
		{
			if (ptold->prprio - ptnew->prprio == 1)
			{
				ptnew->prprio = ptold->prprio;
			}
			else
			{
				ptnew->prprio = ptnew->prprio + (ptold->prprio - ptnew->prprio) / 2;
			}
		}
	}

	/* Force context switch to highest priority ready process */

	currpid = dequeue(readylist);
	ptnew = &proctab[currpid];
	ptnew->prstate = PR_CURR;
	preempt = QUANTUM; /* reset time slice for process	*/
	ctxsw(&ptold->prstkptr, &ptnew->prstkptr);

	/* Old process returns here when resumed */

	return;
}

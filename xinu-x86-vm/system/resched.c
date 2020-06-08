/* resched.c - resched */

#include <xinu.h>
#include <string.h>

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

	// the orignal process is still in the current state.
	if (ptold->prstate == PR_CURR)
	{
		/* process remains running */
		if (ptold->prprio > firstkey(readylist))
		{
			// update current process last touch value to current clock time.
			ptold->last_touched = clktime;

			// original (ptold) process is still the highest process to run.
			// so just keep running it.
			return;
		}

		/* Old process will no longer remain current */

		ptold->prstate = PR_READY;
		insert(currpid, readylist, ptold->prprio);
	}

	// check if any last touched values are lower than 100 from current process.
	int32 i = 0;
	pri16 max_priority = 0;

	for (i = 3; i < NPROC; i++)
	{
		if (i == currpid)
		{
			// no need to check itself.
			continue;
		}

		ptnew = &proctab[i];

		if (ptnew->prstate == PR_FREE)
		{
			// if the state is free then there is nothing to see here.
			continue;
		}
		if (ptnew->prstate != PR_READY && ptnew->prstate != PR_CURR)
		{
			// if the state is free then there is nothing to see here.
			continue;
		}
		if (ptold->prprio < ptnew->prprio)
		{
			// the pritority of the old process is lower then the process
			// we are checking.  No need to do anything else.
			continue;
		}
		if (strncmp(ptnew->prname, "TMNT", 4) == 0)
		{
			// don't check the fun output stuff for this assignment
			continue;
		}
		if (strncmp(ptnew->prname, "tmnt", 4) == 0)
		{
			// don't check the fun output stuff for this assignment
			continue;
		}

		if (max_priority < ptnew->prprio)
		{
			max_priority = ptnew->prprio;
		}
	}

	for (i = 3; i < NPROC; i++)
	{
		if (i == currpid)
		{
			// no need to check itself.
			continue;
		}

		ptnew = &proctab[i];

		if (ptnew->prstate == PR_FREE)
		{
			// if the state is free then there is nothing to see here.
			continue;
		}
		if (ptnew->prstate != PR_READY && ptnew->prstate != PR_CURR)
		{
			// if the state is free then there is nothing to see here.
			continue;
		}
		if (max_priority < ptnew->prprio)
		{
			// the pritority of the old process is lower then the process
			// we are checking.  No need to do anything else.
			continue;
		}
		if (strncmp(ptnew->prname, "TMNT", 4) == 0)
		{
			// don't check the fun output stuff for this assignment
			continue;
		}
		if (strncmp(ptnew->prname, "tmnt", 4) == 0)
		{
			// don't check the fun output stuff for this assignment
			continue;
		}

		if (ptnew->last_touched >= max_priority && ptnew->last_touched < 0)
		{
			continue;
		}

		if (clktime > ptnew->last_touched + 100)
		{
			if (max_priority > ptnew->prprio + 1)
			{
				ptnew->prprio = ptnew->prprio + 1;
				queuetab[i].qkey = ptnew->prprio;
			}
			// // this proces's last used time is older enough.
			// if (max_priority - ptnew->prprio == 1)
			// {
			// 	// the priority is just one away from the max,
			// 	// so just update it to the max
			// 	ptnew->prprio = max_priority;
			// }
			// else
			// {
			// 	int32 percentage = (100 - (((int)clktime) - ((int)ptnew->last_touched))) / 10;
			// 	if (percentage > 1)
			// 	{
			// 		ptnew->prprio = ptnew->prprio + (((max_priority - ptnew->prprio) / 2) / percentage);
			// 	}
			// }
		}
	}

	/* Force context switch to highest priority ready process */

	currpid = dequeue(readylist);
	ptnew = &proctab[currpid];
	ptnew->prstate = PR_CURR;

	// update new process last touch value to current clock time.
	ptnew->last_touched = clktime;

	preempt = QUANTUM; /* reset time slice for process	*/
	ctxsw(&ptold->prstkptr, &ptnew->prstkptr);

	/* Old process returns here when resumed */

	return;
}

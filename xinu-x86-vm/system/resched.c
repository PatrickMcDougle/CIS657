/* resched.c - resched */

#include <xinu.h>
#include <string.h>

// function declearation for local only use functions.
void _resched_q1(uint8, uint8);

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
			if (g_starvation_setting == STARVE_Q1_CONTEXT_SWITCH)
			{
				// update current process last touch value to current clock time.
				ptold->last_touched = clktime;
			}

			// original (ptold) process is still the highest process to run.
			// so just keep running it.
			return;
		}

		/* Old process will no longer remain current */

		ptold->prstate = PR_READY;
		insert(currpid, readylist, ptold->prprio);
	}

	if (g_starvation_setting == STARVE_Q1_CONTEXT_SWITCH)
	{
		_resched_q1(g_starvation_seconds, g_starvation_increment);
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

/*------------------------------------------------------------------------
 *  _resched_q1  -  Prevent Starvation of process that have low priority 
 * 					by upping thier prioroty.
 *------------------------------------------------------------------------
 */
void _resched_q1(
	uint8 starvation_seconds, /* The time to allow starvation to heppen before doing something. */
	uint8 starvation_increment /* Increment the starving process by this ammount. */
)
{
	// check if any last touched values are lower than 100 from current process.
	int32 i = 0;
	pri16 max_priority = 0;
	pri16 min_priority = 16000; // almost half the falue.
	struct procent *process_pt; /* ptr to table entry for checking the process	*/

	for (i = 3; i < NPROC; i++)
	{
		if (i == currpid)
		{
			// no need to check itself.
			continue;
		}

		process_pt = &proctab[i];

		if (process_pt->prstate == PR_FREE)
		{
			// if the state is free then there is nothing to see here.
			continue;
		}
		if (process_pt->prstate != PR_READY && process_pt->prstate != PR_CURR)
		{
			// if the state is not ready and not current then there is
			// nothing to see here.
			continue;
		}
		// if (proctab[currpid].prprio < process_pt->prprio)
		// {
		// 	// the pritority of the old process is lower then the process
		// 	// we are checking.  No need to do anything else.
		// 	continue;
		// }
		if (clktime > process_pt->last_touched + starvation_seconds)
		{
			// if the process is starving, we don't need to continue.
			continue;
		}

		// need to make sure not to include the presentation system calls.
		// ---------------------------------------------------------------
		if (strncmp(process_pt->prname, "TMNT", 4) == 0)
		{
			// don't check the fun output stuff for this assignment
			continue;
		}
		if (strncmp(process_pt->prname, "tmnt", 4) == 0)
		{
			// don't check the fun output stuff for this assignment
			continue;
		}

		// lets find the highest priority
		if (max_priority < process_pt->prprio)
		{
			max_priority = process_pt->prprio;
		}

		// lets find the lowest priority
		if (min_priority > process_pt->prprio)
		{
			min_priority = process_pt->prprio;
		}
	}

	for (i = 3; i < NPROC; i++)
	{
		if (i == currpid)
		{
			// no need to check itself.
			continue;
		}

		process_pt = &proctab[i];

		if (process_pt->prstate == PR_FREE)
		{
			// if the state is free then there is nothing to see here.
			continue;
		}
		if (process_pt->prstate != PR_READY && process_pt->prstate != PR_CURR)
		{
			// if the state is free then there is nothing to see here.
			continue;
		}
		if (max_priority < process_pt->prprio)
		{
			// the pritority of the old process is lower then the process
			// we are checking.  No need to do anything else.
			continue;
		}

		// need to make sure not to include the presentation system calls.
		// ---------------------------------------------------------------
		if (strncmp(process_pt->prname, "TMNT", 4) == 0)
		{
			// don't check the fun output stuff for this assignment
			continue;
		}
		if (strncmp(process_pt->prname, "tmnt", 4) == 0)
		{
			// don't check the fun output stuff for this assignment
			continue;
		}

		if (clktime > process_pt->last_touched + starvation_seconds)
		{
			if (min_priority > process_pt->prprio + starvation_increment)
			{
				// if the process's priority is lower than the increment value
				// from min, then update.
				process_pt->prprio = process_pt->prprio + starvation_increment;
				queuetab[i].qkey = process_pt->prprio;
			}
			else if (min_priority > process_pt->prprio)
			{
				// if the process's priority is between min value and increment
				// value, then increamnet by one.
				// This will prevent other processes from getting into a starving situation.
				process_pt->prprio = min_priority;
				queuetab[i].qkey = min_priority;
			}
			// // this proces's last used time is older enough.
			// if (max_priority  > process_pt->prprio + starvation_increment)
			// {
			// 	// the priority is just one away from the max,
			// 	// so just update it to the max
			// 	process_pt->prprio = max_priority;
			// }
			// else
			// {
			// 	int32 percentage = (100 - (((int)clktime) - ((int)process_pt->last_touched))) / 10;
			// 	if (percentage > 1)
			// 	{
			// 		process_pt->prprio = process_pt->prprio + (((max_priority - process_pt->prprio) / 2) / percentage);
			// 	}
			// }
		}
	}
}
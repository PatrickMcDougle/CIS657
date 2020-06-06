/* finalexam.c - rocksteady, bebop */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  rocksteady  -  a Rhino Mutant for the footclan
 *------------------------------------------------------------------------
 */
syscall rocksteady(
    sid32 mySID,     /* ID of the semaphore between processes */
    sid32 partnerSID /* ID of the semaphore between processes */
)
{
	kprintf("A1 - My first statement appears before Bob's second statement.\n");
	signal(mySID);
	wait(partnerSID);
	kprintf("A2 - This is Alice's second statement.\n");
    return OK;
}

/*------------------------------------------------------------------------
 *  bebop  -  a Warthog Mutant for the footclan
 *------------------------------------------------------------------------
 */
syscall bebop(
    sid32 mySID, /* ID of the semaphore between processes */
    sid32 partnerSID /* ID of the semaphore between processes */)
{
	kprintf("B1 - My first statement appears before Alice's second statement.\n");
	signal(mySID);
	wait(partnerSID);
	kprintf("B2 - This is Bob's second statement.\n");

    return OK;
}

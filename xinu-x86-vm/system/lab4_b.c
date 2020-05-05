/* lab4_b.c - b */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  b  -  Process prints out's B responses.
 *------------------------------------------------------------------------
 */
syscall b(
	sid32 mySID,     /* ID of the semaphore between processes */
	sid32 partnerSID /* ID of the semaphore between processes */
)
{

	// wait(mySID);
	kprintf("B1 - My first statement appears before Alice's second statement.\n");
	signal(mySID);
	wait(partnerSID);
	kprintf("B2 - This is Bob's second statement.\n");

	return OK;
}

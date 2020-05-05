/* lab4_a.c - a */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  a  -  Process prints out's A responses.
 *------------------------------------------------------------------------
 */
syscall a(
	sid32 mySID,	 /* ID of the semaphore between processes */
	sid32 partnerSID /* ID of the semaphore between processes */
)
{
	kprintf("A1 - My first statement appears before Bob's second statement.\n");
	signal(mySID);
	wait(partnerSID);
	kprintf("A2 - This is Alice's second statement.\n");

	return OK;
}

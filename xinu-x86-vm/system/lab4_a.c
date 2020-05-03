/* lab4_a.c - a */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  a  -  Process prints out's A responses.
 *------------------------------------------------------------------------
 */
syscall a(
    sid32 mySID,     /* ID of the semaphore between processes */
    sid32 partnerSID /* ID of the semaphore between processes */
)
{

    kprintf("My first statement appears before Bob's second statement.\n");
    signal(mySID);
    wait(partnerSID);
    kprintf("This is Alice's second statement.\n");

    return OK;
}

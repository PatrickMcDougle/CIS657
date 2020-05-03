/* lab4_b.c - b */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  b  -  Process prints out's B responses.
 *------------------------------------------------------------------------
 */
syscall b(
    sid32 semaphoreID /* ID of the semaphore between processes */
)
{
    intmask mask;          /* saved interrupt mask		*/
    struct dentry *devptr; /* entry in device switch table	*/
    int32 retval;          /* value to return to caller	*/

    mask = disable();
    if (isbaddev(descrp))
    {
        restore(mask);
        return SYSERR;
    }
    devptr = (struct dentry *)&devtab[descrp];
    retval = (*devptr->dvopen)(devptr, name, mode);
    restore(mask);
    return retval;
}

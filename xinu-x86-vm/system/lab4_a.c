/* lab4_a.c - a */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  a  -  Process prints out's A responses.
 *------------------------------------------------------------------------
 */
syscall a(
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

/* xsh_lab4.c - xsh_lab4 */

#include <xinu.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * xsh_lab4 - This shows off the synchronization problem called a 
 *      Rendezvous.
 *------------------------------------------------------------------------
 */
shellcmd xsh_lab4(int nargs, char *args[])
{
    sid32 partnerAsSID;
    sid32 partnerBsSID;

    partnerAsSID = semcreate(1);
    partnerBsSID = semcreate(0);

    resume(create(a, 1024, 12, "lab4 A", 2, partnerAsSID, partnerBsSID));
    resume(create(b, 1024, 11, "lab4 B", 2, partnerBsSID, partnerAsSID));

    printf("\n");

    return SHELL_OK;
}

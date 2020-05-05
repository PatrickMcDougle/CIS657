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

	partnerAsSID = semcreate(0);
	partnerBsSID = semcreate(0);

	pid32 aPID = create(a, 1024, 100, "lab4 A", 2, partnerAsSID, partnerBsSID);
	pid32 bPID = create(b, 1024, 101, "lab4 B", 2, partnerBsSID, partnerAsSID);

	resume(bPID);
	resume(aPID);

	printf("\n");

	return SHELL_OK;
}

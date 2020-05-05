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

	int32 pri1;
	int32 pri2;
	pri1 = 100 + (rand() % 10);
	do
	{
		pri2 = 100 + (rand() % 10);
	} while (pri1 == pri2); // while loop makes sure the priorites are not the same.

	char lab1[20];
	char lab2[20];

	sprintf(lab1, "lab4 A - %d", pri1);
	sprintf(lab2, "lab4 B - %d", pri2);

	// kprintf(lab1);
	// printf("\n");
	// kprintf(lab2);
	// printf("\n");

	pid32 aPID = create(a, 1024, pri1, lab1, 2, partnerAsSID, partnerBsSID);
	pid32 bPID = create(b, 1024, pri2, lab2, 2, partnerBsSID, partnerAsSID);

	if (rand() % 2 == 0)
	{
		resume(bPID);
		resume(aPID);
	}
	else
	{
		resume(aPID);
		resume(bPID);
	}

	printf("\n");

	return SHELL_OK;
}

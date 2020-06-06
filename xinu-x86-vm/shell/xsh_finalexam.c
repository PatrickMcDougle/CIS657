/* xsh_finalexam.c - xsh_footclan */

#include <xinu.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * xsh_footclan - Starts two processes with high priority.
 *------------------------------------------------------------------------
 */
shellcmd xsh_footclan(int nargs, char *args[])
{
	int32 i; /* walks through args array	*/
	sid32 sidRocksteady;
	sid32 sidBebop;
	pri16 priority = 35;

	sidRocksteady = semcreate(0);
	sidBebop = semcreate(0);

	pid32 aPID = create(rocksteady, 1024, priority + 5, "Rocksteady", 2, sidRocksteady, sidBebop);
	pid32 bPID = create(bebop, 1024, priority, "Bebop", 2, sidBebop, sidRocksteady);

	return SHELL_OK;
}

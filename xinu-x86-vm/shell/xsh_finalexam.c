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
	pri16 priority = 50;

	pid32 pidRocksteady;
	pid32 pidBebop;
	pid32 pidShredder;
	pid32 pidKrang;
	pid32 pidFootsoldier;
	pid32 pidTurtle;

	sidRocksteady = semcreate(0);
	sidBebop = semcreate(0);

	// // generate two process priority values that are different from each other.
	// int32 pri1;
	// int32 pri2;
	// pri1 = 100 + (rand() % 10);
	// do
	// {
	// 	pri2 = 100 + (rand() % 10);
	// } while (pri1 == pri2); // while loop makes sure the priorites are not the same.

	pidTurtle = create(turtle, 1024, priority - 30, "Turtles", 0);
	resume(pidTurtle);

	pidShredder = create(shredder, 1024, priority + 15, "Shredder", 0);
	pidKrang = create(krang, 1024, priority + 5, "Krang", 0);
	pidRocksteady = create(rocksteady, 1024, priority, "Rocksteady", 0);
	pidBebop = create(bebop, 1024, priority - 5, "Bebop", 0);
	resume(pidShredder);
	resume(pidKrang);
	resume(pidRocksteady);
	resume(pidBebop);

	for (i = 0; i < 10; ++i)
	{
		pidFootsoldier = create(footsoldier, 1024, priority - 10, "footsoldier", 0);
		resume(pidFootsoldier);
	}

	return SHELL_OK;
}

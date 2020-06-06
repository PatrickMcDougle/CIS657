/* xsh_finalexam.c - xsh_footclan */

#include <xinu.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * xsh_footclan - Starts two processes with high priority.
 *------------------------------------------------------------------------
 */
shellcmd xsh_footclan(int nargs, char *args[])
{
	int32	i;			/* walks through args array	*/

	if (nargs > 1) {
		printf("%s", args[1]);

		for (i = 2; i < nargs; i++) {
			printf(" %s", args[i]);
		}
	}
	printf("\n");

	return 0;
}

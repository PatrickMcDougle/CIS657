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

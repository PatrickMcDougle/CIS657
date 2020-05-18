/* xsh_mtt.c - xsh_mtt1, xsh_mtt2, xsh_mtt3 */

#include <xinu.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * xsh_mtt1 - ??
 *------------------------------------------------------------------------
 */
shellcmd xsh_mtt1(int nargs, char *args[])
{
	int32 i; /* walks through args array	*/

	if (nargs > 1)
	{
		printf("%s", args[1]);

		for (i = 2; i < nargs; i++)
		{
			printf(" %s", args[i]);
		}
	}
	printf("\n");

	return 0;
}

/*------------------------------------------------------------------------
 * xsh_mtt2 - ??
 *------------------------------------------------------------------------
 */
shellcmd xsh_mtt2(int nargs, char *args[])
{
	int32 i; /* walks through args array	*/

	if (nargs > 1)
	{
		printf("%s", args[1]);

		for (i = 2; i < nargs; i++)
		{
			printf(" %s", args[i]);
		}
	}
	printf("\n");

	return 0;
}

/*------------------------------------------------------------------------
 * xsh_mtt3 - ??
 *------------------------------------------------------------------------
 */
shellcmd xsh_mtt3(int nargs, char *args[])
{
	int32 i; /* walks through args array	*/

	if (nargs > 1)
	{
		printf("%s", args[1]);

		for (i = 2; i < nargs; i++)
		{
			printf(" %s", args[i]);
		}
	}
	printf("\n");

	return 0;
}

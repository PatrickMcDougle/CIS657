/* xsh_echo.c - xsh_echo, xsh_eecho */

#include <xinu.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * xhs_echo - write argument strings to stdout
 *------------------------------------------------------------------------
 */
shellcmd xsh_echo(int nargs, char *args[])
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
 * xhs_eecho - write argument strings to stdout after encryption
 *------------------------------------------------------------------------
 */
shellcmd xsh_eecho(int nargs, char *args[])
{
	int32 i; /* walks through args array	*/

	for (i = 1; i < nargs; i++)
	{
		printf("%s ", args[i]);
	}
	printf("\n");

	return 0;
}
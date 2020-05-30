/* xsh_echo.c - xsh_echo, xsh_eecho, xsh_vecho */

#include <xinu.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * xhs_echo - write argument strings to stdout
 *------------------------------------------------------------------------
 */
shellcmd xsh_echo(int nargs, char *args[])
{
	int32 i; /* walks through args array	*/

	for (i = 1; i < nargs; i++)
	{
		printf("%s ", args[i]);
	}
	printf("\n");

	return SHELL_OK;
}

/*------------------------------------------------------------------------
 * xhs_eecho - Encryption args before calling echo.  Verbose mode OFF.
 *------------------------------------------------------------------------
 */
shellcmd xsh_eecho(int nargs, char *args[])
{
	int32 i; /* walks through args array	*/

	if (g_enigma_rotors != NULL)
	{
		for (i = 1; i < nargs; i++)
		{
			encryptf(args[i], FALSE);
		}
	}

	return xsh_echo(nargs, args);
}

/*------------------------------------------------------------------------
 * xsh_vecho - Encryption args before calling echo.  Verbose mode ON.
 *------------------------------------------------------------------------
 */
shellcmd xsh_vecho(int nargs, char *args[])
{
	int32 i; /* walks through args array	*/

	if (g_enigma_rotors != NULL)
	{
		for (i = 1; i < nargs; i++)
		{
			encryptf(args[i], TRUE);
		}
	}

	return xsh_echo(nargs, args);
}
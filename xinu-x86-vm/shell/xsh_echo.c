/* xsh_echo.c - xsh_echo, xsh_eecho */

#include <xinu.h>
#include <stdio.h>

syscall encryptf(char *);

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

	if (g_enigma_rotors != NULL)
	{
		printf("Enigma Set: ");
		for (i = 1; i < nargs; i++)
		{
			encryptf(args[i]);
		}
	}
	else
	{
		for (i = 1; i < nargs; i++)
		{
			printf("%s ", args[i]);
		}
	}
	printf("\n");

	return 0;
}

syscall encryptf(char *string)
{
	char ch;
	ch = *string++;
	int32 val = 0;
	int32 i_val = 0;
	int32 i;
	int32 j;
	while (ch != NULL)
	{
		i_val = 0;
		while (g_enigma_encrypt_chars[i_val] != NULL && g_enigma_encrypt_chars[i_val] != ch)
		{
			++i_val;
		}

		// TODO: @Patrick need to update this at some later time.
		if (g_enigma_encrypt_chars[i_val] != ch)
		{
			kprintf("DID NOT FIND CHAR [%d/%d:%c!=%c]\n",i_val,g_enigma_encrypt_char_count, ch,g_enigma_encrypt_chars[i]);
			return SYSERR;
		}
		val = i_val;
		for (i = 0; i < g_enigma_rotor_count; ++i)
		{
			val = g_enigma_rotors[i * g_enigma_encrypt_char_count + val];
		}
		for (i = g_enigma_rotor_count - 2; i >= 0; --i)
		{
			val = g_enigma_rotors[i * g_enigma_encrypt_char_count + val];
		}

		printf("%c", g_enigma_encrypt_chars[val]);
		ch = *string++;
	}

	return OK;
}
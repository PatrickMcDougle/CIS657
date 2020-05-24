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
		printf("ENIGMA:\n");
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

	for (i = 0; i < g_enigma_rotor_count; ++i)
	{
		printf(" [%d] %d %d %d %d", i,
			   g_enigma_rotor_settings[i].type,
			   g_enigma_rotor_settings[i].shift,
			   g_enigma_rotor_settings[i].start,
			   g_enigma_rotor_settings[i].position);
	}
	printf("\n");

	while (ch != NULL)
	{
		// first find the char in the list to get it's place value.
		i_val = 0;
		while (g_enigma_encrypt_chars[i_val] != NULL && g_enigma_encrypt_chars[i_val] != ch)
		{
			++i_val;
		}
		printf(" [%c:%d]", ch, i_val);

		// TODO: @Patrick need to update this at some later time.
		if (g_enigma_encrypt_chars[i_val] != ch)
		{
			kprintf("DID NOT FIND CHAR [%d/%d:%c!=%c]\n", i_val, g_enigma_encrypt_char_count, ch, g_enigma_encrypt_chars[i_val]);
			return SYSERR;
		}

		val = i_val;
		for (i = 0; i < g_enigma_rotor_count; ++i)
		{
			val = val + g_enigma_rotor_settings[i].position;
			if (val > g_enigma_encrypt_char_count)
			{
				val -= g_enigma_encrypt_char_count;
			}
			val = g_enigma_rotors[i * g_enigma_encrypt_char_count + val].forword;
			val = val - g_enigma_rotor_settings[i].position;
			if (val < 0)
			{
				val += g_enigma_encrypt_char_count;
			}
			printf(" %d", val);
		}

		val = g_enigma_reflector[val];
		printf(" |%d|", val);

		for (i = g_enigma_rotor_count - 1; i >= 0; --i)
		{
			val = val + g_enigma_rotor_settings[i].position;
			if (val > g_enigma_encrypt_char_count)
			{
				val -= g_enigma_encrypt_char_count;
			}
			val = g_enigma_rotors[i * g_enigma_encrypt_char_count + val].backward;
			val = val - g_enigma_rotor_settings[i].position;
			if (val < 0)
			{
				val += g_enigma_encrypt_char_count;
			}
			printf(" %d", val);
		}
		printf(" = ");

		/* all done encoding/decoding now to update rotor positions. */
		i = g_enigma_rotor_count - 1;
		j = 1;
		while (i >= 0)
		{
			if (j == 1)
			{
				g_enigma_rotor_settings[i].position = (g_enigma_rotor_settings[i].position + j) % g_enigma_encrypt_char_count;

				if (g_enigma_rotor_settings[i].position == 0)
				{
					j = 1;
				}
				else
				{
					j = 0;
				}
			}
			--i;
		}

		printf("%c", g_enigma_encrypt_chars[val]);
		printf("\n");
		ch = *string++;
	}

	for (i = 0; i < g_enigma_rotor_count; ++i)
	{
		printf(" [%d] %d %d %d %d", i,
			   g_enigma_rotor_settings[i].type,
			   g_enigma_rotor_settings[i].shift,
			   g_enigma_rotor_settings[i].start,
			   g_enigma_rotor_settings[i].position);
	}
	printf("\n");

	return OK;
}
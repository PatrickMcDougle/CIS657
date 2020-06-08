/* encryptf.c - encryptf */

#include <xinu.h>
#include <stdio.h>
#include <encryptf.h>

/*------------------------------------------------------------------------
 * encryptf - Takes in a string and encrypts that string.
 *------------------------------------------------------------------------
 */
syscall encryptf(char *inputstring, bool8 verbose)
{
	char *string = inputstring;
	char ch = *string++;
	int32 val = 0;
	int32 i_val = 0;
	int32 i;
	int32 p = 0;

	if (verbose == TRUE)
	{
		// code for show and debug.
		for (i = 0; i < g_enigma_rotor_count; ++i)
		{
			kprintf(" [%d] %d %d %d %d", i,
					g_enigma_rotor_settings[i].type,
					g_enigma_rotor_settings[i].shift,
					g_enigma_rotor_settings[i].start,
					g_enigma_rotor_settings[i].position);
		}
		kprintf("\n");
	}

	while (ch != NULL && p < 200)
	{
		// first find the char in the list to get it's place value.

		val = i_val = findCharInEncryptionList(ch, verbose);

		if (isEncryptionValueGood(val))
		{
			val = encryptForwardRotors(val, verbose);

			val = encryptReflector(val, verbose);

			val = encryptBackwardRotors(val, verbose);

			updateRotors();

			if (verbose == TRUE)
			{
				kprintf(" = %c\n", g_enigma_encrypt_chars[val]);
			}

			inputstring[p] = (char)g_enigma_encrypt_chars[val];
		}

		++p;
		ch = *string++;
	}

	if (verbose == TRUE)
	{
		// code for show and debug.
		for (i = 0; i < g_enigma_rotor_count; ++i)
		{
			kprintf(" [%d] %d %d %d %d", i,
					g_enigma_rotor_settings[i].type,
					g_enigma_rotor_settings[i].shift,
					g_enigma_rotor_settings[i].start,
					g_enigma_rotor_settings[i].position);
		}
		kprintf("\n");
	}

	return OK;
}

void updateRotors()
{
	int16 i = 0;
	int16 j = 1;
	/* all done encoding/decoding now to update rotor positions. */

	for (i = g_enigma_rotor_count - 1; i >= 0; --i)
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
	}
}

int32 encryptForwardRotors(int32 val, bool8 verbose)
{
	int16 i;

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

		if (verbose == TRUE)
		{
			kprintf(" %2d", val);
		}
	}

	return val;
}
int32 encryptBackwardRotors(int32 val, bool8 verbose)
{
	int16 i;

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

		if (verbose == TRUE)
		{
			kprintf(" %2d", val);
		}
	}

	return val;
}

int32 encryptReflector(int32 val, bool8 verbose)
{
	val = g_enigma_reflector[val];

	if (verbose == TRUE)
	{
		kprintf(" |%2d|", val);
	}

	return val;
}

int32 findCharInEncryptionList(char ch, bool8 verbose)
{
	int32 i = 0;
	while (g_enigma_encrypt_chars[i] != NULL && g_enigma_encrypt_chars[i] != ch)
	{
		++i;
	}

	// TODO: @Patrick need to update this at some later time.
	if (g_enigma_encrypt_chars[i] != ch)
	{
		if (verbose == TRUE)
		{
			kprintf("DID NOT FIND CHAR [%d/%d:%c!=%c]\n", i, g_enigma_encrypt_char_count, ch, g_enigma_encrypt_chars[i]);
		}
		return -1;
	}

	if (verbose == TRUE)
	{
		kprintf(" [%c:%2d]", ch, i);
	}

	return i;
}
/* xsh_lock.c - xsh_lock */

#include <xinu.h>
#include <string.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * xsh_lock - This command locks the system so that everything is encrypted.
 *------------------------------------------------------------------------
 */
shellcmd xsh_lock(int nargs, char *args[])
{
	bool16 encryptInputSettings = 0;

	/* Check argument count */

	if (nargs < 2)
	{
		fprintf(stderr, "%s: incorrect argument\n", args[0]);
		fprintf(stderr, "Try '%s --help' for more information\n", args[0]);
		return SYSERR;
	}

	/* Output info for '--help' argument */

	if (nargs == 2 && strncmp(args[1], "--help", 7) == 0)
	{
		printf("Usage: %s (-u | -l [-c] [-C] [-n] [-w] [-s] [-i]) [-v] <RS>+ \n\n", args[0]);
		printf("Description:\n");
		printf("\tEncrypts the system.  Echo and print are all encrypted.\n");
		printf("Options:\n");
		printf("\t-c \tLower Case: \tEncrypt lower case characters.\n");
		printf("\t-C \tUpper Case: \tEncrypt upper case characters.\n");
		printf("\t-i \tReset: \tReset the rotor values to the inital start position.\n");
		printf("\t-l \tLock: \t\tTurns on the encryption.  Need one other flag.\n");
		printf("\t-n \tNumbers: \tEncrypt number characters.\n");
		printf("\t-s \tSymbols: \tEncrypt symbols found on the keyboard.\n");
		printf("\t-u \tUnlock: \tTurns off the encryption.\n");
		printf("\t-v \tVerbose: \tTurns on verbose mode and print stuff to standard output.\n");
		printf("\t-w \tWhite Space: \tEncrypt white space, like newline, tab, space.\n");
		printf("\t<RS> \tRotorSettings: \t<Sch>,<Mix>,<Srt>. need at least one (1) set and no more then nine (9) sets.\n");
		printf("\t<Sch> \tSchema: \tValue between 1-5.  This value defines the encryption schema of the Rotor.\n");
		printf("\t<Mix> \tMixer:  \tValue between 0-255.  This value defines the Mix value for the schema.\n");
		printf("\t<Srt> \tStart:  \tValue between 0-255.  This value defines the starting position of the Rotor.\n");
		printf("\t--help \t\t\tDisplay this help and exit\n");
		return OK;
	}

	encryptInputSettings = parse_inputs(nargs, args);

	if (shouldEncryptVerbose(encryptInputSettings))
	{
		print_input_settings(encryptInputSettings);
		print_rotor_settings();
		print_global_settings();
	}

	int16 i = 0;
	int16 j = 0;
	int16 k = 0;
	int16 reflector_shift = 0;
	int32 rotor_set = 0;

	if (shouldEncryptUnlock(encryptInputSettings))
	{
		if (g_enigma_rotors != NULL)
		{
			freemem((char *)g_enigma_rotors, g_enigma_rotor_count * g_enigma_encrypt_char_count * sizeof(struct rotor_vals));
			freemem(g_enigma_encrypt_chars, g_enigma_encrypt_char_count * sizeof(char));
		}
		g_enigma_rotor_count = 0;
		g_enigma_encrypt_char_count = 0;
	}

	else if (shouldEncryptResetRotorsPositions(encryptInputSettings))
	{
		for (j = 0; j < g_enigma_rotor_count; ++j)
		{
			g_enigma_rotor_settings[j].position = g_enigma_rotor_settings[j].start;
		}
	}

	else if (shouldEncryptLock(encryptInputSettings))
	{
		// allocate memory for the rotor array.
		g_enigma_encrypt_chars = getmem(g_enigma_encrypt_char_count * sizeof(char));
		g_enigma_rotors = (struct rotor_vals *)getmem(g_enigma_rotor_count * g_enigma_encrypt_char_count * sizeof(struct rotor_vals));
		g_enigma_reflector = getmem(g_enigma_encrypt_char_count * sizeof(char));

		// setup Rotor values:  Translate rotor int value into char values.

		i = 0;
		if (i < g_enigma_encrypt_char_count && shouldEncryptNumbers(encryptInputSettings))
		{
			++reflector_shift;
			for (k = 0; k < NUM_NUMS; ++k)
			{
				g_enigma_encrypt_chars[i + k] = '0' + k;
			}
			i += NUM_NUMS;
		}
		if (i < g_enigma_encrypt_char_count && shouldEncryptLowerCase(encryptInputSettings))
		{
			++reflector_shift;
			for (k = 0; k < NUM_CHARS; ++k)
			{
				g_enigma_encrypt_chars[i + k] = 'a' + k;
			}
			i += NUM_CHARS;
		}
		if (i < g_enigma_encrypt_char_count && shouldEncryptUpperCase(encryptInputSettings))
		{
			++reflector_shift;
			for (k = 0; k < NUM_CHARS; ++k)
			{
				g_enigma_encrypt_chars[i + k] = 'A' + k;
			}
			i += NUM_CHARS;
		}
		if (i < g_enigma_encrypt_char_count && shouldEncryptWhiteSpace(encryptInputSettings))
		{
			++reflector_shift;
			g_enigma_encrypt_chars[i++] = ' ';
			g_enigma_encrypt_chars[i++] = '\n';
			g_enigma_encrypt_chars[i++] = '\t';
		}
		if (i < g_enigma_encrypt_char_count && shouldEncryptSymbols(encryptInputSettings))
		{
			++reflector_shift;
			for (k = 0; k < 15; ++k)
			{
				g_enigma_encrypt_chars[i + k] = '!' + k;
			}
			i += 15;
			for (k = 0; k < 7; ++k)
			{
				g_enigma_encrypt_chars[i + k] = ':' + k;
			}
			i += 7;
			for (k = 0; k < 6; ++k)
			{
				g_enigma_encrypt_chars[i + k] = '[' + k;
			}
			i += 6;
			for (k = 0; k < 4; ++k)
			{
				g_enigma_encrypt_chars[i + k] = '{' + k;
			}
			i += 4;
		}

		// setup rotors based on input.
		for (i = 0; i < g_enigma_rotor_count; ++i)
		{
			rotor_set = i * g_enigma_encrypt_char_count;
			switch (g_enigma_rotor_settings[i].type)
			{
			case 1:
				++reflector_shift;
				rotor_setup_type_1(rotor_set, g_enigma_rotor_settings[i].shift);
				break;
			case 2:
				++reflector_shift;
				rotor_setup_type_2(rotor_set, g_enigma_rotor_settings[i].shift);
				break;
			case 3:
				++reflector_shift;
				rotor_setup_type_3(rotor_set, g_enigma_rotor_settings[i].shift);
				break;
			case 4:
				++reflector_shift;
				rotor_setup_type_4(rotor_set, g_enigma_rotor_settings[i].shift);
				break;
			case 5:
				++reflector_shift;
				rotor_setup_type_5(rotor_set, g_enigma_rotor_settings[i].shift);
				break;
			case 6:
				++reflector_shift;
				rotor_setup_type_6(rotor_set, g_enigma_rotor_settings[i].shift);
				break;
			case 7:
				++reflector_shift;
				rotor_setup_type_7(rotor_set, g_enigma_rotor_settings[i].shift);
				break;
			case 8:
				++reflector_shift;
				rotor_setup_type_8(rotor_set, g_enigma_rotor_settings[i].shift);
				break;

			default:
				// no setup.
				break;
			}
		}

		reflector_setup(reflector_shift);
	}

	if (shouldEncryptVerbose(encryptInputSettings))
	{
		print_chars_to_encrypt();
		print_rotor();
		print_reflector();
	}

	return SHELL_OK;
}

bool16 parse_inputs(int nargs, char *args[])
{
	bool16 inputSettings = 0;

	char ch;	 /* next character of argument	*/
	char *chptr; /* walks along argument string	*/

	int32 count = 0;
	int32 setting = 0;
	int32 i = 0;

	for (count = 1; count < nargs; ++count)
	{
		i = 0;
		setting = 0;
		chptr = args[count];
		ch = *chptr++;

		// check to see what the first char is.
		switch (ch)
		{
		case '-':
			/* found flag(s) in the input fields. */
			ch = *chptr++;

			while (ch != NULL)
			{
				switch (ch)
				{
				case 'u':
					/* u = unlock the system. */
					inputSettings = 0;
					setEncryptUnlock(inputSettings);

					break;
				case 'l':
					/* l = lock the system. */
					inputSettings = 0;
					setEncryptLock(inputSettings);
					break;
				case 'c':
					/* c = encrypt lower case values. */
					setEncryptLowerCase(inputSettings);
					g_enigma_encrypt_char_count += NUM_CHARS;
					break;
				case 'C':
					/* C = encrypt Upper case values. */
					setEncryptUpperCase(inputSettings);
					g_enigma_encrypt_char_count += NUM_CHARS;
					break;
				case 'n':
					/* n = encrypt number values. */
					setEncryptNumbers(inputSettings);
					g_enigma_encrypt_char_count += NUM_NUMS;
					break;
				case 'w':
					/* w = encrypt white space values. */
					setEncryptWhiteSpace(inputSettings);
					g_enigma_encrypt_char_count += NUM_WHITESPACE;
					break;
				case 's':
					/* s = encrypt symbol values. */
					setEncryptSymbols(inputSettings);
					g_enigma_encrypt_char_count += NUM_SYMBOLS;
					break;

				case 'i':
					/* i = reset the rotors to the initial positions. */
					setEncryptResetRotorsPositions(inputSettings);
					break;
				case 'v':
					/* v = verbose mode and print out info. */
					setEncryptVerbose(inputSettings);
					break;

				default:
					/* should not reach this for flag values.  If so just ignore the values. */
					break;
				}

				/* get the next char value if there is one. */
				ch = *chptr++;
			}

			/* done with the flag '-' switch case */
			break;

		default:
			/* if the value is not a flag, then it must be rotor information.
			 * assume the value is a tuplet value set.  3 ints divided by ','
			 * assume default value of 0 for the last two sets if not present.
			 * */

			while (ch != NULL && i < 3)
			{
				if (ch == ',')
				{
					/* found a divider for rotor values.  So store the value we have. */
					switch (i)
					{
					case 0:
						/* first value found and it is the type of rotor. */
						g_enigma_rotor_settings[g_enigma_rotor_count].type = setting;
						break;
					case 1:
						/* second value found and it is the shift of the rotor encoder. */
						g_enigma_rotor_settings[g_enigma_rotor_count].shift = setting;
						break;
					case 2:
						/* third value found and it is the starting position of the rotor. */
						g_enigma_rotor_settings[g_enigma_rotor_count].start = setting;
						break;
					default:
						/* any values found after this should be ignored */
						break;
					}
					setting = 0;
					++i;
				}
				else if ((ch >= '0') && (ch <= '9'))
				{
					/* found a char that is in the number range.  
					 * So convert char to int and add it to the setting value.
					 * */
					setting = 10 * setting + (ch - '0');
				}

				/* get the next char value if there is one. */
				ch = *chptr++;
			}
			/* need to address the case where setting is not set. */
			switch (i)
			{
			case 2:
				/* just the staring value needs to be set. */
				g_enigma_rotor_settings[g_enigma_rotor_count].start = setting;
				break;
			case 1:
				/* need to set the shift setting and set start position to 0. */
				g_enigma_rotor_settings[g_enigma_rotor_count].shift = setting;
				g_enigma_rotor_settings[g_enigma_rotor_count].start = 0;
				break;

			case 0:
				/* need to set the type setting and set other values to 0. */
				g_enigma_rotor_settings[g_enigma_rotor_count].type = setting;
				g_enigma_rotor_settings[g_enigma_rotor_count].shift = 0;
				g_enigma_rotor_settings[g_enigma_rotor_count].start = 0;
				break;
			default:
				break;
			}

			/* set rotor position to the starting position. */
			g_enigma_rotor_settings[g_enigma_rotor_count].position = g_enigma_rotor_settings[g_enigma_rotor_count].start;

			++g_enigma_rotor_count;

			/* done with the default (rotor values) switch case. */
			break;
		}
	}

	return inputSettings;
}

void print_input_settings(bool16 inputSettings)
{
	/* print out the input values pulled from the input args. */
	printf("INPUT SETTINGS:\n");
	printf(" encrypt lower_char   : %d\n", shouldEncryptLowerCase(inputSettings));
	printf(" encrypt upper_char   : %d\n", shouldEncryptUpperCase(inputSettings));
	printf(" encrypt numbers      : %d\n", shouldEncryptNumbers(inputSettings));
	printf(" encrypt white_space  : %d\n", shouldEncryptWhiteSpace(inputSettings));
	printf(" encrypt symbols      : %d\n", shouldEncryptSymbols(inputSettings));
	printf(" encrypt lock         : %d\n", shouldEncryptLock(inputSettings));
	printf(" encrypt unlock       : %d\n", shouldEncryptUnlock(inputSettings));
	printf(" encrypt verbose      : %d\n", shouldEncryptVerbose(inputSettings));
	printf(" encrypt reset rotors : %d\n", shouldEncryptResetRotorsPositions(inputSettings));
}

void print_global_settings()
{
	printf("GLOBAL SETTINGS:\n");
	printf(" enigma rotor count   : %d\n", g_enigma_rotor_count);
	printf(" enigma char count    : %d\n", g_enigma_encrypt_char_count);
}

void print_rotor_settings()
{
	int16 i;

	printf("ROTOR SETTINGS:\n");
	for (i = 0; i < g_enigma_rotor_count; ++i)
	{
		printf(" %d | %d %d %d %d\n", i,
			   g_enigma_rotor_settings[i].type,
			   g_enigma_rotor_settings[i].shift,
			   g_enigma_rotor_settings[i].start,
			   g_enigma_rotor_settings[i].position);
	}
}

void print_rotor()
{
	int16 i = 0;
	int16 j = 0;
	int32 rotor_set = 0;

	printf("ROTOR SETUP:\n");

	printf("---|", i);
	for (j = 0; j < g_enigma_encrypt_char_count; ++j)
	{
		if (j % PRINT_NUM_LENGTH == 0 && j != 0)
		{
			break;
		}
		printf("-%02d", j);
	}
	printf("\n");

	for (i = 0; i < g_enigma_rotor_count; ++i)
	{
		rotor_set = i * g_enigma_encrypt_char_count;

		printf(" %d |", i);
		for (j = 0; j < g_enigma_encrypt_char_count; ++j)
		{
			if (j % PRINT_NUM_LENGTH == 0 && j != 0)
			{
				printf("\n   |");
			}
			printf(" %02d", g_enigma_rotors[rotor_set + j].forword);
		}
		printf("\n - |");
		for (j = 0; j < g_enigma_encrypt_char_count; ++j)
		{
			if (j % PRINT_NUM_LENGTH == 0 && j != 0)
			{
				printf("\n   |");
			}
			printf(" %02d", g_enigma_rotors[rotor_set + j].backward);
		}
		printf("\n");
	}
	printf("\n");
}

void print_reflector()
{
	int16 j;

	printf("REFLECTOR SETUP:\n");

	printf(" R |");
	for (j = 0; j < g_enigma_encrypt_char_count; ++j)
	{
		if (j % PRINT_NUM_LENGTH == 0 && j != 0)
		{
			printf("\n   |");
		}
		printf(" %02d", g_enigma_reflector[j]);
	}
	printf("\n");
}

void print_chars_to_encrypt()
{
	int16 i;

	printf("ENCRYPT CHARS SETUP:");
	for (i = 0; i < g_enigma_encrypt_char_count; ++i)
	{
		if (i % PRINT_CHAR_LENGTH == 0)
		{
			printf("\n");
		}
		printf(" %c", g_enigma_encrypt_chars[i]);
	}
	printf("\n");
}

void rotor_setup_type_1(int16 rotor_set, int16 shift)
{
	int16 i;
	char t;

	// A B C D E F G H ...
	for (i = 0; i < g_enigma_encrypt_char_count; ++i)
	{
		t = (i + shift) % g_enigma_encrypt_char_count;
		g_enigma_rotors[rotor_set + i].forword = t;
		g_enigma_rotors[rotor_set + t].backward = i;
	}
}

void rotor_setup_type_2(int16 rotor_set, int16 shift)
{
	int16 i;
	int16 j;
	char t;

	// Z Y X W V U T S ...
	for (i = 0, j = g_enigma_encrypt_char_count - 1; i < g_enigma_encrypt_char_count; ++i, --j)
	{
		t = (j + shift) % g_enigma_encrypt_char_count;
		g_enigma_rotors[rotor_set + i].forword = t;
		t = (i + shift) % g_enigma_encrypt_char_count;
		g_enigma_rotors[rotor_set + t].backward = j;
	}
}

void rotor_setup_type_3(int16 rotor_set, int16 shift)
{
	int16 j;
	int16 k;
	char t;

	// B A D C F E H G ...
	for (j = 0; j < g_enigma_encrypt_char_count; ++j)
	{
		t = (j + shift) % g_enigma_encrypt_char_count;
		g_enigma_rotors[rotor_set + j].forword = t;
	}
	for (j = 0; j < g_enigma_encrypt_char_count; j += 2)
	{
		t = g_enigma_rotors[rotor_set + j].forword;
		g_enigma_rotors[rotor_set + j].forword = g_enigma_rotors[rotor_set + j + 1].forword;
		g_enigma_rotors[rotor_set + j + 1].forword = t;
	}
	for (j = 0; j < g_enigma_encrypt_char_count; ++j)
	{
		k = g_enigma_rotors[rotor_set + j].forword;
		g_enigma_rotors[rotor_set + k].backward = j;
	}
}

void rotor_setup_type_4(int16 rotor_set, int16 shift)
{
	int16 j;
	int16 k;
	char t;

	// Y Z W X U V S T ...
	for (j = 0, k = g_enigma_encrypt_char_count - 1; j < g_enigma_encrypt_char_count; ++j, --k)
	{
		t = (k + shift) % g_enigma_encrypt_char_count;
		g_enigma_rotors[rotor_set + j].forword = t;
	}
	for (j = 0; j < g_enigma_encrypt_char_count; j += 2)
	{
		t = g_enigma_rotors[rotor_set + j].forword;
		g_enigma_rotors[rotor_set + j].forword = g_enigma_rotors[rotor_set + j + 1].forword;
		g_enigma_rotors[rotor_set + j + 1].forword = t;
	}
	for (j = 0; j < g_enigma_encrypt_char_count; ++j)
	{
		k = g_enigma_rotors[rotor_set + j].forword;
		g_enigma_rotors[rotor_set + k].backward = j;
	}
}

void rotor_setup_type_5(int16 rotor_set, int16 shift)
{
	int16 j;
	int16 k;
	char t;

	for (j = 0; j < g_enigma_encrypt_char_count; ++j)
	{
		t = (j + shift) % g_enigma_encrypt_char_count;
		g_enigma_rotors[rotor_set + j].forword = t;
	}
	for (j = 0; j < g_enigma_encrypt_char_count; ++j)
	{
		k = (j + j + 1) % g_enigma_encrypt_char_count;
		t = g_enigma_rotors[rotor_set + j].forword;
		g_enigma_rotors[rotor_set + j].forword = g_enigma_rotors[rotor_set + k].forword;
		g_enigma_rotors[rotor_set + k].forword = t;
	}
	for (j = 0; j < g_enigma_encrypt_char_count; ++j)
	{
		k = g_enigma_rotors[rotor_set + j].forword;
		g_enigma_rotors[rotor_set + k].backward = j;
	}
}

void rotor_setup_type_6(int16 rotor_set, int16 shift)
{
	int16 j;
	int16 k;
	char t;

	for (j = 0; j < g_enigma_encrypt_char_count; ++j)
	{
		t = (j + shift) % g_enigma_encrypt_char_count;
		g_enigma_rotors[rotor_set + j].forword = t;
	}
	for (j = 0; j < g_enigma_encrypt_char_count; ++j)
	{
		k = (j + j + j + 1) % g_enigma_encrypt_char_count;
		t = g_enigma_rotors[rotor_set + j].forword;
		g_enigma_rotors[rotor_set + j].forword = g_enigma_rotors[rotor_set + k].forword;
		g_enigma_rotors[rotor_set + k].forword = t;
	}
	for (j = 0; j < g_enigma_encrypt_char_count; ++j)
	{
		k = g_enigma_rotors[rotor_set + j].forword;
		g_enigma_rotors[rotor_set + k].backward = j;
	}
}

void rotor_setup_type_7(int16 rotor_set, int16 shift)
{
	int16 j;
	int16 k;
	char t;

	for (j = 0, k = g_enigma_encrypt_char_count - 1; j < g_enigma_encrypt_char_count; ++j, --k)
	{
		t = (k + shift) % g_enigma_encrypt_char_count;
		g_enigma_rotors[rotor_set + j].forword = t;
	}
	for (j = 0; j < g_enigma_encrypt_char_count; ++j)
	{
		k = (j + j + 1) % g_enigma_encrypt_char_count;
		t = g_enigma_rotors[rotor_set + j].forword;
		g_enigma_rotors[rotor_set + j].forword = g_enigma_rotors[rotor_set + k].forword;
		g_enigma_rotors[rotor_set + k].forword = t;
	}
	for (j = 0; j < g_enigma_encrypt_char_count; ++j)
	{
		k = g_enigma_rotors[rotor_set + j].forword;
		g_enigma_rotors[rotor_set + k].backward = j;
	}
}

void rotor_setup_type_8(int16 rotor_set, int16 shift)
{
	int16 j;
	int16 k;
	char t;

	for (j = 0, k = g_enigma_encrypt_char_count - 1; j < g_enigma_encrypt_char_count; ++j, --k)
	{
		t = (k + shift) % g_enigma_encrypt_char_count;
		g_enigma_rotors[rotor_set + j].forword = t;
	}
	for (j = 0; j < g_enigma_encrypt_char_count; ++j)
	{
		k = (j + j + j + 1) % g_enigma_encrypt_char_count;
		t = g_enigma_rotors[rotor_set + j].forword;
		g_enigma_rotors[rotor_set + j].forword = g_enigma_rotors[rotor_set + k].forword;
		g_enigma_rotors[rotor_set + k].forword = t;
	}
	for (j = 0; j < g_enigma_encrypt_char_count; ++j)
	{
		k = g_enigma_rotors[rotor_set + j].forword;
		g_enigma_rotors[rotor_set + k].backward = j;
	}
}

void reflector_setup(int16 shift)
{
	int16 j;
	int16 k;
	int16 half = g_enigma_encrypt_char_count / 2;
	char t;

	for (j = 0; j < g_enigma_encrypt_char_count; ++j)
	{
		g_enigma_reflector[j] = j;
	}
	for (j = 0, k = g_enigma_encrypt_char_count - half; j < half; ++j, ++k)
	{
		t = g_enigma_reflector[j];
		g_enigma_reflector[j] = g_enigma_reflector[k];
		g_enigma_reflector[k] = t;
	}
}

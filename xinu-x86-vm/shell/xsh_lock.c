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

	int32 retval; /* return value			        */
	char ch;	  /* next character of argument	    */
	char *chptr;  /* walks along argument string	*/
	char delim[] = ",";

	bool8 encrypt_lower_char = FALSE;
	bool8 encrypt_upper_char = FALSE;
	bool8 encrypt_numbers = FALSE;
	bool8 encrypt_white_space = FALSE;
	bool8 encrypt_symbols = FALSE;
	bool8 encrypt_lock = FALSE;
	bool8 encrypt_unlock = FALSE;

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
		printf("Usage: %s ([-u] | [-l [-c] [-C] [-n] [-w] [-s]]) <RS>+ \n\n", args[0]);
		printf("Description:\n");
		printf("\tEncrypts the system.  Echo and print are all encrypted.\n");
		printf("Options:\n");
		printf("\t-u \tUnlock: \tTurns off the encryption.\n");
		printf("\t-l \tLock: \t\tTurns on the encryption.  Need one other flag.\n");
		printf("\t-c \tLower Case: \tEncrypt lower case characters.\n");
		printf("\t-C \tUpper Case: \tEncrypt upper case characters.\n");
		printf("\t-n \tNumbers: \tEncrypt number characters.\n");
		printf("\t-w \tWhite Space: \tEncrypt white space, like newline, tab, space.\n");
		printf("\t-s \tSymbols: \tEncrypt symbols found on the keyboard.\n");
		printf("\t<RS> \tRotorSettings: \t<Sch>,<Mix>,<Srt>. need at least one (1) set and no more then nine (9) sets.\n");
		printf("\t<Sch> \tSchema: \tValue between 1-5.  This value defines the encryption schema of the Rotor.\n");
		printf("\t<Mix> \tMixer:  \tValue between 0-255.  This value defines the Mix value for the schema.\n");
		printf("\t<Srt> \tStart:  \tValue between 0-255.  This value defines the starting position of the Rotor.\n");
		printf("\t--help \t\t\tDisplay this help and exit\n");
		return OK;
	}

	int32 count = 0;
	int32 rotor_args = 0;
	int32 setting = 0;
	int32 rotor_count = 0;
	int32 rotor_settings[10][3];
	int32 i = 0;
	int32 j = 0;

	for (i = 0; i < 10; ++i)
	{
		for (j = 0; j < 3; ++j)
		{
			rotor_settings[i][j] = -1;
		}
	}

	for (count = 1; count < nargs; ++count)
	{
		chptr = args[count];
		ch = *chptr++;
		i = 0;
		switch (args[count][0])
		{
		case '-':
			ch = *chptr++;
			while (ch != NULL)
			{
				switch (ch)
				{
				case 'u':
					encrypt_lower_char = FALSE;
					encrypt_upper_char = FALSE;
					encrypt_numbers = FALSE;
					encrypt_white_space = FALSE;
					encrypt_symbols = FALSE;
					encrypt_unlock = TRUE;
					break;
				case 'l':
					encrypt_lower_char = FALSE;
					encrypt_upper_char = FALSE;
					encrypt_numbers = FALSE;
					encrypt_white_space = FALSE;
					encrypt_symbols = FALSE;
					encrypt_lock = TRUE;
					break;
				case 'c':
					encrypt_lower_char = TRUE;
					break;
				case 'C':
					encrypt_upper_char = TRUE;
					break;
				case 'n':
					encrypt_numbers = TRUE;
					break;
				case 'w':
					encrypt_white_space = TRUE;
					break;
				case 's':
					encrypt_symbols = TRUE;
					break;

				default:
					break;
				}
				ch = *chptr++;
			}
			/* code */
			break;

		default:
			// assume the value is a tuplet value set.  3 ints divided by ','
			// assume default value of 1 for the last two sets if not present.
			while (ch != NULL && i < 3)
			{
				if (ch == ',')
				{
					rotor_settings[rotor_count][i++] = setting;
					setting = 0;
				}
				else if ((ch >= '0') && (ch <= '9'))
				{
					setting = 10 * setting + (ch - '0');
				}
				ch = *chptr++;
			}
			if (i == 2)
			{
				rotor_settings[rotor_count][2] = setting;
				setting = 0;
			}
			else if (i == 1)
			{
				rotor_settings[rotor_count][1] = setting;
				rotor_settings[rotor_count][2] = 0;
				setting = 0;
			}
			else if (i == 0)
			{
				rotor_settings[rotor_count][0] = setting;
				rotor_settings[rotor_count][1] = 0;
				rotor_settings[rotor_count][2] = 0;
				setting = 0;
			}

			++rotor_count;
			break;
		}
	}

	printf("Values:\n");
	printf("encrypt_lower_char %d\n", encrypt_lower_char);
	printf("encrypt_upper_char %d\n", encrypt_upper_char);
	printf("encrypt_numbers %d\n", encrypt_numbers);
	printf("encrypt_white_space %d\n", encrypt_white_space);
	printf("encrypt_symbols %d\n", encrypt_symbols);
	printf("encrypt_lock %d\n", encrypt_lock);
	printf("encrypt_unlock %d\n", encrypt_unlock);

	for (i = 0; i < 10; ++i)
	{
		printf(" %d | ", i);
		for (j = 0; j < 3; ++j)
		{
			printf(" %d ", rotor_settings[i][j]);
		}
		printf("\n");
	}

	/* compute process ID from argument string */

	return 0;
}

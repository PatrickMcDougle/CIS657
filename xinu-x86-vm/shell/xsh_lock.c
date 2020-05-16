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

    int32 retval; /* return value			*/
    char ch;      /* next character of argument	*/
    char *chptr;  /* walks along argument string	*/

    /* Output info for '--help' argument */

    if (nargs == 2 && strncmp(args[1], "--help", 7) == 0)
    {
        printf("Usage: %s [-u] [-l [-c] [-C] [-n] [-w] [-s]] \n\n", args[0]);
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
        printf("\t--help \t\tDisplay this help and exit\n");
        return OK;
    }


    /* Check argument count */

    if (nargs != 2)
    {
        fprintf(stderr, "%s: incorrect argument\n", args[0]);
        fprintf(stderr, "Try '%s --help' for more information\n",
                args[0]);
        return SYSERR;
    }

    /* compute process ID from argument string */

    return 0;
}

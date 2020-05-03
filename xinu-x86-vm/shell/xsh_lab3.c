/* xsh_lab3.c - xsh_lab3 */
#include <xinu.h>
#include <string.h>
#include <stdio.h>

bool8 is_positive_integer(const char *);

/* ---------- ---------- ---------- ---------- ---------- ----------
 * xsh_lab3 - shell command to do whatever you want it to do.
 * ---------- ---------- ---------- ---------- ---------- ----------
 */
shellcmd xsh_lab3(int32 nargs, char *args[])
{
    pri16 priority = 20;

    // Output info for '--help' argument
    if (nargs == 2)
    {
        if (strncmp(args[1], "--help", 7) == 0)
        {
            printf("Usage: %s <priority>\n\n", args[0]);
            printf("Description:\n");
            printf("\tStarts a new process that runs forever with the given priority.\n");
            printf("Options:\n");
            printf("\t<priority>\tThe priority value to use when creating the forever\n");
            printf("\t\t\tprocess.  Valid values are in the range: 1 to 127.\n");
            printf("\t\t\tDefault value is 20.\n");
            printf("\t--help    \tDisplay this help and exit\n");
            return OK;
        }

        if (is_positive_integer(args[1]) == TRUE)
        {
            // going to assume the input is an integer.
            priority = atoi(args[1]);

            // printf("Priority updated: [%d]\n", priority);

            // check to make sure the priority is valid.
            if (priority < 1 || priority > 127)
            {
                fprintf(stderr, "%s: Incorrect argument - value needs to be in the range of 1 to 127 inclusive.\n", args[0]);
                fprintf(stderr, "Try '%s --help' for more information\n", args[0]);
                return SYSERR;
            }
        }
        else
        {
            fprintf(stderr, "%s: Incorrect argument - value is not a postive number\n", args[0]);
            fprintf(stderr, "Try '%s --help' for more information\n", args[0]);
            return SYSERR;
        }
    }

    // Check argument count
    else if (nargs > 2)
    {
        fprintf(stderr, "%s: Incorrect number of arguments\n", args[0]);
        fprintf(stderr, "Try '%s --help' for more information\n", args[0]);
        return SYSERR;
    }

    pid32 pid_runforever = create(runforever, 1024, priority, "runforever", 1, priority);

    resume(pid_runforever);

    return 0;
}

bool8 is_positive_integer(const char *pStr)
{
    if (pStr == NULL || *pStr == '\0')
    {
        return FALSE;
    }

    char c = *pStr;

    // checking to see if the string starts with a +
    if (c == '+')
    {
        // positive values are ok.
        // move to the next character.
        ++pStr;
    }

    while (*pStr)
    {
        c = *pStr;

        // ASCII: '0' ... '9' values.  Anything outside that is not valid.
        if (c < '0' || c > '9')
        {
            return FALSE;
        }

        ++pStr;
    }

    // this string is a valid number.
    return TRUE;
}
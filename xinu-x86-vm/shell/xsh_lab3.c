/* xsh_lab3.c - xsh_lab3 */
#include <xinu.h>
#include <string.h>
#include <stdio.h>

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
            printf("\tstarts a new process that runs forever with the given priority.\n");
            printf("Options:\n");
            printf("\t<priority>\tthe priority value to use when creating the forever process.  Valid values are in the range: 1 to 127.\n");
            printf("\t--help    \tdisplay this help and exit\n");
            return OK;
        }
    }

    // Check argument count
    if (nargs != 2)
    {
        fprintf(stderr, "%s: incorrect argument\n", args[0]);
        fprintf(stderr, "Try '%s --help' for more information\n", args[0]);
        return SYSERR;
    }

    priority = atoi(args[1]);

    printf("Priority: [%d]\n", priority);

    // check to make sure the priority is valid.
    if (priority < 1 && priority > 127)
    {
        return SYSERR;
    }

    pid32 pid_runforever = create(runforever, 1024, priority, "runforever", 1, priority);

    resume(pid_runforever);

    return 0;
}
/* lab5.c - lab5_process */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  lab5_process2 - process prints ID only once and then sleeps for 15 seconds
 *------------------------------------------------------------------------
 */
syscall lab5_process(
    int16 priority /* Priority value for this process */
)
{
    printf("priority & pid: [%d,%d]\n", priority, getpid());

    sleep(15);

    while (TRUE)
    {
        printf("priority & pid: [%d,%d]\n", priority, getpid());
    }
}

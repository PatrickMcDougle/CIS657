/* runforever.c - runforever */
#include <xinu.h>

/* ---------- ---------- ---------- ---------- ---------- ----------
 * runforever - shell command to do whatever you want it to do.
 * ---------- ---------- ---------- ---------- ---------- ----------
 */
void runforever(
    int16 priority /* Priority value for this process */
)
{
    printf("priority & pid: [%d,%d]\n", priority, getpid());

    while (TRUE)
    {
        // do nothing at this time.
    }
}
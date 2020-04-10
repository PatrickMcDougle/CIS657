/* snd.c - sndA, sndB */

#include <xinu.h>

/* sndA - method to send A to the console. */
void sndA(void)
{
    while (TRUE)
    {
        putc(CONSOLE, 'A');
    }
}

/* sndB - method to send A to the console. */
void sndB(void)
{
    while (TRUE)
    {
        putc(CONSOLE, 'B');
    }
}
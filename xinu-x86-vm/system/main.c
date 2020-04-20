/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>

// function/process declaration.
void number_waiter();
void sentance_signaler();
void number_others();
void sentance_others();

// global values to work with.
int32 g_counter = 0;
pid32 g_pid_number;
pid32 g_pid_sentance;
sid32 g_sid_semaphore;

// global define values.
#define _COUNTER_FIRST 21
#define _COUNTER_NEXT 5
#define _COUNTER_MAX 200

/*------------------------------------------------------------------------
 *  main - The one that starts it all.
 *------------------------------------------------------------------------
 */
int main(int argc, char const *argv[])
{
	kprintf("/* ----------- ---------- ----------- ---------- ----------- */\n");
	kprintf("    ___________  _______     ___       ___  ___      _  _    \n");
	kprintf("   |           ||   ____|   /   \\     |   \\/   |    | || |   \n");
	kprintf("    ---|  |---- |  |__     /  ^  \\    |  \\  /  |    | || |_  \n");
	kprintf("       |  |     |   __|   /  /_\\  \\   |  |\\/|  |    |__   _| \n");
	kprintf("       |  |     |  |____ /  _____  \\  |  |  |  |       | |   \n");
	kprintf("       |__|     |_______/__/     \\__\\ |__|  |__|       |_|   \n");
	kprintf("/* ----------- ---------- ----------- ---------- ----------- */\n");

	g_sid_semaphore = semcreate(_COUNTER_FIRST - 1);

	g_pid_number = create(number_waiter, 1024, 11, "print numbers waiter", 0);
	g_pid_sentance = create(sentance_signaler, 1024, 10, "print sentance signaler", 0);

	// g_pid_number = create(number_others, 1024, 11, "print numbers other", 0);
	// g_pid_sentance = create(sentance_others, 1024, 10, "print sentance other", 0);

	resume(g_pid_number);
	resume(g_pid_sentance);

	return OK;
}

/* *********** *********** ********** ********** *********** *********** *
 * Version 1 (A) Patrick's version of the code.
 * *********** *********** ********** ********** *********** *********** */

/*------------------------------------------------------------------------
 *  number_waiter - based of the instructions in the lab.
 *------------------------------------------------------------------------
 */
void number_waiter()
{
	for (g_counter = 1; g_counter <= _COUNTER_MAX; ++g_counter)
	{
		kprintf("%d ", g_counter);
		wait(g_sid_semaphore);
	}
}

/*------------------------------------------------------------------------
 *  sentance_signaler - based of the instructions in the lab.
 *------------------------------------------------------------------------
 */
void sentance_signaler()
{
	do
	{
		kprintf("Team 4 Rocks! v.A\n");
		signaln(g_sid_semaphore, _COUNTER_NEXT);
	} while (g_counter <= _COUNTER_MAX);
}

/* *********** *********** ********** ********** *********** *********** *
 * Version 4 (D) Other team member's version of the code.
 * *********** *********** ********** ********** *********** *********** */

/*------------------------------------------------------------------------
 *  number_others - based on what others did in the group.
 *------------------------------------------------------------------------
 */
void number_others()
{
	for (g_counter = 1; g_counter <= _COUNTER_MAX; ++g_counter)
	{
		kprintf("%d ", g_counter);
		wait(g_sid_semaphore);
	}
	kill(g_pid_sentance);
}

/*------------------------------------------------------------------------
 *  sentance_others - based on what others did in the group.
 *------------------------------------------------------------------------
 */
void sentance_others()
{
	while (TRUE)
	{
		kprintf("Team 4 Rocks! v.D\n");
		signaln(g_sid_semaphore, _COUNTER_NEXT);
	}
}
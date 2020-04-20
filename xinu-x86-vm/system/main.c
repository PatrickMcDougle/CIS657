/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>

void number_print(sid32, sid32);
void sentance_print(sid32, sid32);
void number_waiter();
void sentance_signaler();
void number_print3(sid32, sid32);
void sentance_print3(sid32, sid32);
void number_others();
void sentance_others();

int32 g_counter = 0;
pid32 g_pid_number;
pid32 g_pid_sentance;
sid32 g_sid_number;
sid32 g_sid_sentance;
sid32 g_sid_semaphore;

#define _COUNTER_FIRST 21
#define _COUNTER_NEXT 5
#define _COUNTER_MAX 200

/* main - just saying hello, then exiting */
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

	// g_sid_number = semcreate(0);
	// g_sid_sentance = semcreate(1);

	// g_pid_number = create(number_print, 1024, 10, "print numbers", 2, g_sid_number, g_sid_sentance);
	// g_pid_sentance = create(sentance_print, 1024, 10, "print sentance", 2, g_sid_number, g_sid_sentance);

	g_sid_number = semcreate(0);
	g_sid_sentance = semcreate(_COUNTER_FIRST);

	g_pid_number = create(number_print3, 1024, 10, "print numbers", 2, g_sid_number, g_sid_sentance);
	g_pid_sentance = create(sentance_print3, 1024, 10, "print sentance", 2, g_sid_number, g_sid_sentance);

	// g_sid_semaphore = semcreate(_COUNTER_FIRST - 1);

	// g_pid_number = create(number_waiter, 1024, 11, "print numbers waiter", 0);
	// g_pid_sentance = create(sentance_signaler, 1024, 10, "print sentance signaler", 0);

	// g_pid_number = create(number_others, 1024, 11, "print numbers version 5", 0);
	// g_pid_sentance = create(sentance_others, 1024, 10, "print sentance version 5", 0);

	resume(g_pid_number);
	resume(g_pid_sentance);

	return OK;
}

/* *********** *********** ********** ********** *********** *********** *
 * Version 1 (A) of my code.
 * *********** *********** ********** ********** *********** *********** */

/*------------------------------------------------------------------------
 *  number_print - This is my first attempt at doing the lab based off
 *  what I have read.  This method will print out numbers.  21 numbers for
 *  the first time, 5 additional numbers after that.
 *------------------------------------------------------------------------
 */
void number_print(
	sid32 print_number,
	sid32 print_sentance)
{
	wait(print_sentance);
	for (g_counter = 1; g_counter <= _COUNTER_FIRST; ++g_counter)
	{
		kprintf("%d ", g_counter);
	}
	signal(print_number);

	do
	{
		wait(print_sentance);
		do
		{
			kprintf("%d ", g_counter++);
		} while (g_counter % _COUNTER_NEXT != 2 && g_counter <= _COUNTER_MAX);
		signal(print_number);
	} while (g_counter <= _COUNTER_MAX);
}

/*------------------------------------------------------------------------
 *  sentance_print - This is my first attempt at doing the lab based off
 *  what I have read.  This method will print out a string after each
 *  number set is done printing.
 *------------------------------------------------------------------------
 */
void sentance_print(
	sid32 print_number,
	sid32 print_sentance)
{
	do
	{
		wait(print_number);
		kprintf("Team 4 Rocks! v.A\n");
		signal(print_sentance);
	} while (g_counter <= _COUNTER_MAX);
}

/* *********** *********** ********** ********** *********** *********** *
 * Version 2 (B) of my code.
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
		kprintf("Team 4 Rocks! v.B\n");
		signaln(g_sid_semaphore, _COUNTER_NEXT);
	} while (g_counter <= _COUNTER_MAX);
}

/* *********** *********** ********** ********** *********** *********** *
 * Version 3 (C) of my code.
 * *********** *********** ********** ********** *********** *********** */

/*------------------------------------------------------------------------
 *  number_print3 - based of the instructions in the lab with some tweeks.
 *------------------------------------------------------------------------
 */
void number_print3(
	sid32 print_number,
	sid32 print_sentance)
{
	for (g_counter = 1; g_counter <= _COUNTER_MAX; ++g_counter)
	{
		if (semtab[print_sentance].scount == 0)
		{
			signal(print_number);
		}
		wait(print_sentance);
		kprintf("%d ", g_counter);
	}
	signal(print_number);
}

/*------------------------------------------------------------------------
 *  sentance_print3 - based of the instructions in the lab with some tweeks.
 *------------------------------------------------------------------------
 */
void sentance_print3(
	sid32 print_number,
	sid32 print_sentance)
{
	wait(print_number);
	do
	{
		kprintf("Team 4 Rocks! v.C\n");
		signaln(print_sentance, _COUNTER_NEXT);
		wait(print_number);
	} while (g_counter <= _COUNTER_MAX);
}

/* *********** *********** ********** ********** *********** *********** *
 * Version 4 (D) of the code.
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
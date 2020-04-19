/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>

void number_print(sid32, sid32);
void sentance_print(sid32, sid32);
void number_print2();
void sentance_print2();
void number_print3(sid32, sid32);
void sentance_print3(sid32, sid32);
void number_print4();
void sentance_print4();
void number_print5();
void sentance_print5();

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

	g_sid_number = semcreate(0);
	g_sid_sentance = semcreate(1);

	g_pid_number = create(number_print, 1024, 30, "print numbers", 2, g_sid_number, g_sid_sentance);
	g_pid_sentance = create(sentance_print, 1024, 20, "print sentance", 2, g_sid_number, g_sid_sentance);

	// g_sid_number = semcreate(0);
	// g_sid_sentance = semcreate(_COUNTER_FIRST);

	// g_pid_number = create(number_print3, 1024, 30, "print numbers", 2, g_sid_number, g_sid_sentance);
	// g_pid_sentance = create(sentance_print3, 1024, 20, "print sentance", 2, g_sid_number, g_sid_sentance);

	// g_sid_semaphore = semcreate(0);

	// g_pid_number = create(number_print4, 1024, 30, "print numbers version 4", 0);
	// g_pid_sentance = create(sentance_print4, 1024, 20, "print sentance version 4", 0);

	// g_sid_semaphore = semcreate(_COUNTER_FIRST - 1);

	// g_pid_number = create(number_print2, 1024, 30, "print numbers", 0);
	// g_pid_sentance = create(sentance_print2, 1024, 20, "print sentance", 0);

	// g_pid_number = create(number_print5, 1024, 30, "print numbers version 5", 0);
	// g_pid_sentance = create(sentance_print5, 1024, 20, "print sentance version 5", 0);

	resume(g_pid_number);
	resume(g_pid_sentance);

	return OK;
}

/* ********** ********** ********** ********** ********** *
 * Version 1 (A) of my code.
 * ********** ********** ********** ********** ********** */

/* number_print - This is my first attempt at doing the lab based off what I have read. 
 *  This method will print out numbers.  21 for the first time, 5 additional numbers
 *  after that. */
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

/* sentance_print - This is my first attempt at doing the lab based off what I have read.
 *  This method will print out a string. */
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

/* ********** ********** ********** ********** ********** *
 * Version 2 (B) of my code.
 * ********** ********** ********** ********** ********** */

/* number_print2 - based of the instructions in the lab. */
void number_print2()
{
	g_counter = 1;
	do
	{
		kprintf("%d ", g_counter);
		wait(g_sid_semaphore);
	} while (++g_counter <= _COUNTER_MAX);
}

/* sentance_print2 - based of the instructions in the lab. */
void sentance_print2()
{
	do
	{
		kprintf("Team 4 Rocks! v.B\n");
		signaln(g_sid_semaphore, _COUNTER_NEXT);
	} while (g_counter <= _COUNTER_MAX);
}

/* ********** ********** ********** ********** ********** *
 * Version 3 (C) of my code.
 * ********** ********** ********** ********** ********** */

/* number_print3 - based of the instructions in the lab with some tweeks. */
void number_print3(
	sid32 print_number,
	sid32 print_sentance)
{
	g_counter = 1;
	do
	{
		wait(print_sentance);
		kprintf("%d ", g_counter);
		signal(print_number);
	} while (++g_counter <= _COUNTER_MAX);
}

/* sentance_print3 - based of the instructions in the lab with some tweeks. */
void sentance_print3(
	sid32 print_number,
	sid32 print_sentance)
{
	do
	{
		wait(print_number);
		kprintf("Team 4 Rocks! v.C\n");
		signaln(print_sentance, _COUNTER_NEXT);
	} while (g_counter <= _COUNTER_MAX);
}

/* ********** ********** ********** ********** ********** *
 * Version 4 (D) of my code.
 * ********** ********** ********** ********** ********** */

/* number_print - This is my first attempt at doing the lab based off what I have read. 
 *  This method will print out numbers.  21 for the first time, 5 additional numbers
 *  after that. */
void number_print4()
{
	for (g_counter = 1; g_counter <= _COUNTER_FIRST; ++g_counter)
	{
		kprintf("%d ", g_counter);
	}
	wait(g_sid_semaphore);

	do
	{
		do
		{
			kprintf("%d ", g_counter++);
		} while (g_counter % _COUNTER_NEXT != 2 && g_counter <= _COUNTER_MAX);
		if (g_counter <= _COUNTER_MAX)
		{
			wait(g_sid_semaphore);
		}
	} while (g_counter <= _COUNTER_MAX);
}

/* sentance_print - This is my first attempt at doing the lab based off what I have read.
 *  This method will print out a string. */
void sentance_print4()
{
	do
	{
		kprintf("Team 4 Rocks! v.D\n");
		signal(g_sid_semaphore);
	} while (g_counter <= _COUNTER_MAX);
}

/* ********** ********** ********** ********** ********** *
 * Version 5 (E) of my code.
 * ********** ********** ********** ********** ********** */

/* number_print5 - based of the instructions in the lab. */
void number_print5()
{
	for (g_counter = 1; g_counter <= _COUNTER_MAX; ++g_counter)
	{
		kprintf("%d ", g_counter);
		wait(g_sid_semaphore);
	}
	kill(g_pid_sentance);
}

/* sentance_print5 - based of the instructions in the lab. */
void sentance_print5()
{
	while (TRUE)
	{
		kprintf("Team 4 Rocks! v.E\n");
		signaln(g_sid_semaphore, _COUNTER_NEXT);
	}
}
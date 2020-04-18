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

int counter = 0;
pid32 pid_number;
pid32 pid_sentance;
sid32 print_number;
sid32 print_sentance;
sid32 print_sem;

#define _COUNTER_MAX 2000

/* main - just saying hello, then exiting */
int main(int argc, char const *argv[])
{

	printf("/* ----------- ---------- ----------- ---------- ----------- */\n");
	printf("    ___________  _______     ___       ___  ___      _  _    \n");
	printf("   |           ||   ____|   /   \\     |   \\/   |    | || |   \n");
	printf("    ---|  |---- |  |__     /  ^  \\    |  \\  /  |    | || |_  \n");
	printf("       |  |     |   __|   /  /_\\  \\   |  |\\/|  |    |__   _| \n");
	printf("       |  |     |  |____ /  _____  \\  |  |  |  |       | |   \n");
	printf("       |__|     |_______/__/     \\__\\ |__|  |__|       |_|   \n");
	printf("/* ----------- ---------- ----------- ---------- ----------- */\n");

	// print_number = semcreate(0);
	// print_sentance = semcreate(1);

	// pid_number = create(number_print, 1024, 20, "print numbers", 2, print_number, print_sentance);
	// pid_sentance = create(sentance_print, 1024, 20, "print sentance", 2, print_number, print_sentance);

	// print_sem = semcreate(20);

	// pid_number = create(number_print2, 1024, 20, "print numbers", 0);
	// pid_sentance = create(sentance_print2, 1024, 20, "print sentance", 0);

	// print_number = semcreate(0);
	// print_sentance = semcreate(21);

	// pid_number = create(number_print3, 1024, 20, "print numbers", 2, print_number, print_sentance);
	// pid_sentance = create(sentance_print3, 1024, 20, "print sentance", 2, print_number, print_sentance);

	print_sem = semcreate(20);

	pid_number = create(number_print4, 1024, 20, "print numbers version 4", 0);
	pid_sentance = create(sentance_print4, 1024, 20, "print sentance version 4", 0);

	resume(pid_number);
	resume(pid_sentance);

	return OK;
}

/* number_print - This is my first attempt at doing the lab based off what I have read. 
 *  This method will print out numbers.  21 for the first time, 5 additional numbers
 *  after that. */
void number_print(
	sid32 print_number,
	sid32 print_sentance)
{
	// int counter = 0;
	int five = 0;

	wait(print_sentance);
	for (counter = 1; counter < 22; ++counter)
	{
		kprintf("%d ", counter);
	}
	signal(print_number);

	do
	{
		wait(print_sentance);
		do
		{
			kprintf("%d ", counter++);
		} while (counter % 5 != 2);
		signal(print_number);
	} while (counter < _COUNTER_MAX);
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
	} while (counter < _COUNTER_MAX);
}

/* number_print2 - based of the instructions in the lab. */
void number_print2()
{
	do
	{
		kprintf("%d ", ++counter);
		wait(print_sem);
	} while (counter < _COUNTER_MAX);
}

/* sentance_print2 - based of the instructions in the lab. */
void sentance_print2()
{
	do
	{
		kprintf("Team 4 Rocks! v.B3\n");
		signaln(print_sem, 5);
	} while (counter < _COUNTER_MAX);
}

/* number_print3 - based of the instructions in the lab with some tweeks. */
void number_print3(
	sid32 print_number,
	sid32 print_sentance)
{
	do
	{
		wait(print_sentance);
		printf("%d ", ++counter);
		signal(print_number);
	} while (counter < _COUNTER_MAX);
}

/* sentance_print3 - based of the instructions in the lab with some tweeks. */
void sentance_print3(
	sid32 print_number,
	sid32 print_sentance)
{
	do
	{
		wait(print_number);
		printf("Team 4 Rocks! v.C\n");
		signaln(print_sentance, 5);
	} while (counter < _COUNTER_MAX);
}

/* number_print - This is my first attempt at doing the lab based off what I have read. 
 *  This method will print out numbers.  21 for the first time, 5 additional numbers
 *  after that. */
void number_print4()
{
	// int counter = 0;
	int five = 0;

	for (counter = 1; counter < 22; ++counter)
	{
		printf("%d ", counter);
	}
	wait(print_sem);

	do
	{
		do
		{
			printf("%d ", counter++);
		} while (counter % 5 != 2);
		wait(print_sem);
	} while (counter < _COUNTER_MAX);
}

/* sentance_print - This is my first attempt at doing the lab based off what I have read.
 *  This method will print out a string. */
void sentance_print4()
{
	do
	{
		printf("Team 4 Rocks! v.D\n");
		signal(print_sem);
	} while (counter < _COUNTER_MAX);
}
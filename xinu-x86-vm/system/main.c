/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>

void number_print(sid32, sid32);
void sentance_print(sid32, sid32);
void number_print2(sid32);
void sentance_print2(sid32);
void number_print3(sid32, sid32);
void sentance_print3(sid32, sid32);

int counter = 0;

#define _COUNTER_MAX 2000

/* main - just saying hello, then exiting */
int main(int argc, char const *argv[])
{
	sid32 print_number;
	sid32 print_sentance;
	sid32 print_sem;

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

	// resume(create(number_print, 1024, 20, "print numbers", 2, print_number, print_sentance));
	// resume(create(sentance_print, 1024, 20, "print sentance", 2, print_number, print_sentance));

	// print_sem = semcreate(20);

	// resume(create(number_print2, 1024, 20, "print numbers", 1, print_sem));
	// resume(create(sentance_print2, 1024, 20, "print sentance", 1, print_sem));

	print_number = semcreate(0);
	print_sentance = semcreate(21);

	resume(create(number_print3, 1024, 20, "print numbers", 2, print_number, print_sentance));
	resume(create(sentance_print3, 1024, 20, "print sentance", 2, print_number, print_sentance));

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
		printf("%d ", counter);
	}
	signal(print_number);

	do
	{
		five = 0;
		wait(print_sentance);
		while (five++ < 5)
		{
			printf("%d ", counter);
		}
		signal(print_number);
	} while (++counter < _COUNTER_MAX);
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
		printf("Team 4 Rocks! ");
		signal(print_sentance);
	} while (counter < _COUNTER_MAX);
}

/* number_print2 - based of the instructions in the lab. */
void number_print2(
	sid32 print_sem)
{
	do
	{
		printf("%d ", ++counter);
		wait(print_sem);
	} while (counter < _COUNTER_MAX);
}

/* sentance_print2 - based of the instructions in the lab. */
void sentance_print2(
	sid32 print_sem)
{
	do
	{
		printf("Team 4 Rocks! ");
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
		printf("Team 4 Rocks! ");
		signaln(print_sentance, 5);
	} while (counter < _COUNTER_MAX);
}

// int main(int argc, char **argv)
// {
// 	uint32 retval;

// 	resume(create(shell, 8192, 50, "shell", 1, CONSOLE));

// 	/* Wait for shell to exit and recreate it */

// 	recvclr();
// 	while (TRUE)
// 	{
// 		retval = receive();
// 		kprintf("\n\n\rMain process recreating shell\n\n\r");
// 		resume(create(shell, 4096, 1, "shell", 1, CONSOLE));
// 	}
// 	while (1)
// 		;

// 	return OK;
// }

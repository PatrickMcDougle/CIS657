/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>

/* main - just saying hello, then exiting */
int main(int argc, char const *argv[])
{
	printf("/* ---------- ---------- ---------- ---------- */\n");
	printf("    _____      _        _      _    \n");
	printf("   |  __ \\    | |      (_)    | |   \n");
	printf("   | |__) |_ _| |_ _ __ _  ___| | __\n");
	printf("   |  ___/ _` | __| '__| |/ __| |/ /\n");
	printf("   | |  | (_| | |_| |  | | (__|   < \n");
	printf("   |_|   \\__,_|\\__|_|  |_|\\___|_|\\_\\\n");
	printf("/* ---------- ---------- ---------- ---------- */\n\n");

	resume(create(sndA, 1024, 20, "sending A", 0));
	resume(create(sndB, 1024, 40, "sending B", 0));

	return OK;
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

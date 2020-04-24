/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	uint32 retval;

	kprintf("/* ----------- ---------- ----------- ---------- ----------- */\n");
	kprintf("    ___________  _______     ___       ___  ___      _  _    \n");
	kprintf("   |           ||   ____|   /   \\     |   \\/   |    | || |   \n");
	kprintf("    ---|  |---- |  |__     /  ^  \\    |  \\  /  |    | || |_  \n");
	kprintf("       |  |     |   __|   /  /_\\  \\   |  |\\/|  |    |__   _| \n");
	kprintf("       |  |     |  |____ /  _____  \\  |  |  |  |       | |   \n");
	kprintf("       |__|     |_______/__/     \\__\\ |__|  |__|       |_|   \n");
	kprintf("/* ----------- ---------- ----------- ---------- ----------- */\n");

	resume(create(shell, 8192, 50, "shell", 1, CONSOLE));

	/* Wait for shell to exit and recreate it */

	recvclr();
	while (TRUE)
	{
		retval = receive();
		kprintf("\n\n\rMain process recreating shell\n\n\r");
		resume(create(shell, 4096, 1, "shell", 1, CONSOLE));
	}
	while (TRUE)
		;

	return OK;
}

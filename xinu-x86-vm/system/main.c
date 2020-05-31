/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	uint32 retval;

	pid32 cpid = getpid();
	sendk(cpid, 1);
	sendk(cpid, 2);
	sendk(cpid+1, 3);
	sendk(cpid+1, 4);
	sendk(cpid, 5);
	sendk(cpid, 6);
	sendk(cpid+1, 7);
	sendk(cpid, 8);
	sendk(cpid, 9);

	kprintf("Message: %d\n", receivek());
	kprintf("Message: %d\n", receivek());

	sendk(cpid, 15);
	sendk(cpid, 16);
	
	kprintf("Message: %d\n", receivek());
	kprintf("Message: %d\n", receivek());

	sendk(cpid, 25);
	sendk(cpid, 26);
	
	kprintf("Message: %d\n", receivek());
	kprintf("Message: %d\n", receivek());

	resume(create(shell, 8192, 50, "shell", 1, CONSOLE));

	/* Wait for shell to exit and recreate it */

	recvclr();
	while (TRUE)
	{
		retval = receive();
		kprintf("\n\n\rMain process recreating shell\n\n\r");
		resume(create(shell, 4096, 1, "shell", 1, CONSOLE));
	}
	while (1)
		;

	return OK;
}

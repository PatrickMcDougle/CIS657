/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	uint32 retval;
	pid32 processID2;
	pid32 processID3;
	pid32 processID4;

	processID2 = create(lab5_process, 512, 5, "Process 2", 1, 5);
	processID3 = create(lab5_process, 512, 10, "Process 3", 1, 10);
	processID4 = create(lab5_process, 512, 20, "Process 4", 1, 20);

	resume(processID2);
	resume(processID3);
	resume(processID4);

	// resume(create(shell, 8192, 50, "shell", 1, CONSOLE));

	// /* Wait for shell to exit and recreate it */

	// recvclr();
	// while (TRUE) {
	// 	retval = receive();
	// 	kprintf("\n\n\rMain process recreating shell\n\n\r");
	// 	resume(create(shell, 4096, 1, "shell", 1, CONSOLE));
	// }
	// while (1);

	return OK;
}

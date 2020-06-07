/* finalexam.c - rocksteady, bebop, turtle */

#include <xinu.h>
#include <stdio.h>

int16 g_line_current = 0;
int16 g_line_next = 0;

/*------------------------------------------------------------------------
 *  rocksteady  -  a Rhino Mutant for the footclan
 *------------------------------------------------------------------------
 */
syscall rocksteady()
{
	int16 my_line = g_line_next++;
	char *my_speak = NULL;
	intmask mask; /* saved interrupt mask		*/

	while (TRUE)
	{
		mask = disable();

		switch (rand() % 1000)
		{
		case 1:
			my_speak = "Ooh, I'm gonna enjoy this!                    ";
			break;
		case 2:
			my_speak = "Hahaha! One smashed up chump, comin' RIGHT up!";
			break;

		default:
			my_speak = NULL;
			break;
		}
		print_output(my_line, "Rocksteady", my_speak);

		restore(mask);

		sleepms(1);
	}

	return OK;
}

/*------------------------------------------------------------------------
 *  bebop  -  a Warthog Mutant for the footclan
 *------------------------------------------------------------------------
 */
syscall bebop()
{
	int16 my_line = g_line_next++;
	char *my_speak = NULL;
	intmask mask; /* saved interrupt mask		*/

	while (TRUE)
	{
		mask = disable();

		switch (rand() % 1000)
		{
		case 1:
			my_speak = "Now we gotcha right where we want ya!";
			break;
		case 2:
			my_speak = "We got a score to settle!            ";
			break;

		default:
			my_speak = NULL;
			break;
		}
		print_output(my_line, "Bebop", my_speak);

		restore(mask);

		sleepms(1);
	}

	return OK;
}

/*------------------------------------------------------------------------
 *  shredder  -  The leader of the footclan
 *------------------------------------------------------------------------
 */
syscall shredder()
{
	int16 my_line = g_line_next++;
	char *my_speak = NULL;
	intmask mask; /* saved interrupt mask		*/

	while (TRUE)
	{
		mask = disable();

		switch (rand() % 1000)
		{
		case 1:
			my_speak = "Destroy them all! Ha ha. Aw it feels so good to be so bad.";
			break;
		case 2:
			my_speak = "Far from it. Tonight, I dine on turtle soup!              ";
			break;

		default:
			my_speak = NULL;
			break;
		}
		print_output(my_line, "Shredder", my_speak);

		restore(mask);

		sleepms(1);
	}
	return OK;
}

/*------------------------------------------------------------------------
 *  krang  -  a being from Demension X
 *------------------------------------------------------------------------
 */
syscall krang()
{
	int16 my_line = g_line_next++;
	char *my_speak = NULL;
	intmask mask; /* saved interrupt mask		*/

	while (TRUE)
	{
		mask = disable();

		switch (rand() % 1000)
		{
		case 1:
			my_speak = "If I had hands I would cover my ears, if I had them!               ";
			break;
		case 2:
			my_speak = "Sounds like the perfect job for a couple of yo-yos. I made a funny.";
			break;
		case 3:
			my_speak = "What have you to say now, mini mutants?                            ";
			break;

		default:
			my_speak = NULL;
			break;
		}
		print_output(my_line, "Krang", my_speak);

		restore(mask);

		sleepms(1);
	}
	return OK;
}

/*------------------------------------------------------------------------
 *  footsoldier  -  a being from Demension X
 *------------------------------------------------------------------------
 */
syscall footsoldier()
{
	int16 my_line = g_line_next++;
	char *my_speak = NULL;
	intmask mask; /* saved interrupt mask		*/
	char my_name[18];

	fprintf(my_name, "Footsoldier %d", my_line);

	while (TRUE)
	{
		mask = disable();

		switch (rand() % 5)
		{
		case 1:
			my_speak = "HI YAH!!!";
			break;
		case 2:
			my_speak = "Kiai!!!!!";
			break;

		default:
			my_speak = NULL;
			break;
		}
		print_output(my_line, &my_name[0], my_speak);

		restore(mask);

		sleepms(1);
	}
	return OK;
}

/*------------------------------------------------------------------------
 *  turtle  -  a Turtle Mutant that is starving.
 *------------------------------------------------------------------------
 */
syscall turtle()
{
	int16 my_line = g_line_next++;
	char *my_speak = NULL;
	intmask mask; /* saved interrupt mask		*/

	while (TRUE)
	{
		mask = disable();

		switch (rand() % 3)
		{
		case 1:
			my_speak = "Cowabunga!";
			break;
		case 2:
			my_speak = "Radical!  ";
			break;

		default:
			my_speak = NULL;
			break;
		}
		print_output(my_line, "Turtle", my_speak);

		restore(mask);

		sleepms(1);
	}
	return OK;
}

/*------------------------------------------------------------------------
 *  print_output  -  a helper function that works for everyone
 *------------------------------------------------------------------------
 */
void print_output(int16 my_line, char *my_name, char *my_speak)
{
	kprintf("%c", 13); // carriage return
	while (g_line_current < my_line)
	{
		kprintf("%c", 10); // line feed
		++g_line_current;
	}
	while (g_line_current > my_line)
	{
		kprintf("%c", 11); // vertical tab
		--g_line_current;
	}
	if (my_speak != NULL)
	{
		kprintf("%14s : %s", my_name, my_speak);
	}
	else
	{
		kprintf("%14s : ", my_name);
	}
}

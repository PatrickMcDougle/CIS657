/* finalexam.c - rocksteady, bebop, turtle */

#include <xinu.h>
#include <stdio.h>

#define SLEEP_TIME 2

int16 g_line_current = 0;
int16 g_line_next = 0;

/*------------------------------------------------------------------------
 *  rocksteady  -  a Rhino Mutant for the footclan
 *------------------------------------------------------------------------
 */
syscall rocksteady()
{
	int16 my_line = g_line_next++;
	char my_name[18];

	char *my_frases[] = {
		"Ooh, I'm gonna enjoy this!                    ",
		"Hahaha! One smashed up chump, comin' RIGHT up!"};
	int16 frases_length = 2;

	sprintf(my_name, "Rocksteady %d", my_line);

	continuous_jabber(my_line, my_name, my_frases, frases_length);

	return OK;
}

/*------------------------------------------------------------------------
 *  bebop  -  a Warthog Mutant for the footclan
 *------------------------------------------------------------------------
 */
syscall bebop()
{
	int16 my_line = g_line_next++;
	char my_name[18];

	char *my_frases[] = {
		"Now we gotcha right where we want ya!",
		"We got a score to settle!            "};
	int16 frases_length = 2;

	sprintf(my_name, "Bebop %d", my_line);

	continuous_jabber(my_line, my_name, my_frases, frases_length);

	return OK;
}

/*------------------------------------------------------------------------
 *  shredder  -  The leader of the footclan
 *------------------------------------------------------------------------
 */
syscall shredder()
{
	int16 my_line = g_line_next++;
	char my_name[18];

	char *my_frases[] = {
		"Destroy them all! Ha ha. Aw it feels so good to be so bad.",
		"Far from it. Tonight, I dine on turtle soup!              "};
	int16 frases_length = 2;

	sprintf(my_name, "Shredder %d", my_line);

	continuous_jabber(my_line, my_name, my_frases, frases_length);

	return OK;
}

/*------------------------------------------------------------------------
 *  krang  -  a being from Demension X
 *------------------------------------------------------------------------
 */
syscall krang()
{
	int16 my_line = g_line_next++;
	char my_name[18];

	char *my_frases[] = {
		"If I had hands I would cover my ears, if I had them!               ",
		"Sounds like the perfect job for a couple of yo-yos. I made a funny.",
		"What have you to say now, mini mutants?                            "};
	int16 frases_length = 3;

	sprintf(my_name, "Krang %d", my_line);

	continuous_jabber(my_line, my_name, my_frases, frases_length);
	
	return OK;
}

/*------------------------------------------------------------------------
 *  footsoldier  -  a being from Demension X
 *------------------------------------------------------------------------
 */
syscall footsoldier()
{
	int16 my_line = g_line_next++;
	char my_name[18];

	char *my_frases[] = {
		"HI YAH!",
		"Kiai!!!"};
	int16 frases_length = 2;

	sprintf(my_name, "Footsoldier %d", my_line);

	continuous_jabber(my_line, my_name, my_frases, frases_length);

	return OK;
}

/*------------------------------------------------------------------------
 *  turtle  -  a Turtle Mutant that is starving.
 *------------------------------------------------------------------------
 */
syscall turtle()
{
	int16 my_line = g_line_next++;

	char *my_frases[] = {
		"Cowabunga!  ",
		"Pizza power!",
		"Radical!    "};
	int16 frases_length = 2;

	sleepms(100);

	continuous_jabber(my_line, "Turtles", my_frases, frases_length);

	return OK;
}

void continuous_jabber(int16 my_line, char *my_name, char *my_frases[], int16 frases_length)
{
	int16 i;
	char *my_speak = NULL;
	intmask mask; /* saved interrupt mask		*/

	while (TRUE)
	{
		mask = disable();

		i = rand() % (frases_length + frases_length + frases_length + frases_length);
		if (i < frases_length)
		{
			my_speak = my_frases[i];
		}
		else
		{
			my_speak = NULL;
		}
		print_jabber(my_line, my_name, my_speak, getprio(getpid()));

		restore(mask);

		sleepms(SLEEP_TIME);
	}	
}

/*------------------------------------------------------------------------
 *  print_jabber  -  a helper function that works for everyone
 *------------------------------------------------------------------------
 */
void print_jabber(int16 my_line, char *my_name, char *my_speak, pri16 priority)
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
		kprintf("%14s [%d] %s", my_name, priority, my_speak);
	}
	else
	{
		kprintf("%14s [%d] ", my_name, priority);
	}
}

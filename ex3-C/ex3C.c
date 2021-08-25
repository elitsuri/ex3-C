/*
* ex3c.c: pipe - "Get to N"
*
*  Created on: Nov 26, 2017
*      Author: eliyho tsuri
*       Login: eliyhots
*		  Id: 201610672
*
* The program consists of three processes that father and sons
* Each of the processes of book series Grill (,1,0,-1)
* Any process that you Grill 0 sends him to father proocess,Each
* process promotes responsible when received.Or he writes to the pipe
* and the pipe to another process the following process gets the tube
* number reader And promote the eighth respectively.
* The first of the three processes that its count reaches N win
*
*/
// --------------------- Include Section -----------------------
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>  // for wait()
#include <unistd.h>    // for pipe()
#include <sys/types.h>
#include <stdbool.h>
#include <time.h>
// ------------------------- Define ----------------------------
#define NUM_OF_CHILDREN 2
#define N 2
// ------------------------ Prototype --------------------------
void create_process();
int set_number();
void do_child(int my_pipe[], int num);
// -------------------------- Main -----------------------------
int main()
{

	create_process(); // create_process

	return (EXIT_SUCCESS);
}
// --------------------- create_process ------------------------
/*
* This function create 2 processing (childs) each process or
* grilled kid No. and promotes its eight respectively, If the
* desired Grill is writing it to a pipe so that other processes read from.
*/
void create_process()
{
	pid_t status;
	int my_pipe[2];
	int count_first = 0;   // for first child
	int count_second = 0;  // for second child
	int count_father = 0;  // for the father
	int num; // for set num function

			 // create first process
	status = fork();
	if (status < 0)
	{
		perror("error in fork");
		exit(EXIT_FAILURE);
	}
	if (status == 0)
	{
		num = set_number();
		if (num < 0)
			count_first++;
		else
		{
			do_child(my_pipe, num);
			close(my_pipe[1]);
			read(my_pipe[0], &num, sizeof(int));
			if (num == 0)
				count_father++;
			else
				count_second++;
			close(my_pipe[0]);
		}
	}
	else
		wait(NULL); // father wait

					 // create second process
	status = fork();
	if (status < 0)
	{
		perror("error in fork");
		exit(EXIT_FAILURE);
	}
	if (status == 0)
	{
		num = set_number();
		if (num > 0)
			count_second++;
		else
		{
			do_child(my_pipe, num);
			close(my_pipe[1]);
			read(my_pipe[0], &num, sizeof(int));
			if (num == 0)
				count_father++;
			else
				count_first++;
			close(my_pipe[0]);
		}
	}
	else
		wait(NULL);  // father wait

					  // on this part if some of the counters getting N he print and finish
	if (count_first == N || count_second == N || count_father == N)
		printf("%d ", num);
	exit(EXIT_SUCCESS);
}
// ----------------------- do_child ---------------------------
/*
* This function getting the pipe and number (-1,0,1), the function
* write to the pipe the number and send that to the other process
*/
void do_child(int my_pipe[], int num)
{
	if (pipe(my_pipe) == -1)
	{
		perror("cannot open pipe");
		exit(EXIT_FAILURE);
	}
	else
	{
		close(my_pipe[0]);
		write(my_pipe[1], &num, sizeof(int));
		close(my_pipe[1]);
	}
}
// ----------------------- set_number --------------------------
/*
* This function setting numbers (-1,0,1) to the
* number if the function lottery 3 we return -1
*/
int set_number()
{
	int num;
	srand(time(NULL));
	num = (rand() % 3);
	if (num == 2)
		num = -1;
	return num;
}

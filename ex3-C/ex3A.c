/*
* ex3a.c: pipe - "Sorrting arrays"
*
*  Created on: Nov 22, 2017
*      Author: eliyho tsuri
*       Login: eliyhots
*		  Id: 201610672
*
* The father sets a value and make 10 times the following commands:
*  1) A data array Grill
*  2) Creates two children to sorts the array whit bubble/quick
*  3) Father gets the kids the average and sort through the pipe.
* The first child tax magril produced for the same number of non-sorted array
* The child writes to father the name search and the average search time
* And then the second child magril child begets a number for the number
* stored by using binary search
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
#include <string.h>
// ------------------------- Define ----------------------------
#define SIZE 10
#define NUM_OF_CHILDS 2
#define N 80
// ------------------------ Prototype --------------------------
void set_numbers(int my_array[]);
void sort_array(int my_array[]);
void serial_search(int my_array[]);
void bubble_sort(int pipe_fd[], int my_array[]);
void quick_sort(int my_array[], int from, int to);
void binary_search(int my_array[]);
void swap(int my_array[], int big, int small);
int partition(int my_array[], int from, int to);
void do_parent(int pipe_fd[], pid_t status[], int size);
// -------------------------- Main -----------------------------
int main()
{
	int my_array[SIZE];

	int index;

	for (index = 0; index < SIZE; index++)
	{
		set_numbers(my_array);
		sort_array(my_array);
	}
	return (EXIT_SUCCESS);
}
// ---------------------- sort_array ---------------------------
/*
* Each round in a loop the function takes an array and sorts
* it in two ways, bubble sort and quick sort ,then the children
* passed, how long it took him to imlav sort the arrays in two ways.
* The kids pass the string and search time parent through the pipe.
*/
void sort_array(int my_array[])
{
	pid_t status[NUM_OF_CHILDS]; // for childs
	int pipe_fd[2]; // for pipe
	time_t start, end; // for the time sort
	int child, time_sort; // num child,sort time
	char str_bubble[] = "Bubble Sort:"; // string to father
	char str_quick[] = "Quick Sort:";   // string to father




	for (child = 0; child < NUM_OF_CHILDS; child++)
	{
		if (pipe(pipe_fd) == -1)
		{
			perror("cannot open pipe");
			exit(EXIT_FAILURE);
		}
		status[child] = fork();
		if (status[child] < 0)
		{
			perror("error in fork");
			exit(EXIT_FAILURE);
		}
		if (status[child] == 0)
		{
			if (child == 0)
			{
				serial_search(my_array);
				close(pipe_fd[0]);
				write(pipe_fd[1], str_bubble, strlen(str_bubble));
				start = time(NULL);
				bubble_sort(pipe_fd, my_array);
				end = time(NULL);
				time_sort = (int)end - (int)start;
				write(pipe_fd[1], &time_sort, sizeof(int));
				close(pipe_fd[1]);
			}
			if (child == 1)
			{
				close(pipe_fd[0]);
				write(pipe_fd[1], str_quick, strlen(str_quick));
				start = time(NULL);
				quick_sort(my_array, 0, SIZE - 1);
				binary_search(my_array);
				end = time(NULL);
				time_sort = (int)end - (int)start;
				write(pipe_fd[1], &time_sort, sizeof(int));
				close(pipe_fd[1]);
			}
		}
		else
			do_parent(pipe_fd, status, NUM_OF_CHILDS);
	}
}
// ----------------------- do_parent ---------------------------
/*
* Parent function gets the pipe where children claimed the string
* and time their search the pid and the brothers size The father
* is waiting until all the children are finished, and then
* prints the string and the time taken searching or sorting
*
*/
void do_parent(int pipe_fd[], pid_t status[], int size)
{
	char buff[N];
	int child;
	int average = 0;

	close(pipe_fd[1]);
	for (child = 0; child < size; child++)
	{
		waitpid(status[child], NULL, 0);
		average = read(pipe_fd[0], buff, N);
	}
	printf("%s %d \n", buff, average / SIZE);
	close(pipe_fd[0]);
	exit(EXIT_SUCCESS);
}
// ---------------------- serial_search ------------------------
/*
* this function getting int array, rand number,and search number on the
* array, the function print how long it took him find the wanted number
*/
void serial_search(int my_array[])
{
	int pipe_fd[2];
	pid_t status[SIZE];
	char str_serial[] = "Serial Search:";
	int index;
	int sort_time;
	bool found = false;



	for (index = 0; index < SIZE; index++)
	{
		if (pipe(pipe_fd) == -1)
		{
			perror("cannot open pipe");
			exit(EXIT_FAILURE);
		}
		int wanted = rand() % SIZE;

		status[index] = fork();
		if (status[index] < 0)
		{
			perror("error in fork");
			exit(EXIT_FAILURE);
		}
		if (status[index] == 0)
		{
			time_t start = time(NULL);
			if (my_array[index] == wanted)
			{
				found = true;
				break;
			}
			time_t end = time(NULL);
			sort_time = (int)end - (int)start;
			close(pipe_fd[0]);
			write(pipe_fd[1], str_serial, strlen(str_serial));
			write(pipe_fd[1], &sort_time, sizeof(int));
			close(pipe_fd[1]);
		}
		else
			do_parent(pipe_fd, status, SIZE);
	}
}
// ---------------------- binary_search ------------------------
/*
* this function getting array ,the function search on the array
* wanted number the function write the time fort on the pipe
*/
void binary_search(int my_array[])
{
	int pipe_fd[2];
	pid_t status[SIZE];
	char str_binary[] = "Binary Search:";
	int sort_time;
	int index;
	int hi = SIZE - 1;
	bool found = false;

	for (index = 0; index < SIZE; index++)
	{
		if (pipe(pipe_fd) == -1)
		{
			perror("cannot open pipe");
			exit(EXIT_FAILURE);
		}
		int wanted = rand() % SIZE;

		status[index] = fork();
		if (status[index] < 0)
		{
			perror("error in fork");
			exit(EXIT_FAILURE);
		}
		if (status[index] == 0)
		{
			time_t start = time(NULL);
			while (index <= SIZE - 1 && !found)
			{
				int mid = (index + hi) / 2;
				if (my_array[mid] == wanted)
					found = true;
				else if (my_array[mid] < wanted)
					index = mid + 1;
				else
					hi = mid - 1;
			}
			time_t end = time(NULL);
			sort_time = (int)end - (int)start;
			close(pipe_fd[0]);
			write(pipe_fd[1], str_binary, strlen(str_binary));
			write(pipe_fd[1], &sort_time, sizeof(int));
			close(pipe_fd[1]);
		}
		else
			do_parent(pipe_fd, status, SIZE);
	}
}
// ---------------------- bubble sort --------------------------
/*
* this function getting primes numbers array and the
* function sorting the array whit bubble sort algorithm
*/
void bubble_sort(int pipe_fd[], int my_array[])
{
	int round, place;

	for (round = 0; round < SIZE; round++)
	{
		for (place = 0; place < SIZE - round - 1; place++)
			if (my_array[place] > my_array[place + 1])
				swap(my_array, place, place + 1);
	}
}
// ----------------------- quick_sort --------------------------
/*
* this function getting array and sorting the array whit
* quick_sort the function return the array sortting
*/
void quick_sort(int my_array[], int from, int to)
{
	int pivot_place = partition(my_array, from, to);

	if (from < pivot_place - 1)
		quick_sort(my_array, from, pivot_place - 1);

	if (to > pivot_place + 1)
		quick_sort(my_array, pivot_place + 1, to);
}
// ----------------------- partition ---------------------------
/*
* this function called by quick sort the function swap the numbers
*/
int partition(int my_array[], int from, int to)
{
	int index;
	int pivot_place = from;

	for (index = from + 1; index <= to; index++)
		if (my_array[index] < my_array[pivot_place])
		{
			swap(my_array, index, pivot_place + 1);
			swap(my_array, pivot_place + 1, pivot_place);
			pivot_place++;
		}
	return pivot_place;
}
// ---------------------- set_numbers --------------------------
/*
* This function getting empty array numbers and set numbers
*/
void set_numbers(int my_array[])
{
	int index;

	srand(time(NULL));
	for (index = 0; index < SIZE; index++)
		my_array[index] = rand() % SIZE;

}
// ------------------------- swap ------------------------------
/*
* this function getting int array, small number, big number
* the function swap the small and big numbers
*/
void swap(int my_array[], int big, int small)
{
	int temp = my_array[big];
	my_array[big] = my_array[small];
	my_array[small] = temp;
}

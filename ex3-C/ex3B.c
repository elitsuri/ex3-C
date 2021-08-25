/*
* ex3b.c: pipe - "Primes numbers"
*
*  Created on: Nov 22, 2017
*      Author: eliyho tsuri
*       Login: eliyhots
*		  Id: 201610672
*
* The father sets a value in a particular size and Grill into
* numbers the father create NUM_OF_CHILDREN Which will test at
* the same time the data in the array and display the first number
* found in Each child is responsible for a certain number of prime
* numbers that are in the array Each child moves through the pipe
* the father the primes for which it was responsible,The father
* receives the first array sorts it using bubble sort and print it.
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
#include <math.h>
// ------------------------- Define ----------------------------
#define ARR_SIZE 10
#define NUM_OF_CHILDREN 2
// ------------------------ Prototype --------------------------
void set_numbers(int my_array[]);
void create_process(int my_array[], int child);
bool check_prime(int num);
void do_child(int my_pipe[], int my_array[], int child);
void do_parent(int my_pipe[]);
void bubble_sort(int my_array[]);
void swap(int my_array[], int big, int small);
void print(int my_array[]);
pid_t status;
// -------------------------- Main -----------------------------
int main()
{
	int my_array[ARR_SIZE] = { 0 };  // for set numbers
	int child;               // for create child's

	set_numbers(my_array);  // set number function

							 // we create NUM_OF_CHILDREN child's
	for (child = 0; child < NUM_OF_CHILDREN; child++)
		create_process(my_array, child);

	return (EXIT_SUCCESS);
}
// -------------------- create_children ------------------------
/*
* The function takes an array of numbers, and the number of
* child each round loop function produces a child and send
* the array, pipe and the num child to do_child function
*/
void create_process(int my_array[], int child)
{

	status = fork();
	int my_pipe[2];

	if (pipe(my_pipe) == -1)
	{
		perror("cannot open pipe");
		exit(EXIT_FAILURE);
	}
	if (status < 0)
	{
		perror("error in fork");
		exit(EXIT_FAILURE);
	}
	if (status == 0)
	{
		do_child(my_pipe, my_array, child);
		wait(NULL);
	}
}
// ------------------------ do_child ---------------------------
/*
* The function takes an array pipe , array of numbers, and the
* number of child, each round loop function produces a child checks
* are the first numbers are in and if the remainder of the number in
* the number of all children is a boy, if a child puts the number to
* other tube passes then the boy sends his array of the primes array
*/
void do_child(int my_pipe[], int my_array[], int child)
{
	int index;
	int primes[ARR_SIZE] = { 0 };
	int index_prime = 0;


	for (index = 0; index < ARR_SIZE; index++)
	{
		if (check_prime(my_array[index]) &&
			child == my_array[index] % NUM_OF_CHILDREN)
		{
			close(my_pipe[0]);
			write(my_pipe[1], &my_array[index], sizeof(int));
			close(my_pipe[1]);

			close(my_pipe[1]);
			read(my_pipe[0], &my_array[index], sizeof(int));
			close(my_pipe[0]);

			primes[index_prime] = my_array[index];
			index_prime++;
		}
	}
	do_parent(primes);
}
// ----------------------- do_parent ---------------------------
/*
* This function getting the primes numbers from the son,
* the function sort the array and after that print the array
*/
void do_parent(int primes_numbers[])
{
	bubble_sort(primes_numbers);
	print(primes_numbers);
	exit(EXIT_SUCCESS);
}
// ---------------------- set_numbers --------------------------
/*
* This function getting empty array numbers and set numbers
*/
void set_numbers(int my_array[])
{
	int index;

	srand(time(NULL));
	for (index = 0; index < ARR_SIZE; index++)
		my_array[index] = rand() % ARR_SIZE;
}
// --------------------- check_prime ---------------------------
/*
* This function getting number and check if the number is prime
*/
bool check_prime(int num)
{
	int div;
	if (num == 0 || num == 1)
		return false;
	for (div = 2; div <= num / 2; div++)
		if (num % div == 0)
			return false;

	return true;
}
// ---------------------- bubble sort --------------------------
/*
* this function getting primes numbers array and the
* function sorting the array whit bubble sort algorithm
*/
void bubble_sort(int my_array[])
{
	int round, place;

	for (round = 0; round < ARR_SIZE; round++)
	{
		for (place = 0; place < ARR_SIZE - round - 1; place++)
			if (my_array[place] > my_array[place + 1])
				swap(my_array, place, place + 1);
	}
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
// ------------------------- print -----------------------------
/*
* This function getting the array primes and print all the prime number
*/
void print(int my_array[])
{
	int index;
	for (index = 0; index < ARR_SIZE; index++)
		if (my_array[index] != 0)
			printf("%d ", my_array[index]);
	printf("\n");
}

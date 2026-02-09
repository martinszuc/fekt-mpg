/*
MPC-MPG 
1st Laboratory: introduction to C++
made by: Michal Svento, Ondrej Mokry

this is
a multiline comment

*/

// this is a single line comment

// Including libraries into your script
#include <iostream>
// #include "mylocalheader.h"		// use "" for local header files, <> for standard libraries


using namespace std;	// calling just cout, not std::cout => cleaner code

int num_array[10] = { 1,3,3,4,2,6,7,8,9,10 };   // global variable, visible to every function in this file


/* Functions */

/* 
Function 'multiply' without pointer
input: 2 int; output: 1 int 
*/

// Task 4 
// TODO: Here start your code

              





/*
Sum function
This function will sum up all values in an array
input: 1 int array, return   
*/



int sum(int number_array[10], int arr_size) {

	// Local variables, after function call vars no longer exist.
	int sum = 0; // sum init 
	int i; // if I want to use i outside the for cycle

	// cout << "index\tnumber\tsum\n"; // Uncomment if you want to print a table header
	
	// Task 5: "for" cycle through array
	// You can use indexing using array[index]
	// TODO: Here starts your code









	return sum;
}

/* Functions using pointers */


/* function: multiply with pointer
input: 2 int, 1 pointer to int; output: void */
void multiply_ptr(int a, int b, int* c)
{
	*c = a * b;
	// Void doesn't return anything
}
/* Sum and mean

function: Just as an illustration  */

void sum_and_mean(int* number_array,int* sum, float* mean, int arr_size) {

	// pointer init
	int* ptr = number_array;
	int i; // if I want to use i outside of the for cycle
	cout << "\nArr size " << arr_size << endl;
	cout << "index\tnumber\tsum\n";
	// for cycle over the whole array
	for (i = 0; i < arr_size; i++)
	{
		*sum += *(ptr + i);
		cout << i << "\t" << * (ptr + i) << "\t" << *sum << endl;
	}
	*mean = float(*sum) / i; // pointer to mean variable / overwriting the value at the given address
}

int main() {


	/* Variables */ 
	// Both local
	int a = 1;	// with initialization
	int b;	// without initialization - other types float, string, boolean;	
	
	/*
		Math operations: 
		+		-  
		*		/
		% modulo operation
	*/

	// Console print, function cout
	cout << "This is MPG lab #" << a << endl<< "Task 1" << endl;
	 /*
	 Task 1
		Divide two numbers
		Use vars a,b
		Initialize ans as the result of a divided by b
		Then print out in console
	*/

	b = 4;
	float ans = 0.0;
	// Task 1 
	// TODO: Here start your code
	// Advice: always watch out for the types
	ans = (float)a / b;


	cout << "The division " << a << "/" << b << " should be 0.25 and it is: " <<  ans << "\n\n";


	// Global vars: see in the beginning, num_array
	
	/* Pointers */
	// make connection with address where the variable is stored
	// one of multiple usages: variant of the multiply function, without return, storing solution in previously defined variable
	
	// Using pointers in arrays
	int * ptr = num_array;		// In ptr the address of the first value is stored
	// int * ptr = &num_array[0];  Equivalent to previous line
	
	*(ptr + 4) = 5;				// using * the value at address ptr + 4 is changed to 5
	//ptr = &num_array[4]; An equivalent approach
	//*ptr = 5;
	
	cout << "Task 2" << endl;
	/* 
		Task 2
			Change the second value in the array to 2
			Use pointer ptr or initialize a new pointer
	*/

	// Task 2
	// TODO: Here start your code


	cout << "The second entry in the array should be 2 and it is: " << num_array[1] << "\n \n";
	
	/*
	How to write conditions:
		==	equal			!= different
		>	greater			< smaller
		>=  greater or eq  <= smaller or eq

		also boolean: and, or                   
	*/

	cout << "Task 3" << endl;
	/*
		Task 3
			Verify if a number is positive and divisible by 3
			Use if, else
			Print the statement in console
	*/

	int val = 7;
	// Task 3
	// TODO:  Here start your code
	



	/* Functions */
	// should be defined before the main function, since code is compiled from top to bottom

	
	cout << "\nTask 4" << endl;
	/*
		Task 4
			Write the function for multiplication at the beginning of this file
			It should take two integers as the input and return one integer
	*/

	int c = 0, d = 0;
	// uncomment this block if you are happy with your multiply function
	// Task is at the beginning 
	//c = multiply(a, b); // classic function with int return
	cout << "Product of " << a << " and " << b << " without ptr is " << c << ".\n"; // Print in console


	// Example of the same function using pointers
	multiply_ptr(a, b, &d); // & operator returns the address of what follows
	cout << "Product of " << a << " and " << b << " with ptr is " << d << endl; // Print in console

	/* Conditions */
	if (c == d) {
		cout << "Functions have identical outputs \n";
	}
	else
	{
		cout << "Bad programmer, it should be the same! \n";
	}


	cout << "\nTask 5" << endl;
	/*
		Task 5
			Complete the sum function with a for cycle
			Task is under task 4
	*/
	
	int arr_size = sizeof(num_array) / sizeof(int);		// Size of array
	// Sum function
	int sum_0 = sum(num_array, arr_size);
	cout << "Sum of array is: " << sum_0 << ".\n";
	// not so good for memory

	// Function 
	// INPUT: array of numbers, address of mean - we assign a number to this var;
	// OUTPUT: sum as normal return and mean as pointer value.
	// Uncomment below for an equivalent solution using pointers
	//float mean;
	//int sum_1 = 0;
	//sum_and_mean(num_array, &sum_1 ,&mean, arr_size);
	//cout << "Sum of array is: " << sum_1 << ". Mean of array is " << mean <<".\n";




	cout << "\nTask 6" << endl;
	/*
		 Task 6
		1. Create your own header file mylocalheader.h
		2. Define there a 2D array 5x5 with hardcoded values
		3. Include the header file in this script
		4. Calculate the sum of all values in this 2D array here in the main function
	*/

	// Task 6 -- write your code here

	return 0;
}
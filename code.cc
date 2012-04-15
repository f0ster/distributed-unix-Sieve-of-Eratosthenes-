//Simple c++ implementation for the Sieve of Eratosthenes
//A prime-finding algorithm:
//--------------------------
//	Given a list of n integers greater than 2
//	For each integer in that list, remove all multiples greater than it
//	Move to the next remaining integer and repeat this
//	Stop iterating after sqrt(n) iterations
//--------------------------
//This is not the best implementation but it isn't half bad either
//Took about 2 seconds to sieve through 1000000 integers on my computer
//
//Written by Matt Squire :: www.insideReality.net
//Written and released to the public for educational purposes (for both my own and others)
//Free non-commercial use is permitted


#define LISTSIZE 10000000	//Number of integers to sieve
#include <stdio.h>
#include <math.h>

int main()
{
	long long list[LISTSIZE];
	long long listMax = (long long)sqrt(LISTSIZE), primeEstimate = (long long)(LISTSIZE/log(LISTSIZE));

	//Generate list of LISTSIZE long longegers >= 2
	for(long long i=0; i < LISTSIZE; i++)
		list[i] = i+2;

	//Go through list finding primes
	for(long long i=0; i < listMax; i++)
	{
		//If the entry has been set to 0 ('removed'), skip it
		if(list[i] > 0)
		{
			//Remove all multiples of this prime
			//Starting from the next entry in the list
			//And going up in steps of size i
			for(int j = i+1; j < LISTSIZE; j++)
			{
				if((list[j] % list[i]) == 0)
					list[j] = 0;
			}
		}
	}

	//Output the primes
	int primesFound = 0;
	printf("Primes for %d integers\n", LISTSIZE);
	for(int i=0; i < LISTSIZE; i++)
	{
		if(list[i] > 0)
		{
			primesFound++;
			//It could be wise to comment this out for extremely large lists
			printf("-- %d --\n", list[i]);
		}
	}
	printf("Total primes found: %d (estimate was %d)\n", primesFound, primeEstimate);

	return 0;
}



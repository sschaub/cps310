/*
Solution for the extra-credit homework which enhances meltdownPrerequisites--v2.cpp

Compile:  gcc -O0 -march=native meltdownPrerequisites--v2--homework-solution.cpp
* -O0 is capital "o" followed by a zero.  It turns-off optimization.
* Turning-off optimization avoids problems from the compiler leaving-out
  statements that it might percieve as doing nothing.

Run:  ./a.out
Tested on:  Ubuntu 18.04.3


Sample output, just shows the tail-end:

Failed deduction:  Iteration:9650. Random-number:164.  Read-times:  fastest:383, second-fastest:385, random-number-page:422
Failed deduction:  Iteration:9821. Random-number:235.  Read-times:  fastest:374, second-fastest:374, random-number-page:440

Number of iterations:10000  Failed deductions:  number:96,  fraction:0.010

*/
#include <emmintrin.h>
#include <x86intrin.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <float.h>
#include <time.h>

#define NUMBER_OF_ITERATIONS 10000
#define PAGE_SIZE 4096
#define CACHE_LINE_SIZE 64
#define DEBUG 0 // To turn-on, set to 1.  Otherwise, set to 0.


// Struct for parms, for function findPageWithFastestRead()
struct findPageWithFastestRead_parms {
    // Pointer to 256-pages of memory (input)
    u_int8_t * twoHundredFiftySixPages;
    // randomNumber we're tring to deduce (input)
    int randomNumber;
    // Calculated read-times (output)
    u_int64_t fastestTime;
    u_int64_t secondFastestTime;
    u_int64_t randomNumberPageTime;
    // Page with minimum read-time (output)
    int pageWithMinimumTime;
};


// For each page, read the first byte
// Returns the index of the page with the fastest read
void findPageWithFastestRead(struct findPageWithFastestRead_parms *parms)
{
    unsigned int junk=0;
    register u_int64_t time1, time2;
    volatile u_int8_t *readAddress;
    int i;

    parms->fastestTime = __UINT64_MAX__;
    parms->secondFastestTime = __UINT64_MAX__;

    // Loop through all pages
    // For each page, read the first byte and determine how long it takes.
    // If the page's read is the fastest or second-fastest thus far, record that.
    for(i = 0; i < 256; i++){
        // Compute address of value to read from probe array
        readAddress = &(parms->twoHundredFiftySixPages[i*PAGE_SIZE]); // What about i * CACHE_LINE_SIZE?
        
        // Read the CPU’s timestamp counter (TSC) before the memory read
        time1 = __rdtscp(&junk);
        
        // Read first byte in the page
        junk = *readAddress;
        
        // Read the CPU’s TSC after the memory read.
        // Their difference is the time (in terms of number of CPU cycles) 
        // spent in the memory read.        
        time2 = __rdtscp(&junk) - time1;
    
        // If this is the page for the randomNumber, record it's read-time
        if (i == parms->randomNumber){
            parms->randomNumberPageTime = time2;
        }
        
        // Record if this page is the fastest read thus far.
        if ( time2 < parms->fastestTime ) {
            parms->secondFastestTime = parms->fastestTime;
            parms->fastestTime = time2;
            parms->pageWithMinimumTime = i;
        }
        // This else-if fixes a bug in meltdownPrerequisites.cpp
        // Record if this page is the second-fastest read thus far.        
        else if ( time2 < parms->secondFastestTime ) {
            parms->secondFastestTime = time2;
        }
    }
    
    return;
    
}


int main()
{
	int i, j, returnValue, randomNumber;
    int numberOfFailedDeductions = 0;
	u_int8_t byteValue;
    struct findPageWithFastestRead_parms parms;
    float fractionOfFailedDeductions;

    // Pointer to 256-pages of memory
    u_int8_t * twoHundredFiftySixPages;

	// Allocate 256 pages of memory, on a page boundary
	//
	// posix_memalign() is like malloc() and new(), but memory is aligned.
	// * First parameter (twoHundredFiftySixPages) is the address of the 
	//   pointer twoHundredFiftySixPages
	// * Second parameter (PAGE_SIZE) says to align memory on a page boundary
	// * Third parameter (256 * PAGE_SIZE) is size to allocate, in bytes
	//
	returnValue = posix_memalign( (void **)&twoHundredFiftySixPages, 
	                              PAGE_SIZE, 
	                              (256 * PAGE_SIZE) );
	if ( returnValue != 0 ){
		perror("posix_memalign() failed");
        exit (0);
	}


    // Seed random-number generator
    srand(time(0));


    // Loop to repeatedly generate a random number, then
    // deduce it by using the memory cache contents.
    // For each iteration, print needed results and collect needed stats.
    for (j = 1; j <= NUMBER_OF_ITERATIONS; j++){
    

        // Generate a random number between 0 and 255.  
        // The random number is not needed for the exploit.  
        randomNumber = rand() % 256;

        
        // For the 256-pages, ensure they are flushed from the memory-cache.
        // clflush() clears a cache-line (64 btyes), for the memory-address specified
        for (i = 0; i < (256 * PAGE_SIZE); i += PAGE_SIZE) {
                asm volatile("clflush (%0)\n\t"
                             : 
                             : "r"(&twoHundredFiftySixPages[i])
                             : "memory");
        }


        // Now, we'll use the cache to determine what randomNumber is.
        // We could just print randomNumber.
        // Using the cache to determine randomNumber will illustrate how  
        // the exploit works.
        // The exploit will do something very similar.

        
        // In the 256-pages, read the first byte in page-number randomNumber.
        // This will put that byte in the memory-cache
        byteValue = twoHundredFiftySixPages[(randomNumber * PAGE_SIZE)];

        
        // Next, read the first byte in every page.
        //
        // The byte read at offset (randomNumber * PAGE_SIZE) will load from cache.
        // All other reads will not be from cache.
        //
        // So, the byte read at offset (randomNumber * PAGE_SIZE) will be much faster
        // than all other reads.
        //
        // Determine the time elapsed for each read.
        // The read that is fastest will be from page-number N.
        // N should be the same as randomNumber.

        parms.twoHundredFiftySixPages = twoHundredFiftySixPages;
        parms.randomNumber = randomNumber;
        findPageWithFastestRead(&parms);

        // If deduction failed, record it
        if ( parms.pageWithMinimumTime != randomNumber ){
            numberOfFailedDeductions++;
            printf("Failed deduction:  ");
        }
        if (DEBUG || (parms.pageWithMinimumTime != randomNumber)){
            printf("Iteration:%d. Random-number:%d.  "
                   "Read-times:  fastest:%lu, second-fastest:%lu, " 
                   "random-number-page:%lu\n", 
                   j, randomNumber, 
                   parms.fastestTime, parms.secondFastestTime, parms.randomNumberPageTime);
        }

    } // END of:  for-loop

    fractionOfFailedDeductions = (float) numberOfFailedDeductions / (float) NUMBER_OF_ITERATIONS;

    printf("\nNumber of iterations:%d  Failed deductions:  number:%d,  fraction:%.3f\n", 
           NUMBER_OF_ITERATIONS, numberOfFailedDeductions, fractionOfFailedDeductions);

	return 0;

}
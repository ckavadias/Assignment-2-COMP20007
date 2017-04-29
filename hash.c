/*
 Constantinos Kavadias, ckavadias@student.unimelb.edu.au
 STUDENT ID 664790, Assignment 2, hash.c module, COMP20007

  >> warnings will appear at compilation in regards to passing unsigned
 	char to char variables, this is only done in instances where numeric 
 	comparison or data copying is required and so the warnings can be ignored
*/
#include "hash.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAXSTRLEN 256

/*random factor array globally defined for Univesal Hash*/
unsigned int randoms[MAXSTRLEN];

/*function as described in extra.c module*/
extern void debug(char* message, unsigned int num);

/* Used as the second hashing function on double hash */
unsigned int linear_probe(void *e, unsigned int size) {
	/*simply increase current hash by one to make a linear step*/
	(void)e;
	(void)size;
    return 1;
}

/* Very simple hash */
unsigned int worst_hash(void *e, unsigned int size) {
    (void) e;
    (void) size;
    return 0;
}

/* Basic numeric hash function */
unsigned int num_hash(long n, unsigned int size) {
    return n % size;
}

/* Bad hash function */
unsigned int bad_hash(char *key, unsigned int size) {
	static unsigned int a = 0;
	
	debug("bad_hash started", 0);
	
	/* randomly generate a, only perform once to 
	ensure static nature maintained for future hashes*/
	if(!a){
		a = rand();
		a %= size;
	}
	debug("The value of a is:", a);
	
    return (a*key[0])%size;
}

/* Universal hash function as described in Dasgupta et al 1.5.2 */
unsigned int universal_hash(unsigned char *key, unsigned int size) {
	static int constructed = 0, function_calls = 0;
	unsigned int character, hash = 0;
	
	function_calls++;
	debug("\nuniversal_hash started", function_calls);
	debug("", 0);
	
	/*if randoms has not already been populated, populate array it is important
	this occurs only once to maintain static nature of the array*/
	if(!constructed){
		for( character = 0; character < MAXSTRLEN; character++){
		randoms[character] = rand()%size;
		}
		constructed = 1;
	}
	
	/*create hash from summation as in Dasgupta et al 1.5.2*/
	character = 0;
	debug("String is: ", 0);
	debug((char*)key, 0);
	
	while(key[character]){
		debug("Factor i is: ", randoms[character]);
		
		hash += (key[character]*randoms[character]);
		character++;
		
		debug("Character count: ", character);
	}
	hash%=size;
	
	debug("String length: ", character);
	debug("HASH IS: ", hash);
	
    return hash;
}

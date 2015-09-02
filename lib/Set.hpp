//
//  Set.h
//  MST
//
//  Created by Cristiano Sousa on 12/11/13.
//  Copyright (c) 2013 Cristiano Sousa. All rights reserved.
//

#ifndef SET_HEADER
#define SET_HEADER

#include <stdlib.h>
#include <string.h>

typedef struct Set{
	unsigned int size;
	unsigned int *elems;
} Set;


typedef struct Sets{
	Set *sets;
	unsigned int *node_set;
	unsigned int nnodes;
	unsigned int n_sets;
	unsigned int max_sets;
	
	
	void make_set(unsigned int val);
	void allocate();
	void deallocate();
	void set_union(unsigned int a, unsigned int b);
	unsigned int find_set(unsigned int val);
	Sets(unsigned int nnodes);
	
	~Sets();
	
} Sets;

#endif
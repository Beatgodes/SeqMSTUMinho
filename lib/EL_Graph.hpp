//
//  AL_Graph.h
//  MST
//
//  Edge list, used for construction and when sort by edges is needed
//  Created by Cristiano Sousa on 17/11/13.
//  Copyright (c) 2013 Cristiano Sousa. All rights reserved.
//

#ifndef EL_GRAPH_HEADER
#define EL_GRAPH_HEADER

#include "CSR_Graph.hpp"

typedef struct edge{
	unsigned int src;
	unsigned int dst;
	unsigned int weight;
} Edge;

typedef struct EL_Graph{
	unsigned int nnodes;
	unsigned int nedges;
	Edge *edges;
	
	
	void readFromFile(char *filename);
	void writeToFile(char *filename);
	void fromCSR(CSR_Graph *g);
	void allocate();
	void deallocate();
	
	EL_Graph(CSR_Graph *g);
	EL_Graph(char *filename);
	EL_Graph(unsigned int nnodes, unsigned int nedges);
	
	~EL_Graph();
	
} EL_Graph;

int compareEdgeWt(const void * a, const void * b);

#endif
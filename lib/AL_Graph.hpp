//
//  AL_Graph.h
//  MST
//
//  Adjacency list, used for construction
//  Created by Cristiano Sousa on 17/11/13.
//  Copyright (c) 2013 Cristiano Sousa. All rights reserved.
//

#ifndef AL_GRAPH_HEADER
#define AL_GRAPH_HEADER

#include <stdlib.h>
#include <vector>
#include "CSR_Graph.hpp"

typedef struct AL_Edge{
	unsigned int dst;
	unsigned int weight;
	struct AL_Edge *next;
	//AL_Edge(unsigned int dst, unsigned int weight);
	
} AL_Edge;

/*
AL_Edge::AL_Edge(unsigned int dst, unsigned int weight){
	this->dst = dst;
	this->weight = weight;
}
*/
typedef struct AL_Graph{
	unsigned int nnodes;
	unsigned int nedges;
	unsigned int *outdegree;
	AL_Edge **nodes;
	AL_Edge *pool;
	//unsigned int pool_top;

	
	void insert_edge(unsigned int src, unsigned int dst, unsigned int weight);
	void insert_edge_nolock(unsigned int src, unsigned int dst, unsigned int weight);
	
	void insert_pair(unsigned int src, unsigned int dst, unsigned int weight);
	void insert_pair_nolock(unsigned int src, unsigned int dst, unsigned int weight);

	void pooled_insert_edge(unsigned int src, unsigned int dst, unsigned int weight);
	void pooled_insert_edge_nolock(unsigned int src, unsigned int dst, unsigned int weight);
	
	void pooled_insert_pair(unsigned int src, unsigned int dst, unsigned int weight);
	void pooled_insert_pair_nolock(unsigned int src, unsigned int dst, unsigned int weight);
	
	void allocate();
	void deallocate();
	CSR_Graph* toCSR();

	AL_Graph(unsigned int nnodes, unsigned int nedges);
	AL_Graph(unsigned int nnodes);
	
	~AL_Graph();
	
} AL_Graph;



#endif

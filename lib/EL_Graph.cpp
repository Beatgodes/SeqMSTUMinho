#include "EL_Graph.hpp"

EL_Graph::~EL_Graph(){
	deallocate();
}

/**
 * Transforms a CSR_Graph to an EL_Graph
 *
 **/
EL_Graph::EL_Graph(CSR_Graph *g){
	fromCSR(g);
}

/**
 * Creates a new EL_Graph from a file.
 * Uses an intermedite CSR_Graph to do so
 **/
EL_Graph::EL_Graph(char *filename){
	CSR_Graph *g = new CSR_Graph(filename);
	fromCSR(g);
	delete g;
}

/**
 * Allocates an empty EL_Graph with a given size
 *
 **/
EL_Graph::EL_Graph(unsigned int nnodes, unsigned int nedges){
	this->nnodes = nnodes;
	this->nedges = nedges;
	allocate();
}

/**
 * Allocates the necessary memory for the EL_Graph
 * Needs to have nnodes and nedges set
 **/
void EL_Graph::allocate(){
	edges = (Edge*)malloc(sizeof(Edge) * nedges);
}

/**
 * Frees up memory
 *
 **/
void EL_Graph::deallocate(){
	free(edges);
}

/**
 * Creates an EL_Graph from a CSR_Graph
 *
 **/
void EL_Graph::fromCSR(CSR_Graph *g){
	nnodes = g->nnodes;
	nedges = g->nedges;
	
	allocate();
	
	unsigned int i = 0, j = 0;
	unsigned int curr_edge = 0;
	for(i = 0; i < nnodes; i++)
	{
		for(j = 0; j < g->getOutDegree(i); j++)
		{
			edges[curr_edge].src = i;
			edges[curr_edge].dst = g->getDestination(i, j);
			edges[curr_edge].weight = g->getWeight(i, j);
			
			curr_edge++;
		}
	}
}


int compareEdgeWt(const void * a, const void * b)
{
	Edge *aa = (Edge*)a;
	Edge *bb = (Edge*)b;
	
	if(aa->weight < bb->weight) return -1;
	if(aa->weight > bb->weight) return 1;
	
	return 0;
}

int compareEdgeSrcDst(const void * a, const void * b)
{
	Edge *aa = (Edge*)a;
	Edge *bb = (Edge*)b;
	
	if(aa->src < bb->src) return -1;
	if(aa->src > bb->src) return 1;
	
	if(aa->dst < bb->dst) return -1;
	if(aa->dst > bb->dst) return 1;
	
	return 0;
}

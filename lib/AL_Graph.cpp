#include "AL_Graph.hpp"
#include <omp.h>

AL_Graph::~AL_Graph(){
	deallocate();
}

AL_Graph::AL_Graph(unsigned int nnodes, unsigned int nedges){
	this->nnodes = nnodes;
	this->nedges = 0;
	this->pool = (AL_Edge*)malloc(sizeof(AL_Edge) * nedges);
	//this->pool_top = 0;
	allocate();
}

AL_Graph::AL_Graph(unsigned int nnodes){
	this->nnodes = nnodes;
	this->nedges = 0;
	this->pool = NULL;
	allocate();
}

CSR_Graph* AL_Graph::toCSR(){
	CSR_Graph *g = new CSR_Graph(nnodes, nedges);
	g->allocate();
		
	unsigned top_edge = 1;

	for(unsigned int i = 0; i < nnodes; i++)
	{
		unsigned outdegree = 0;
		g->psrc[i] = top_edge;

		for(AL_Edge *tmp = nodes[i]; tmp; tmp = tmp->next)
		{
			g->edgessrcdst[top_edge] = tmp->dst;
			g->edgessrcwt[top_edge] = tmp->weight;
			outdegree++;
			top_edge++;
		}
		
		g->outdegree[i] = outdegree;
	}

	return g;
}

void AL_Graph::allocate(){
	nodes = (AL_Edge**)calloc(nnodes, sizeof(AL_Edge*));
	outdegree = (unsigned int*)calloc(nnodes, sizeof(unsigned int));

}

void AL_Graph::deallocate(){
	unsigned int i;
	AL_Edge *ptr = NULL, *tmp = NULL;

	if(!pool)
	{
		for(i = 0; i < nnodes; i++)
		{
			ptr = nodes[i];
			while(ptr)
			{
				tmp = ptr;
				ptr = ptr->next;
				free(tmp);
			}
		}
	}
	else
	{
		free(pool);
	}

	free(nodes);
}

void AL_Graph::insert_edge(unsigned int src, unsigned int dst, unsigned int weight){
	AL_Edge *ptr = (AL_Edge*)malloc(sizeof(AL_Edge));
	
	ptr->dst = dst;
	ptr->weight = weight;
	__atomic_exchange(&(nodes[src]), &ptr, &(ptr->next), __ATOMIC_SEQ_CST);

	__sync_fetch_and_add(&outdegree[src], 1);

	__sync_fetch_and_add(&nedges, 1);
}

void AL_Graph::insert_edge_nolock(unsigned int src, unsigned int dst, unsigned int weight){
	AL_Edge *ptr = (AL_Edge*)malloc(sizeof(AL_Edge));
	
	ptr->dst = dst;
	ptr->weight = weight;
	ptr->next = nodes[src];
	nodes[src] = ptr;
	
	outdegree[src]++;

	nedges++;
}

void AL_Graph::insert_pair(unsigned int src, unsigned int dst, unsigned int weight){
	AL_Edge *ptr = (AL_Edge*)malloc(sizeof(AL_Edge));
	ptr->dst = dst;
	ptr->weight = weight;
	__atomic_exchange(&(nodes[src]), &ptr, &(ptr->next), __ATOMIC_SEQ_CST);
	__sync_fetch_and_add(&outdegree[src], 1);

	ptr = (AL_Edge*)malloc(sizeof(AL_Edge));
	ptr->dst = src;
	ptr->weight = weight;
	__atomic_exchange(&(nodes[dst]), &ptr, &(ptr->next), __ATOMIC_SEQ_CST);
	__sync_fetch_and_add(&outdegree[dst], 1);

	__sync_fetch_and_add(&nedges, 2);
}

void AL_Graph::insert_pair_nolock(unsigned int src, unsigned int dst, unsigned int weight){
	AL_Edge *ptr = (AL_Edge*)malloc(sizeof(AL_Edge));
	ptr->dst = dst;
	ptr->weight = weight;
	ptr->next = nodes[src];
	nodes[src] = ptr;
	outdegree[src]++;


	ptr = (AL_Edge*)malloc(sizeof(AL_Edge));
	ptr->dst = src;
	ptr->weight = weight;
	ptr->next = nodes[dst];
	nodes[dst] = ptr;
	outdegree[dst]++;
	
	nedges+= 2;
}







void AL_Graph::pooled_insert_edge(unsigned int src, unsigned int dst, unsigned int weight){
	unsigned pos = __sync_fetch_and_add(&nedges, 1);

	AL_Edge *ptr = &(pool[pos]);
	
	ptr->dst = dst;
	ptr->weight = weight;
	__atomic_exchange(&(nodes[src]), &ptr, &(ptr->next), __ATOMIC_SEQ_CST);
	__sync_fetch_and_add(&outdegree[src], 1);
}

void AL_Graph::pooled_insert_edge_nolock(unsigned int src, unsigned int dst, unsigned int weight){
	AL_Edge *ptr = &(pool[nedges]);

	ptr->dst = dst;
	ptr->weight = weight;
	ptr->next = nodes[src];
	nodes[src] = ptr;

	outdegree[src]++;
	
	nedges++;
}

void AL_Graph::pooled_insert_pair(unsigned int src, unsigned int dst, unsigned int weight){
	unsigned pos = __sync_fetch_and_add(&nedges, 2);

	AL_Edge *ptr = &(pool[pos]);
	ptr->dst = dst;
	ptr->weight = weight;
	__atomic_exchange(&(nodes[src]), &ptr, &(ptr->next), __ATOMIC_SEQ_CST);
	__sync_fetch_and_add(&outdegree[src], 1);

	ptr = &(pool[pos+1]);
	ptr->dst = src;
	ptr->weight = weight;
	__atomic_exchange(&(nodes[dst]), &ptr, &(ptr->next), __ATOMIC_SEQ_CST);
	__sync_fetch_and_add(&outdegree[dst], 1);
}

void AL_Graph::pooled_insert_pair_nolock(unsigned int src, unsigned int dst, unsigned int weight){
	AL_Edge *ptr = &(pool[nedges]);
	nedges++;
	
	ptr->dst = dst;
	ptr->weight = weight;
	ptr->next = nodes[src];
	nodes[src] = ptr;
	outdegree[src]++;
	

	ptr = &(pool[nedges]);
	nedges++;

	ptr->dst = src;
	ptr->weight = weight;
	ptr->next = nodes[dst];
	nodes[dst] = ptr;
	outdegree[dst]++;
}

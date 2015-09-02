//
//  main.cpp
//  reverse_delete
//
//  Created by Cristiano Sousa on 25/03/14.
//  Copyright (c) 2014 Cristiano Sousa. All rights reserved.
//

#include "header.h"
#include "CSR_Graph.hpp"
#include <vector>
#include <algorithm>


class EdgeNumbered{
public:
	
	unsigned int src;
	unsigned int dst;
	unsigned int weight;
	unsigned int edge;
	//EdgeNumbered(unsigned weight);
	EdgeNumbered(const unsigned int src, const unsigned int dst, const unsigned int weight, const unsigned int edge);
};


EdgeNumbered::EdgeNumbered(const unsigned int src, const unsigned int dst, const unsigned int weight, const unsigned int edge){
	this->src = src;
	this->dst = dst;
	this->weight = weight;
	this->edge = edge;
}



int compareEdgeNumberedWt(EdgeNumbered a, EdgeNumbered b)
{
	return a.weight > b.weight;
}

CSR_Graph* reverse_delete(CSR_Graph *g){
	std::vector<EdgeNumbered> edges;

	unsigned int i,j;
	
	for(i = 0; i < g->nnodes; i++)
	{
		unsigned k = g->getFirstEdge(i);
		for(j = 0; j < g->getOutDegree(i); j++)
		{
			edges.push_back(EdgeNumbered(i, g->getDestination(i, j), g->getWeight(i, j), k + j));
			//edges.emplace_back(g->getWeight(i, j));
		}
	}
	
	unsigned int mst_edges = 2 * (g->nnodes - 1);
	
	printf("Graph has %d edges, MST is supposed to have %d\n", g->nedges, mst_edges);
	printf("An excess of %d edges\n", g->nedges - mst_edges);
	
	unsigned int window_size = g->nedges - mst_edges;
	unsigned int window_begin = 0;
	unsigned int window_end = window_size;
	unsigned int current_edges = g->nedges;
	
	unsigned *actual_outdegree = (unsigned int *)malloc(sizeof(unsigned) * g->nnodes);
	memcpy(actual_outdegree, g->outdegree, sizeof(unsigned) * g->nnodes);

	std::vector<EdgeNumbered>::iterator window_begin_it = edges.begin(), window_end_it = edges.begin();
	while(current_edges > mst_edges && window_end_it != edges.end())
	{
		printf("current edges: %d\n", current_edges);
		window_begin_it = edges.begin() + window_begin;
		window_end_it = edges.begin() + window_end;
		
		if(window_end_it > edges.end()) window_end_it = edges.end();
		
		printf("Sorting...\n");
		std::partial_sort(window_begin_it, window_end_it, edges.end(), compareEdgeNumberedWt);
		
		printf("Begin removing..\n");
		for( ; window_begin_it <= window_end_it && current_edges > mst_edges; window_begin_it++)
		{
			if(unlikely(g->edgessrcwt[window_begin_it->edge] == 0)) continue;
			
			unsigned src = window_begin_it->src;
			if(unlikely(actual_outdegree[src] == 1)) continue;

			unsigned dst = window_begin_it->dst;
			if(unlikely(actual_outdegree[dst] == 1)) continue;


			unsigned reverse_edge = g->findDestination(dst, src, window_begin_it->weight);
			g->edgessrcdst[window_begin_it->edge] = g->nnodes;
			g->edgessrcdst[reverse_edge] = g->nnodes;
	
			if(!g->exists_path(src, dst))
			{
				g->edgessrcdst[window_begin_it->edge] = dst;
				g->edgessrcdst[reverse_edge] = src;
			}
			else
			{
				g->edgessrcwt[window_begin_it->edge] = 0;
				g->edgessrcwt[reverse_edge] = 0;
				current_edges -= 2;
				actual_outdegree[src]--;
				actual_outdegree[dst]--;
			}
		}
		
		window_size = window_size / 2;
		window_begin = window_end + 1;
		window_end += window_size;
		
	}
	

	return g;
}


int main(int argc, char * argv[])
{
	if(argc != 2)
	{
		printf("Usage: %s filename\n", argv[0]);
		return 1;
	}
	
	if(!file_exists(argv[1]))
	{
		printf("File %s does not exist!\n", argv[1]);
		return 1;
	}
	
	
	
	CSR_Graph *g = new CSR_Graph(argv[1]);
	if(!g->check()) return 1;
	
	clock_t start = clock();
	CSR_Graph *mst = reverse_delete(g);
	clock_t end = clock();
	
	float seconds = (float)(end - start) / CLOCKS_PER_SEC;
	printf("%s\n", argv[1]);
	printf("Reverse delete took %f seconds\n", seconds);
	printf("Total MST weight: %lu\n", mst->getTotalWeight());
    printf("nodes: %d\n", mst->nnodes);
    printf("edges: %d\n", mst->nedges);
	
	//delete g;
	
	//mst->writeToFile(const_cast<char*>("kruskal.mst"));
	//mst->check();
	delete mst;


	
    return 0;
}


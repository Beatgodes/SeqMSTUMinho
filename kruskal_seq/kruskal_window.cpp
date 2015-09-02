#include "header.h"
#include <boost/pending/disjoint_sets.hpp>
#include "CSR_Graph.hpp"
#include "AL_Graph.hpp"
#include <vector>
#include <algorithm>

class EdgeNumbered{
public:
	
	unsigned int src;
	unsigned int dst;
	unsigned int weight;
	EdgeNumbered(const unsigned int src, const unsigned int dst, const unsigned int weight);
};


EdgeNumbered::EdgeNumbered(const unsigned int src, const unsigned int dst, const unsigned int weight){
	this->src = src;
	this->dst = dst;
	this->weight = weight;
}

int compareEdgeNumberedWt(EdgeNumbered a, EdgeNumbered b)
{
	return a.weight < b.weight;
}


CSR_Graph* kruskal_window(CSR_Graph *g){

	std::vector<EdgeNumbered> edges;

	unsigned int i,j;
	
	for(i = 0; i < g->nnodes; i++)
	{
		for(j = 0; j < g->getOutDegree(i); j++)
		{
			unsigned dst = g->getDestination(i, j);
			if (i < dst) edges.push_back(EdgeNumbered(i, dst, g->getWeight(i, j)));
		}
	}

	unsigned int mst_edges = g->nnodes - 1;
	unsigned int excess_edges = (g->nedges/2) - mst_edges;

	AL_Graph *mst = new AL_Graph(g->nnodes, 2 * (g->nnodes -1));
	printf("Graph has %d edges, MST is supposed to have %d\n", g->nedges, mst_edges);
	printf("An excess of %d edges\n", excess_edges);
	printf("Added to vector: %lu\n", edges.size());
		
	std::vector<unsigned int>rank(g->nnodes);
	std::vector<unsigned int> parent(g->nnodes);
	boost::disjoint_sets<unsigned int*, unsigned int*> sets(&rank[0], &parent[0]);
	for(i = 0; i < g->nnodes; i++) sets.make_set(i);
	

	unsigned int window_size = excess_edges;
	unsigned int window_begin = 0;
	unsigned int window_end = window_size;

	unsigned int added_edges = 0;
	unsigned int removed_edges = 0;

	std::vector<EdgeNumbered>::iterator window_begin_it = edges.begin(), window_end_it = edges.begin();

	while(removed_edges < excess_edges && added_edges < mst_edges && window_end_it < edges.end() - 1)
	{
		printf("removed %d edges\n", removed_edges);
		printf("added %d edges\n", added_edges);
		window_begin_it = edges.begin() + window_begin;
		window_end_it = edges.begin() + window_end;

		if(window_end_it >= edges.end()) window_end_it = edges.end() - 1;

		printf("Sorting...\n");
		std::partial_sort(window_begin_it, window_end_it, edges.end(), compareEdgeNumberedWt);

		printf("Algorithming...\n");
		for( ; window_begin_it <= window_end_it && removed_edges < excess_edges && added_edges < mst_edges; window_begin_it++)
		{
			unsigned int a = window_begin_it->src;
			unsigned int b = window_begin_it->dst;
			
			unsigned int s_a = sets.find_set(a);
			unsigned int s_b = sets.find_set(b);
			
			if(s_a != s_b)
			{
				sets.link(s_a, s_b);
				mst->pooled_insert_pair_nolock(a, b, window_begin_it->weight);
				
				added_edges++;
			}
			else
			{
				removed_edges++;
			}
		}

		//window_size = window_size / 2;
		window_begin = window_end + 1;
		window_end += window_size;

	}

	printf("finishing loop\n");
	printf("removed %d edges\n", removed_edges);
	printf("added %d edges\n", added_edges);

	for( ; window_begin_it < edges.end() && added_edges < mst_edges; window_begin_it++)
	{
		mst->pooled_insert_pair_nolock(window_begin_it->src, window_begin_it->dst, window_begin_it->weight);
		added_edges++;
	}

	printf("finishing\n");
	printf("removed %d edges\n", removed_edges);
	printf("added %d edges\n", added_edges);

	// cleanup and output result
	delete g;
	CSR_Graph *ret = mst->toCSR();
	delete mst;
	
	return ret;
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
	CSR_Graph *mst = kruskal_window(g);
	clock_t end = clock();

	float seconds = (float)(end - start) / CLOCKS_PER_SEC;
	printf("%s\n", argv[1]);
	printf("Reverse delete took %f seconds\n", seconds);
	printf("Total MST weight: %lu\n", mst->getTotalWeight());
    printf("nodes: %d\n", mst->nnodes);
    printf("edges: %d\n", mst->nedges);
	
	//delete g;
	
	//mst->writeToFile(const_cast<char*>("kruskal.mst"));
	mst->check();
	mst->find_duplicates();
	
	delete mst;


	
    return 0;
}
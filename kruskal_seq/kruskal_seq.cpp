#include "kruskal_seq.h"

CSR_Graph* kruskal(CSR_Graph *gg){
	
	// Convert to edge list graph
	unsigned int nnodes = gg->nnodes;
	
	EL_Graph *g = new EL_Graph(gg);
	AL_Graph *mst = new AL_Graph(nnodes, 2 * (nnodes -1));
		
	std::vector<unsigned int>rank(nnodes);
	std::vector<unsigned int> parent(nnodes);
	boost::disjoint_sets<unsigned int*, unsigned int*> sets(&rank[0], &parent[0]);
	
	unsigned int i = 0;
	
	for(i = 0; i < nnodes; i++) sets.make_set(i);
	
	qsort((void*)(g->edges), g->nedges, sizeof(Edge), compareEdgeWt);
	
	unsigned int nedges = g->nedges;
	unsigned int top_edge = 0;
	
	for(i = 0; top_edge < (nnodes - 1) && i < nedges; i++)
	{
		unsigned int a = g->edges[i].src;
		unsigned int b = g->edges[i].dst;
		
		unsigned int s_a = sets.find_set(a);
		unsigned int s_b = sets.find_set(b);
		
		if(s_a != s_b)
		{
			sets.link(s_a, s_b);
			
			//mst->insert_edge(a, b, g->edges[i].weight);
			//mst->insert_edge(b, a, g->edges[i].weight);
			mst->pooled_insert_pair_nolock(a, b, g->edges[i].weight);
			
			top_edge++;
		}
	}
	//sets.compress_sets(&rank[0], &rank[nnodes]);
	//	for(i = 0; i < nnodes; i++) printf("%d %d\n", i, rank[i]);
	//	printf("\n");
	//for(i = 0; i < nnodes; i++) printf("%d %d\n", i, parent[i]);
		
	// cleanup and output result
	delete g;
	CSR_Graph *ret = mst->toCSR();
	delete mst;

	sets.compress_sets(rank.begin(), rank.end());
	sets.compress_sets(parent.begin(), parent.end());
	printf("number of connected components: %lu\n", sets.count_sets(&rank[0], &rank[nnodes]));
	printf("number of connected components: %lu\n", sets.count_sets(&parent[0], &parent[nnodes]));
	printf("number of connected components: %lu\n", sets.count_sets(rank.begin(), rank.end()));
	printf("number of connected components: %lu\n", sets.count_sets(parent.begin(), parent.end()));
	
	/*int count = 1;
	int prev = rank[0];
	for(i = 1; i < nnodes; i++)
	{
		if(prev != rank[i])
		{
			count++;
			prev = rank[i];
		}
	}

	printf("counted %d\n", count);
*/
	return ret;
}
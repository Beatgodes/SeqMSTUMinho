#include "boruvka_seq.h"

typedef struct NodePair{
	unsigned int src;
	unsigned int dst;
	unsigned int cost;
	
	NodePair(unsigned int src, unsigned int dst, unsigned int cost);
	
} NodePair;

NodePair::NodePair(unsigned int src, unsigned int dst, unsigned int cost){
	this->src = src;
	this->dst = dst;
	this->cost = cost;
}


struct increasingOrderNodePair
{
    bool operator() (const NodePair & lhs, const NodePair & rhs) const
    {
        return lhs.cost > rhs.cost;
    }
};

typedef boost::heap::compare<increasingOrderNodePair> NodePairCompare;
typedef boost::heap::pairing_heap<NodePair, NodePairCompare> NodePairHeap;
typedef NodePairHeap::handle_type NodePairHandle;

CSR_Graph* boruvka(CSR_Graph *g){
	Sets *sets = new Sets(g->nnodes);
	AL_Graph *mst = new AL_Graph(g->nnodes, 2 * (g->nnodes - 1));
	//NodePairHeap *heap = (NodePairHeap*)malloc(g->nnodes * sizeof(NodePairHeap));
	
	unsigned int i, j, k;
	unsigned int min_node = g->nnodes;
	unsigned int min_wt = UINT_MAX;
	unsigned int min_src = g->nnodes;
	
	/*for(i = 0; i < g->nnodes; i++)
	{
		for(unsigned ii = 0; ii < g->getOutDegree(i); ii++)
		{
			j = g->getDestination(i, ii);
			k = g->getWeight(i, ii);
			heap[i].emplace(NodePair(i, j, k));
		}
	}*/
	
	//std::vector<unsigned int>  rank (g->nnodes);
	//std::vector<unsigned int>  parent (g->nnodes);
	//boost::disjoint_sets<unsigned int*, unsigned int*> sets(&rank[0], &parent[0]);
	bool *closed_component = (bool*)calloc(g->nnodes, sizeof(bool));

	while(sets->n_sets > 0)
	{
		for(i = 0; i < g->nnodes; i++)
		{
			if(closed_component[i] || sets->sets[i].size == 0) continue;
			// iterate all nodes of the set
			for(j = 0; j < sets->sets[i].size; j++)
			{
				
				// for each neighbor
				unsigned int act = sets->sets[i].elems[j];
				for(k = 0; k < g->getOutDegree(act); k++)
				{
					
					unsigned int dst = g->getDestination(act, k);
					
					
					if(sets->find_set(dst) != sets->find_set(act))
					{
						unsigned int dst_wt = g->getWeight(act, k);
						if(dst_wt < min_wt)
						{
							min_wt = dst_wt;
							min_node = dst;
							min_src = act;
						}
					}
				}
			}
			
			
			if(min_wt != UINT_MAX)
			{
				sets->set_union(i, sets->find_set(min_node));
				
				mst->pooled_insert_pair_nolock(min_src, min_node, min_wt);
				//mst->insert_edge(min_src, min_node, min_wt);
				//mst->insert_edge(min_node, min_src, min_wt);
				
				min_wt = UINT_MAX;
			}
			else
			{
				closed_component[i] = true;
				sets->n_sets--;
			}
			
			/*if(sets->sets[i].size > 0)
			{
				while(!heap[i].empty())
				{
					NodePair np = heap[i].top(); heap[i].pop();
					if(sets->find_set(np.src) != sets->find_set(np.dst))
					{
						unsigned other_set = sets->find_set(np.dst);
						sets->set_union(i, other_set);
						
						if(sets->sets[i].size > 0)
						{
							heap[i].merge(heap[other_set]);
						}
						else
						{
							heap[other_set].merge(heap[i]);
						}
						
						mst->insert_pair(np.src, np.dst, np.cost);
						break;
					}
				}
			}*/
		}
	}
	
	int count = 0;
	for(i = 0; i < g->nnodes; i++)
	{
		if(closed_component[i]) count++;
	}

	printf("connected components %d\n", count);

	// cleanup and output result
	delete sets;
	CSR_Graph *ret = mst->toCSR();
	delete mst;
	
	return ret;
	
}
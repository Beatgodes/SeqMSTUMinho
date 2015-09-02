//
//  main.cpp
//  MST
//
//  Created by Cristiano Sousa on 11/11/13.
//  Copyright (c) 2013 Cristiano Sousa. All rights reserved.
//

#include "prim_seq.h"

CSR_Graph* prim(CSR_Graph *g, unsigned int initial){
	unsigned int *dist = (unsigned int*)malloc(sizeof(unsigned int) * g->nnodes);
	NodePairHandle *handlers = (NodePairHandle*)calloc(g->nnodes, sizeof(NodePairHandle));
	NodePairHeap heap;
	AL_Graph *mst = new AL_Graph(g->nnodes, 2 * (g->nnodes - 1));
	
	unsigned int i = 0, j = 0, k = 0, current = initial, outdegree;
	
	for(i = 0; i < g->nnodes; i++)
	{
		dist[i] = UINT_MAX;
	}

	int n_sets = 0;
	for(int control = 0; control < g->nnodes; control++)
	{
		if(dist[control] == UINT_MAX)
		{	
			current = control;
			n_sets++;

			dist[current] = 0;

			outdegree = g->getOutDegree(current);
			for(i = 0; i < outdegree; i++)
			{
				j = g->getDestination(current, i);
				k = g->getWeight(current, i);

				if(k < dist[j])
				{
					if(dist[j] == UINT_MAX)
					{
						handlers[j] = heap.emplace(current, j, k);
					}
					else
					{
						(*handlers[j]).src = current;
						(*handlers[j]).cost = k;
						heap.decrease(handlers[j]);
					}
					
					dist[j] = k;
				}
			}

			while(!heap.empty())
			{
				const NodePair np = heap.top();
				heap.pop();
				
				current = np.dst;

				//if(unlikely(dist[current] == 0)) continue;
				dist[current] = 0;
				
				mst->pooled_insert_pair_nolock(np.src, np.dst, np.cost);

				outdegree = g->getOutDegree(current);
				for(i = 0; i < outdegree; i++)
				{
					j = g->getDestination(current, i);			
					k = g->getWeight(current, i);
					
					if(k < dist[j])
					{
						if(dist[j] == UINT_MAX)
						{
							handlers[j] = heap.emplace(current, j, k);
						}
						else
						{
							(*handlers[j]).src = current;
							(*handlers[j]).cost = k;
							heap.decrease(handlers[j]);
						}
						
						dist[j] = k;
					}
				}
			}



		}
	}

	printf("total sets %d\n", n_sets);

	free(dist);
	free(handlers);
	CSR_Graph *ret = mst->toCSR();
	delete mst;
	
	return ret;
}

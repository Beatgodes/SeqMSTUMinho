#include "header.h"

#include "kruskal_seq.h"
#include "CSR_Graph.hpp"

int main(int argc, char *argv[]){
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
	//if(!g->check()) return 1;
	
	clock_t start = clock();
	CSR_Graph *mst = kruskal(g);
	clock_t end = clock();
	
	float seconds = (float)(end - start) / CLOCKS_PER_SEC;
	printf("%s\n", argv[1]);
	printf("Kruskal took %.2f seconds\n", 1000.0f*seconds);
	printf("Total MST weight: %lu\n", mst->getTotalWeight());
    printf("nodes: %d\n", mst->nnodes);
    printf("edges: %d\n", mst->nedges);
	
	delete g;
	
	//mst->writeToFile(const_cast<char*>("kruskal.mst"));
	//mst->check();
	delete mst;
	
	return 0;
}


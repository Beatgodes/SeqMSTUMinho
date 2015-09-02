#include "Set.hpp"

Sets::~Sets(){
	deallocate();
}

void Sets::deallocate(){
	unsigned int i = 0;
	for(i = 0; i < max_sets; i++)
	{
		free(sets[i].elems);
	}
	
	free(sets);
	free(node_set);
}

void Sets::allocate(){
	node_set = (unsigned int*)malloc(sizeof(unsigned int) * nnodes);
	sets = (Set*)malloc(sizeof(Set) * nnodes);
}

Sets::Sets(unsigned int nnodes){
	this->nnodes = nnodes;
	this->n_sets = nnodes;
	this->max_sets = nnodes;
	allocate();
	
	unsigned i;
	for(i = 0; i < nnodes; i++)
	{
		node_set[i] = i;
	}
	
	for(i = 0; i < nnodes; i++)
	{
		make_set(i);
	}
}

void Sets::make_set(unsigned int val){
	sets[val].size = 1;
	sets[val].elems = (unsigned int*)malloc(sizeof(unsigned int));
	sets[val].elems[0] = val;
}

void Sets::set_union(unsigned int a, unsigned int b){
	unsigned int new_size = sets[a].size + sets[b].size;
	
	if(sets[a].size < sets[b].size)
	{
		sets[b].elems = (unsigned int*)realloc(sets[b].elems, new_size * sizeof(unsigned int));
		memcpy(&(sets[b].elems[sets[b].size]), sets[a].elems, sets[a].size * sizeof(unsigned int));
		
		free(sets[a].elems);
		sets[b].size = new_size;
		sets[a].size = 0;
		sets[a].elems = NULL;
		
		
		for(unsigned int tmp = 0; tmp < sets[b].size; tmp++)
		{
			node_set[sets[b].elems[tmp]] = b;
		}
		
		n_sets--;
	}
	else
	{
		sets[a].elems = (unsigned int*)realloc(sets[a].elems, new_size * sizeof(unsigned int));
		memcpy(&(sets[a].elems[sets[a].size]), sets[b].elems, sets[b].size * sizeof(unsigned int));
		
		free(sets[b].elems);
		sets[a].size = new_size;
		sets[b].size = 0;
		sets[b].elems = NULL;
		
		for(unsigned int tmp = 0; tmp < sets[a].size; tmp++)
		{
			node_set[sets[a].elems[tmp]] = a;
		}
		
		n_sets--;
	}
}

unsigned int Sets::find_set(unsigned int val){
	return node_set[val];
}

LIBS =  
INCLUDE = -Iinclude/ -I/usr/local/Cellar/boost/1.58.0/include -Ilib/

WARNINGS = -Wall -Wextra
SUPPRESS_WARNINGS = -Wno-long-long -Wno-unused-value -Wno-unused-local-typedefs -Wno-sign-compare -Wno-unused-but-set-variable -Wno-unused-parameter -Wno-unused-function -Wno-reorder -Wno-strict-aliasing
CFLAGS = -O3 $(WARNINGS) $(SUPPRESS_WARNINGS) -std=c++11 

CC = g++-4.9


all: prim_seq kruskal_seq boruvka_seq kruskal_window reverse_delete

# SEQ

prim_seq: lib/CSR_Graph.o lib/AL_Graph.o lib/NodePair.o prim_seq/prim_seq.o prim_seq/main.o
	$(CC) $(CFLAGS) $(INCLUDE) $(LIBS) $^ -o bin/$@

kruskal_seq: lib/CSR_Graph.o lib/AL_Graph.o lib/EL_Graph.o kruskal_seq/kruskal_seq.o kruskal_seq/main.o 
	$(CC) $(CFLAGS) $(INCLUDE) $(LIBS) $^ -o bin/$@

boruvka_seq: lib/Set.o lib/CSR_Graph.o lib/AL_Graph.o boruvka_seq/boruvka_seq.o boruvka_seq/main.o 
	$(CC) $(CFLAGS) $(INCLUDE) $(LIBS) $^ -o bin/$@

kruskal_window: lib/CSR_Graph.o lib/AL_Graph.o kruskal_seq/kruskal_window.o
	$(CC) $(CFLAGS) $(INCLUDE) $(LIBS) $^ -o bin/$@

reverse_delete: lib/CSR_Graph.o reverse_delete/main.o
	$(CC) $(CFLAGS) $(INCLUDE) $(LIBS) $^ -o bin/$@


%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDE) $(LIBS) -c $^ -o $@


#TEST
test: prim_seq boruvka_seq kruskal_seq
	@echo ----
	bin/prim_seq $(GRAPH)
	@echo ----
	bin/boruvka_seq $(GRAPH)
	@echo ----
	bin/kruskal_seq $(GRAPH)
	@echo ----
	bin/kruskal_window $(GRAPH)
	@echo ----
	bin/reverse_delete $(GRAPH)
	@echo --------------
	

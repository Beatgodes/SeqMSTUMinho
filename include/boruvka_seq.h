//
//  boruvka_seq.h
//  MST
//
//  Created by Cristiano Sousa on 12/11/13.
//  Copyright (c) 2013 Cristiano Sousa. All rights reserved.
//

#ifndef BORUVKA_SEQ_HEADER
#define BORUVKA_SEQ_HEADER

#include <boost/pending/disjoint_sets.hpp>
#include <boost/heap/pairing_heap.hpp>
#include "CSR_Graph.hpp"
#include "AL_Graph.hpp"
#include "Set.hpp"

CSR_Graph* boruvka(CSR_Graph *g);

#endif

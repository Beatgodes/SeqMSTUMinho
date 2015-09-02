//
//  prim_seq.h
//  MST
//
//  Created by Cristiano Sousa on 11/11/13.
//  Copyright (c) 2013 Cristiano Sousa. All rights reserved.
//

#ifndef PRIM_SEQ_HEADER
#define PRIM_SEQ_HEADER

#include "NodePair.hpp"
#include "CSR_Graph.hpp"
#include "AL_Graph.hpp"

CSR_Graph* prim(CSR_Graph *g, unsigned int initial = 0);

#endif

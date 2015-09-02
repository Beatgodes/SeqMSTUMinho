//
//  NodePair.h
//  MST
//
//  Created by Cristiano Sousa on 07/04/14.
//  Copyright (c) 2014 Cristiano Sousa. All rights reserved.
//

#ifndef NODE_PAIR_HEADER
#define NODE_PAIR_HEADER

#include <boost/heap/pairing_heap.hpp>

typedef struct NodePair{
	unsigned int src;
	unsigned int dst;
	unsigned int cost;
	
	NodePair(unsigned int src, unsigned int dst, unsigned int cost);
	
} NodePair;

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

#endif

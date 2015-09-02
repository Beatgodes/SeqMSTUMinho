//
//  NodePair.cpp
//  MST
//
//  Created by Cristiano Sousa on 07/04/14.
//  Copyright (c) 2014 Cristiano Sousa. All rights reserved.
//

#include "NodePair.hpp"

NodePair::NodePair(unsigned int src, unsigned int dst, unsigned int cost){
	this->src = src;
	this->dst = dst;
	this->cost = cost;
}
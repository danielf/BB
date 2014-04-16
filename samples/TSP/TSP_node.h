#ifndef BB_SAMPLES_TSP_TSP_NODE_H
#define BB_SAMPLES_TSP_TSP_NODE_H

#include "samples/TSP/TSP_instance.h"
#include "samples/TSP/TSP_solution.h"

#include "BB/BB"

class TSP_node : public BB::node {
	public:
		double lb();
		double ub();
		void branch(BB::computation* comp);
};

#endif

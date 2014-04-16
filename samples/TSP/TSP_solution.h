#ifndef BB_SAMPLES_TSP_TSP_SOLUTION_H
#define BB_SAMPLES_TSP_TSP_SOLUTION_H

#include "BB/BB"

#include <utility>
#include <vector>

using std::pair;
using std::vector;

class TSP_solution : public BB::solution {
	public:
		void initialize(const TSP_instance& instance);
		string serialize() const;
		void addEdge(int i, int j);
	private:
		typedef pair<int, int> edge;
		vector<edge> edges;
};

#endif

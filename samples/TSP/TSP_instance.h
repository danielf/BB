#ifndef BB_SAMPLES_TSP_TSP_INSTANCE_H
#define BB_SAMPLES_TSP_TSP_INSTANCE_H

#include "BB/BB"

#include <utility>
#include <vector>
#include <istream>

using std::pair;
using std::vector;
using std::istream;

class TSP_instance : public BB::instance {
	public:
		void apply(BB::delta& D);
		bool read(istream& in);
		void prepare(const BB::computation& comp);
		int getN() const;
		int getStatus(int i, int j) const;
		double getEdge(int i, int j) const;
		bool isInfeasible() const;
		pair<int, int> getConnection(int i) const;
	private:
		int N;
		vector<vector<double>> edges;
		vector<vector<int>> status;
		double total_enforced;
		vector<pair<int, int>> connections;
		bool dfs(int i, int parent, vector<int>& mark, int m) const;
};

#endif

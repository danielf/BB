#include "samples/TSP/TSP_instance.h"

#include "samples/TSP/TSP_delta.h"

#include <algorithm>

using std::get;
using std::max_element;
using std::min_element;

bool TSP_instance::read(istream& in) {
	total_enforced = 0;
	in >> N;
	status = vector<vector<int>>(N, vector<int>(N, 0));
	edges = vector<vector<double>>(N, vector<double>(N, 0.));
	for (int i = 0; i < N; i++) for (int j = 0; j < N; j++)
		in >> edges[i][j];
	for (int i = 0; i < N; i++) status[i][i] = -1;
	return true;
}

void TSP_instance::apply(BB::delta& _D) {
	int D = (static_cast<TSP_delta&>(_D)).value;
	bool enf = D >= 0;
	if (!enf) D = ~D;
	int i = D / N;
	int j = D % N;
	status[j][i] =
	status[i][j] = enf?1:-1;
	if (enf) total_enforced += edges[i][j];
}

bool TSP_instance::dfs(int i, int parent, vector<int>& mark, int m) const {
	if (mark[i] == m) return true;
	if (mark[i] != -1) return false;
	mark[i] = m;
	if (connections[i].first == -1) return false;
	if (parent != connections[i].first && dfs(connections[i].first, i, mark, m)) return true;
	if (connections[i].second == -1) return false;
	if (parent != connections[i].second && dfs(connections[i].second, i, mark, m)) return true;
	return false;
}

void TSP_instance::prepare(const BB::computation& comp) {
	// Check for things we already know:
	infeasible = false;

	double val = 0.;
	for (int i = 0; i < N; i++) for (int j = 0; j < i; j++)
		if (status[i][j] == 1) val += edges[i][j];

	for (int i = 0; i < N; i++) for (int j = 0; j < N; j++)
		if (status[i][j] == 0 && val + edges[i][j] >= comp.getUpperBound())
			status[i][j] = -1;

	for (int i = 0; i < N; i++) {
		int ce = 0, cf = 0;
		for (int j = 0; j < N; j++) {
			if (status[i][j] == -1) cf++;
			else if (status[i][j] == 1) ce++;
		}
		if (ce >= 3 || cf >= N-1) {
			infeasible = true;
			return;
		}
		int rewrite = 0;
		if (ce == 2) rewrite = -1;
		if (cf == N-2) rewrite = 1;
		if (rewrite) for (int j = 0; j < N; j++) if (status[i][j] == 0) status[i][j] = rewrite;
		// fill in the connections:
		connections[i].first = connections[i].second = -1;
		ce = 0;
		for (int j = 0; j < N; j++) if (status[i][j] == 1) {
			if (ce == 0) connections[i].first = j;
			else connections[i].second = j;
			ce++;
		}
	}


	// Check for subtours:
	vector<int> mark(N, -1);
	for (int i = 0; i < N; i++)
		if (dfs(i, -1, mark, i)) {
			// If it is more than one cycle, we are doomed:
			infeasible = *min_element(mark.begin(), mark.end()) != *max_element(mark.begin(), mark.end());
			break;
		}
}


int TSP_instance::getN() const {
	return N;
}

int TSP_instance::getStatus(int i, int j) const {
	return status[i][j];
}

double TSP_instance::getEdge(int i, int j) const {
	return edges[i][j];
}

pair<int, int> TSP_instance::getConnection(int i) const {
	return connections[i];
}

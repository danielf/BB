#include "samples/TSP/TSP_node.h"

#include "samples/TSP/TSP_delta.h"

#include <algorithm>
#include <tuple>
#include <vector>

using std::get;
using std::sort;
using std::tuple;
using std::vector;

// Common part for both bounds. Create MST (Kruskal)

struct union_find {
	vector<int> rank, parent;
	union_find(int N) {
		rank.resize(N);
		parent.resize(N);
		for (int i = 0; i < N; i++) {
			rank[i] = 0;
			parent[i] = i;
		}
	}
	int find(int i) {
		if (parent[i] != i) parent[i] = find(parent[i]);
		return parent[i];
	}
	bool unio(int i, int j) {
		i = find(i); j = find(j);
		if (i == j) return false;
		if (rank[i] < rank[j]) parent[i] = j;
		else parent[j] = i;
		if (rank[i] == rank[j]) rank[i]++;
		return true;
	}
};

double TSP_node::lb() {
	const TSP_instance& instance = static_cast<const TSP_instance&>(this->_instance);
	const int N = instance.getN();
	union_find UF(N);
	int count = 0;
	double ans = 0;
	typedef tuple<double, int, int> edge;
	vector<edge> edges;
	for (int i = 0; i < N; i++) for (int j = 0; j < N; j++)	{
		if (instance.getStatus(i,j) == 1) {
			UF.unio(i, j);
			count++;
			ans += instance.getEdge(i, j);
		} else if (instance.getStatus(i, j) == 0) edges.emplace_back(instance.getEdge(i,j), i, j);
	}
	if (count == N) return ans;
	sort(edges.begin(), edges.end());
	for (auto& x : edges) if (UF.unio(get<1>(x), get<2>(x))) {
		count++;
		ans += get<0>(x);
	}
	return ans;
}

double TSP_node::ub() {
	const TSP_instance& instance = static_cast<const TSP_instance&>(this->_instance);
	const int N = instance.getN();
	int start = -1;
	for (int i = 0; i < N; i++) if (instance.getConnection(i).second == -1) {
		start = i;
		break;
	}
	if (start == -1) { // We have fixed a complete tour, check for its cost:
		double ans = 0.;
		for (int i = 0; i < N; i++)
			ans += instance.getEdge(i, instance.getConnection(i).first) +
				     instance.getEdge(i, instance.getConnection(i).second);
		return ans/2;
	}
	vector<bool> mark(N, false);
	mark[start] = true;
	int cur = start;
	int count = 1;
	double ans = 0.;
	while (count < N) {
		// if cur has one unvisited neighbor follow the chain
		int neigh = instance.getConnection(cur).first;
		if (neigh != -1) {
			while (!mark[neigh]) {
				ans += instance.getEdge(cur, neigh);
				mark[neigh] = true;
				count++;
				int temp = neigh;
				if (instance.getConnection(neigh).second != -1)
					neigh = instance.getConnection(neigh).first + instance.getConnection(neigh).second - cur;
				cur = temp;
			}
			continue;
		}
		int best = -1;
		double bestVal;
		for (int i = 0; i < N; i++) if (!mark[i] && instance.getConnection(i).second == -1)
			if (best == -1 || instance.getEdge(cur, i) < bestVal) {
				best = i;
				bestVal = instance.getEdge(cur, i);
			}
		ans += bestVal;
		cur = best;
		mark[cur] = true;
		count++;
	}
	ans += instance.getEdge(cur, start);
	return ans;
}

void TSP_node::branch() {
	int choicei = -1, choicej;
	const TSP_instance& instance = static_cast<const TSP_instance&>(this->_instance);
	const int N = instance.getN();
	for (int i = 0; i < N; i++) for (int j = 0; j < N; j++) if (instance.getStatus(i,j) == 0)
		if (choicei == -1 || instance.getEdge(i,j) > instance.getEdge(choicei, choicej)) {
			choicei = i;
			choicej = j;
		}
	TSP_delta delta;
	delta.value = choicei*N + choicej;
	emit(delta);
	delta.value = ~delta.value;
	emit(delta);
}


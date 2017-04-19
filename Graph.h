#include <vector>
#include <iostream>
#include <algorithm>
#include "time.h"
#include "math.h"

using namespace std;

class Graph {
	int lastA, lastB, totalGain, half;
	int vNumb, eNumb;
	bool **edge;
	int *part, *gain;
	vector <pair<int, int> > def_a, def_b;
public:
	Graph(int size=0);
	Graph(int sizeV, int sizeE, bool** edgeArr);
	Graph(const Graph &g);
	~Graph();

	void print_matrix();
	void print_defs();
	void print_gains();

	void def_calc();
	void def_recalc();
	void def_balance();
	bool exchange_pair(int stage);

	int max_gain();
	int total_gain() {return totalGain;}
	void create_parts();
	int* create_result();
	int cut_size();
};
#include <iostream>
#include "Graph.h"
#include "stdlib.h"
#include <ctime>
#include "tbb/task_scheduler_init.h" 
#include "tbb/parallel_for.h"
#include "tbb/blocked_range.h"
#include <tbb/partitioner.h>

using namespace tbb; 
using namespace std;

class GraphOptimize
{
	int const iterNumb;
	Graph G_, G1;
	int **const resArr;
	int const vNumb;
public:
	GraphOptimize(int iter_, Graph G, int** resArr_, int vNumb_):iterNumb(iter_), resArr(resArr_), vNumb(vNumb_), G_(G) {}
	
	void printG() { G_.print_matrix();}

	void operator()(const blocked_range<int>& r) const { 
		int begin = r.begin(),  end = r.end();
		for (int j = begin; j != end; j++)
		{
			Graph G1(G_);
			G1.create_parts();
			G1.def_calc();
			G1.def_balance();
			for (int i = 1; i <= vNumb / 2 + 1; i++) {
				if (!G1.exchange_pair(i))
					break;
				G1.def_recalc();
				G1.def_balance();
			}
			resArr[j] = G1.create_result();
		}
	}

};

int minCutSize(int** arr, int n, int vNumb){
	int minSize = arr[0][vNumb];
	int minInd = 0;
	for (int i = 1; i < n; i++){
		if (arr[i][vNumb] < minSize){
			minInd = i;
			minSize = arr[i][vNumb];
		}
	}
	return minInd;
}

void printResult(int* arr, int size)
{
	for (int i = 1; i < 3; i++){
		cout << i << " part:\n";
		for (int j = 0; j < size; j++){
			if (arr[j] == i)
				cout << j << ' ';
		}
		cout << endl;
	}
}

int main(int argc, char** argv) {
	int vNumb = 1, iterNumb = 100, eNumb(0), j;
	float timePar, timeSeq;

    while(vNumb % 2 != 0)
	{
	   cout << "Enter number of vertexes" << endl;
	   std::cin >> vNumb;
	}

	int** resArr = new int*[iterNumb];
	Graph G(vNumb);

	// Main loop of exchanges, sequental
	clock_t startT = clock();
	for (j = 0; j < iterNumb; j++){
		G.create_parts();
		G.def_calc();
		G.def_balance();
		for (int i = 1; i <= vNumb / 2 + 1; i++) {
			if (!G.exchange_pair(i))
				break;
			G.def_recalc();
			G.def_balance();
		}
		resArr[j] = G.create_result();
	}
	clock_t endT = clock();

	timeSeq = (float)((endT - startT)/CLOCKS_PER_SEC);
	//G.print_matrix();
	j = minCutSize(resArr, iterNumb, vNumb);
	cout << "Sequental version:" << endl << "Time = " << timeSeq << endl;
	//cout << "Result:" << endl;
	//printResult(resArr1[j], vNumb);
	cout << endl << endl;

	// Main loop of exchanges, parallel
	//int** resArr = new int*[iterNumb];
	task_scheduler_init init(4);
	GraphOptimize go = GraphOptimize(iterNumb, G, resArr, vNumb);
	//go.printG();
	startT = clock();
	parallel_for (blocked_range<int>(0, iterNumb), go);
	endT = clock();
	init.terminate();
	 
	timePar = (float)((endT - startT)/CLOCKS_PER_SEC);
	j = minCutSize(resArr, iterNumb, vNumb);
	cout << "Parallel version:" << endl << "Time = " << timePar << endl;
	//cout << "Result:" << endl;
	//printResult(resArr[j], vNumb);
	cout << endl << endl;

	cout << "Speedup: " << timeSeq / timePar << endl;

	system("pause");
	return 0;
}
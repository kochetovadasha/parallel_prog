#include "Graph.h"

#define PART_A (1)
#define PART_B (2)

// Sorting of deficits according to the second element
struct sort_def {
	bool operator()(const std::pair<int, int> &left, const std::pair<int, int> &right) {
		return left.second > right.second;
	}
};

Graph::Graph(int size) {
	vNumb = size;
	half = size / 2;
	eNumb = 0;
	totalGain = 0;
	edge = new bool*[size];
	part = new int[size];
	gain = new int[half + 1];

	// Create matrix
	for (int i = 0; i < size; i++) {
		edge[i] = new bool[size];
		for (int j = 0; j < size; j++)
			edge[i][j] = false;
	}

	// Generate matrix
	int prob;
	for (int i = 0; i < size; i++){
		for (int j = i + 1; j < size; j++) {
			prob = rand() % 100;
			if (prob < 65) {
				edge[i][j] = true;
				edge[j][i] = true;
				eNumb++;
			}
		}
	}
}

//Graph::Graph(int sizeV, int sizeE, bool** edgeArr){
//	vNumb = sizeV;
//	half = sizeV / 2;
//	eNumb = sizeE;
//	totalGain = 0;
//	edge = new bool*[sizeV];
//	part = new int[sizeV];
//	gain = new int[half + 1];
//
//	// Create matrix
//	for (int i = 0; i < sizeV; i++) {
//		edge[i] = new bool[sizeV];
//		for (int j = 0; j < sizeV; j++)
//			edge[i][j] = false;
//	}
//
//	// Copy matrix
//	for (int i = 0; i < sizeV; i++){
//		for (int j = 0; j < sizeV; j++) {
//				edge[i][j] = edgeArr[i][j];
//		}
//	}
//}

Graph::Graph(const Graph &g){
	vNumb = g.vNumb;
	half = g.half;
	eNumb = g.eNumb;
	totalGain = 0;
	edge = new bool*[vNumb];
	part = new int[vNumb];
	gain = new int[half + 1];

	// Create matrix
	for (int i = 0; i < vNumb; i++) {
		edge[i] = new bool[vNumb];
		for (int j = 0; j < vNumb; j++)
			edge[i][j] = false;
	}

	// Copy matrix
	for (int i = 0; i < vNumb; i++){
		for (int j = 0; j < vNumb; j++) {
			edge[i][j] = g.edge[i][j];
		}
	}
}

Graph::~Graph() {
	delete[] part;
	delete[] gain;
	for (int i(0); i < vNumb; i++)
		delete[] edge[i];
	delete edge;
}

void Graph::print_matrix() {
	cout << "Graph has " << eNumb << " edges\n\n";
	for (int i = 0; i < vNumb; i++) {
		for (int j = 0; j < vNumb; j++)
			cout << edge[i][j] << ' ';
		cout << endl;
	}
	cout << endl;
}

// Print couples of deficits
//void Graph::print_defs() {
//	if (!def_a.empty() && !def_b.empty()){
//		for (vector<pair<int, int> >::iterator it = def_a.begin(); it != def_a.end(); ++it)
//			cout << it->first << ":" << it->second << " ";
//		cout << endl;
//		for (vector<pair<int, int> >::iterator it = def_b.begin(); it != def_b.end(); ++it)
//			cout << it->first << ":" << it->second << " ";
//		cout << endl << endl;
//	}
//}
//
//void Graph::print_gains() {
//	for (int i=1; i < vNumb / 2 + 1; i++)
//		cout << gain[i] << " ";
//	cout << endl;
//}

void Graph::create_parts(){
	int n = 0;
	for (int i = 0; i < vNumb; i++)
		part[i] = 0;
	do{
		for (int j = 0; j < vNumb; j++){
			int ch = rand() % 100;
			if (ch > 50 && part[j] != PART_A){
				part[j] = PART_A;
				n++;
				if (n == half)
					break;
			}
		}
	} while (n < half);
	for (int i = 0; i < vNumb; i++){
		if (part[i] != PART_A)
			part[i] = PART_B;
	}
}

// Calculate deficits
void Graph::def_calc() {
	for (int i = 0; i <= half; i++)
		gain[i] = 0;
	def_a.clear();
	def_b.clear();
	int def;
	for (int i = 0; i < vNumb; i++) {
		def = 0;
		for (int j = 0; j < vNumb; j++){
			if (edge[i][j]){
				if (part[i] == part[j]) 
					def--; 
				else def++;
			}
		}
		if (part[i] == PART_A)
			def_a.push_back(std::make_pair(i, def));
		else def_b.push_back(std::make_pair(i, def));
	}
}

// Recalculate deficits after couple exchange
void Graph::def_recalc() {
	for (vector<pair<int, int> >::iterator it = def_a.begin(); it != def_a.end(); ++it)
		it->second += 2 * edge[it->first][lastA] - 2 * edge[it->first][lastB];
	for (vector<pair<int, int> >::iterator it = def_b.begin(); it != def_b.end(); ++it)
		it->second += 2 * edge[it->first][lastB] - 2 * edge[it->first][lastA];
}

// Ordering deficiencies after couple exchange
void Graph::def_balance() {
	std::sort(def_a.begin(), def_a.end(), sort_def());
	std::sort(def_b.begin(), def_b.end(), sort_def());
}

// Exchange of couple of vertexes
bool Graph::exchange_pair(int stage) {
	int tgain;
	lastA = def_a.front().first;
	lastB = def_b.front().first;
	tgain = def_a.front().second + def_b.front().second - 2 * edge[lastA][lastB];
	if (tgain > 0){
		def_a.erase(def_a.begin());
		def_b.erase(def_b.begin());
		gain[stage] = gain[stage - 1] + tgain;
		totalGain += tgain;
		if (part[lastA] == PART_A){
			part[lastA] = PART_B;
			part[lastB] = PART_A;
		}
		else{
			part[lastA] = PART_A;
			part[lastB] = PART_B;
		}
		//cout << "Vertexes " << last_a << " and " << last_b << " have been exchanged!" << endl;
		return true;
	}
	return false;
}

// Calculate maximum single benefit after a series of couple exchanges
//int Graph::max_gain() {
//	int gmax = 0;
//	for (int i = 0; i <= vNumb / 2; i++)
//		if (gain[i] > gmax)		
//			gmax = gain[i];
//	return gmax;
//}

//Returns array with total gain and parts
int* Graph::create_result(){
	int* res = new int[vNumb+1];
	res[vNumb] = this->cut_size();
	for (int i = 0; i < vNumb; i++){
		res[i] = part[i];
	}
	return res;
}

int Graph::cut_size(){
	int size = 0;
	for (int i = 0; i < vNumb; i++)
		for (int j = 0; j < vNumb; j++){
			if (edge[i][j] && (part[i] != part[j]))
				size++;
	}
	return size / 2;
}
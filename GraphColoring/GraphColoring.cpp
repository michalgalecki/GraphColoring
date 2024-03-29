// GraphColoring.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>
#include "Node.h"
#include <chrono>
#include <ctime>


using namespace std;

const int N = 7;
static const int COLORS_NUMBER = Node::COLORS_NUMBER;
Node* graph[N][N];
Node* list[N*N];
int results = 0;
int backs = 0;


void initializeGraph() {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			Node *node = new Node(COLORS_NUMBER);
			graph[i][j] = node;
			list[i*N + j] = node;
		}
	}
}

void addNeighbours() {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (i - 1 >= 0) graph[i][j]->addNeighbour(graph[i - 1][j]);
			if (i + 1 < N) graph[i][j]->addNeighbour(graph[i + 1][j]);
			if (j - 1 >= 0) graph[i][j]->addNeighbour(graph[i][j - 1]);
			if (j + 1 < N) graph[i][j]->addNeighbour(graph[i][j + 1]);
			if (i - 1 >= 0 && j - 1 >= 0) graph[i][j]->addDiagonalNeighbour(graph[i - 1][j - 1]);
			if (i - 1 >= 0 && j + 1 < N) graph[i][j]->addDiagonalNeighbour(graph[i - 1][j + 1]);
			if (i + 1 < N && j - 1 >= 0) graph[i][j]->addDiagonalNeighbour(graph[i + 1][j - 1]);
			if (i + 1 < N && j + 1 < N) graph[i][j]->addDiagonalNeighbour(graph[i + 1][j + 1]);
			if (i - 2 >= 0) graph[i][j]->addDiagonalNeighbour(graph[i - 2][j]);
			if (i + 2 < N) graph[i][j]->addDiagonalNeighbour(graph[i + 2][j]);
			if (j - 2 >= 0) graph[i][j]->addDiagonalNeighbour(graph[i][j - 2]);
			if (j + 2 < N) graph[i][j]->addDiagonalNeighbour(graph[i][j + 2]);
		}
	}
}

void printGraph() {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cout << graph[i][j]->currentValue << " ";
		}
		cout << endl;
	}
}

void backtracking() {
	int i = 0;
	while (i < N*N) {
		if (!list[i]->findNextValue()) {
			if (i == 0) return;
			list[i]->restoreDefaultDomain();
			backs++;
			i = i - 1;
		} else {
			i = i + 1;
		}
		if (i == N * N) {
			//cout << "Rozwiazanie: " << results << endl;
			//printGraph();
			list[i-1]->restoreDefaultDomain();
			i = i - 2;
			backs = 0;
			results++;
		}
	}
}

void printAllDomains() {
	cout << "Domeny:" << endl;
	for (int i = 0; i < N * N; i++) {
		for (int j = 0; j < Node::COLORS_NUMBER; j++) {
			cout << list[i]->domainStatus[j] << " ";
		}
		cout << endl;
	}
}

void forwardChecking() {
	int i = 0;
	while (i < N*N) {
		//cout << endl << i << endl;
		//printAllDomains();
		if (!list[i]->checkForward()) {
			if (i == 0) return;
			list[i]->restoreMyDomain();
			list[i]->restoreNeighboursDomain();
			backs++;
			i = i - 1;
			//printGraph();
			//cout << "Powrot" << endl;
		} else {
			i = i + 1;
			//printGraph();
			//cout << "Bez powrotu" << endl;
		}
		//printAllDomains();
		if (i == N * N) {
			//cout << "Rozwiazanie: " << results << endl;
			//printGraph();
			list[i - 1]->restoreMyDomain();
			list[i - 1]->restoreNeighboursDomain();
			i = i - 2;
			backs = 0;
			results++;
			//printAllDomains();
		}
	}
}

int main() {

	initializeGraph();
	addNeighbours();
	long ms = chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now().time_since_epoch()).count();
	backtracking();
	//forwardChecking();
	cout << "Czas: " << chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now().time_since_epoch()).count() - ms << "ms" << " Rozwiazan: " << results << endl;
	//system("pause");
    return 0;
}


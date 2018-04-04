#pragma once
#include <vector>
#include <iostream>


using namespace std;

class Node {
public:
	Node();
	Node(int colorsNum);

	static const int COLORS_NUMBER = 8;
	static const int MAX_NEIGHBOURS_NUMBER = 4;
	static const int MAX_DIAGONAL_NEIGHBOURS_NUMBER = 8;
	static const int MIN_NEIGHBOUR_DIFFERENCE = 2;
	static const int MIN_DIAGONAL_NEIGHBOUR_DIFFERENCE = 1;
	int currentValue;

	void addNeighbour(Node *node);
	void addDiagonalNeighbour(Node* node);
	bool findNextValue();
	bool assignUnusedValue();
	bool checkConstraints();
	void restoreDefaultDomain();

	//forward-checking
	bool domainStatus[COLORS_NUMBER];
	bool checkForward();
	void restoreMyDomain();
	void restoreNeighboursDomain();

	~Node();
private:
	bool isAssigned;
	int domain[COLORS_NUMBER];
	Node* neighbours[MAX_NEIGHBOURS_NUMBER];
	Node* diagonalNeighbours[MAX_DIAGONAL_NEIGHBOURS_NUMBER];
	int currentDomainIndex;
	int numberOfColors;
	int neighboursCount;
	int diagonalNeighboursCount;
	int allNeighboursCount;

	//forward-checking
	//bool domainStatus[COLORS_NUMBER];
	Node* allNeighbours[MAX_NEIGHBOURS_NUMBER + MAX_DIAGONAL_NEIGHBOURS_NUMBER];
	bool neighboursStatus[MAX_NEIGHBOURS_NUMBER + MAX_DIAGONAL_NEIGHBOURS_NUMBER];
	int neighboursDeletedValues[MAX_NEIGHBOURS_NUMBER + MAX_DIAGONAL_NEIGHBOURS_NUMBER][COLORS_NUMBER];
	bool myDeletedValues[COLORS_NUMBER];
	bool assignUnusedValueFC();
	bool modifyNeighboursDomain();
	bool isDomainEmpty();
};


#include "stdafx.h"
#include "Node.h"

Node::Node() {
}

Node::Node(int colorsNum) {
	numberOfColors = colorsNum;

	for (int i = 0; i < numberOfColors; i++) {
		domain[i] = i;
		domainStatus[i] = true;
		myDeletedValues[i] = false;
	}
	
	for (int i = 0; i < MAX_NEIGHBOURS_NUMBER + MAX_DIAGONAL_NEIGHBOURS_NUMBER; i++) {
		for (int j = 0; j < COLORS_NUMBER; j++) {
			neighboursDeletedValues[i][j] = false;
		}
	}

	neighboursCount = 0;
	diagonalNeighboursCount = 0;
	allNeighboursCount = 0;
	isAssigned = false;
	currentDomainIndex = 0;
	currentValue = -1;
}

void Node::addNeighbour(Node *node) {
	neighbours[neighboursCount++] = node;
	allNeighbours[allNeighboursCount++] = node;
}

void Node::addDiagonalNeighbour(Node* node) {
	diagonalNeighbours[diagonalNeighboursCount++] = node;
	allNeighbours[allNeighboursCount++] = node;
}

bool Node::findNextValue() {
	while (currentDomainIndex < numberOfColors) {
		if (assignUnusedValue()) {
			isAssigned = true;
			return true;
		}
	}
	return false;
}

bool Node::assignUnusedValue() {
	currentValue = domain[currentDomainIndex++];
	return checkConstraints();
}

bool Node::checkConstraints() {
	for (int i = 0; i < neighboursCount; i++) {
		if (neighbours[i]->isAssigned) {
			int difference = abs(currentValue - neighbours[i]->currentValue);
			if (difference < MIN_NEIGHBOUR_DIFFERENCE) {
				return false;
			}
		}
	}
	for (int i = 0; i < diagonalNeighboursCount; i++) {
		if (diagonalNeighbours[i]->isAssigned) {
			int difference = abs(currentValue - diagonalNeighbours[i]->currentValue);
			if (difference < MIN_DIAGONAL_NEIGHBOUR_DIFFERENCE) {
				return false;
			}
		}
	}
	return true;
}

void Node::restoreDefaultDomain() {
	isAssigned = false;
	currentDomainIndex = 0;
}

bool Node::checkForward() {
	while (assignUnusedValueFC()) {
		isAssigned = true;
		if (modifyNeighboursDomain()) {
			//cout << "Modify true" << endl;
			return true;
		} else {
			//cout << "Modify false" << endl;
		}
	}
	return false;
}

void Node::restoreMyDomain() {
	isAssigned = false;
	currentValue = -1;
	for (int i = 0; i < COLORS_NUMBER; i++) {
		if (myDeletedValues[i]) {
			domainStatus[i] = true;
		}
	}
}

bool Node::assignUnusedValueFC() {
	for (int i = 0; i < numberOfColors; i++) {
		if (domainStatus[i]) {
			//cout << "Przydzielam wartosc: " << i << endl;
			restoreNeighboursDomain();
			domainStatus[i] = false;
			currentValue = i;
			myDeletedValues[currentValue] = true;
			isAssigned = true;
			return true;
		}
	}
	return false;
}

bool Node::modifyNeighboursDomain() {
	for (int i = 0; i < neighboursCount; i++) {
		if (!neighbours[i]->isAssigned) {
			//cout << "Zwykly: " << i << endl;
			if (neighbours[i]->domainStatus[currentValue]) {
				//cout << "Wylaczam: " << currentValue << endl;
				neighbours[i]->domainStatus[currentValue] = false;
				neighboursDeletedValues[i][currentValue] = true;
				neighboursStatus[i] = true;
			}
			if (currentValue - 1 >= 0 && neighbours[i]->domainStatus[currentValue - 1]) {
				//cout << "Wylaczam: " << currentValue - 1 << endl;
				neighbours[i]->domainStatus[currentValue - 1] = false;
				neighboursDeletedValues[i][currentValue - 1] = true;
				neighboursStatus[i] = true;
			}
			if (currentValue + 1 < COLORS_NUMBER && neighbours[i]->domainStatus[currentValue + 1]) {
				//cout << "Wylaczam: " << currentValue + 1 << endl;
				neighbours[i]->domainStatus[currentValue + 1] = false;
				neighboursDeletedValues[i][currentValue + 1] = true;
				neighboursStatus[i] = true;
			}
			if (neighbours[i]->isDomainEmpty()) {	
				//cout << "Zwykly: " << i << endl;
				return false;
			}
		}
	}
	for (int i = 0; i < diagonalNeighboursCount; i++) {
		if (!diagonalNeighbours[i]->isAssigned) {
			//cout << "Niezwykly: " << i << endl;
			if (diagonalNeighbours[i]->domainStatus[currentValue]) {
				//cout << "Wylaczam: " << currentValue << endl;
				diagonalNeighbours[i]->domainStatus[currentValue] = false;
				neighboursDeletedValues[neighboursCount + i][currentValue] = true;
				neighboursStatus[neighboursCount + i] = true;
			}
			if (diagonalNeighbours[i]->isDomainEmpty()) {
				//cout << "Niezwykly: " << i << endl;
				return false;
			}
		}
	}
	return true;
}

bool Node::isDomainEmpty() {
	for (int i = 0; i < COLORS_NUMBER; i++) {
		if (domainStatus[i]) {
			return false;
		}
	}
	return true;
}

void Node::restoreNeighboursDomain() {
	/*cout << "Zmodyfikowani: " << endl;
	for (int i = 0; i < neighboursCount + diagonalNeighboursCount; i++) {
		for (int j = 0; j < COLORS_NUMBER; j++) {
			cout << neighboursDeletedValues[i][j] << " ";
		}
		cout << endl;

	}
	cout << "Koniec" << endl;*/
	for (int i = 0; i < neighboursCount + diagonalNeighboursCount; i++) {
		if (neighboursStatus[i]) {
			int neighbourType = i < neighboursCount ? 0 : 1;
			for (int j = 0; j < COLORS_NUMBER; j++) {
				if (neighboursDeletedValues[i][j]) {
					neighboursDeletedValues[i][j] = false;
					if (neighbourType == 0) {
						for (int j = 0; j < COLORS_NUMBER; j++) {
							neighbours[i]->myDeletedValues[j] = false;
						}
						neighbours[i]->domainStatus[j] = true;
					} else {
						for (int j = 0; j < COLORS_NUMBER; j++) {
							diagonalNeighbours[i - neighboursCount]->myDeletedValues[j] = false;
						}
						diagonalNeighbours[i - neighboursCount]->domainStatus[j] = true;
					}
				}
			}
		}
	}
}

Node::~Node() {
}

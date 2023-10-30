#include "../include/Grid.h"
#include <iostream>

Grid::Grid() {
	CELL_SIZE = 0;
	WIDTH = -1;
	HEIGHT = -1;
}

Grid::Grid(int cellSize, int boundsWidth, int boundsHeight) {
	CELL_SIZE = cellSize;
	WIDTH = boundsWidth / CELL_SIZE;
	HEIGHT = boundsHeight / CELL_SIZE;
	cells.resize(WIDTH * HEIGHT, std::vector<Circle*>());
}

void Grid::resetCells() {
	for (auto& cell : cells) {
		cell.clear();
	}
}

int Grid::positionToCellIdx(const Vec2D& pos) {
	return int(pos.y() / CELL_SIZE) + (int(pos.x() / CELL_SIZE) * HEIGHT);
}

void Grid::partitionObjects(std::vector<Circle>& objects) {
	for (auto& object : objects) {
		int cellIdx = positionToCellIdx(object.pos);
		Circle* objPtr = &object;
		// sometimes if a 
		try {
			cells.at(cellIdx).push_back(objPtr);
		}
		catch (const std::exception& e) {
			std::cout << "Index Out of Range:\n";
			std::cout << "\tIndex " + std::to_string(cellIdx) + " in 'cells' vector of size " + std::to_string(cells.size()) << std::endl;
		}
		
	}
}

std::string Grid::toString() {
	std::string gridString("");
	gridString += "Cell size: " + std::to_string(CELL_SIZE) + "\n";
	gridString += "Cell count: " + std::to_string(cells.size()) + "\n";
	gridString += "Dimensions:\n\tw: " + std::to_string(WIDTH) + "\th: " + std::to_string(HEIGHT) + "\n";

	for (int i = HEIGHT - 1; i >= 0; i--) {
		gridString += "\n[ ";
		for (int j = i; j <= HEIGHT * (WIDTH - 1) + i; j += HEIGHT) {
			gridString += std::to_string(cells.at(j).size()) + " ";
		}
		gridString += "]";
	}
	return gridString;
}
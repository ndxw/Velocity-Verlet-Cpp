#include "../include/Grid.h"
#include <iostream>

/// <summary>
/// Constructs a null grid.
/// </summary>
Grid::Grid() {
	CELL_SIZE = 0;
	WIDTH = -1;
	HEIGHT = -1;
}

/// <summary>
/// Constructs a grid with the given parameters.
/// </summary>
/// <param name="cellSize">Cell size, in pixels. Ideally, the maximum size of the objects being used.</param>
/// <param name="boundsWidth">Width of the area the grid should cover, in pixels.</param>
/// <param name="boundsHeight">Height of the area the grid should cover, in pixels.</param>
Grid::Grid(int cellSize, int boundsWidth, int boundsHeight) {
	CELL_SIZE = cellSize;
	WIDTH = boundsWidth / CELL_SIZE;
	HEIGHT = boundsHeight / CELL_SIZE;
	cells.resize(WIDTH * HEIGHT, std::vector<Circle*>());
}

void Grid::setGridWidth(const int boundsWidth) {
	WIDTH = boundsWidth / CELL_SIZE;
	cells.resize(WIDTH * HEIGHT, std::vector<Circle*>());
}

void Grid::setGridHeight(const int boundsHeight) {
	HEIGHT = boundsHeight / CELL_SIZE;
	cells.resize(WIDTH * HEIGHT, std::vector<Circle*>());
}

/// <summary>
/// Empties all grid cells.
/// </summary>
void Grid::resetCells() {
	for (auto& cell : cells) {
		cell.clear();
	}
}

/// <summary>
/// Takes a position vector and calculates the cell index that contains the position.
/// </summary>
/// <param name="pos"></param>
/// <returns>Cell index, may not be within grid bounds.</returns>
int Grid::positionToCellIdx(const Vec2D& pos) {
	return int(pos.y() / CELL_SIZE) + (int(pos.x() / CELL_SIZE) * HEIGHT);
}

/// <summary>
/// Takes a list of objects and adds a pointer to each of them to their corresponding cell in the grid.
/// </summary>
/// <param name="objects"></param>
void Grid::partitionObjects(std::vector<Circle>& objects, const RectBounds& bounds) {
	for (auto& object : objects) {
		int cellIdx = positionToCellIdx(object.pos);
		Circle* objPtr = &object;

		try {
			cells.at(cellIdx).push_back(objPtr);
		}
		catch (const std::exception& e) {
			std::cout << "Index Out of Range:\n";
			std::cout << "\tIndex " + std::to_string(cellIdx) + " in 'cells' vector of size " + std::to_string(cells.size()) + "\n";
			std::cout << info() + "\n";
			std::cout << bounds.toString() + "\n" << std::endl;
		}
	}
}

/// <summary>
/// Takes a cell index and determines if it is in the top-most row.
/// </summary>
/// <param name="cellIdx">Must be a value between 0 and <c>grid.WIDTH * grid.HEIGHT - 1</c> inclusive.</param>
/// <returns>true | false</returns>
bool Grid::isTopRow(int cellIdx) { return (cellIdx % HEIGHT) == (HEIGHT - 1); }
/// <summary>
/// Takes a cell index and determines if it is in the bottom-most row.
/// </summary>
/// <param name="cellIdx">Must be a value between 0 and <c>grid.WIDTH * grid.HEIGHT - 1</c> inclusive.</param>
/// <returns>true | false</returns>
bool Grid::isBottomRow(int cellIdx) { return (cellIdx % HEIGHT) == 0; }
/// <summary>
/// Takes a cell index and determines if it is in the left-most column.
/// </summary>
/// <param name="cellIdx">Must be a value between 0 and <c>grid.WIDTH * grid.HEIGHT - 1</c> inclusive.</param>
/// <returns>true | false</returns>
bool Grid::isLeftCol(int cellIdx) { return cellIdx < HEIGHT; }
/// <summary>
/// Takes a cell index and determines if it is in the right-most column.
/// </summary>
/// <param name="cellIdx">Must be a value between 0 and <c>grid.WIDTH * grid.HEIGHT - 1</c> inclusive.</param>
/// <returns>true | false</returns>
bool Grid::isRightCol(int cellIdx) { return cellIdx >= (cells.size() - HEIGHT); }

/// <summary>
/// Converts the grid to string format.
/// </summary>
/// <returns>A string containing information including cell size, cell count, grid dimensions, and the grid itself.</returns>
std::string Grid::toString() {
	std::string gridString("");

	for (int i = HEIGHT - 1; i >= 0; i--) {
		gridString += "\n[ ";
		for (int j = i; j <= HEIGHT * (WIDTH - 1) + i; j += HEIGHT) {
			gridString += std::to_string(cells.at(j).size()) + " ";
		}
		gridString += "]";
	}
	return gridString;
}

std::string Grid::info() {
	std::string gridString("");

	gridString += "Cell size: " + std::to_string(CELL_SIZE) + "\n";
	gridString += "Cell count: " + std::to_string(cells.size()) + "\n";
	gridString += "Dimensions:\n\tw: " + std::to_string(WIDTH) + "\th: " + std::to_string(HEIGHT);

	return gridString;
}
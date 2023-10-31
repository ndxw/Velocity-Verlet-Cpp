#ifndef GRID_H
#define GRID_H

#include <vector>
#include "Objects.h"

class Grid
{
public:
	std::vector<std::vector<Circle*>> cells;
	int CELL_SIZE;
	int WIDTH;
	int HEIGHT;

	Grid();
	Grid(int, int, int);

	void resetCells();
	int positionToCellIdx(const Vec2D&);
	void partitionObjects(std::vector<Circle>&);
	std::string toString();
};

#endif
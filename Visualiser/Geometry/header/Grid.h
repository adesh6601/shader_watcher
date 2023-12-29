#pragma once
#include <vector>
#include "pch.h"

class GEOMETRY_API Grid
{
public:
	Grid();
	~Grid();

	void drawGrid(int size, std::vector<double>& vertices, std::vector<double>& colors);
};

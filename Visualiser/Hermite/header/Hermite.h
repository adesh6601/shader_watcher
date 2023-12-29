#pragma once
#include "pch.h"
#include "Point3D.h"
#include <vector>

class HERMITE_API Hermite
{
public:
	Hermite();
	~Hermite();

	void drawCurve(Point3D inP0, Point3D inP1, Point3D inP2, Point3D inP3, std::vector<float>& mVertices, std::vector<float>& mColors);
};

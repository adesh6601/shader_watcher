#pragma once
#include "pch.h"
#include "Point3D.h"
#include <vector>


class BEZIER_API Bezier
{
public:
	Bezier();
	~Bezier();

	void drawCurve(Point3D inP0, Point3D inP1, Point3D inP2, Point3D inP3, std::vector<float>& mVertices, std::vector<float>& mColors);

};
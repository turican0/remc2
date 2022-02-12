#pragma once

#ifndef PROJECTION_POLYGON
#define PROJECTION_POLYGON

class ProjectionPolygon
{
public:

	int X = 0;
	int Y = 0;
	int U = 0;
	int V = 0;
	int Brightness = 0;
	int Unknown = 0;

	ProjectionPolygon(int* vertexs);
};

#endif //PROJECTION_POLYGON
#include "ProjectionPolygon.h"

ProjectionPolygon::ProjectionPolygon(int* vertexs)
{
	X = vertexs[0];
	Y = vertexs[1];
	U = vertexs[2];
	V = vertexs[3];
	Brightness = vertexs[4];
}
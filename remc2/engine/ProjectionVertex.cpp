#include "ProjectionVertex.h"
#include <string>

ProjectionVertex::ProjectionVertex(int* vertexs)
{
	X = vertexs[0];
	Y = vertexs[1];
	U = vertexs[2];
	V = vertexs[3];
	Brightness = vertexs[4];
}

std::string ProjectionVertex::Print()
{
	return "X: " + std::to_string(X) + " Y: " + std::to_string(Y) + " U: " + std::to_string(U) + " V: " + std::to_string(V) + " B: " + std::to_string(Brightness);
}
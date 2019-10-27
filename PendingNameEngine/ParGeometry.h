#ifndef _PARGEOMETRY_H_
#define _PARGEOMETRY_H_
#include "Globals.h"

class ParGeometry
{
public:
	ParGeometry();
	~ParGeometry();
	void GeometryGenerator(uint type);

public:
	char* name = nullptr;
};


#endif // !_PARGEOMETRY_H_



#ifndef _PARGEOMETRY_H_
#define _PARGEOMETRY_H_
#include "Globals.h"
#include "MathGeoLib/include/MathGeoLib.h"

struct par_shapes_mesh_s;

class ParGeometry
{
public:
	ParGeometry();
	~ParGeometry();
	void GeometryGenerator(par_shapes_mesh_s* par, float3 pos, char* name);


};


#endif // !_PARGEOMETRY_H_



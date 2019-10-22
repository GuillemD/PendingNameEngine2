#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include "MathGeoLib/include/MathGeoLib.h"

class Transform
{
public:
	Transform();
	~Transform();


	float3 pos;;
	Quat rot;
	float3 scale;

	float3 euler_rot;
};

#endif // !_TRANSFORM_H_




#ifndef _COMPONENTCAMERA_H_
#define _COMPONENTCAMERA_H_

#include "Component.h"

class ComponentCamera :
	public Component
{
public:
	ComponentCamera(GameObject* p);
	~ComponentCamera();
};



#endif // !_COMPONENTCAMERA_H_


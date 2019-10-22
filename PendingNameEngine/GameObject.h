#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

#include "Globals.h"

class GameObject
{
public:
	GameObject();
	~GameObject();

	void Update();
	void Draw();
public:
	GameObject* parent;
	bool selected;
	std::string go_name;

private:
	bool is_active;
	bool is_static;
	std::vector<GameObject*> childs;
	
};
#endif // !_GAMEOBJECT_H_

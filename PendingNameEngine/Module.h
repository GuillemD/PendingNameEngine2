#ifndef _MODULE_H_
#define _MODULE_H_
#include "Globals.h"
#include "Parson/parson.h"

class Application;

class Module
{
private :
	bool enabled;
public:
	const char* name = "";

public:

	Module(bool start_enabled = true) 
	{}

	virtual ~Module()
	{}

	virtual bool Init(JSON_Object* conf) 
	{
		return true; 
	}

	virtual bool Start()
	{
		return true;
	}

	virtual update_status PreUpdate(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status Update(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status PostUpdate(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual bool CleanUp() 
	{ 
		return true; 
	}

	const char* GetName() const { return name; }

	virtual void Save(JSON_Object* config){}
};

extern Application* App;

#endif //_MODULE_H_
#ifndef _MODULE_H_
#define _MODULE_H_
#include "Globals.h"

#include "rjson.h"

using namespace rapidjson;

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

	virtual bool Init() 
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

	virtual bool Save(Document& document, FileWriteStream& os) 
	{ 
		Document::AllocatorType& alloc = document.GetAllocator();
		Writer<FileWriteStream> writer(os);
		return true; 
	}
	virtual bool Load(Document* doc) { return true; }
};

extern Application* App;

#endif //_MODULE_H_
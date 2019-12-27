#ifndef _GLOBALS_H_
#define _GLOBALS_H_

// Warning disabled ---
#pragma warning( disable : 4577 ) // Warning that exceptions are disabled
#pragma warning( disable : 4530 )

#include <windows.h>
#include <stdio.h>
#include <vector>
#include <list>
#include <string>

#define LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);
#define CONSOLELOG(format, ...) consolelog(__FILE__, __LINE__, format, __VA_ARGS__);

void log(const char file[], int line, const char* format, ...);
void consolelog(const char file[], int line, const char* format, ...);

#define CAP(n) ((n <= 0.0f) ? n=0.0f : (n >= 1.0f) ? n=1.0f : n=n)

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
#define HALF_PI 1.57079632679489661923f
#define PI 3.14159265358979323846f

#define ASSETS_FOLDER "Assets/"
#define SETTINGS_FOLDER "Settings/"
#define LIBRARY_FOLDER "Library/"
#define LIBRARY_AUDIO_FOLDER "Library/Audio/"
#define LIBRARY_TEXTURES_FOLDER "/Library/Textures/"
#define LIBRARY_MESH_FOLDER "Library/Meshes/"
#define LIBRARY_ANIMATION_FOLDER "Library/Animations/"
#define LIBRARY_SCENE_FOLDER "Library/Scenes/"
#define LIBRARY_MODEL_FOLDER "Library/Models/"
#define LIBRARY_MATERIAL_FOLDER "Library/Materials/"
#define LIBRARY_STATE_MACHINE_FOLDER "Library/StateMachines/"


#ifndef _DEBUG
#define AK_OPTIMIZED
#endif




typedef unsigned int uint;
typedef long long UID;
// Deletes a buffer
#define RELEASE( x )\
    {\
       if( x != nullptr )\
       {\
         delete x;\
	     x = nullptr;\
       }\
    }

// Deletes an array of buffers
#define RELEASE_ARRAY( x )\
	{\
       if( x != nullptr )\
       {\
           delete[] x;\
	       x = nullptr;\
		 }\
	 }

enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

// Configuration -----------
#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 950
#define VSYNC true
#define TITLE "PendingNameEngine2"
#define ORGANISATION "CITM (UPC)"
#define VERSION "0.1.0"

//ImGui
#define BLUE ImVec4(0,0,1,1)
#define RED ImVec4(1,0,0,1)
#define GREEN ImVec4(0,1,0,1)
#define WHITE ImVec4(1,1,1,1)
#define YELLOW ImVec4(1,1,0,1)

//rapidjson
#define RJSON_MAJOR_COMPILED 1
#define RJSON_MINOR_COMPILED 1
#define RJSON_PATCH_COMPILED 0

#endif //_GLOBALS_H_
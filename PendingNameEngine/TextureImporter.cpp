#include "TextureImporter.h"
#include "Application.h"
#include "Texture.h"
#include "ComponentMaterial.h"
#include "ComponentMesh.h"

#include "OpenGL.h"
#include "DevIL/include/il.h"
#include "DevIL/include/ilut.h"


#pragma comment( lib, "DevIL/libx86/DevIL.lib" )
#pragma comment( lib, "DevIL/libx86/ILU.lib" )
#pragma comment( lib, "DevIL/libx86/ILUT.lib" )

#include <fstream>


TextureImporter::TextureImporter()
{
	
}


TextureImporter::~TextureImporter()
{
}

bool TextureImporter::Start()
{
	bool ret=true;
	CONSOLELOG("Initialising il");
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
	
	CreateCheckersTexture();

	return ret;
}

bool TextureImporter::CleanUp()
{
	bool ret = true;
	LOG("Cleaning textures up");
	aiDetachAllLogStreams();
	ilShutDown();
	return ret;
}


bool  TextureImporter::AddTextureToLibrary(const char* path, std::string& output, std::string& tex_name)
{
	//Chop the extension and the path from the actual name
	std::string name = path;
	std::replace(name.begin(), name.end(), '\\', '/');
	uint lastSlash = name.find_last_of('/');
	uint dot = name.find_last_of('.');
	name = name.substr(lastSlash + 1, dot - lastSlash - 1);
	std::string assetsPath;
	assetsPath = "Assets/Meshes_Textures/" + name + ".png";
	std::string libPath;
	libPath = "Library/Textures/" + name + ".dds";

	ILuint imageID;
	ILboolean success;

	ilGenImages(1, &imageID);
	ilBindImage(imageID);

	success = ilLoadImage(assetsPath.c_str());

	if (success)
	{
		ILinfo ImageInfo;
		iluGetImageInfo(&ImageInfo);

		if (ilGetError() == IL_NO_ERROR)
		{
			ILuint size;
			ILubyte *data;
			ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);// To pick a specific DXT compression use
			size = ilSaveL(IL_DDS, NULL, 0); // Get the size of the data buffer
			if (size > 0) {
				data = new ILubyte[size]; // allocate data buffer
				if (ilSaveL(IL_DDS, data, size) > 0) // Save to buffer with the ilSaveIL function
				{
					std::ofstream dataFile(libPath, std::fstream::out | std::fstream::binary);
					dataFile.write((const char*)data, size);
					dataFile.close();
				}
				RELEASE_ARRAY(data);
			}
		}
	}
	else
	{
		ILenum error = ilGetError();
		LOG("Image load failed - IL reports error: %s", iluErrorString(error));
		return false;
	}

	ilDeleteImages(1, &imageID);

	tex_name = name;
	output = libPath;

	return true;

}

Texture* TextureImporter::LoadTextureFromPath(const char * path)
{
	CONSOLELOG("Importing texture %s ...", path);

	if (App->scene->scene_gameobjects.empty())
		return nullptr;

	Texture* t = new Texture();

	ILuint image_id;
	GLuint tex_id;
	ILenum error;
	ILboolean imageloaded;

	ilGenImages(1, &image_id);
	ilBindImage(image_id);

	imageloaded = ilLoadImage(path);

	if (imageloaded) {
		ILinfo info;
		iluGetImageInfo(&info);

		if (info.Origin == IL_ORIGIN_UPPER_LEFT)
		{
			iluFlipImage();
		}

		imageloaded = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
		if (!imageloaded)
		{
			error = ilGetError();
			CONSOLELOG("DevIL failed to convert image %s. Error: %s . :(", path, iluErrorString(error));
			return nullptr;
		}

		t->SetWidth(ilGetInteger(IL_IMAGE_WIDTH));
		t->SetHeight(ilGetInteger(IL_IMAGE_HEIGHT));
		t->tex_path = path;
			
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glGenTextures(1, &tex_id);

		glBindTexture(GL_TEXTURE_2D, tex_id);

		t->SetTextureId(tex_id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 
								0, 
							GL_RGB,
		ilGetInteger(IL_IMAGE_WIDTH),
		ilGetInteger(IL_IMAGE_HEIGHT),
								0, 
		ilGetInteger(IL_IMAGE_FORMAT),
				GL_UNSIGNED_BYTE,
					ilGetData());


		CONSOLELOG("Image width: %d", t->GetWidth());
		CONSOLELOG("Image height: %d",t->GetHeight());


		CONSOLELOG("Texture %s loaded correctly", path);

	}
	else
	{
		error = ilGetError();
		CONSOLELOG("DevIL: Unable to load image correctly. Error: %s. :( Texture_id set to %d", iluErrorString(error), tex_id);
		return nullptr;
	}

	
	return t;
}

int TextureImporter::GetVersion() const
{
	return IL_VERSION;
}
#define CHECKERS_WIDTH 64
#define CHECKERS_HEIGHT 64

bool TextureImporter::CreateCheckersTexture()
{
	GLubyte checkImage[CHECKERS_HEIGHT][CHECKERS_WIDTH][4];

	for (int i = 0; i < CHECKERS_HEIGHT; i++) {
		for (int j = 0; j < CHECKERS_WIDTH; j++) {
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
			checkImage[i][j][0] = (GLubyte)c;
			checkImage[i][j][1] = (GLubyte)c;
			checkImage[i][j][2] = (GLubyte)c;
			checkImage[i][j][3] = (GLubyte)255;
		}
	}

	uint ImageName = 0;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glGenTextures(1, &ImageName);
	glBindTexture(GL_TEXTURE_2D, ImageName);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CHECKERS_WIDTH, CHECKERS_HEIGHT,
		0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);

	
	App->scene->checkers = new Texture(ImageName, CHECKERS_WIDTH, CHECKERS_HEIGHT, "Checkers");
	CONSOLELOG("Checkers Texture created");
	return true;
}

#include "TextureImporter.h"
#include "Application.h"


#include "OpenGL.h"
#include "DevIL/include/il.h"
#include "DevIL/include/ilut.h"


#pragma comment( lib, "DevIL/libx86/DevIL.lib" )
#pragma comment( lib, "DevIL/libx86/ILU.lib" )
#pragma comment( lib, "DevIL/libx86/ILUT.lib" )


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

int TextureImporter::GetILVersion()
{
	return IL_VERSION;
}

uint TextureImporter::LoadTextureFromPath(const char * path)
{
	CONSOLELOG("Importing texture %s", path);

	ILuint image_id;
	GLuint tex_id;
	ILenum error;

	ilGenImages(1, &image_id);
	ilBindImage(image_id);

	ILboolean imageloaded = true;

	imageloaded = ilLoadImage(path);

	if (imageloaded) {
		ILinfo info;
		iluGetImageInfo(&info);

		if (info.Origin == IL_ORIGIN_UPPER_LEFT)
		{
			iluFlipImage();
		}

		ILboolean converted = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
		if (!converted)
		{
			tex_id = 0;
			CONSOLELOG("DevIL failed to convert image %s. Error: %s", path, iluErrorString(ilGetError()));
			return tex_id;
		}
		int width = ilGetInteger(IL_IMAGE_WIDTH);
		int height = ilGetInteger(IL_IMAGE_HEIGHT);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		glGenTextures(1, &tex_id);

		glBindTexture(GL_TEXTURE_2D, tex_id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());


		CONSOLELOG("Image width: %d", ilGetInteger(IL_IMAGE_WIDTH));
		CONSOLELOG("Image height: %d", ilGetInteger(IL_IMAGE_HEIGHT));


		CONSOLELOG("Texture %s loaded correctly", path);


		for (std::vector<Mesh*>::iterator it = App->scene->scene_meshes.begin(); it != App->scene->scene_meshes.end(); it++)
		{
			(*it)->texcoords_id = tex_id;
		}

	}
	else
	{
		tex_id = 0;
		error = ilGetError();
		CONSOLELOG("DevIL: Unable to load image correctly. Error: %s. Texture_id set to %d", iluErrorString(error), tex_id);
		return tex_id;
	}

	return tex_id;
}

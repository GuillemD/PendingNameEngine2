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
	CONSOLELOG("Initialising il")
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
}


TextureImporter::~TextureImporter()
{
}

bool TextureImporter::Start()
{
	bool ret=true;

	/*current_path ="";*/

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
	CONSOLELOG("Trying to import texture from %s . ", path);
	GLuint texture_id=0;

	ILenum error;
	ILboolean image_loaded_correctly = ilLoadImage(path);

	

	if (image_loaded_correctly) {
		ILuint image_id;
		ilGenImages(1, &image_id);
		ilBindImage(image_id);

		ILinfo info;
		iluGetImageInfo(&info);

		if (info.Origin == IL_ORIGIN_UPPER_LEFT) {
			iluFlipImage();
		}
		ILboolean converted = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
		CONSOLELOG("Devilising teture. >:)");
		if (!converted)
		{
			texture_id = 0;
			CONSOLELOG("DevIL failed to convert image %s. Error: %s", path, iluErrorString(ilGetError()));
			return texture_id;
		}
		else {
			int width = ilGetInteger(IL_IMAGE_WIDTH);
			int height = ilGetInteger(IL_IMAGE_HEIGHT);

			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

			glGenTextures(1, &texture_id);

			glBindTexture(GL_TEXTURE_2D, texture_id);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());


			CONSOLELOG("Image width: %d", ilGetInteger(IL_IMAGE_WIDTH));
			CONSOLELOG("Image height: %d", ilGetInteger(IL_IMAGE_HEIGHT));


			CONSOLELOG("Texture loaded correctly from %s .", path);

			for (std::vector<Mesh*>::iterator it = App->scene->scene_meshes.begin(); it != App->scene->scene_meshes.end(); it++) {
				(*it)->texcoords_id = texture_id;
			}
		}

	}	
	else {
		texture_id = 0;
		CONSOLELOG("Failed to loadtexture from %s . Error: %s",path, iluErrorString(error));
	}


	return texture_id;
}

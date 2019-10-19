#include "TextureImporter.h"
#include "Application.h"
#include "Texture.h"

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


uint TextureImporter::LoadTextureFromPath(const char * path)
{
	CONSOLELOG("Importing texture %s", path);

	Texture* t = new Texture();

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
			exit(-1);
		}

		
		t->SetWidth(ilGetInteger(IL_IMAGE_WIDTH));
		t->SetHeight(ilGetInteger(IL_IMAGE_HEIGHT));
		t->SetName(path); //temporary
			
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
					t->GetWidth(), 
					t->GetHeight(),
								0, 
		ilGetInteger(IL_IMAGE_FORMAT),
				GL_UNSIGNED_BYTE,
					ilGetData());


		CONSOLELOG("Image width: %d", t->GetWidth());
		CONSOLELOG("Image height: %d",t->GetHeight());


		CONSOLELOG("Texture %s loaded correctly", path);

		App->scene->textures.push_back(t);

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

int TextureImporter::GetVersion() const
{
	return IL_VERSION;
}

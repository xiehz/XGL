#include "stdafx.h"
#include "GL/glew.h"
#include "xgl\Error.h"
#include "XGLUtil.h"
#include "XCubemapTexture.h"
#include "Magick++.h"
#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

static const GLenum types[6] = { 
GL_TEXTURE_CUBE_MAP_POSITIVE_X,
GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
GL_TEXTURE_CUBE_MAP_NEGATIVE_Z };


XGLModel::XCubemapTexture::XCubemapTexture(const std::string & dir, 
	const std::string & posXFileName, 
	const std::string & negXFileName, 
	const std::string & posYFileName, 
	const std::string & negYFileName, 
	const std::string & posZFileName, 
	const std::string & negZFileName)
{
	m_fileNames[0] = dir + posXFileName;
	m_fileNames[1] = dir + negXFileName;

	m_fileNames[2] = dir + posYFileName;
	m_fileNames[3] = dir + negYFileName;
		
	m_fileNames[4] = dir + posZFileName;
	m_fileNames[5] = dir + negZFileName;

	m_texObj = 0;
}

XGLModel::XCubemapTexture::~XCubemapTexture()
{
	if (m_texObj)
		glDeleteTextures(1, &m_texObj);
}

bool XGLModel::XCubemapTexture::load()
{
	glGenTextures(1, &m_texObj);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texObj);

	Magick::Image* pImage = NULL;
	Magick::Blob blob;

	for (unsigned int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(types); i++) {
		pImage = new Magick::Image(m_fileNames[i]);

		try {
			pImage->write(&blob, "RGBA");
		}
		catch (Magick::Error& Error) {
			XGLERROR(Error.what());
			delete pImage;
			return false;
		}

		glTexImage2D(types[i], 0, GL_RGB, pImage->columns(), pImage->rows(), 0, GL_RGBA,
			GL_UNSIGNED_BYTE, blob.data());
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		delete pImage;
	}

	return true;
}

void XGLModel::XCubemapTexture::bindTexture(GLenum textureUnit)
{
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texObj);
}

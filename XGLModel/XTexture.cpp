#include "stdafx.h"
#include <iostream>
#include "XTexture.h"

XGLModel::XTexture::XTexture(GLenum target, const std::string & imageName)
{
	this->m_texTarget = target;
	this->m_texFile = imageName;
}

XGLModel::XTexture::~XTexture()
{

}

bool XGLModel::XTexture::Load()
{
	Magick::Image image;
	Magick::Blob blob;

	try {
		image.read(m_texFile);
		image.write(&blob, "RGBA");
	}
	catch (Magick::Error& Error) {
		std::cout << "Error loading texture '" << m_texFile << "': " << Error.what() << std::endl;
		return false;
	}

	glGenTextures(1, &m_texObj);
	glBindTexture(m_texTarget, m_texObj);
	glTexImage2D(m_texTarget, 0, GL_RGBA, image.columns(), image.rows(), 0, GL_RGBA, GL_UNSIGNED_BYTE, blob.data());
	glTexParameterf(m_texTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(m_texTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(m_texTarget, 0);

	return true;
}

void XGLModel::XTexture::Bind(GLenum texUnit)
{
	glActiveTexture(texUnit);
	glBindTexture(m_texTarget, m_texObj);
}

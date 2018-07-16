#include "stdafx.h"
#include "XGLModel.h"
#include "xgl/Vec3f"
#include "XRandomTexture.h"

using namespace XGL;
float RandomFloat()
{
	float Max = RAND_MAX;
	return ((float)rand() / Max);
}

XGLModel::XRandomTexture::XRandomTexture()
{
}


XGLModel::XRandomTexture::~XRandomTexture()
{
}

bool XGLModel::XRandomTexture::Init(unsigned int Size)
{
	Vec3f* pRandomData = new Vec3f[Size];
	for (unsigned int i = 0; i < Size; i++) {
		pRandomData[i].set(RandomFloat(), RandomFloat(), RandomFloat());
	}

	glGenTextures(1, &m_texObj);
	glBindTexture(GL_TEXTURE_1D, m_texObj);
	glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB, Size, 0, GL_RGB, GL_FLOAT, pRandomData);
	glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);

	delete[] pRandomData;

	return GLCheckError();
}

void XGLModel::XRandomTexture::Bind(GLenum texUnit)
{
	glActiveTexture(texUnit);
	glBindTexture(GL_TEXTURE_1D, m_texObj);
}

#include "stdafx.h"
#include "xgl/XOrbitCamera.h"
#include "IXMesh.h"
#include "XGBuffer.h"
#include "DirLightShader.h"
#include "PointLightShader.h"
#include "DSSphere.h"

namespace XGLModel {

	REGISTER(DSSphere)

}

XGLModel::DSSphere::DSSphere()
{
	m_LightSphere = 0;
}


XGLModel::DSSphere::~DSSphere()
{
	if (m_LightSphere)
		delete m_LightSphere;
	m_LightSphere = 0;
}

void XGLModel::DSSphere::initGL()
{
	m_LightSphere = new IXMesh();
	m_LightSphere->LoadMesh("E:/2018/opengl/Assimp/data/sphere.obj");
}


void XGLModel::DSSphere::draw()
{
	Matrixf mv = camera->getInverseMatrix();
	mv.preMult(m_WorldMT);
	Matrixf mvp = projectMatrix;
	mvp.preMult(mv);
	glUniformMatrix4fv(g_mvp, 1, GL_FALSE, mvp.ptr());
	m_LightSphere->Render();
}

void XGLModel::DSSphere::initUniform()
{
	g_mvp = glGetUniformLocation(program, "g_mvp");
	if (g_mvp < 0)
		XGLERROR("get mvp uniform failed");
}

void XGLModel::DSSphere::setCamera(const XGL::Matrixf& camera)
{
	this->camera->setByInverseMatrix(camera);
}
void XGLModel::DSSphere::setModelByPointLight(const TagPointLight& pointlight)
{
	float b = CalcPointLightBSphere(pointlight);
	b = 3;
	Matrixf scale = Matrixf::scale(b, b, b);
	XGL::Vec3f world = pointlight.Eposition * this->camera->getMatrix();

	m_WorldMT= Matrixf::translate(world);
	m_WorldMT.preMult(scale);
}


float XGLModel::DSSphere::CalcPointLightBSphere(const XGLModel::TagPointLight& Light)
{
	float MaxChannel = std::fmax(std::fmax(Light.Color.x(), Light.Color.y()), Light.Color.z());

	float ret = (-Light.Attenuation.Linear + sqrtf(Light.Attenuation.Linear * Light.Attenuation.Linear - 4 * Light.Attenuation.Exp * (Light.Attenuation.Exp - 256 * MaxChannel * Light.DiffuseIntensity)))
		/
		(2 * Light.Attenuation.Exp);

	return ret;
}
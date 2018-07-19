#include "stdafx.h"
#include <sstream>
#include "XGLUtil.h"
#include "xgl/Error.h"
#include "DirLightShader.h"

XGLModel::DirLightShader::DirLightShader()
{
	 
}

XGLModel::DirLightShader::~DirLightShader()
{
}

void XGLModel::DirLightShader::initUniform()
{
	g_mv = glGetUniformLocation(program, "g_mv");
	g_normal = glGetUniformLocation(program, "g_normal");
	g_mvp = glGetUniformLocation(program, "g_mvp");
	m_lightLocation.Color = glGetUniformLocation(program, "g_directionlight.Color");
	m_lightLocation.AmbientIntensity = glGetUniformLocation(program, "g_directionlight.AmbientIntensity");
	m_lightLocation.DiffuseIntensity = glGetUniformLocation(program, "g_directionlight.DiffuseIntensity");
	m_lightLocation.Direction = glGetUniformLocation(program, "g_directionlight.Direction");

	 g_shineness = glGetUniformLocation(program,"g_shineness");
	 g_materialIntensity = glGetUniformLocation(program, "g_materialIntensity");

	 g_tex_pos = glGetUniformLocation(program, "g_sampler_pos");
	 g_tex_diffuse = glGetUniformLocation(program, "g_sampler_diffuse");
	 g_tex_normal = glGetUniformLocation(program, "g_sampler_normal");
	 g_screensize = glGetUniformLocation(program, "g_screensize");
}

void XGLModel::DirLightShader::updateMT(const XGL::Matrixf & mv, const XGL::Matrixf & pers)
{
	glUniformMatrix4fv(g_mv, 1, GL_FALSE ,mv.ptr());

	XGL::Matrixf normal = XGL::Matrixf::inverse(mv);
	normal.transpose(normal);
	glUniformMatrix4fv(g_normal, 1, GL_FALSE , normal.ptr());

	XGL::Matrixf mvp = pers;
	mvp.preMult(mv);

	glUniformMatrix4fv(g_mvp, 1, GL_FALSE , mvp.ptr());

}

void XGLModel::DirLightShader::updateScreen(unsigned int width, unsigned int height)
{
	glUniform2f(g_screensize, width, height);
}

void XGLModel::DirLightShader::updateSampler(int pos, int diffuse, int normal)
{
	glUniform1i(g_tex_pos, pos);
	glUniform1i(g_tex_diffuse, diffuse);
	glUniform1i(g_tex_normal, normal);
}

void XGLModel::DirLightShader::updateLight(const TagDirectionLight& pl, float materialIntensity, float shineness)
{
	glUniform1f(m_lightLocation.AmbientIntensity, pl.AmbientIntensity);
	glUniform1f(m_lightLocation.DiffuseIntensity, pl.DiffuseIntensity);
	glUniform3f(m_lightLocation.Color, pl.Color.x(), pl.Color.y(), pl.Color.z());
	glUniform3f(m_lightLocation.Direction, pl.Direction.x(), pl.Direction.y(), pl.Direction.z());

	glUniform1f(g_shineness, shineness);
	glUniform1f(g_materialIntensity, materialIntensity);

}

void XGLModel::DirLightShader::initGL()
{
}

void XGLModel::DirLightShader::draw()
{
}

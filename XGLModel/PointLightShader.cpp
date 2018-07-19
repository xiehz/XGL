#include "stdafx.h"
#include <assert.h>
#include <sstream>
#include <string>
#include "XGLUtil.h"
#include "xgl/Error.h"
#include "PointLightShader.h"


XGLModel::PointLightShader::PointLightShader(int n):N(n)
{
	m_pointLocation = new TagPointLightLocation[N];
}

XGLModel::PointLightShader::~PointLightShader()
{
	delete[] m_pointLocation;
}

void XGLModel::PointLightShader::initUniform()
{
	for (int i = 0; i < N; ++i)
	{
		std::ostringstream stream;
		stream << "g_pointlight[" << i << "].";
		std::string prefix = stream.str();
		stream.clear();

		std::string name = prefix + "Color";
		m_pointLocation[i].Color = glGetUniformLocation(program, name.c_str());

		name = prefix + "AmbientIntensity";
		m_pointLocation[i].AmbientIntensity = glGetUniformLocation(program, name.c_str());

		name = prefix + "DiffuseIntensity";
		m_pointLocation[i].DiffuseIntensity = glGetUniformLocation(program, name.c_str());

		name = prefix + "Eposition";
		m_pointLocation[i].Eposition = glGetUniformLocation(program, name.c_str());

		name = prefix + "Attenuation.Constant";
		m_pointLocation[i].Attenuation.Constant = glGetUniformLocation(program, name.c_str());

		name = prefix + "Attenuation.Linear";
		m_pointLocation[i].Attenuation.Linear = glGetUniformLocation(program, name.c_str());

		name = prefix + "Attenuation.Exp";
		m_pointLocation[i].Attenuation.Exp = glGetUniformLocation(program, name.c_str());

	}
	g_shineness = glGetUniformLocation(program, "g_shineness");
	g_materialIntensity = glGetUniformLocation(program, "g_materialIntensity");
	g_mv = glGetUniformLocation(program, "g_mv");
	g_normal = glGetUniformLocation(program, "g_normal");
	g_mvp = glGetUniformLocation(program, "g_mvp");
	g_N = glGetUniformLocation(program, "g_N");

	g_tex_pos = glGetUniformLocation(program, "g_sampler_pos");
	g_tex_diffuse = glGetUniformLocation(program, "g_sampler_diffuse");
	g_tex_normal = glGetUniformLocation(program, "g_sampler_normal");
	g_screensize = glGetUniformLocation(program, "g_screensize");
}

void XGLModel::PointLightShader::updateSampler(int pos, int diffuse, int normal)
{
	glUniform1i(g_tex_pos, pos);
	glUniform1i(g_tex_diffuse, diffuse);
	glUniform1i(g_tex_normal, normal);
}

void XGLModel::PointLightShader::activeLights()
{
	glUniform1ui(g_N, N);
}

void XGLModel::PointLightShader::updateScreen(unsigned int width, unsigned int height)
{
	glUniform2f(g_screensize, width, height);
}


void XGLModel::PointLightShader::updateLight(const TagPointLight& pl, const unsigned int index, float materialIntensity, float shineness)
{
	assert(index < N);

	glUniform1f(m_pointLocation[index].AmbientIntensity, pl.AmbientIntensity);
	glUniform1f(m_pointLocation[index].DiffuseIntensity, pl.DiffuseIntensity);
	glUniform3f(m_pointLocation[index].Color, pl.Color.x(), pl.Color.y(), pl.Color.z());
	glUniform3f(m_pointLocation[index].Eposition, pl.Eposition.x(), pl.Eposition.x(), pl.Eposition.x());
	glUniform1f(m_pointLocation[index].Attenuation.Constant, pl.Attenuation.Constant);
	glUniform1f(m_pointLocation[index].Attenuation.Linear, pl.Attenuation.Linear);
	glUniform1f(m_pointLocation[index].Attenuation.Exp, pl.Attenuation.Exp);
	glUniform1f(g_shineness, shineness);
	glUniform1f(g_materialIntensity, materialIntensity);

}
void XGLModel::PointLightShader::updateMT(const XGL::Matrixf & mv, const XGL::Matrixf & pers)
{
	glUniformMatrix4fv(g_mv, 1, GL_FALSE, mv.ptr());

	XGL::Matrixf normal = XGL::Matrixf::inverse(mv);
	normal.transpose(normal);
	glUniformMatrix4fv(g_normal, 1, GL_FALSE, normal.ptr());

	XGL::Matrixf mvp = pers;
	mvp.preMult(mv);

	glUniformMatrix4fv(g_mvp, 1, GL_FALSE, mvp.ptr());

}

void XGLModel::PointLightShader::initGL()
{
}

void XGLModel::PointLightShader::draw()
{
}

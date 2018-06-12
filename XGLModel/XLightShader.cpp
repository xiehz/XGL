#include "stdafx.h"
#include "XGLUtil.h"
#include "XLightShader.h"
#include "xgl/Error.h"


XGLModel::XLightShader::XLightShader()
{
}

XGLModel::XLightShader::~XLightShader()
{
}

void XGLModel::XLightShader::initUniform(GLuint program)
{
	specular_Intensity = glGetUniformLocation(program, "g_specularIntensity");
	//getErrorInformation(GetLastError());
	shineness = glGetUniformLocation(program, "g_shineness");

	spotlight_direction = glGetUniformLocation(program, "spotlight.Direction");
	spotlight_cutoff = glGetUniformLocation(program, "spotlight.Cutoff");
	spotlight_eposition = glGetUniformLocation(program, "spotlight.Eposition");
	spotlight_attenuation_constant = glGetUniformLocation(program, "spotlight.Attenuation.Constant");
	spotlight_attenuation_linear = glGetUniformLocation(program, "spotlight.Attenuation.Linear");
	spotlight_attenuation_exp = glGetUniformLocation(program, "spotlight.Attenuation.Exp");
	spotlight_light_color = glGetUniformLocation(program, "spotlight.Light.Color");
	spotlight_light_ambient_intensity = glGetUniformLocation(program, "spotlight.Light.AmbientIntensity");
	spotlight_light_diffuse_intensity = glGetUniformLocation(program, "spotlight.Light.DiffuseIntensity");

	if (
		specular_Intensity< 0
		|| spotlight_direction< 0
		|| spotlight_cutoff< 0
		|| shineness< 0
		|| spotlight_eposition< 0
		|| spotlight_attenuation_constant< 0
		|| spotlight_attenuation_linear< 0
		|| spotlight_attenuation_exp< 0
		|| spotlight_light_color< 0
		|| spotlight_light_ambient_intensity< 0
		|| spotlight_light_diffuse_intensity < 0)
	{
		XGLERROR("get uniform failed");
	}

}

void XGLModel::XLightShader::updateUniform(const SpotLighter & spl , float specular, float s)
{
	glUniform1f(specular_Intensity, specular);
	glUniform1f(shineness, s);

	glUniform3f(spotlight_eposition, spl.Eposition.x(), spl.Eposition.y(), spl.Eposition.z());
	glUniform3f(spotlight_direction, spl.Direction.x(), spl.Direction.y(), spl.Direction.z());
	glUniform1f(spotlight_cutoff, spl.Cutoff);
	glUniform1f(spotlight_attenuation_constant, spl.Attenuation.Constant);
	glUniform1f(spotlight_attenuation_linear, spl.Attenuation.Linear);
	glUniform1f(spotlight_attenuation_exp, spl.Attenuation.Exp);
	glUniform3f(spotlight_light_color, spl.Color.x(), spl.Color.y(), spl.Color.z());
	glUniform1f(spotlight_light_ambient_intensity, spl.AmbientIntensity);
	glUniform1f(spotlight_light_diffuse_intensity, spl.DiffuseIntensity);
}


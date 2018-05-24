#pragma once

#include "XGLModel.h"
#include "TutorialFactory.h"

namespace XGLModel {
	class  SpecularLight : public TutorialInterface
	{
	public:
		SpecularLight();
		virtual ~SpecularLight();

		// Í¨¹ý TutorialInterface ¼Ì³Ð
		virtual void init() override;
		virtual void draw() override;
		virtual void initShader() override;
	private:
		GLint u_mv;
		GLint u_perspective;
		GLint u_dlight_ambient;
		GLint u_dlight_ambient_density;
		GLint u_dlight_diffuse_direction;
		GLint u_dlight_diffuse_density;
		GLint u_dlight_diffuse;
		GLint u_dlight_specular;
		GLint u_dlight_specular_density;
		GLint u_dlight_specular_exp;
		GLint u_sampler2d;

		GLuint texObj;
		GLuint texUnit;
		GLuint texUnitIndex;
	};
}

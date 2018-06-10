#pragma once

#include "XGLModel.h"
#include "TutorialFactory.h"

namespace XGLModel {

	class DiffuseLight : public TutorialInterface
	{
	public:
		DiffuseLight();
		virtual ~DiffuseLight();

		// Í¨¹ý TutorialInterface ¼Ì³Ð
		virtual void initGL() override;
		virtual void draw() override;
		virtual void initUniform() override;

	private:
		GLint u_mv;
		GLint u_perspective;
		GLint u_dlight_ambient;
		GLint u_dlight_ambient_density;
		GLint u_dlight_diffuse_direction;
		GLint u_dlight_diffuse_density;
		GLint u_dlight_diffuse;
		GLint u_sampler2d;

		GLuint texObj;
		GLuint texUnit;
		GLuint texUnitIndex;
	};


}


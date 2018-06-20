#pragma once
#include <string>
#include <GL/glew.h>
namespace XGLModel {
	class XCubemapTexture
	{
	public:
		XCubemapTexture(const std::string& dir,
			const std::string& posXFileName,
			const std::string& negXFileName,
			const std::string& posYFilename,
			const std::string& negYFilename,
			const std::string& posZFilename,
			const std::string& negZFilename);
		~XCubemapTexture();

		bool load();

		void bindTexture(GLenum textureUnit);
	private:
		std::string m_fileNames[6];
		GLuint m_texObj;
	};

}


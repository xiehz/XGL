#pragma once
#include <vector>
#include <string>
#include "GL/glew.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "xgl/Quat"
#include "xgl/Matrixf"
#include "XGLModel.h"
#include "Vertex.h"
#include "XTexture.h"

namespace XGLModel {

	class BoreoheMesh
	{
		struct XVAOEntry;
	public:
		BoreoheMesh();
		~BoreoheMesh();

		virtual bool LoadMesh(unsigned int count, float radius);


		virtual void Render(unsigned int NumInstances,  const float* WorldMats);

		void BoreoheMesh::Clear();

	protected:
		GLuint m_VAO;
		GLuint m_Buffers[3];
		unsigned int m_Count;
	};









}


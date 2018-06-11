#pragma once
#include <vector>
#include <string>
#include "GL/glew.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "XGLModel.h"
#include "Vertex.h"
#include "XTexture.h"

namespace XGLModel {

#define INVALID_MATERIAL 0xFFFFFFFF
#define INVALID_UNIFORM_LOCATION 0xffffffff
#define INVALID_OGL_VALUE 0xffffffff
	struct MeshEntry {
		MeshEntry();
		~MeshEntry();

		void Init(const std::vector<Vertex>& vertices, const std::vector<unsigned int> indices);

		GLuint VB;
		GLuint IB;
		unsigned int NumIndices;
		unsigned int MaterialIndex;
	};

	class IXMesh
	{
	public:
		IXMesh();
		~IXMesh();
	public:
		virtual bool LoadMesh(const std::string& filename);
		virtual void Render();
	protected:
		bool InitFromScene(const aiScene* pScene, const std::string& filename);
		void InitMesh(unsigned int index, const aiMesh* pMesh);
		bool InitMaterials(const aiScene* pScene, const std::string& filename);
		void Clear();


	protected:
		std::vector<MeshEntry> m_Entries;
		std::vector<XTexture*> m_pTextures;
	};
}
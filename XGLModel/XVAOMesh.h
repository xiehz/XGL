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

	class XVAOMesh
	{
		struct XVAOEntry;
	public:
		XVAOMesh();
		~XVAOMesh();

		bool LoadMesh(const std::string& Filename);

		void Render();

		void Render(unsigned int NumInstances,  const float* WorldMats);

		XGL::Quat& GetOrientation() { return m_orientation; }

	private:
		bool InitFromScene(const aiScene* pScene, const std::string& Filename);
		void InitMesh(const aiMesh* paiMesh,
			std::vector<XGL::Vec3f>& Positions,
			std::vector<XGL::Vec3f>& Normals,
			std::vector<XGL::Vec2f>& TexCoords,
			std::vector<unsigned int>& Indices);
		void XVAOMesh::Clear();

		bool InitMaterials(const aiScene* pScene, const std::string& Filename);
	private:
		GLuint m_VAO;
		GLuint m_Buffers[5];
	private:
		std::vector<XVAOEntry> m_Entries;
		std::vector<XTexture*> m_Textures;
		XGL::Quat m_orientation;


		struct XVAOEntry {
			XVAOEntry()
			{
				NumIndices = 0;
				BaseVertex = 0;
				BaseIndex = 0;
				MaterialIndex = INVALID_MATERIAL;
			}

			unsigned int NumIndices;
			unsigned int BaseVertex;
			unsigned int BaseIndex;
			unsigned int MaterialIndex;
		};
	};









}


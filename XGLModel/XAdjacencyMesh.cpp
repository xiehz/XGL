#include "stdafx.h"
#include "XAdjacencyMesh.h"

XGLModel::XAdjacencyMesh::XAdjacencyMesh()
{
	m_withAdjacency = true;
}


XGLModel::XAdjacencyMesh::~XAdjacencyMesh()
{
	m_VAO = 0;

	m_pScene = NULL;

}

void XGLModel::XAdjacencyMesh::Clear()
{
	for (uint i = 0; i < m_Textures.size(); i++) {
		delete m_Textures[i];
	}

	if (m_Buffers[0] != 0) {
		glDeleteBuffers(ARRAY_SIZE_IN_ELEMENTS(m_Buffers), m_Buffers);
	}

	if (m_VAO != 0) {
		glDeleteVertexArrays(1, &m_VAO);
		m_VAO = 0;
	}
}

bool XGLModel::XAdjacencyMesh::LoadMesh(const string& Filename)
{

	// Release the previously loaded mesh (if it exists)
	Clear();

	// Create the VAO
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	// Create the buffers for the vertices attributes
	glGenBuffers(ARRAY_SIZE_IN_ELEMENTS(m_Buffers), m_Buffers);

	bool Ret = false;

	m_pScene = m_Importer.ReadFile(Filename.c_str(), aiProcess_Triangulate |
		aiProcess_GenSmoothNormals |
		aiProcess_FlipUVs |
		aiProcess_JoinIdenticalVertices);
	if (m_pScene) {
		Ret = InitFromScene(m_pScene, Filename);
	}
	else {
		printf("Error parsing '%s': '%s'\n", Filename.c_str(), m_Importer.GetErrorString());
	}

	// Make sure the VAO is not changed from the outside
	glBindVertexArray(0);

	return Ret;
}


bool XGLModel::XAdjacencyMesh::InitFromScene(const aiScene* pScene, const string& Filename)
{
	m_Entries.resize(pScene->mNumMeshes);
	m_Textures.resize(pScene->mNumMaterials);

	vector<Vector3f> Positions;
	vector<Vector3f> Normals;
	vector<Vector2f> TexCoords;
	vector<uint> Indices;

	uint NumVertices = 0;
	uint NumIndices = 0;

	uint VerticesPerPrim = m_withAdjacency ? 6 : 3;

	// Count the number of vertices and indices
	for (uint i = 0; i < m_Entries.size(); i++) {
		m_Entries[i].MaterialIndex = pScene->mMeshes[i]->mMaterialIndex;
		m_Entries[i].NumIndices = pScene->mMeshes[i]->mNumFaces * VerticesPerPrim;
		m_Entries[i].BaseVertex = NumVertices;
		m_Entries[i].BaseIndex = NumIndices;

		NumVertices += pScene->mMeshes[i]->mNumVertices;
		NumIndices += m_Entries[i].NumIndices;
	}

	// Reserve space in the vectors for the vertex attributes and indices
	Positions.reserve(NumVertices);
	Normals.reserve(NumVertices);
	TexCoords.reserve(NumVertices);
	Indices.reserve(NumIndices);

	// Initialize the meshes in the scene one by one
	for (uint i = 0; i < m_Entries.size(); i++) {
		const aiMesh* paiMesh = pScene->mMeshes[i];
		InitMesh(i, paiMesh, Positions, Normals, TexCoords, Indices);
	}

	if (!InitMaterials(pScene, Filename)) {
		return false;
	}

	// Generate and populate the buffers with vertex attributes and the indices
	glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[POS_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Positions[0]) * Positions.size(), &Positions[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(POSITION_LOCATION);
	glVertexAttribPointer(POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[TEXCOORD_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TexCoords[0]) * TexCoords.size(), &TexCoords[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(TEX_COORD_LOCATION);
	glVertexAttribPointer(TEX_COORD_LOCATION, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[NORMAL_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Normals[0]) * Normals.size(), &Normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(NORMAL_LOCATION);
	glVertexAttribPointer(NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffers[INDEX_BUFFER]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices[0]) * Indices.size(), &Indices[0], GL_STATIC_DRAW);

	return GLCheckError();
}


static uint GetOppositeIndex(const aiFace& Face, const XGLModel::XEdge& e)
{
	for (uint i = 0; i < 3; i++) {
		uint Index = Face.mIndices[i];

		if (Index != e._a && Index != e._b) {
			return Index;
		}
	}

	assert(0);

	return 0;
}


void XGLModel::XAdjacencyMesh::FindAdjacencies(const aiMesh* paiMesh, vector<unsigned int>& Indices)
{
	// Step 1 - find the two triangles that share every edge
	for (uint i = 0; i < paiMesh->mNumFaces; i++) {
		const aiFace& face = paiMesh->mFaces[i];

		XFace Unique;

		// If a position vector is duplicated in the VB we fetch the 
		// index of the first occurrence.
		for (uint j = 0; j < 3; j++) {
			uint Index = face.mIndices[j];
			aiVector3D& v = paiMesh->mVertices[Index];

			if (m_posMap.find(v) == m_posMap.end()) {
				m_posMap[v] = Index;
			}
			else {
				Index = m_posMap[v];
			}

			Unique._indices[j] = Index;
		}

		m_faces.push_back(Unique);

		XEdge e1(Unique._indices[0], Unique._indices[1]);
		XEdge e2(Unique._indices[1], Unique._indices[2]);
		XEdge e3(Unique._indices[2], Unique._indices[0]);

		m_indexMap[e1].AddNeighbor(i);
		m_indexMap[e2].AddNeighbor(i);
		m_indexMap[e3].AddNeighbor(i);
	}

	// Step 2 - build the index buffer with the adjacency info
	for (uint i = 0; i < paiMesh->mNumFaces; i++) {
		const XFace& face = m_faces[i];

		for (uint j = 0; j < 3; j++) {
			XEdge e(face._indices[j], face._indices[(j + 1) % 3]);
			assert(m_indexMap.find(e) != m_indexMap.end());
			Neighbors n = m_indexMap[e];
			uint OtherTri = n.GetOther(i);

			assert(OtherTri != -1);

			const XFace& OtherFace = m_faces[OtherTri];
			uint OppositeIndex = OtherFace.GetOppositeIndex(e);

			Indices.push_back(face._indices[j]);
			Indices.push_back(OppositeIndex);
		}
	}
}


void XGLModel::XAdjacencyMesh::InitMesh(uint MeshIndex,
	const aiMesh* paiMesh,
	vector<Vector3f>& Positions,
	vector<Vector3f>& Normals,
	vector<Vector2f>& TexCoords,
	vector<uint>& Indices)
{
	const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

	// Populate the vertex attribute vectors
	for (uint i = 0; i < paiMesh->mNumVertices; i++) {
		const aiVector3D* pPos = &(paiMesh->mVertices[i]);
		const aiVector3D* pNormal = &(paiMesh->mNormals[i]);
		const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

		Positions.push_back(Vector3f(pPos->x, pPos->y, pPos->z));
		Normals.push_back(Vector3f(pNormal->x, pNormal->y, pNormal->z));
		TexCoords.push_back(Vector2f(pTexCoord->x, pTexCoord->y));
	}

	if (m_withAdjacency) {
		FindAdjacencies(paiMesh, Indices);
	}
	else {
		// Populate the index buffer
		for (uint i = 0; i < paiMesh->mNumFaces; i++) {
			const aiFace& Face = paiMesh->mFaces[i];
			assert(Face.mNumIndices == 3);
			Indices.push_back(Face.mIndices[0]);
			Indices.push_back(Face.mIndices[1]);
			Indices.push_back(Face.mIndices[2]);
		}
	}
}

bool XGLModel::XAdjacencyMesh::InitMaterials(const aiScene* pScene, const string& Filename)
{
	// Extract the directory part from the file name
	string::size_type SlashIndex = Filename.find_last_of("/");
	string Dir;

	if (SlashIndex == string::npos) {
		Dir = ".";
	}
	else if (SlashIndex == 0) {
		Dir = "/";
	}
	else {
		Dir = Filename.substr(0, SlashIndex);
	}

	bool Ret = true;

	// Initialize the materials
	for (uint i = 0; i < pScene->mNumMaterials; i++) {
		const aiMaterial* pMaterial = pScene->mMaterials[i];

		m_Textures[i] = NULL;

		if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
			aiString Path;

			if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
				string p(Path.data);

				if (p.substr(0, 2) == ".\\") {
					p = p.substr(2, p.size() - 2);
				}

				string FullPath = Dir + "/" + p;

				m_Textures[i] = new XTexture(GL_TEXTURE_2D, FullPath.c_str());

				if (!m_Textures[i]->Load()) {
					printf("Error loading texture '%s'\n", FullPath.c_str());
					delete m_Textures[i];
					m_Textures[i] = NULL;
					Ret = false;
				}
				else {
					printf("%d - loaded texture '%s'\n", i, FullPath.c_str());
				}
			}
		}
	}

	return Ret;
}


void XGLModel::XAdjacencyMesh::Render()
{
	glBindVertexArray(m_VAO);

	uint Topology = m_withAdjacency ? GL_TRIANGLES_ADJACENCY : GL_TRIANGLES;

	for (uint i = 0; i < m_Entries.size(); i++) {
		const uint MaterialIndex = m_Entries[i].MaterialIndex;

		assert(MaterialIndex < m_Textures.size());

		if (m_Textures[MaterialIndex]) {
			m_Textures[MaterialIndex]->Bind(COLOR_TEXTURE_UNIT);
		}

		glDrawElementsBaseVertex(Topology,
			m_Entries[i].NumIndices,
			GL_UNSIGNED_INT,
			(void*)(sizeof(uint) * m_Entries[i].BaseIndex),
			m_Entries[i].BaseVertex);
	}

	// Make sure the VAO is not changed from the outside    
	glBindVertexArray(0);
}






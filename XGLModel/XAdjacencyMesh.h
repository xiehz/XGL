#pragma once
#include <assert.h>
#include <map>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "XVAOMesh.h"

using namespace std;

typedef unsigned int uint;
typedef XGL::Vec3f Vector3f;
typedef XGL::Vec2f Vector2f;

namespace XGLModel {

	struct XEdge
	{
		uint _a, _b;
		XEdge(uint a, uint b) {
			assert(a != b);

			if (a < b)
			{
				_a = a;
				_b = b;
			}
			else {
				_a = b;
				_b = a;
			}
		}
	};

	struct Neighbors
	{
		uint _na;
		uint _nb;

		Neighbors() {
			_na = _nb = (uint) - 1;
		}

		void AddNeighbor(uint n)
		{
			if (_na == -1)
				_na = n;
			else if(_nb == -1){
				_nb = n;
			}
			else {
				assert(0);
			}
		}

		uint GetOther(uint me)
		{
			if (_na == me)
				return _nb;
			else if (_nb == me)
				return _na;
			else
				assert(0);
			return 0;
		}

	};

	struct CompareXEdges {
		bool operator()(const XEdge& e1, const XEdge& e2)const {
			if (e1._a < e2._a) {
				return true;
			}
			else if (e1._a == e2._a) {
				return (e1._b < e2._b);
			}
			else {
				return false;
			}
		}
	};

	struct CompareVectors
	{
		bool operator()(const aiVector3D& a, const aiVector3D& b)const
		{
			if (a.x < b.x) {
				return true;
			}
			else if (a.x == b.x) {
				if (a.y < b.y) {
					return true;
				}
				else if (a.y == b.y) {
					if (a.z < b.z) {
						return true;
					}
				}
			}

			return false;
		}
	};

	struct XFace
	{
		uint _indices[3];

		uint GetOppositeIndex(const XEdge& e)const {
			for (int i = 0; i < 3; ++i)
			{
				if (_indices[i] != e._a && _indices[i] != e._b)
					return _indices[i];
			}

			assert(0);
			return 0;
		}
	};

	class XAdjacencyMesh : XVAOMesh
	{
	public:
		XAdjacencyMesh(bool adjacency = true);
		~XAdjacencyMesh();

		virtual bool LoadMesh(const std::string& Filename) override;

		virtual void Render()override;

	private:
		bool InitFromScene(const aiScene* pScene, const std::string& Filename);
		void FindAdjacencies(const aiMesh * paiMesh, vector<unsigned int>& Indices);
		void InitMesh(uint MeshIndex,
			const aiMesh* paiMesh,
			vector<Vector3f>& Positions,
			vector<Vector3f>& Normals,
			vector<Vector2f>& TexCoords,
			vector<unsigned int>& Indices);
		void Clear();

		bool InitMaterials(const aiScene* pScene, const std::string& Filename);
	private:
		std::map<XEdge, Neighbors, CompareXEdges> m_indexMap;
		std::map<aiVector3D, uint, CompareVectors> m_posMap;
		std::vector<XFace> m_faces;
		bool m_withAdjacency;
		const aiScene* m_pScene;
		Assimp::Importer m_Importer;
	};

}


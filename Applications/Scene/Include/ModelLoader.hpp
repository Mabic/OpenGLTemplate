#ifndef MODEL_LOADER_HPP
#define MODEL_LOADER_HPP

#include <assimp/scene.h>
#include <string>
#include <vector>
#include <Mesh.hpp>

class ModelLoader {
public:
	ModelLoader() = delete;
	ModelLoader(const std::string& pathToFile);
	ModelLoader(const ModelLoader&) = delete;
	ModelLoader(ModelLoader&&) = delete;
	~ModelLoader();

	ModelLoader operator=(ModelLoader&&) = delete;
	ModelLoader operator=(const ModelLoader&) = delete;

	const std::vector<Mesh>& GetMeshes() const {
		return m_meshes;
	}
private:
	void ProcessNode(const aiNode* node, const aiScene* scene);
	std::string GetCurrentDirectory(const std::string& path) const;

	std::vector<Mesh> m_meshes;
	std::string m_directory;
};

#endif // !MODEL_LOADER_HPP

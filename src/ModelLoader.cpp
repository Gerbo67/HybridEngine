#include "ModelLoader.h"
#include "MeshComponent.h"
#include "Prerequisites.h"

void 
ModelLoader::init() {
}

void 
ModelLoader::update() {
}

void 
ModelLoader::render() {
}

void 
ModelLoader::destroy() {
}

MeshComponent 
ModelLoader::Load(const std::string& filename) {
    MeshComponent combinedMesh;
    objl::Loader loader;

    if (loader.LoadFile(filename)) {
        if (loader.LoadedMeshes.empty()) {
            ERROR("ModelLoader", "Load", "El archivo .obj no contiene mallas.");
            return combinedMesh;
        }

        // Iterar sobre TODAS las mallas cargadas en el archivo
        for (const auto& current_mesh : loader.LoadedMeshes) {
            unsigned int vertex_offset = static_cast<unsigned int>(combinedMesh.m_vertex.size());

            // Añadimos los vértices de la malla actual a nuestra malla combinada
            for (const auto& objlVertex : current_mesh.Vertices) {
                SimpleVertex vertex;
                vertex.Pos = XMFLOAT3(objlVertex.Position.X, objlVertex.Position.Y, objlVertex.Position.Z);
                
                if (!loader.LoadedMeshes[0].Vertices.empty()) {
                     vertex.Tex = XMFLOAT2(objlVertex.TextureCoordinate.X, 1.0f - objlVertex.TextureCoordinate.Y);
                } else {
                     vertex.Tex = XMFLOAT2(0.0f, 0.0f);
                }
                
                combinedMesh.m_vertex.push_back(vertex);
            }

            for (unsigned int index : current_mesh.Indices) {
                combinedMesh.m_index.push_back(index + vertex_offset);
            }
        }

        // Actualizamos el conteo total de vértices e índices.
        combinedMesh.m_numVertex = static_cast<int>(combinedMesh.m_vertex.size());
        combinedMesh.m_numIndex = static_cast<int>(combinedMesh.m_index.size());
        combinedMesh.m_name = "CombinedModel";
    }
    else {
        ERROR("ModelLoader", "Load", "No se pudo cargar el archivo del modelo .obj.");
    }

    return combinedMesh;
}
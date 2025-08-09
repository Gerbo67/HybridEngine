#include "ModelLoader.h"
#include "OBJ_Loader.h"

MeshComponent
ModelLoader::LoadOBJModel(const std::string& filePath) {
    MeshComponent mesh;
    objl::Loader loader;

    if (!loader.LoadFile(filePath)) {
        return mesh;
    }

    mesh.m_name = filePath;

    const unsigned int numVertices = loader.LoadedVertices.size();
    const unsigned int numIndices = loader.LoadedIndices.size();

    // Reservar memoria exacta para evitar reallocs
    mesh.m_vertex.resize(numVertices);
    mesh.m_index = std::move(loader.LoadedIndices); // Mover directamente

    // Usar acceso por índice para evitar cópias extra
    for (unsigned int i = 0; i < numVertices; ++i) {
        const auto& v = loader.LoadedVertices[i];

        mesh.m_vertex[i] = SimpleVertex{
            {v.Position.X, v.Position.Y, v.Position.Z},
            {v.TextureCoordinate.X, 1.0f - v.TextureCoordinate.Y}
        };
    }

    mesh.m_numVertex = numVertices;
    mesh.m_numIndex = numIndices;

    return mesh;
}


bool
ModelLoader::InitializeFBXManager() {
    // Initialize the FBX SDK manager
    lSdkManager = FbxManager::Create();
    if (!lSdkManager) {
        ERROR("ModelLoader", "FbxManager::Create()", "Unable to create FBX Manager!");
        return false;
    } else {
        MESSAGE("ModelLoader", "ModelLoader", "Autodesk FBX SDK version " << lSdkManager->GetVersion())
    }

    // Create an IOSettings object
    FbxIOSettings* ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
    lSdkManager->SetIOSettings(ios);

    // Create an FBX Scene
    lScene = FbxScene::Create(lSdkManager, "MyScene");
    if (!lScene) {
        ERROR("ModelLoader", "FbxScene::Create()", "Unable to create FBX Scene!");
        return false;
    } else {
        MESSAGE("ModelLoader", "ModelLoader", "FBX Scene created successfully.")
    }
    return true;
}

bool
ModelLoader::LoadFBXModel(const std::string& filePath) {
    // 01. Initialize the SDK from FBX Manager
    if (InitializeFBXManager()) {
        // 02. Create an importer using the SDK manager
        FbxImporter* lImporter = FbxImporter::Create(lSdkManager, "");
        if (!lImporter) {
            ERROR("ModelLoader", "FbxImporter::Create()", "Unable to create FBX Importer!");
            return false;
        } else {
            MESSAGE("ModelLoader", "ModelLoader", "FBX Importer created successfully.");
        }

        // 03. Use the first argument as the filename for the importer
        if (!lImporter->Initialize(filePath.c_str(), -1, lSdkManager->GetIOSettings())) {
            ERROR("ModelLoader", "FbxImporter::Initialize()",
                  "Unable to initialize FBX Importer! Error: " << lImporter->GetStatus().GetErrorString());
            lImporter->Destroy();
            return false;
        } else {
            MESSAGE("ModelLoader", "ModelLoader", "FBX Importer initialized successfully.");
        }

        // 04. Import the scene from the file into the scene
        if (!lImporter->Import(lScene)) {
            ERROR("ModelLoader", "FbxImporter::Import()",
                  "Unable to import FBX Scene! Error: " << lImporter->GetStatus().GetErrorString());
            lImporter->Destroy();
            return false;
        } else {
            MESSAGE("ModelLoader", "ModelLoader", "FBX Scene imported successfully.");
            modelName = lImporter->GetFileName();
        }

        // 05. Destroy the importer
        lImporter->Destroy();
        MESSAGE("ModelLoader", "ModelLoader", "FBX Importer destroyed successfully.");

        // 06. Process the model from the scene
        FbxNode* lRootNode = lScene->GetRootNode();

        if (lRootNode) {
            MESSAGE("ModelLoader", "ModelLoader", "Processing model from the scene root node.");
            for (int i = 0; i < lRootNode->GetChildCount(); i++) {
                ProcessFBXNode(lRootNode->GetChild(i));
            }
            return true;
        } else {
            ERROR("ModelLoader", "FbxScene::GetRootNode()",
                  "Unable to get root node from FBX Scene!");
            return false;
        }
    }
    return false;
}

void
ModelLoader::ProcessFBXNode(FbxNode* node) {
    // 01. Process all the node's meshes
    if (node->GetNodeAttribute()) {
        if (node->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eMesh) {
            ProcessFBXMesh(node);
        }
    }

    // 02. Recursively process each child node
    for (int i = 0; i < node->GetChildCount(); i++) {
        ProcessFBXNode(node->GetChild(i));
    }
}

void
ModelLoader::ProcessFBXMesh(FbxNode* node) {
    FbxMesh* mesh = node->GetMesh();
    if (!mesh)
        return;

    // Vectores para guardar los datos finales de la malla
    std::vector<SimpleVertex> vertices;
    std::vector<unsigned int> indices;

    // Obtener el elemento de coordenadas UV. Es la única variable que necesitamos verificar.
    FbxGeometryElementUV* uvElement = mesh->GetElementUV(0);

    // Un contador para el índice de los vértices de los polígonos, clave para 'eByPolygonVertex'
    int polyVertexCounter = 0;

    // Iterar sobre todos los polígonos de la malla
    for (int i = 0; i < mesh->GetPolygonCount(); i++) {
        const int polySize = mesh->GetPolygonSize(i);
        const int baseIndex = vertices.size();

        // Procesar cada vértice del polígono
        for (int j = 0; j < polySize; j++) {
            SimpleVertex currentVertex;

            // 1. Obtener la POSICIÓN del vértice a través del Control Point
            const int controlPointIndex = mesh->GetPolygonVertex(i, j);
            const FbxVector4 pos = mesh->GetControlPointAt(controlPointIndex);
            currentVertex.Pos = XMFLOAT3((float)pos[0], (float)pos[1], (float)pos[2]);

            // 2. Obtener la COORDENADA UV
            // Verificamos si el elemento UV existe antes de intentar usarlo.
            if (uvElement) {
                // Ahora que sabemos que uvElement es válido, podemos obtener los arrays y modos de forma segura.
                const FbxGeometryElement::EMappingMode mappingMode = uvElement->GetMappingMode();
                const FbxGeometryElement::EReferenceMode referenceMode = uvElement->GetReferenceMode();
                const FbxLayerElementArrayTemplate<FbxVector2>& uvArray = uvElement->GetDirectArray();
                const FbxLayerElementArrayTemplate<int>& uvIndexArray = uvElement->GetIndexArray();

                int uvIndex = -1;
                switch (mappingMode) {
                case FbxGeometryElement::eByControlPoint:
                    if (referenceMode == FbxGeometryElement::eDirect) {
                        uvIndex = controlPointIndex;
                    } else {
                        // eIndexToDirect
                        uvIndex = uvIndexArray.GetAt(controlPointIndex);
                    }
                    break;

                // Dentro del switch(mappingMode)
                case FbxGeometryElement::eByPolygonVertex:
                    if (referenceMode == FbxGeometryElement::eDirect) {
                        uvIndex = polyVertexCounter;
                    } else {
                        // eIndexToDirect
                        uvIndex = uvIndexArray.GetAt(polyVertexCounter);
                    }
                    break;
                default:
                    break;
                }

                if (uvIndex != -1) {
                    const FbxVector2 uv = uvArray.GetAt(uvIndex);
                    currentVertex.Tex = XMFLOAT2((float)uv[0], 1.0f - (float)uv[1]);
                } else {
                    currentVertex.Tex = XMFLOAT2(0.0f, 0.0f); // UV por defecto si algo falla
                }
            } else {
                // Si no hay elemento UV en la malla, asignamos una UV por defecto.
                currentVertex.Tex = XMFLOAT2(0.0f, 0.0f);
            }

            vertices.push_back(currentVertex);
            polyVertexCounter++;
        }

        // 3. Crear los ÍNDICES para triangular el polígono (Fan Triangulation)
        for (int k = 1; k < polySize - 1; ++k) {
            indices.push_back(baseIndex);
            indices.push_back(baseIndex + k);
            indices.push_back(baseIndex + k + 1);
        }
    }

    // Crear el componente de malla con los datos procesados
    MeshComponent meshData;
    meshData.m_name = node->GetName();
    meshData.m_vertex = std::move(vertices);
    meshData.m_index = std::move(indices);
    meshData.m_numVertex = meshData.m_vertex.size();
    meshData.m_numIndex = meshData.m_index.size();

    meshes.push_back(meshData);
}

void
ModelLoader::ProcessFBXMaterials(FbxSurfaceMaterial* material) {
    if (material) {
        FbxProperty prop = material->FindProperty(FbxSurfaceMaterial::sDiffuse);
        if (prop.IsValid()) {
            int textureCount = prop.GetSrcObjectCount<FbxTexture>();
            for (int i = 0; i < textureCount; ++i) {
                FbxTexture* texture = FbxCast<FbxTexture>(prop.GetSrcObject<FbxTexture>(i));
                if (texture) {
                    textureFileNames.push_back(texture->GetName());
                }
            }
        }
    }
}

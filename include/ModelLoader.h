#pragma once
#include "Prerequisites.h"
#include "MeshComponent.h"
#include "fbxsdk.h"

/**
 * @brief Sistema de carga de modelos 3D que soporta formatos OBJ y FBX para importación de assets.
 * 
 * ModelLoader proporciona funcionalidad completa para cargar modelos 3D desde archivos externos,
 * procesando geometría, materiales y texturas. Soporta el pipeline de importación FBX con SDK nativo
 * y carga simple de modelos OBJ, convirtiendo datos externos en componentes de malla utilizables
 * por el motor de renderizado.
 */
class
ModelLoader {
public:
    ModelLoader() = default;
    ~ModelLoader() = default;

    /**
     * @brief Carga un modelo 3D desde un archivo OBJ.
     * @param filePath Ruta al archivo OBJ a cargar
     * @return Componente de malla con los datos del modelo cargado
     */
    MeshComponent
    LoadOBJModel(const std::string& filePath);

    /**
     * @brief Inicializa el manager del SDK de FBX para operaciones de carga.
     * @return true si la inicialización fue exitosa, false en caso contrario
     */
    bool
    InitializeFBXManager();

    /**
     * @brief Carga un modelo 3D desde un archivo FBX.
     * @param filePath Ruta al archivo FBX a cargar
     * @return true si la carga fue exitosa, false en caso contrario
     */
    bool
    LoadFBXModel(const std::string& filePath);

    /**
     * @brief Procesa un nodo FBX y sus hijos recursivamente.
     * @param node Nodo FBX a procesar
     */
    void
    ProcessFBXNode(FbxNode* node);

    /**
     * @brief Extrae datos de geometría de una malla FBX.
     * @param node Nodo FBX que contiene la malla a procesar
     */
    void
    ProcessFBXMesh(FbxNode* node);

    /**
     * @brief Procesa información de materiales de un objeto FBX.
     * @param material Material FBX a procesar
     */
    void
    ProcessFBXMaterials(FbxSurfaceMaterial* material);

    /**
     * @brief Obtiene los nombres de archivos de texturas encontrados durante la carga.
     * @return Vector de nombres de archivos de textura
     */
    std::vector<std::string>
    GetTextureFileNames() const { return textureFileNames; }

private:
    FbxManager* lSdkManager; ///< Manager del SDK FBX para operaciones de carga
    FbxScene* lScene; ///< Escena FBX cargada en memoria
    std::vector<std::string> textureFileNames; ///< Lista de nombres de archivos de textura

public:
    std::string modelName; ///< Nombre del modelo cargado
    std::vector<MeshComponent> meshes; ///< Vector de mallas extraídas del modelo
};

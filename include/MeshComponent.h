#pragma once
#include "Prerequisites.h"
#include "ECS\Component.h"

class DeviceContext;

/**
 * @brief Componente ECS que encapsula datos geométricos de mallas 3D para renderizado.
 * 
 * MeshComponent almacena y gestiona información de geometría incluyendo vértices, índices y
 * metadatos de mallas. Forma parte del sistema ECS proporcionando funcionalidad de renderizado
 * de objetos 3D con soporte para topologías indexadas y datos de vértices complejos.
 */
class
MeshComponent : public Component {
public:
    MeshComponent() :
        m_numVertex(0), m_numIndex(0), Component(ComponentType::MESH) {
    }

    virtual
    ~MeshComponent() = default;

    void
    init() override {
    };

    /**
     * @brief Actualiza el actor.
     * @param deltaTime El tiempo transcurrido desde la última actualización.
     */
    void
    update(float deltaTime) override {
    }

    /**
     * @brief Renderiza el actor.
     * @param deviceContext Contexto del dispositivo para operaciones gráficas.
     */
    void
    render(DeviceContext& deviceContext) override {
    }

    void
    destroy() override {
    }

    /**
     * @brief Carga datos de geometría desde vectores de vértices e índices.
     * @param vertices Vector de vértices con datos completos de geometría
     * @param indices Vector de índices para definir la topología de la malla
     */
    void
    loadMeshData(const std::vector<SimpleVertex>& vertices, const std::vector<unsigned int>& indices);

public:
    std::string m_name; ///< Nombre identificador de la malla
    std::vector<SimpleVertex> m_vertex; ///< Vector de datos de vértices
    std::vector<unsigned int> m_index; ///< Vector de índices para topología
    int m_numVertex; ///< Número total de vértices en la malla
    int m_numIndex; ///< Número total de índices en la malla
};

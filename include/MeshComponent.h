#pragma once
#include "Prerequisites.h"
//#include "ECS\Component.h"

class DeviceContext;

/**
 * @class MeshComponent
 * @brief Representa un componente de malla para un objeto en la escena.
 *
 * Esta clase (aunque actualmente no hereda de Component) contiene los búferes de vértices e índices
 * que definen la geometría de un objeto renderizable.
 */
class
  MeshComponent /*: public Component*/ {
public:
    MeshComponent() : m_numVertex(0), m_numIndex(0)/*, Component(ComponentType::MESH)*/ {}

    virtual
    ~MeshComponent() = default;

    /**
     * @brief Actualiza el actor.
     * @param deltaTime El tiempo transcurrido desde la �ltima actualizaci�n.
     * @param deviceContext Contexto del dispositivo para operaciones gr�ficas.
     */
    void
    update(float deltaTime) /*override*/ {}

    /**
     * @brief Renderiza el actor.
     * @param deviceContext Contexto del dispositivo para operaciones gr�ficas.
     */
    void
    render(DeviceContext& deviceContext) /*override*/ {}
public:
    std::string m_name;
    std::vector<SimpleVertex> m_vertex;
    std::vector<unsigned int> m_index;
    int m_numVertex;
    int m_numIndex;
};
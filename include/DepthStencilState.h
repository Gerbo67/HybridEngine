#pragma once
#include "Prerequisites.h"

class Device;
class DeviceContext;

/**
 * @brief Gestiona los estados de prueba de profundidad y stencil de DirectX para renderizado 3D preciso y manejo de oclusión.
 * 
 * DepthStencilState controla las operaciones de buffer de profundidad y pruebas de stencil, esenciales para el
 * ordenamiento-z adecuado en escenas 3D. Proporciona pruebas de profundidad configurables, operaciones de stencil
 * y valores de referencia para técnicas de renderizado avanzadas como volúmenes de sombra y enmascarado de objetos.
 */
class
DepthStencilState {
public:
    DepthStencilState() = default;
    ~DepthStencilState() = default;

    /**
     * @brief Inicializa el estado de profundidad y stencil con configuraciones específicas.
     * @param device Dispositivo DirectX para crear el estado
     * @param enableDepth Si debe habilitarse la prueba de profundidad
     * @param enableStencil Si debe habilitarse la prueba de stencil
     * @return Resultado HRESULT de la operación
     */
    HRESULT
    init(Device& device, bool enableDepth = true, bool enableStencil = false);

    void
    update();

    /**
     * @brief Aplica el estado de profundidad y stencil al pipeline de renderizado.
     * @param deviceContext Contexto del dispositivo para aplicar el estado
     * @param stencilRef Valor de referencia para las pruebas de stencil
     * @param reset Si debe reiniciar el estado
     */
    void
    render(DeviceContext& deviceContext, unsigned int stencilRef = 0, bool reset = false);

    void
    destroy();

private:
    ID3D11DepthStencilState* m_depthStencilState = nullptr; ///< Estado de profundidad y stencil nativo de DirectX 11
};

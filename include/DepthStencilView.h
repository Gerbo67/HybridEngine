#pragma once
#include "Prerequisites.h"

class Device;
class DeviceContext;
class Texture;

/**
 * @brief Proporciona acceso a datos de buffer de profundidad y stencil para operaciones de prueba de profundidad del pipeline de renderizado.
 * 
 * DepthStencilView crea una interfaz de vista a texturas de profundidad/stencil, permitiendo al pipeline gráfico
 * realizar pruebas de profundidad y operaciones de stencil. Esencial para la precisión del renderizado 3D y efectos
 * avanzados como mapeo de sombras y técnicas de renderizado volumétrico.
 */
class
DepthStencilView {
public:
    DepthStencilView() = default;
    ~DepthStencilView() = default;

    /**
     * @brief Inicializa la vista de profundidad y stencil con una textura específica.
     * @param device Dispositivo DirectX para crear la vista
     * @param depthStencil Textura de profundidad y stencil de origen
     * @param format Formato de la vista de profundidad
     * @return Resultado HRESULT de la operación
     */
    HRESULT
    init(Device& device, Texture& depthStencil, DXGI_FORMAT format);

    void
    update();

    void
    render(DeviceContext& deviceContext);

    void
    destroy();

public:
    ID3D11DepthStencilView* m_depthStencilView = nullptr; ///< Vista de profundidad y stencil nativa de DirectX 11
};

#pragma once
#include "Prerequisites.h"

// Forward Declarations
class Device;
class DeviceContext;
class Texture;
class DepthStencilView;

/**
 * @brief Vista de objetivo de renderizado que define dónde se dibujan los píxeles durante el renderizado.
 * 
 * RenderTargetView proporciona una interfaz para renderizar a texturas o superficies específicas,
 * permitiendo técnicas como render-to-texture, post-procesamiento y renderizado fuera de pantalla.
 * Gestiona la configuración de objetivos de renderizado y operaciones de limpieza de buffers.
 */
class
RenderTargetView {
public:
    RenderTargetView() = default;
    ~RenderTargetView() = default;

    /**
     * @brief Inicializa la vista de objetivo de renderizado con un buffer trasero.
     * @param device Dispositivo DirectX para crear la vista
     * @param backBuffer Textura del buffer trasero
     * @param Format Formato de píxeles de la vista
     * @return Resultado HRESULT de la operación
     */
    HRESULT
    init(Device& device, Texture& backBuffer, DXGI_FORMAT Format);

    /**
     * @brief Inicializa la vista con configuraciones específicas de dimensión.
     * @param device Dispositivo DirectX para crear la vista
     * @param inTex Textura de entrada para la vista
     * @param ViewDimension Dimensión de la vista (1D, 2D, 3D, etc.)
     * @param Format Formato de píxeles de la vista
     * @return Resultado HRESULT de la operación
     */
    HRESULT
    init(Device& device,
         Texture& inTex,
         D3D11_RTV_DIMENSION ViewDimension,
         DXGI_FORMAT Format);

    void
    update();

    /**
     * @brief Renderiza estableciendo la vista como objetivo y limpiando con color.
     * @param deviceContext Contexto del dispositivo para operaciones de renderizado
     * @param depthStencilView Vista de profundidad asociada
     * @param numViews Número de vistas de renderizado
     * @param ClearColor Color para limpiar el objetivo de renderizado
     */
    void
    render(DeviceContext& deviceContext,
           DepthStencilView& depthStencilView,
           unsigned int numViews,
           const float ClearColor[4]);

    /**
     * @brief Renderiza estableciendo la vista como objetivo sin limpieza.
     * @param deviceContext Contexto del dispositivo para operaciones de renderizado
     * @param numViews Número de vistas de renderizado
     */
    void
    render(DeviceContext& deviceContext,
           unsigned int numViews);

    void
    destroy();

private:
    ID3D11RenderTargetView* m_renderTargetView = nullptr; ///< Vista de objetivo de renderizado DirectX 11 nativa
};

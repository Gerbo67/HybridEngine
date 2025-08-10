#pragma once
#include "Prerequisites.h"

class Device;
class DeviceContext;
class Window;
class Texture;

/**
 * @brief Cadena de intercambio que gestiona el doble buffer y la presentación de frames en pantalla.
 * 
 * SwapChain maneja la alternancia entre buffers de renderizado para eliminar el parpadeo y proporcionar
 * renderizado fluido. Configura características avanzadas como MSAA para anti-aliasing y gestiona
 * la sincronización entre el GPU y la pantalla para presentación óptima de frames.
 */
class
SwapChain {
public:
    SwapChain() = default;
    ~SwapChain() = default;

    /**
     * @brief Inicializa la cadena de intercambio con configuraciones específicas.
     * @param device Dispositivo DirectX para crear la cadena
     * @param deviceContext Contexto del dispositivo para operaciones
     * @param backBuffer Buffer trasero para renderizado
     * @param window Ventana asociada para presentación
     * @return Resultado HRESULT de la operación
     */
    HRESULT
    init(Device& device,
         DeviceContext& deviceContext,
         Texture& backBuffer,
         Window window);

    void
    update();

    void
    render();

    void
    destroy();

    /**
     * @brief Presenta el frame actual intercambiando los buffers.
     */
    void
    present();

public:
    IDXGISwapChain* m_swapChain = nullptr; ///< Cadena de intercambio DXGI nativa
    D3D_DRIVER_TYPE m_driverType = D3D_DRIVER_TYPE_NULL; ///< Tipo de driver DirectX utilizado

private:
    D3D_FEATURE_LEVEL m_featureLevel = D3D_FEATURE_LEVEL_11_0; ///< Nivel de características DirectX

    /**
     * Para evitar que los píxeles se vean con un efecto de "serrucho" (aliasing) en DirectX 11,
     * se utiliza Multisample Anti-Aliasing (MSAA) que suaviza los bordes de los objetos
     * al muestrear varios puntos por píxel para mejorar la calidad visual.
     */
    unsigned int m_sampleCount; ///< Número de muestras MSAA (ej. 4x MSAA)
    unsigned int m_qualityLevels; ///< Niveles de calidad disponibles para MSAA

    // Interfaces DXGI para gestión avanzada
    IDXGIDevice* m_dxgiDevice = nullptr; ///< Dispositivo DXGI para funcionalidad extendida
    IDXGIAdapter* m_dxgiAdapter = nullptr; ///< Adaptador gráfico DXGI
    IDXGIFactory* m_dxgiFactory = nullptr; ///< Factory DXGI para creación de objetos
};

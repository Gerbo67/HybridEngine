#pragma once
#include "Prerequisites.h"

class Device;
class DeviceContext;
class Window;
class Texture;

/**
 * @class SwapChain
 * @brief Gestiona la cadena de intercambio para la presentación de fotogramas.
 *
 * Esta clase es responsable de crear y gestionar la cadena de intercambio,
 * que es una colección de búferes utilizados para mostrar gráficos en la pantalla.
 */
class 
SwapChain {
public:
    SwapChain()  = default;
    ~SwapChain() = default;

    HRESULT
    init(Device & device, 
         DeviceContext & deviceContext, 
         Texture & backBuffer, 
         Window window);

    void 
    update();
  
    void 
    render();
  
    void 
    destroy();

    /**
     * @brief Presenta el contenido del búfer de reserva en la pantalla.
     */
    void
    present();

public:
    IDXGISwapChain* m_swapChain = nullptr;
    D3D_DRIVER_TYPE m_driverType = D3D_DRIVER_TYPE_NULL;
private:
    D3D_FEATURE_LEVEL m_featureLevel = D3D_FEATURE_LEVEL_11_0;
    // MSAA Configuration
    /*
     * Para evitar que los p�xeles se vean con un efecto de "serrucho" (aliasing) en DirectX 11 con C++,
     * se puede utilizar una t�cnica llamada anti-aliasing.
     *
     * Multisample Anti-Aliasing (MSAA)
     * MSAA es una t�cnica que suaviza los bordes de los objetos al muestrear varios puntos por p�xel.
     * Para habilitar MSAA en DirectX 11:
     *
     */
    unsigned int m_sampleCount;  // Number of MSAA samples (4x MSAA)
    unsigned int m_qualityLevels;

    // Punteros a las interfaces DXGI
    IDXGIDevice* m_dxgiDevice = nullptr;
    IDXGIAdapter* m_dxgiAdapter = nullptr;
    IDXGIFactory* m_dxgiFactory = nullptr;
};

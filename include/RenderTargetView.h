#pragma once
#include "Prerequisites.h"

// Forward Declarations
class Device;
class DeviceContext;
class Texture;
class DepthStencilView;

/**
 * @class RenderTargetView
 * @brief Representa una vista de destino de renderizado.
 *
 * Esta clase encapsula la vista de un recurso que se puede usar como destino de renderizado,
 * como el búfer de reserva o una textura.
 */
class 
RenderTargetView {
public:
    RenderTargetView()  = default;
    ~RenderTargetView() = default;

    HRESULT
  init(Device& device, Texture &backBuffer, DXGI_FORMAT Format);
  
    HRESULT
    init(Device& device, 
         Texture& inTex, 
         D3D11_RTV_DIMENSION ViewDimension, 
         DXGI_FORMAT Format);

    void 
    update();
  
    void 
    render(DeviceContext & deviceContext, 
           DepthStencilView & depthStencilView, 
           unsigned int numViews, 
           const float ClearColor[4]);
  
    void 
    render(DeviceContext & deviceContext, 
           unsigned int numViews);
  
    void 
    destroy();

private:
    ID3D11RenderTargetView* m_renderTargetView = nullptr;
};
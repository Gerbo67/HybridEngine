#pragma once
#include "Prerequisites.h"
class Window;
class DeviceContext;

/**
 * @class Viewport
 * @brief Define un área de renderizado en la pantalla.
 *
 * El viewport especifica las dimensiones y la posición del área en la que se dibujará la escena.
 */
class 
Viewport {
public:
    Viewport() = default;
    ~Viewport() = default;

    HRESULT
  init(const Window & window);
  
    HRESULT
    init(unsigned int width, unsigned int height);

    void 
    update();
  
    void 
    render(DeviceContext& deviceContext);
  
    void 
    destroy();

public:
    D3D11_VIEWPORT m_viewport;
};
#pragma once
#include "Prerequisites.h"

/**
 * @class Window
 * @brief Gestiona la ventana de la aplicación.
 *
 * Esta clase es responsable de crear, actualizar, renderizar y destruir la ventana.
 * Contiene el handle de la ventana y sus dimensiones.
 */
class 
Window {
public:
    Window() = default;
    ~Window() = default;

    HRESULT
    init(HINSTANCE hInstance, int nCmdShow, WNDPROC wndproc);

    void 
    update();
	
    void 
    render();
	
    void 
    destroy();

public:
    HWND m_hWnd = nullptr;
    unsigned int m_width;
    unsigned int m_height;
private:
    HINSTANCE m_hInst = nullptr;
    RECT m_rect;
    std::string m_windowName = "Hybrid Engine";
};
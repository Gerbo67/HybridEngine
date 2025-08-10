#pragma once
#include "Prerequisites.h"

/**
 * @brief Gestión de ventana Win32 que proporciona la superficie de renderizado principal para la aplicación.
 * 
 * Window encapsula toda la funcionalidad de creación y gestión de ventanas Win32, incluyendo configuración
 * de dimensiones, manejo de eventos del sistema operativo y proporcionando el contexto necesario para
 * la inicialización de DirectX. Actúa como la interfaz principal entre el sistema operativo y el motor gráfico.
 */
class
Window {
public:
    Window() = default;
    ~Window() = default;

    /**
     * @brief Inicializa y crea la ventana Win32 con configuraciones específicas.
     * @param hInstance Instancia de la aplicación Win32
     * @param nCmdShow Modo de visualización de la ventana
     * @param wndproc Procedimiento de ventana para manejo de mensajes
     * @return Resultado HRESULT de la operación
     */
    HRESULT
    init(HINSTANCE hInstance, int nCmdShow, WNDPROC wndproc);

    void
    update();

    void
    render();

    void
    destroy();

public:
    HWND m_hWnd = nullptr; ///< Handle de la ventana Win32
    unsigned int m_width; ///< Ancho de la ventana en píxeles
    unsigned int m_height; ///< Alto de la ventana en píxeles

private:
    HINSTANCE m_hInst = nullptr; ///< Instancia de la aplicación
    RECT m_rect; ///< Rectángulo de dimensiones de la ventana
    std::string m_windowName = "Hybrid Engine"; ///< Título de la ventana
};

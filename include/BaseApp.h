#pragma once
#include "Prerequisites.h"
#include "Window.h"

class
BaseApp {
public:
    BaseApp()  = default;
    ~BaseApp() = default;

    HRESULT
    init();
	
    void 
    update();
	
    void 
    render();
	
    void 
    destroy();

    /**
     * @brief Ejecuta el bucle principal de la aplicación.
     * @param hInstance Manejador de la instancia de la aplicación.
     * @param hPrevInstance Manejador de la instancia previa de la aplicación (obsoleto).
     * @param lpCmdLine Línea de comandos.
     * @param nCmdShow Indicador de cómo se debe mostrar la ventana.
     * @param wndproc Puntero al procedimiento de ventana.
     * @return int Código de salida de la aplicación.
     */
    int 
    run(HINSTANCE hInstance, 
            HINSTANCE hPrevInstance, 
            LPWSTR lpCmdLine, 
            int nCmdShow, 
            WNDPROC wndproc);

private:
    Window m_window;
};


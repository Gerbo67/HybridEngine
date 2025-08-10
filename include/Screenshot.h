#pragma once
#include "Prerequisites.h"
#include "UserInterface.h"

class Window;
class SwapChain;
class Texture;

/**
 * @brief Sistema de captura de pantalla que permite guardar el contenido renderizado como imagen.
 * 
 * Screenshot proporciona funcionalidad para capturar el buffer de renderizado actual y guardarlo
 * como archivo de imagen. Incluye integración con la interfaz de usuario para activación manual
 * y gestiona la extracción de datos del GPU para conversión a formatos de imagen estándar.
 */
class
Screenshot {
public:
    Screenshot() = default;

    ~Screenshot() {
    };

    /**
     * @brief Captura una imagen del contenido actual de la pantalla.
     * @param window Ventana de la aplicación
     * @param swapChain Cadena de intercambio con el contenido a capturar
     * @param backBuffer Buffer trasero con los datos de imagen
     */
    void
    captureScreenshot(Window window, SwapChain swapChain, Texture& backBuffer);

    /**
     * @brief Renderiza controles de interfaz para activar la captura de pantalla.
     * @param window Ventana de la aplicación
     * @param swapChain Cadena de intercambio para la captura
     * @param backBuffer Buffer trasero con los datos de imagen
     */
    void
    ui(Window window, SwapChain swapChain, Texture& backBuffer);
};

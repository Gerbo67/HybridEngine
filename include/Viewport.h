#pragma once
#include "Prerequisites.h"
class Window;
class DeviceContext;

/**
 * @brief Configuración del viewport que define la región de renderizado en la pantalla.
 * 
 * Viewport especifica las dimensiones y coordenadas de la región donde se renderizarán los gráficos,
 * controlando la transformación de coordenadas del espacio de pantalla normalizado a píxeles finales.
 * Esencial para definir áreas de renderizado, ventanas múltiples y configuraciones de cámara específicas.
 */
class
Viewport {
public:
    Viewport() = default;
    ~Viewport() = default;

    /**
     * @brief Inicializa el viewport basado en las dimensiones de una ventana.
     * @param window Ventana de referencia para obtener dimensiones
     * @return Resultado HRESULT de la operación
     */
    HRESULT
    init(const Window& window);

    /**
     * @brief Inicializa el viewport con dimensiones específicas.
     * @param width Ancho del viewport en píxeles
     * @param height Alto del viewport en píxeles
     * @return Resultado HRESULT de la operación
     */
    HRESULT
    init(unsigned int width, unsigned int height);

    void
    update();

    void
    render(DeviceContext& deviceContext);

    void
    destroy();

public:
    D3D11_VIEWPORT m_viewport; ///< Estructura de viewport DirectX 11 nativa
};

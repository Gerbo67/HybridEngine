#pragma once
#include "Prerequisites.h"

class Device;
class DeviceContext;

/**
 * @brief Gestiona el estado de rasterización para controlar cómo se procesan las primitivas en el pipeline gráfico.
 * 
 * Rasterizer configura el comportamiento de la etapa de rasterización incluyendo modos de relleno,
 * culling de caras, configuración de wireframe y otras propiedades que determinan cómo las primitivas
 * geométricas se convierten en píxeles durante el proceso de renderizado.
 */
class
Rasterizer {
public:
    Rasterizer() = default;
    ~Rasterizer() = default;

    /**
     * @brief Inicializa el estado de rasterizador con configuraciones por defecto.
     * @param device Dispositivo DirectX para crear el estado
     * @return Resultado HRESULT de la operación
     */
    HRESULT
    init(Device device);

    void
    update();

    void
    render(DeviceContext& deviceContext);

    void
    destroy();

private:
    ID3D11RasterizerState* m_rasterizerState = nullptr; ///< Estado de rasterizador DirectX 11 nativo
};

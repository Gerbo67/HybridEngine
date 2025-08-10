#pragma once
#include "Prerequisites.h"

class Device;
class DeviceContext;

/**
 * @brief Encapsula la funcionalidad de estado de mezcla de DirectX 11 para operaciones de mezcla alfa y combinación de colores.
 * 
 * BlendState proporciona una interfaz de alto nivel para gestionar estados de mezcla de DirectX, permitiendo
 * efectos de transparencia sofisticados, mezcla aditiva y operaciones personalizadas de combinación de colores.
 * Soporta factores de mezcla configurables y máscaras de muestreo para técnicas de renderizado avanzadas.
 */
class
BlendState {
public:
    BlendState() = default;
    ~BlendState() = default;

    HRESULT
    init(Device& device);

    void
    update() {
    };

    /**
     * @brief Aplica el estado de mezcla al pipeline de renderizado.
     * @param deviceContext Contexto del dispositivo para aplicar el estado
     * @param blendFactor Factor de mezcla personalizado (opcional)
     * @param sampleMask Máscara de muestreo para control de píxeles
     * @param reset Si debe reiniciar el estado de mezcla
     */
    void
    render(DeviceContext& deviceContext,
           float* blendFactor = nullptr,
           unsigned int sampleMask = 0xffffffff,
           bool reset = false);

    void
    destroy();

private:
    ID3D11BlendState* m_blendState = nullptr; ///< Estado de mezcla nativo de DirectX 11
};

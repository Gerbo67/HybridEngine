#pragma once
#include "Prerequisites.h"

class Device;
class DeviceContext;

/**
 * @brief Estado de sampler que controla cómo se filtran y direccionan las texturas durante el muestreo.
 * 
 * SamplerState define las propiedades de filtrado, direccionamiento y comportamiento de las texturas
 * cuando son muestreadas por los shaders. Controla aspectos como filtrado lineal/punto, wrapping,
 * clamping y otros parámetros esenciales para el renderizado de texturas de alta calidad.
 */
class
SamplerState {
public:
    SamplerState() = default;
    ~SamplerState() = default;

    /**
     * @brief Inicializa el estado de sampler con configuraciones por defecto.
     * @param device Dispositivo DirectX para crear el estado
     * @return Resultado HRESULT de la operación
     */
    HRESULT
    init(Device& device);

    void
    update();

    /**
     * @brief Aplica el estado de sampler al pipeline de renderizado.
     * @param deviceContext Contexto del dispositivo para aplicar el estado
     * @param StartSlot Slot inicial para vincular el sampler
     * @param NumSamplers Número de samplers a establecer
     */
    void
    render(DeviceContext& deviceContext,
           unsigned int StartSlot,
           unsigned int NumSamplers);

    void
    destroy();

public:
    ID3D11SamplerState* m_sampler = nullptr; ///< Estado de sampler DirectX 11 nativo
};
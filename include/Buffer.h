#pragma once
#include "Prerequisites.h"

class Device;
class DeviceContext;
class MeshComponent;

/**
 * @brief Envoltorio versátil de buffer DirectX que soporta operaciones de buffers de vértices, índices y constantes.
 * 
 * Buffer proporciona una interfaz unificada para gestionar diferentes tipos de buffers de DirectX 11 incluyendo
 * buffers de vértices para datos de geometría, buffers de índices para topología de mallas, y buffers constantes
 * para parámetros de shaders. Soporta actualizaciones dinámicas y configuraciones de renderizado flexibles con
 * vinculación de slots personalizable y especificaciones de formato.
 */
class
Buffer {
public:
    Buffer() = default;
    ~Buffer() = default;

    HRESULT
    init(Device& device, const MeshComponent& mesh, unsigned int bindFlag);

    /**
     * @brief Inicializa un buffer con un tamaño específico en bytes.
     * @param device Dispositivo DirectX para crear el buffer
     * @param ByteWidth Tamaño del buffer en bytes
     * @return Resultado HRESULT de la operación
     */
    HRESULT
    init(Device& device, unsigned int ByteWidth);

    void
    update(DeviceContext& deviceContext,
           ID3D11Resource* pDstResource,
           unsigned int DstSubresource,
           const D3D11_BOX* pDstBox,
           const void* pSrcData,
           unsigned int SrcRowPitch,
           unsigned int SrcDepthPitch);

    /**
     * @brief Vincula el buffer al pipeline de renderizado.
     * @param deviceContext Contexto del dispositivo para aplicar el buffer
     * @param StartSlot Slot inicial para vincular el buffer
     * @param NumBuffers Número de buffers a vincular
     * @param setPixelShader Si debe vincularse al pixel shader
     * @param format Formato de los datos del buffer
     */
    void
    render(DeviceContext& deviceContext,
           unsigned int StartSlot,
           unsigned int NumBuffers,
           bool setPixelShader = false,
           DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN);

    void
    destroy();

    /**
     * @brief Crea un buffer DirectX con descriptores y datos específicos.
     * @param device Dispositivo DirectX para crear el buffer
     * @param desc Descriptor del buffer con configuraciones
     * @param initData Datos iniciales para el buffer
     * @return Resultado HRESULT de la operación de creación
     */
    HRESULT
    createBuffer(Device& device,
                 D3D11_BUFFER_DESC& desc,
                 D3D11_SUBRESOURCE_DATA* initData);

private:
    ID3D11Buffer* m_buffer = nullptr; ///< Buffer nativo de DirectX 11
    unsigned int m_stride = 0; ///< Tamaño en bytes de cada elemento
    unsigned int m_offset = 0; ///< Desplazamiento en bytes desde el inicio
    unsigned int m_bindFlag = 0; ///< Flags de vinculación del buffer
};

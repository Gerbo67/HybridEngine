#pragma once
#include "Prerequisites.h"

/**
 * @brief Contexto de dispositivo DirectX que ejecuta comandos de renderizado y gestiona el estado del pipeline gráfico.
 * 
 * DeviceContext actúa como la interfaz principal para enviar comandos de renderizado al GPU. Gestiona
 * la configuración del pipeline gráfico incluyendo shaders, buffers, estados de renderizado y ejecuta
 * operaciones de dibujo. Es responsable de coordinar todos los aspectos del renderizado en tiempo real.
 */
class
DeviceContext {
public:
    DeviceContext() = default;
    ~DeviceContext() = default;

    void
    init();

    void
    update();

    void
    render();

    void
    destroy();

    /**
     * @brief Establece los viewports para la etapa de rasterización.
     * @param NumViewports Número de viewports a establecer
     * @param pViewports Array de estructuras de viewport
     */
    void
    RSSetViewports(unsigned int NumViewports, const D3D11_VIEWPORT* pViewports);

    /**
     * @brief Vincula recursos de shader al pixel shader.
     * @param StartSlot Slot inicial para vincular recursos
     * @param NumViews Número de vistas de recursos a vincular
     * @param ppShaderResourceViews Array de vistas de recursos de shader
     */
    void
    PSSetShaderResources(unsigned int StartSlot,
                         unsigned int NumViews,
                         ID3D11ShaderResourceView* const* ppShaderResourceViews);

    /**
     * @brief Establece el layout de entrada para la etapa de Input Assembler.
     * @param pInputLayout Layout de entrada que describe el formato de vértices
     */
    void
    IASetInputLayout(ID3D11InputLayout* pInputLayout);

    /**
     * @brief Establece el vertex shader para la etapa de vértices.
     * @param pVertexShader Vertex shader a establecer
     * @param ppClassInstances Instancias de clase para shaders dinámicos
     * @param NumClassInstances Número de instancias de clase
     */
    void
    VSSetShader(ID3D11VertexShader* pVertexShader,
                ID3D11ClassInstance* const* ppClassInstances,
                unsigned int NumClassInstances);

    /**
     * @brief Establece el pixel shader para la etapa de píxeles.
     * @param pPixelShader Pixel shader a establecer
     * @param ppClassInstances Instancias de clase para shaders dinámicos
     * @param NumClassInstances Número de instancias de clase
     */
    void
    PSSetShader(ID3D11PixelShader* pPixelShader,
                ID3D11ClassInstance* const * ppClassInstances,
                unsigned int NumClassInstances);

    /**
     * @brief Actualiza un subrecurso con nuevos datos desde la CPU.
     * @param pDstResource Recurso de destino a actualizar
     * @param DstSubresource Índice del subrecurso de destino
     * @param pDstBox Región del recurso a actualizar
     * @param pSrcData Datos fuente desde la CPU
     * @param SrcRowPitch Tamaño de una fila en bytes
     * @param SrcDepthPitch Tamaño de una capa de profundidad en bytes
     */
    void
    UpdateSubresource(ID3D11Resource* pDstResource,
                      unsigned int DstSubresource,
                      const D3D11_BOX* pDstBox,
                      const void* pSrcData,
                      unsigned int SrcRowPitch,
                      unsigned int SrcDepthPitch);

    /**
     * @brief Vincula buffers de vértices a la etapa de Input Assembler.
     * @param StartSlot Slot inicial para vincular buffers
     * @param NumBuffers Número de buffers a vincular
     * @param ppVertexBuffers Array de buffers de vértices
     * @param pStrides Array de tamaños de stride para cada buffer
     * @param pOffsets Array de offsets para cada buffer
     */
    void
    IASetVertexBuffers(unsigned int StartSlot,
                       unsigned int NumBuffers,
                       ID3D11Buffer* const * ppVertexBuffers,
                       const unsigned int* pStrides,
                       const unsigned int* pOffsets);

    /**
     * @brief Establece el buffer de índices para la etapa de Input Assembler.
     * @param pIndexBuffer Buffer de índices a establecer
     * @param Format Formato de los datos de índice
     * @param Offset Offset en bytes desde el inicio del buffer
     */
    void
    IASetIndexBuffer(ID3D11Buffer* pIndexBuffer,
                     DXGI_FORMAT Format,
                     unsigned int Offset);

    /**
     * @brief Establece estados de sampler para el pixel shader.
     * @param StartSlot Slot inicial para vincular samplers
     * @param NumSamplers Número de samplers a establecer
     * @param ppSamplers Array de estados de sampler
     */
    void
    PSSetSamplers(unsigned int StartSlot,
                  unsigned int NumSamplers,
                  ID3D11SamplerState* const* ppSamplers);

    /**
     * @brief Establece el estado de rasterizador para la etapa de rasterización.
     * @param pRasterizerState Estado de rasterizador a aplicar
     */
    void
    RSSetState(ID3D11RasterizerState* pRasterizerState);

    /**
     * @brief Establece el estado de mezcla para la etapa de Output Merger.
     * @param pBlendState Estado de mezcla a aplicar
     * @param BlendFactor Factor de mezcla como array de 4 floats
     * @param SampleMask Máscara de muestreo para control de píxeles
     */
    void
    OMSetBlendState(ID3D11BlendState* pBlendState,
                    const float BlendFactor[4],
                    unsigned int SampleMask);

    /**
     * @brief Establece los objetivos de renderizado y vista de profundidad para la etapa de Output Merger.
     * @param NumViews Número de vistas de objetivo de renderizado
     * @param ppRenderTargetViews Array de vistas de objetivo de renderizado
     * @param pDepthStencilView Vista de profundidad y stencil
     */
    void
    OMSetRenderTargets(unsigned int NumViews,
                       ID3D11RenderTargetView* const* ppRenderTargetViews,
                       ID3D11DepthStencilView* pDepthStencilView);

    /**
     * @brief Establece la topología primitiva para la etapa de Input Assembler.
     * @param Topology Tipo de topología primitiva (triángulos, líneas, puntos, etc.)
     */
    void
    IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY Topology);

    /**
     * @brief Limpia una vista de objetivo de renderizado con un color específico.
     * @param pRenderTargetView Vista de objetivo de renderizado a limpiar
     * @param ColorRGBA Color de limpieza como array RGBA de 4 floats
     */
    void
    ClearRenderTargetView(ID3D11RenderTargetView* pRenderTargetView,
                          const float ColorRGBA[4]);

    /**
     * @brief Limpia la vista de profundidad y stencil con valores específicos.
     * @param pDepthStencilView Vista de profundidad y stencil a limpiar
     * @param ClearFlags Flags que especifican qué limpiar (profundidad/stencil)
     * @param Depth Valor de profundidad para limpiar
     * @param Stencil Valor de stencil para limpiar
     */
    void
    ClearDepthStencilView(ID3D11DepthStencilView* pDepthStencilView,
                          unsigned int ClearFlags,
                          float Depth,
                          UINT8 Stencil);

    /**
     * @brief Establece buffers constantes para el vertex shader.
     * @param StartSlot Slot inicial para vincular buffers
     * @param NumBuffers Número de buffers constantes a establecer
     * @param ppConstantBuffers Array de buffers constantes
     */
    void
    VSSetConstantBuffers(unsigned int StartSlot,
                         unsigned int NumBuffers,
                         ID3D11Buffer* const* ppConstantBuffers);

    /**
     * @brief Establece buffers constantes para el pixel shader.
     * @param StartSlot Slot inicial para vincular buffers
     * @param NumBuffers Número de buffers constantes a establecer
     * @param ppConstantBuffers Array de buffers constantes
     */
    void
    PSSetConstantBuffers(unsigned int StartSlot,
                         unsigned int NumBuffers,
                         ID3D11Buffer* const* ppConstantBuffers);

    /**
     * @brief Dibuja primitivas indexadas usando el buffer de índices actual.
     * @param IndexCount Número de índices a dibujar
     * @param StartIndexLocation Ubicación del primer índice a leer del buffer
     * @param BaseVertexLocation Valor agregado a cada índice antes de leer vértices
     */
    void
    DrawIndexed(unsigned int IndexCount,
                unsigned int StartIndexLocation,
                int BaseVertexLocation);

private:

public:
    ID3D11DeviceContext* m_deviceContext = nullptr; ///< Contexto de dispositivo DirectX 11 nativo
};

#pragma once
#include "Prerequisites.h"

/**
 * @brief Envoltorio central del dispositivo DirectX 11 que gestiona la interfaz de hardware GPU y la creación de recursos.
 * 
 * Device sirve como la interfaz principal al hardware gráfico, proporcionando métodos de fábrica para
 * crear recursos DirectX incluyendo texturas, buffers, shaders y estados de renderizado. Actúa como
 * el componente central para toda la gestión de recursos GPU y configuración del pipeline gráfico dentro
 * de la arquitectura HybridEngine.
 */
class
Device {
public:
    Device() = default;
    ~Device() = default;

    void
    init();

    void
    update();

    void
    render();

    void
    destroy();

    /**
     * @brief Crea una vista de objetivo de renderizado para texturas o superficies de renderizado.
     * @param pResource Recurso base para la vista
     * @param pDesc Descriptor de la vista de objetivo de renderizado
     * @param ppRTView Puntero de salida para la vista creada
     * @return Resultado HRESULT de la operación
     */
    HRESULT
    CreateRenderTargetView(ID3D11Resource* pResource,
                           const D3D11_RENDER_TARGET_VIEW_DESC* pDesc,
                           ID3D11RenderTargetView** ppRTView);

    /**
     * @brief Crea una textura 2D con configuraciones específicas.
     * @param pDesc Descriptor de la textura 2D
     * @param pInitialData Datos iniciales para la textura
     * @param ppTexture2D Puntero de salida para la textura creada
     * @return Resultado HRESULT de la operación
     */
    HRESULT
    CreateTexture2D(const D3D11_TEXTURE2D_DESC* pDesc,
                    const D3D11_SUBRESOURCE_DATA* pInitialData,
                    ID3D11Texture2D** ppTexture2D);

    /**
     * @brief Crea una vista de profundidad y stencil para pruebas de profundidad.
     * @param pResource Recurso de textura de profundidad
     * @param pDesc Descriptor de la vista de profundidad y stencil
     * @param ppDepthStencilView Puntero de salida para la vista creada
     * @return Resultado HRESULT de la operación
     */
    HRESULT
    CreateDepthStencilView(ID3D11Resource* pResource,
                           const D3D11_DEPTH_STENCIL_VIEW_DESC* pDesc,
                           ID3D11DepthStencilView** ppDepthStencilView);

    /**
     * @brief Crea un vertex shader compilado a partir de bytecode.
     * @param pShaderBytecode Bytecode compilado del shader
     * @param BytecodeLength Longitud del bytecode en bytes
     * @param pClassLinkage Enlace de clase para shaders dinámicos
     * @param ppVertexShader Puntero de salida para el vertex shader
     * @return Resultado HRESULT de la operación
     */
    HRESULT
    CreateVertexShader(const void* pShaderBytecode,
                       unsigned int BytecodeLength,
                       ID3D11ClassLinkage* pClassLinkage,
                       ID3D11VertexShader** ppVertexShader);

    /**
     * @brief Crea un layout de entrada que describe el formato de los datos de vértice.
     * @param pInputElementDescs Array de descriptores de elementos de entrada
     * @param NumElements Número de elementos en el array
     * @param pShaderBytecodeWithInputSignature Bytecode del shader con firma de entrada
     * @param BytecodeLength Longitud del bytecode
     * @param ppInputLayout Puntero de salida para el layout de entrada
     * @return Resultado HRESULT de la operación
     */
    HRESULT
    CreateInputLayout(const D3D11_INPUT_ELEMENT_DESC* pInputElementDescs,
                      unsigned int NumElements,
                      const void* pShaderBytecodeWithInputSignature,
                      unsigned int BytecodeLength,
                      ID3D11InputLayout** ppInputLayout);

    /**
     * @brief Crea un pixel shader compilado a partir de bytecode.
     * @param pShaderBytecode Bytecode compilado del shader
     * @param BytecodeLength Longitud del bytecode en bytes
     * @param pClassLinkage Enlace de clase para shaders dinámicos
     * @param ppPixelShader Puntero de salida para el pixel shader
     * @return Resultado HRESULT de la operación
     */
    HRESULT
    CreatePixelShader(const void* pShaderBytecode,
                      unsigned int BytecodeLength,
                      ID3D11ClassLinkage* pClassLinkage,
                      ID3D11PixelShader** ppPixelShader);

    /**
     * @brief Crea un buffer DirectX para datos de vértices, índices o constantes.
     * @param pDesc Descriptor del buffer
     * @param pInitialData Datos iniciales del buffer
     * @param ppBuffer Puntero de salida para el buffer creado
     * @return Resultado HRESULT de la operación
     */
    HRESULT
    CreateBuffer(const D3D11_BUFFER_DESC* pDesc,
                 const D3D11_SUBRESOURCE_DATA* pInitialData,
                 ID3D11Buffer** ppBuffer);

    /**
     * @brief Crea un estado de sampler para filtrado y direccionamiento de texturas.
     * @param pSamplerDesc Descriptor del estado de sampler
     * @param ppSamplerState Puntero de salida para el estado de sampler
     * @return Resultado HRESULT de la operación
     */
    HRESULT
    CreateSamplerState(const D3D11_SAMPLER_DESC* pSamplerDesc,
                       ID3D11SamplerState** ppSamplerState);

    /**
     * @brief Crea un estado de mezcla para operaciones de blending de colores.
     * @param pBlendStateDesc Descriptor del estado de mezcla
     * @param ppBlendState Puntero de salida para el estado de mezcla
     * @return Resultado HRESULT de la operación
     */
    HRESULT
    CreateBlendState(const D3D11_BLEND_DESC* pBlendStateDesc,
                     ID3D11BlendState** ppBlendState);

    /**
     * @brief Crea un estado de profundidad y stencil para configurar pruebas de profundidad.
     * @param pDepthStencilDesc Descriptor del estado de profundidad y stencil
     * @param ppDepthStencilState Puntero de salida para el estado creado
     * @return Resultado HRESULT de la operación
     */
    HRESULT
    CreateDepthStencilState(const D3D11_DEPTH_STENCIL_DESC* pDepthStencilDesc,
                            ID3D11DepthStencilState** ppDepthStencilState);

    /**
     * @brief Crea un estado de rasterizador para configurar el comportamiento de rasterización.
     * @param pRasterizerDesc Descriptor del estado de rasterizador
     * @param ppRasterizerState Puntero de salida para el estado de rasterizador
     * @return Resultado HRESULT de la operación
     */
    HRESULT
    CreateRasterizerState(const D3D11_RASTERIZER_DESC* pRasterizerDesc,
                          ID3D11RasterizerState** ppRasterizerState);

public:
    ID3D11Device* m_device = nullptr; ///< Dispositivo DirectX 11 nativo
};

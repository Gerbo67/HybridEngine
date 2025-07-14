#pragma once
#include "Prerequisites.h"

/**
 * @class Device
 * @brief Representa el dispositivo de renderizado de Direct3D.
 *
 * Esta clase encapsula el dispositivo Direct3D, que es responsable de crear recursos
 * como texturas, búferes y sombreadores.
 */
class 
Device {
public:
	Device() = default;
	~Device()= default;

	void
	init();
	
	void 
	update();
	
	void 
	render();
	
	void 
	destroy();

	/**
	 * @brief Crea una vista de destino de renderizado.
	 * @param pResource Puntero al recurso que se utilizará como destino de renderizado.
	 * @param pDesc Puntero a una descripción de la vista de destino de renderizado.
	 * @param ppRTView Dirección de un puntero a una interfaz ID3D11RenderTargetView.
	 * @return HRESULT Código de éxito o error.
	 */
	HRESULT 
	CreateRenderTargetView(ID3D11Resource *pResource,
												 const D3D11_RENDER_TARGET_VIEW_DESC *pDesc,
												 ID3D11RenderTargetView **ppRTView);

	/**
	 * @brief Crea una textura 2D.
	 * @param pDesc Puntero a una descripción de la textura 2D.
	 * @param pInitialData Puntero a los datos de inicialización de la subfuente.
	 * @param ppTexture2D Dirección de un puntero a una interfaz ID3D11Texture2D.
	 * @return HRESULT Código de éxito o error.
	 */
	HRESULT
	CreateTexture2D(const D3D11_TEXTURE2D_DESC* pDesc,
									const D3D11_SUBRESOURCE_DATA* pInitialData,
									ID3D11Texture2D** ppTexture2D);

	/**
	 * @brief Crea una vista de galería de símbolos de profundidad.
	 * @param pResource Puntero al recurso que se utilizará como vista de galería de símbolos de profundidad.
	 * @param pDesc Puntero a una descripción de la vista de galería de símbolos de profundidad.
	 * @param ppDepthStencilView Dirección de un puntero a una interfaz ID3D11DepthStencilView.
	 * @return HRESULT Código de éxito o error.
	 */
	HRESULT 
	CreateDepthStencilView(ID3D11Resource* pResource,
												 const D3D11_DEPTH_STENCIL_VIEW_DESC* pDesc,
												 ID3D11DepthStencilView** ppDepthStencilView);

	/**
	 * @brief Crea un sombreador de vértices.
	 * @param pShaderBytecode Puntero al bytecode del sombreador compilado.
	 * @param BytecodeLength Longitud del bytecode del sombreador.
	 * @param pClassLinkage Puntero a una interfaz de vinculación de clases.
	 * @param ppVertexShader Dirección de un puntero a una interfaz ID3D11VertexShader.
	 * @return HRESULT Código de éxito o error.
	 */
	HRESULT 
	CreateVertexShader(const void* pShaderBytecode,
										 unsigned int BytecodeLength,
										 ID3D11ClassLinkage* pClassLinkage,
										 ID3D11VertexShader** ppVertexShader);

	/**
	 * @brief Crea un diseño de entrada.
	 * @param pInputElementDescs Puntero a una matriz de descripciones de elementos de entrada.
	 * @param NumElements Número de elementos de entrada en la matriz.
	 * @param pShaderBytecodeWithInputSignature Puntero al bytecode del sombreador compilado que contiene la firma de entrada.
	 * @param BytecodeLength Longitud del bytecode del sombreador.
	 * @param ppInputLayout Dirección de un puntero a una interfaz ID3D11InputLayout.
	 * @return HRESULT Código de éxito o error.
	 */
	HRESULT 
	CreateInputLayout(const D3D11_INPUT_ELEMENT_DESC* pInputElementDescs,
										unsigned int NumElements,
										const void* pShaderBytecodeWithInputSignature,
										unsigned int BytecodeLength,
										ID3D11InputLayout** ppInputLayout);

	/**
	 * @brief Crea un sombreador de píxeles.
	 * @param pShaderBytecode Puntero al bytecode del sombreador compilado.
	 * @param BytecodeLength Longitud del bytecode del sombreador.
	 * @param pClassLinkage Puntero a una interfaz de vinculación de clases.
	 * @param ppPixelShader Dirección de un puntero a una interfaz ID3D11PixelShader.
	 * @return HRESULT Código de éxito o error.
	 */
	HRESULT 
	CreatePixelShader(const void* pShaderBytecode,
										unsigned int BytecodeLength,
										ID3D11ClassLinkage* pClassLinkage,
										ID3D11PixelShader** ppPixelShader);

	/**
	 * @brief Crea un búfer.
	 * @param pDesc Puntero a una descripción del búfer.
	 * @param pInitialData Puntero a los datos de inicialización de la subfuente.
	 * @param ppBuffer Dirección de un puntero a una interfaz ID3D11Buffer.
	 * @return HRESULT Código de éxito o error.
	 */
	HRESULT 
	CreateBuffer(const D3D11_BUFFER_DESC* pDesc,
							 const D3D11_SUBRESOURCE_DATA* pInitialData,
							 ID3D11Buffer** ppBuffer);

	/**
	 * @brief Crea un estado de muestreo.
	 * @param pSamplerDesc Puntero a una descripción del estado de muestreo.
	 * @param ppSamplerState Dirección de un puntero a una interfaz ID3D11SamplerState.
	 * @return HRESULT Código de éxito o error.
	 */
	HRESULT
	CreateSamplerState(const D3D11_SAMPLER_DESC *pSamplerDesc,
										 ID3D11SamplerState** ppSamplerState);

	/**
	 * @brief Crea un estado de mezcla.
	 * @param pBlendStateDesc Puntero a una descripción del estado de mezcla.
	 * @param ppBlendState Dirección de un puntero a una interfaz ID3D11BlendState.
	 * @return HRESULT Código de éxito o error.
	 */
	HRESULT 
	CreateBlendState(const D3D11_BLEND_DESC* pBlendStateDesc,
									 ID3D11BlendState** ppBlendState);

	/**
	 * @brief Crea un estado de galería de símbolos de profundidad.
	 * @param pDepthStencilDesc Puntero a una descripción del estado de galería de símbolos de profundidad.
	 * @param ppDepthStencilState Dirección de un puntero a una interfaz ID3D11DepthStencilState.
	 * @return HRESULT Código de éxito o error.
	 */
	HRESULT 
	CreateDepthStencilState(const D3D11_DEPTH_STENCIL_DESC* pDepthStencilDesc,
													ID3D11DepthStencilState** ppDepthStencilState);

	/**
	 * @brief Crea un estado de rasterizador.
	 * @param pRasterizerDesc Puntero a una descripción del estado de rasterizador.
	 * @param ppRasterizerState Dirección de un puntero a una interfaz ID3D11RasterizerState.
	 * @return HRESULT Código de éxito o error.
	 */
	HRESULT
	CreateRasterizerState(const D3D11_RASTERIZER_DESC* pRasterizerDesc,
												ID3D11RasterizerState** ppRasterizerState);
public:
	ID3D11Device* m_device = nullptr;
};

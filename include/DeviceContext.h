#pragma once
#include "Prerequisites.h"

class 
DeviceContext {
public:
    DeviceContext()  = default;
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
     * @brief Establece las áreas de visualización en el destino de renderizado.
     * @param NumViewports Número de áreas de visualización a establecer.
     * @param pViewports Puntero a una matriz de estructuras D3D11_VIEWPORT.
     */
    void 
    RSSetViewports(unsigned int NumViewports, const D3D11_VIEWPORT* pViewports);

    /**
     * @brief Limpia la vista de galería de símbolos de profundidad.
     * @param pDepthStencilView Puntero a la vista de galería de símbolos de profundidad a limpiar.
     * @param ClearFlags Indicadores que especifican qué partes de la vista limpiar.
     * @param Depth Valor de profundidad con el que limpiar.
     * @param Stencil Valor de galería de símbolos con el que limpiar.
     */
    void 
    ClearDepthStencilView(ID3D11DepthStencilView* pDepthStencilView,
                                                unsigned int ClearFlags,
                                                float Depth,
                                                UINT8 Stencil);

    /**
     * @brief Limpia la vista de destino de renderizado con un color específico.
     * @param pRenderTargetView Puntero a la vista de destino de renderizado a limpiar.
     * @param ColorRGBA Matriz de cuatro componentes que representa el color con el que limpiar.
     */
    void 
    ClearRenderTargetView(ID3D11RenderTargetView* pRenderTargetView, 
                                                const float ColorRGBA[4]);

    /**
     * @brief Establece los destinos de renderizado y la vista de galería de símbolos de profundidad.
     * @param NumViews Número de vistas de destino de renderizado a establecer.
     * @param ppRenderTargetViews Puntero a una matriz de punteros a vistas de destino de renderizado.
     * @param pDepthStencilView Puntero a la vista de galería de símbolos de profundidad.
     */
    void 
    OMSetRenderTargets(unsigned int NumViews,
                                         ID3D11RenderTargetView* const* ppRenderTargetViews,
                                         ID3D11DepthStencilView* pDepthStencilView);
public:
    ID3D11DeviceContext* m_deviceContext = nullptr;
};


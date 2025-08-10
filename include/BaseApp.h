#pragma once
#include "Prerequisites.h"
#include "Window.h"
#include "Device.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "Texture.h"
#include "RenderTargetView.h"
#include "DepthStencilView.h"
#include "Viewport.h"
#include "ShaderProgram.h"
#include "Buffer.h"
#include "MeshComponent.h"
#include "UserInterface.h"
#include "ModelLoader.h"
#include "ECS\Actor.h"

/**
 * @brief Clase principal de la aplicación que gestiona el bucle principal del motor y el pipeline de renderizado DirectX.
 * 
 * BaseApp sirve como la base fundamental del HybridEngine, orquestando los ciclos de inicialización,
 * actualización y renderizado. Administra recursos de DirectX incluyendo dispositivos, cadenas de intercambio,
 * objetivos de renderizado y proporciona capacidades de mapeo de sombras. La clase implementa un
 * sistema de cámara orbital y soporta gestión dinámica de actores a través de una arquitectura ECS.
 */
class
BaseApp {
public:
    BaseApp() = default;
    ~BaseApp() = default;

    HRESULT
    init();

    void
    update();

    void
    render();

    void
    destroy();

    /**
     * @brief Ejecuta el bucle principal de la aplicación con manejo de ventanas Win32.
     * @param hInstance Instancia actual de la aplicación
     * @param hPrevInstance Instancia previa (no utilizada en Win32 moderno)
     * @param lpCmdLine Línea de comandos como string Unicode
     * @param nCmdShow Especifica cómo debe mostrarse la ventana
     * @param wndproc Procedimiento de ventana para manejo de mensajes
     * @return Código de salida de la aplicación
     */
    int
    run(HINSTANCE hInstance,
        HINSTANCE hPrevInstance,
        LPWSTR lpCmdLine,
        int nCmdShow,
        WNDPROC wndproc);

private:
    // Componentes principales del motor gráfico
    Window m_window; ///< Gestión de la ventana de la aplicación
    Device m_device; ///< Dispositivo DirectX11
    SwapChain m_swapChain; ///< Cadena de intercambio para doble buffer
    DeviceContext m_deviceContext; ///< Contexto del dispositivo para comandos de renderizado
    Texture m_backBuffer; ///< Buffer trasero para renderizado
    RenderTargetView m_renderTargetView; ///< Vista del objetivo de renderizado
    Texture m_depthStencil; ///< Textura de profundidad y stencil
    DepthStencilView m_depthStencilView; ///< Vista de profundidad y stencil
    Viewport m_viewport; ///< Configuración del viewport
    ShaderProgram m_shaderProgram; ///< Programa de shaders principal
    ModelLoader m_modelLoader; ///< Cargador de modelos 3D

    // Buffers de la cámara
    Buffer m_neverChanges; ///< Buffer constante que nunca cambia
    Buffer m_changeOnResize; ///< Buffer que cambia al redimensionar la ventana

    Texture m_PlaneTexture; ///< Textura del plano base
    XMMATRIX m_View; ///< Matriz de vista de la cámara
    XMMATRIX m_Projection; ///< Matriz de proyección

    // Parámetros de la cámara orbital
    float m_camYawDeg = 0.0f; ///< Ángulo de rotación horizontal en grados
    float m_camPitchDeg = 15.0f; ///< Ángulo de inclinación vertical en grados
    float m_camDistance = 10.0f; ///< Distancia de la cámara al objetivo
    XMFLOAT3 m_camTarget = XMFLOAT3(0.0f, -5.0f, 0.0f); ///< Punto objetivo de la cámara

    // Variables para el plano y la iluminación
    XMFLOAT4 m_LightPos; ///< Posición de la luz en el espacio 3D

    // Recursos para mapeo de sombras
    Texture m_shadowTexture; ///< Textura de profundidad para sombras (sin tipo específico)
    Texture m_shadowSRVTexture; ///< Vista de recurso de shader para la textura de sombras
    DepthStencilView m_shadowDSV; ///< Vista de profundidad y stencil para el mapa de sombras
    Viewport m_shadowViewport; ///< Viewport para el pase de sombras
    Buffer m_lightBuffer; ///< Buffer constante para matrices y posición de luz
    CBLight cbLight; ///< Datos del lado CPU para el buffer de luz

    float ClearColor[4] = {0.0f, 0.125f, 0.3f, 1.0f}; ///< Color de limpieza del frame buffer

    MeshComponent planeMesh; ///< Componente de malla para el plano base

    CBNeverChanges cbNeverChanges; ///< Estructura de datos constantes que nunca cambian
    CBChangeOnResize cbChangesOnResize; ///< Estructura de datos que cambian al redimensionar
    UserInterface m_userInterface; ///< Sistema de interfaz de usuario

    // Se eliminó el puntero específico al Actor de la pistola.
    EU::TSharedPointer<Actor> m_APlane;
    // Este vector contendrá todos los actores, incluyendo los importados.
    std::vector<EU::TSharedPointer<Actor>> m_actors;
};

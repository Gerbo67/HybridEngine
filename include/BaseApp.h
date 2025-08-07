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
#include "InputLayout.h"
#include "ShaderProgram.h"
#include "Buffer.h"
#include "MeshComponent.h"
#include "BlendState.h"
#include "DepthStencilState.h"
#include "UserInterface.h"
#include "ModelLoader.h"
#include "ECS\Actor.h"

class
BaseApp {
public:
    BaseApp()  = default;
    ~BaseApp() = default;

    HRESULT
    init();
	
    void 
    update();
	
    void 
    render();
	
    void 
    destroy();

    int 
    run(HINSTANCE hInstance, 
            HINSTANCE hPrevInstance, 
            LPWSTR lpCmdLine, 
            int nCmdShow, 
            WNDPROC wndproc);

private:
    Window m_window;
    Device m_device;
    SwapChain m_swapChain;
    DeviceContext m_deviceContext;
    Texture m_backBuffer;
    RenderTargetView m_renderTargetView;
    Texture m_depthStencil;
    DepthStencilView m_depthStencilView;
    Viewport m_viewport;
    ShaderProgram m_shaderProgram;
    ModelLoader m_modelLoader;
    
    // Camera Buffers
    Buffer m_neverChanges;
    Buffer m_changeOnResize;

    // Se eliminó la textura específica de la pistola.
    Texture m_PlaneTexture;
    XMMATRIX m_View;
    XMMATRIX m_Projection;

    //----- Variables para el plano y la luz -----//
    XMFLOAT4 m_LightPos;

    float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };

    // Se eliminó el MeshComponent específico de la pistola.
    MeshComponent planeMesh;

    CBNeverChanges cbNeverChanges;
    CBChangeOnResize cbChangesOnResize;
    UserInterface m_userInterface;

    // Se eliminó el puntero específico al Actor de la pistola.
    EU::TSharedPointer<Actor> m_APlane;
    // Este vector contendrá todos los actores, incluyendo los importados.
    std::vector<EU::TSharedPointer<Actor>> m_actors;
};
#include "BaseApp.h"

// =============================
// Ciclo principal de la aplicación
// =============================
int BaseApp::run(HINSTANCE hInstance, int nCmdShow)
{
    if (FAILED(this->init(hInstance, nCmdShow)))
    {
        this->destroy();
        return 0;
    }

    MSG msg = { 0 };
    while (WM_QUIT != msg.message)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            this->update();
            this->render();
        }
    }

    this->destroy();
    return (int)msg.wParam;
}

// =============================
// Inicialización de recursos
// =============================
HRESULT BaseApp::init(HINSTANCE hInstance, int nCmdShow)
{
    HRESULT hr = S_OK;

    if (FAILED(g_window.init(hInstance, nCmdShow, BaseApp::WndProc)))
        return E_FAIL;

    hr = g_swapChain.init(g_device, g_deviceContext, g_backBuffer, g_window);
    if (FAILED(hr)) return hr;

    hr = g_renderTargetView.init(g_device, g_backBuffer, DXGI_FORMAT_R8G8B8A8_UNORM);
    if (FAILED(hr)) return hr;

    hr = g_depthStencil.init(g_device, g_window.m_width, g_window.m_height, DXGI_FORMAT_D24_UNORM_S8_UINT, D3D11_BIND_DEPTH_STENCIL, 4, 0);
    if (FAILED(hr)) return hr;

    hr = g_depthStencilView.init(g_device, g_depthStencil, DXGI_FORMAT_D24_UNORM_S8_UINT);
    if (FAILED(hr)) return hr;

    hr = g_viewport.init(g_window);
    if (FAILED(hr)) return hr;

    std::vector<D3D11_INPUT_ELEMENT_DESC> layout;
    layout.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
    layout.push_back({ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
    hr = g_shaderProgram.init(g_device, "HybridEngine.fx", layout);
    if (FAILED(hr)) return hr;

    SimpleVertex vertices[] = {
        {XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f)}, {XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f)},
        {XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f)}, {XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f)},
        {XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f)}, {XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f)},
        {XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f)}, {XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f)},
        {XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f)}, {XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f)},
        {XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 1.0f)}, {XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f)},
        {XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f)}, {XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f)},
        {XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 1.0f)}, {XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f)},
        {XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f)}, {XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f)},
        {XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 1.0f)}, {XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT2(0.0f, 1.0f)},
        {XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f)}, {XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f)},
        {XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f)}, {XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f)},
    };
    unsigned int indices[] = {
        3, 1, 0, 2, 1, 3, 6, 4, 5, 7, 4, 6, 11, 9, 8, 10, 9, 11,
        14, 12, 13, 15, 12, 14, 19, 17, 16, 18, 17, 19, 22, 20, 21, 23, 20, 22
    };
    for (int i = 0; i < 24; i++) cubeMesh.m_vertex.push_back(vertices[i]);
    for (int i = 0; i < 36; i++) cubeMesh.m_index.push_back(indices[i]);

    hr = m_vertexBuffer.init(g_device, cubeMesh, D3D11_BIND_VERTEX_BUFFER);
    if (FAILED(hr)) return hr;
    hr = m_indexBuffer.init(g_device, cubeMesh, D3D11_BIND_INDEX_BUFFER);
    if (FAILED(hr)) return hr;

    g_deviceContext.m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    hr = m_neverChanges.init(g_device, sizeof(CBNeverChanges)); if (FAILED(hr)) return hr;
    hr = m_changeOnResize.init(g_device, sizeof(CBChangeOnResize)); if (FAILED(hr)) return hr;
    hr = m_changeEveryFrame.init(g_device, sizeof(CBChangesEveryFrame)); if (FAILED(hr)) return hr;
    hr = D3DX11CreateShaderResourceViewFromFile(g_device.m_device, "seafloor.dds", NULL, NULL, &g_pTextureRV, NULL);
    if (FAILED(hr)) return hr;
    D3D11_SAMPLER_DESC sampDesc;
    ZeroMemory(&sampDesc, sizeof(sampDesc));
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
    hr = g_device.CreateSamplerState(&sampDesc, &g_pSamplerLinear);
    if (FAILED(hr)) return hr;
    XMVECTOR Eye = XMVectorSet(0.0f, 3.0f, -6.0f, 0.0f);
    XMVECTOR At = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    g_View = XMMatrixLookAtLH(Eye, At, Up);
    g_Projection = XMMatrixPerspectiveFovLH(XM_PIDIV4, g_window.m_width / (FLOAT)g_window.m_height, 0.01f, 100.0f);
    cbNeverChanges.mView = XMMatrixTranspose(g_View);
    cbChangesOnResize.mProjection = XMMatrixTranspose(g_Projection);
    SimpleVertex planeVertices[] = {
        {XMFLOAT3(-20.0f, 0.0f, -20.0f), XMFLOAT2(0.0f, 0.0f)}, {XMFLOAT3(20.0f, 0.0f, -20.0f), XMFLOAT2(1.0f, 0.0f)},
        {XMFLOAT3(20.0f, 0.0f, 20.0f), XMFLOAT2(1.0f, 1.0f)}, {XMFLOAT3(-20.0f, 0.0f, 20.0f), XMFLOAT2(0.0f, 1.0f)},
    };
    WORD planeIndices[] = { 0, 2, 1, 0, 3, 2 };
    for (int i = 0; i < 4; i++) planeMesh.m_vertex.push_back(planeVertices[i]);
    for (int i = 0; i < 6; i++) planeMesh.m_index.push_back(planeIndices[i]);

    hr = m_planeVertexBuffer.init(g_device, planeMesh, D3D11_BIND_VERTEX_BUFFER); if (FAILED(hr)) return hr;
    hr = m_planeIndexBuffer.init(g_device, planeMesh, D3D11_BIND_INDEX_BUFFER); if (FAILED(hr)) return hr;
    hr = m_constPlane.init(g_device, sizeof(CBChangesEveryFrame)); if (FAILED(hr)) return hr;
    hr = g_shaderShadow.CreateShader(g_device, PIXEL_SHADER, "HybridEngine.fx"); if (FAILED(hr)) return hr;
    hr = m_constShadow.init(g_device, sizeof(CBChangesEveryFrame)); if (FAILED(hr)) return hr;
    hr = g_shadowBlendState.init(g_device); if (FAILED(hr)) return hr;
    D3D11_DEPTH_STENCIL_DESC dsDesc;
    ZeroMemory(&dsDesc, sizeof(dsDesc));
    dsDesc.DepthEnable = TRUE;
    dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
    dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
    dsDesc.StencilEnable = FALSE;
    hr = g_device.CreateDepthStencilState(&dsDesc, &g_pShadowDepthStencilState);
    if (FAILED(hr)) return hr;
    g_vMeshColor = XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);

    return S_OK;
}

// =============================
// Actualización de la escena
// =============================
void BaseApp::update()
{
    static float t = 0.0f;
    static DWORD dwTimeStart = 0;
    DWORD dwTimeCur = GetTickCount();
    if (dwTimeStart == 0) dwTimeStart = dwTimeCur;
    t = (dwTimeCur - dwTimeStart) / 1000.0f;

    m_neverChanges.update(g_deviceContext, nullptr, 0, nullptr, &cbNeverChanges, 0, 0);
    m_changeOnResize.update(g_deviceContext, nullptr, 0, nullptr, &cbChangesOnResize, 0, 0);

    g_World = XMMatrixTranslation(0.0f, 2.0f, 0.0f) * XMMatrixRotationY(t) * XMMatrixScaling(1.0f, 1.0f, 1.0f);
    g_vMeshColor.x = (sinf(t * 1.0f) + 1.0f) * 0.5f;
    g_vMeshColor.y = (cosf(t * 3.0f) + 1.0f) * 0.5f;
    g_vMeshColor.z = (sinf(t * 5.0f) + 1.0f) * 0.5f;
    cb.mWorld = XMMatrixTranspose(g_World);
    cb.vMeshColor = g_vMeshColor;
    m_changeEveryFrame.update(g_deviceContext, nullptr, 0, nullptr, &cb, 0, 0);

    g_PlaneWorld = XMMatrixTranslation(0.0f, -5.0f, 0.0f);
    cbPlane.mWorld = XMMatrixTranspose(g_PlaneWorld);
    cbPlane.vMeshColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    m_constPlane.update(g_deviceContext, nullptr, 0, nullptr, &cbPlane, 0, 0);

    XMFLOAT4 g_LightPos(2.0f, 4.0f, -2.0f, 1.0f);
    float dot = g_LightPos.y;
    XMMATRIX shadowMatrix = XMMATRIX(dot, -g_LightPos.x, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                                     0.0f, -g_LightPos.z, dot, 0.0f, 0.0f, -1.0f, 0.0f, dot);
    XMMATRIX shadowWorld = g_World * shadowMatrix;
    cbShadow.mWorld = XMMatrixTranspose(shadowWorld);
    cbShadow.vMeshColor = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.5f);
    m_constShadow.update(g_deviceContext, nullptr, 0, nullptr, &cbShadow, 0, 0);
}

// =============================
// Renderizado de la escena
// =============================
void BaseApp::render()
{
    float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };
    float blendFactor[4] = { 0.f, 0.f, 0.f, 0.f };

    g_renderTargetView.render(g_deviceContext, g_depthStencilView, 1, ClearColor);
    g_viewport.render(g_deviceContext);
    g_depthStencilView.render(g_deviceContext);
    g_shaderProgram.render(g_deviceContext);
    m_neverChanges.render(g_deviceContext, 0, 1);
    m_changeOnResize.render(g_deviceContext, 1, 1);

    m_planeVertexBuffer.render(g_deviceContext, 0, 1);
    m_planeIndexBuffer.render(g_deviceContext, 0, 1, false, DXGI_FORMAT_R32_UINT);
    m_constPlane.render(g_deviceContext, 2, 1);
    m_constPlane.render(g_deviceContext, 2, 1, true);
    g_deviceContext.m_deviceContext->PSSetShaderResources(0, 1, &g_pTextureRV);
    g_deviceContext.m_deviceContext->PSSetSamplers(0, 1, &g_pSamplerLinear);
    g_deviceContext.m_deviceContext->DrawIndexed(planeMesh.m_index.size(), 0, 0);

    m_vertexBuffer.render(g_deviceContext, 0, 1);
    m_indexBuffer.render(g_deviceContext, 0, 1, false, DXGI_FORMAT_R32_UINT);
    m_changeEveryFrame.render(g_deviceContext, 2, 1);
    m_changeEveryFrame.render(g_deviceContext, 2, 1, true);
    g_deviceContext.m_deviceContext->PSSetShaderResources(0, 1, &g_pTextureRV);
    g_deviceContext.m_deviceContext->PSSetSamplers(0, 1, &g_pSamplerLinear);
    g_deviceContext.m_deviceContext->DrawIndexed(cubeMesh.m_index.size(), 0, 0);

    g_shaderShadow.render(g_deviceContext, PIXEL_SHADER);
    g_shadowBlendState.render(g_deviceContext, blendFactor, 0xffffffff);
    g_deviceContext.m_deviceContext->OMSetDepthStencilState(g_pShadowDepthStencilState, 0);
    m_vertexBuffer.render(g_deviceContext, 0, 1);
    m_indexBuffer.render(g_deviceContext, 0, 1, false, DXGI_FORMAT_R32_UINT);
    m_constShadow.render(g_deviceContext, 2, 1, true);
    g_deviceContext.m_deviceContext->DrawIndexed(cubeMesh.m_index.size(), 0, 0);
    g_shadowBlendState.render(g_deviceContext, blendFactor, 0xffffffff, true);
    g_deviceContext.m_deviceContext->OMSetDepthStencilState(nullptr, 0);

    g_swapChain.present();
}

// =============================
// Liberación de recursos
// =============================
void BaseApp::destroy()
{
    if (g_deviceContext.m_deviceContext) g_deviceContext.m_deviceContext->ClearState();

    g_shadowBlendState.destroy();
    if (g_pShadowDepthStencilState) g_pShadowDepthStencilState->Release();
    g_shaderShadow.destroy();
    m_planeVertexBuffer.destroy();
    m_planeIndexBuffer.destroy();
    if (g_pSamplerLinear) g_pSamplerLinear->Release();
    if (g_pTextureRV) g_pTextureRV->Release();
    m_neverChanges.destroy();
    m_changeOnResize.destroy();
    m_changeEveryFrame.destroy();
    m_constPlane.destroy();
    m_constShadow.destroy();
    m_vertexBuffer.destroy();
    m_indexBuffer.destroy();
    g_shaderProgram.destroy();
    g_depthStencil.destroy();
    g_depthStencilView.destroy();
    g_renderTargetView.destroy();
    g_swapChain.destroy();
    if (g_deviceContext.m_deviceContext) g_deviceContext.m_deviceContext->Release();
    if (g_device.m_device) g_device.m_device->Release();
}

// =============================
// Procedimiento de ventana
// =============================
LRESULT CALLBACK BaseApp::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;

    switch (message)
    {
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}
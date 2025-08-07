#include "BaseApp.h"
#include "ECS/Transform.h" // Necesario para manipular el componente Transform

HRESULT BaseApp::init() {
    HRESULT hr = S_OK;

    hr = m_swapChain.init(m_device, m_deviceContext, m_backBuffer, m_window);
    if (FAILED(hr)) {
        ERROR("Main", "InitDevice", ("Failed to initialize SwpaChian. HRESULT: " + std::to_string(hr)).c_str());
        return hr;
    }

    hr = m_renderTargetView.init(m_device, m_backBuffer, DXGI_FORMAT_R8G8B8A8_UNORM);
    if (FAILED(hr)) {
        ERROR("Main", "InitDevice", ("Failed to initialize RenderTargetView. HRESULT: " + std::to_string(hr)).c_str());
        return hr;
    }

    hr = m_depthStencil.init(m_device, m_window.m_width, m_window.m_height, DXGI_FORMAT_D24_UNORM_S8_UINT,
                             D3D11_BIND_DEPTH_STENCIL, 4, 0);
    if (FAILED(hr)) {
        ERROR("Main", "InitDevice", ("Failed to initialize DepthStencil. HRESULT: " + std::to_string(hr)).c_str());
        return hr;
    }

    hr = m_depthStencilView.init(m_device, m_depthStencil, DXGI_FORMAT_D24_UNORM_S8_UINT);
    if (FAILED(hr)) {
        ERROR("Main", "InitDevice", ("Failed to initialize DepthStencilView. HRESULT: " + std::to_string(hr)).c_str());
        return hr;
    }

    hr = m_viewport.init(m_window);
    if (FAILED(hr)) {
        ERROR("Main", "InitDevice", ("Failed to initialize Viewport. HRESULT: " + std::to_string(hr)).c_str());
        return hr;
    }

    std::vector<D3D11_INPUT_ELEMENT_DESC> Layout;
    Layout.push_back({"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,
                      D3D11_INPUT_PER_VERTEX_DATA, 0});
    Layout.push_back({"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,
                      D3D11_INPUT_PER_VERTEX_DATA, 0});

    hr = m_shaderProgram.init(m_device, "HybridEngine.fx", Layout);
    if (FAILED(hr)) {
        ERROR("Main", "InitDevice", ("Failed to initialize ShaderProgram. HRESULT: " + std::to_string(hr)).c_str());
        return hr;
    }

    // ===================================================================================
    // BLOQUE DE CÓDIGO DE LA PISTOLA ELIMINADO
    // La carga de modelos ahora se gestiona dinámicamente a través de la interfaz.
    // ===================================================================================

    // Set Plane Actor (Esta parte se mantiene)
    m_APlane = EU::MakeShared<Actor>(m_device);
    if (!m_APlane.isNull()) {
        SimpleVertex planeVertices[] = {
            {XMFLOAT3(-20.0f, 0.0f, -20.0f), XMFLOAT2(0.0f, 0.0f)},
            {XMFLOAT3(20.0f, 0.0f, -20.0f), XMFLOAT2(1.0f, 0.0f)},
            {XMFLOAT3(20.0f, 0.0f, 20.0f), XMFLOAT2(1.0f, 1.0f)},
            {XMFLOAT3(-20.0f, 0.0f, 20.0f), XMFLOAT2(0.0f, 1.0f)},
        };
        WORD planeIndices[] = {0, 2, 1, 0, 3, 2};

        planeMesh.m_vertex.assign(planeVertices, planeVertices + 4);
        planeMesh.m_index.assign(planeIndices, planeIndices + 6);
        planeMesh.m_numVertex = 4;
        planeMesh.m_numIndex = 6;

        hr = m_PlaneTexture.init(m_device, "Textures/Default", DDS);
        if (FAILED(hr)) {
            ERROR("Main", "InitDevice", ("Failed to initialize Plane Texture. HRESULT: " + std::to_string(hr)).c_str());
            return hr;
        }

        std::vector<MeshComponent> PlaneMeshes;
        PlaneMeshes.push_back(planeMesh);
        std::vector<Texture> PlaneTextures;
        PlaneTextures.push_back(m_PlaneTexture);

        m_APlane->setMesh(m_device, PlaneMeshes);
        m_APlane->setTextures(PlaneTextures);
        m_APlane->getComponent<Transform>()->setTransform(EU::Vector3(0.0f, -5.0f, 0.0f), EU::Vector3(0.0f, 0.0f, 0.0f),
                                                          EU::Vector3(1.0f, 1.0f, 1.0f));
        m_APlane->setCastShadow(false);
        m_actors.push_back(m_APlane);
    } else {
        ERROR("Main", "InitDevice", "Failed to create Plane Actor.");
        return E_FAIL;
    }

    hr = m_neverChanges.init(m_device, sizeof(CBNeverChanges));
    if (FAILED(hr))
        return hr;
    hr = m_changeOnResize.init(m_device, sizeof(CBChangeOnResize));
    if (FAILED(hr))
        return hr;

    XMVECTOR Eye = XMVectorSet(0.0f, 3.0f, -6.0f, 0.0f);
    XMVECTOR At = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    m_View = XMMatrixLookAtLH(Eye, At, Up);
    m_Projection = XMMatrixPerspectiveFovLH(XM_PIDIV4, m_window.m_width / (FLOAT)m_window.m_height, 0.01f, 100.0f);

    cbNeverChanges.mView = XMMatrixTranspose(m_View);
    cbChangesOnResize.mProjection = XMMatrixTranspose(m_Projection);

    m_LightPos = XMFLOAT4(2.0f, 4.0f, -2.0f, 1.0f);

    m_userInterface.init(m_window.m_hWnd, m_device.m_device, m_deviceContext.m_deviceContext);

    // ===================================================================================
    // CÓDIGO ACTUALIZADO: Conexión del callback con normalización de escala.
    // ===================================================================================
    m_userInterface.onImportModel = [this](const std::string& modelPath, const std::string& texturePath) {
        ModelLoader fbxLoader;
        // 1. Cargar el modelo FBX (sin cambios)
        if (!fbxLoader.LoadFBXModel(modelPath)) {
            ERROR("BaseApp", "onImportModel", "Failed to load FBX model.");
            return;
        }
        if (fbxLoader.meshes.empty() || fbxLoader.meshes[0].m_vertex.empty()) {
            ERROR("BaseApp", "onImportModel", "Model is empty or has no vertices.");
            return;
        }

        // 2. Calcular la Bounding Box (AABB) del modelo completo
        XMVECTOR minPoint = XMLoadFloat3(&fbxLoader.meshes[0].m_vertex[0].Pos);
        XMVECTOR maxPoint = minPoint;

        for (const auto& mesh : fbxLoader.meshes) {
            for (const auto& vertex : mesh.m_vertex) {
                XMVECTOR point = XMLoadFloat3(&vertex.Pos);
                minPoint = XMVectorMin(minPoint, point);
                maxPoint = XMVectorMax(maxPoint, point);
            }
        }

        // 3. Calcular el centro, el tamaño y el factor de escala para la normalización
        XMVECTOR center = (minPoint + maxPoint) / 2.0f;
        XMVECTOR size = maxPoint - minPoint;
        float largestDimension = max(XMVectorGetX(size), max(XMVectorGetY(size), XMVectorGetZ(size)));

        const float TARGET_SIZE = 3.0f; // El tamaño deseado para la dimensión más grande del modelo
        float scaleFactor = 1.0f;
        if (largestDimension > 0.0001f) {
            scaleFactor = TARGET_SIZE / largestDimension;
        }

        // 4. Crear nuevas mallas con los vértices centrados y re-escalados
        std::vector<MeshComponent> normalizedMeshes;
        for (const auto& originalMesh : fbxLoader.meshes) {
            MeshComponent newMesh = originalMesh; // Copiar metadatos
            newMesh.m_vertex.clear();

            for (const auto& originalVertex : originalMesh.m_vertex) {
                SimpleVertex newVertex = originalVertex;
                XMVECTOR originalPos = XMLoadFloat3(&originalVertex.Pos);

                // Aplicar la transformación: Centrar y luego escalar
                XMVECTOR newPos = (originalPos - center) * scaleFactor;

                XMStoreFloat3(&newVertex.Pos, newPos);
                newMesh.m_vertex.push_back(newVertex);
            }
            normalizedMeshes.push_back(newMesh);
        }

        // 5. Crear y configurar el Actor con la malla normalizada
        EU::TSharedPointer<Actor> newActor = EU::MakeShared<Actor>(m_device);
        if (newActor.isNull()) {
            ERROR("BaseApp", "onImportModel", "Failed to create new Actor.");
            return;
        }

        std::vector<Texture> textures;
        if (!texturePath.empty()) {
            Texture newTexture;
            if (SUCCEEDED(newTexture.init(m_device, texturePath.c_str(), DDS))) {
                textures.push_back(newTexture);
            }
        }

        newActor->setMesh(m_device, normalizedMeshes); // Usar las mallas normalizadas
        newActor->setTextures(textures);

        // La escala ahora es (1, 1, 1) porque el modelo base ya está en el tamaño correcto
        newActor->getComponent<Transform>()->setTransform(
            EU::Vector3(0.0f, 0.0f, 0.0f), // Posición
            EU::Vector3(0.0f, 0.0f, 0.0f), // Rotación
            EU::Vector3(1.0f, 1.0f, 1.0f)); // Escala

        newActor->setCastShadow(true);
        m_actors.push_back(newActor);
    };

    return S_OK;
}

void
BaseApp::update() {
    m_userInterface.update();

    // Asegurarse de que el índice seleccionado sea válido para el vector de actores
    if (m_userInterface.selectedActorIndex >= 0 && m_userInterface.selectedActorIndex < m_actors.size()) {
        m_userInterface.objectControlPanel(m_actors[m_userInterface.selectedActorIndex]);
    }
    m_userInterface.mainMenuBar();
    m_userInterface.outliner(m_actors);

    static float t = 0.0f;
    static DWORD dwTimeStart = 0;
    DWORD dwTimeCur = GetTickCount();
    if (dwTimeStart == 0)
        dwTimeStart = dwTimeCur;
    t = (dwTimeCur - dwTimeStart) / 1000.0f;

    cbNeverChanges.mView = XMMatrixTranspose(m_View);
    m_neverChanges.update(m_deviceContext, nullptr, 0, nullptr, &cbNeverChanges, 0, 0);
    cbChangesOnResize.mProjection = XMMatrixTranspose(m_Projection);
    m_changeOnResize.update(m_deviceContext, nullptr, 0, nullptr, &cbChangesOnResize, 0, 0);

    for (auto& actor : m_actors) {
        if (!actor.isNull()) {
            actor->update(t, m_deviceContext);
        }
    }
}

void
BaseApp::render() {
    m_renderTargetView.render(m_deviceContext, m_depthStencilView, 1, ClearColor);
    m_viewport.render(m_deviceContext);
    m_depthStencilView.render(m_deviceContext);

    m_shaderProgram.render(m_deviceContext);
    m_neverChanges.render(m_deviceContext, 0, 1);
    m_changeOnResize.render(m_deviceContext, 1, 1);

    for (auto& actor : m_actors) {
        if (!actor.isNull()) {
            actor->render(m_deviceContext);
        }
    }

    m_userInterface.render();
    m_swapChain.present();
}

void
BaseApp::destroy() {
    // CORRECCIÓN: Se eliminó la llamada a m_userInterface.destroy()
    // El destructor de UserInterface se encarga de la limpieza automáticamente.
    if (m_deviceContext.m_deviceContext)
        m_deviceContext.m_deviceContext->ClearState();

    // Limpiar actores y sus recursos
    for (auto& actor : m_actors) {
        if (!actor.isNull()) {
            actor->destroy();
        }
    }
    m_actors.clear();

    m_neverChanges.destroy();
    m_changeOnResize.destroy();
    m_shaderProgram.destroy();
    m_depthStencil.destroy();
    m_depthStencilView.destroy();
    m_renderTargetView.destroy();
    m_swapChain.destroy();

    if (m_deviceContext.m_deviceContext)
        m_deviceContext.m_deviceContext->Release();
    if (m_device.m_device)
        m_device.m_device->Release();
}

int
BaseApp::run(HINSTANCE hInstance,
             HINSTANCE hPrevInstance,
             LPWSTR lpCmdLine,
             int nCmdShow,
             WNDPROC wndproc) {
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    if (FAILED(m_window.init(hInstance, nCmdShow, wndproc))) {
        return 0;
    }

    if (FAILED(init())) {
        destroy();
        return 0;
    }

    MSG msg = {0};
    while (WM_QUIT != msg.message) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        } else {
            update();
            render();
        }
    }

    destroy();
    return (int)msg.wParam;
}

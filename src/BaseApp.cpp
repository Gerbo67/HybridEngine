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

        hr = m_PlaneTexture.init(m_device, L"Textures/Grass.dds", DDS);
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
        m_APlane->setReceiveShadow(true);
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

    // Initialize shadow mapping resources
    const unsigned int shadowSize = 1024;
    hr = m_shadowTexture.init(m_device, shadowSize, shadowSize, DXGI_FORMAT_R24G8_TYPELESS,
                              D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE, 1, 0);
    if (FAILED(hr)) {
        ERROR("Main", "InitDevice", ("Failed to create shadow depth texture. HRESULT: " + std::to_string(hr)).c_str());
        return hr;
    }

    hr = m_shadowDSV.init(m_device, m_shadowTexture, DXGI_FORMAT_D24_UNORM_S8_UINT);
    if (FAILED(hr)) {
        ERROR("Main", "InitDevice", ("Failed to create shadow DSV. HRESULT: " + std::to_string(hr)).c_str());
        return hr;
    }

    hr = m_shadowSRVTexture.init(m_device, m_shadowTexture, DXGI_FORMAT_R24_UNORM_X8_TYPELESS);
    if (FAILED(hr)) {
        ERROR("Main", "InitDevice", ("Failed to create shadow SRV. HRESULT: " + std::to_string(hr)).c_str());
        return hr;
    }

    hr = m_shadowViewport.init(shadowSize, shadowSize);
    if (FAILED(hr)) {
        ERROR("Main", "InitDevice", ("Failed to initialize shadow viewport. HRESULT: " + std::to_string(hr)).c_str());
        return hr;
    }

    hr = m_lightBuffer.init(m_device, sizeof(CBLight));
    if (FAILED(hr)) {
        ERROR("Main", "InitDevice", ("Failed to create light constant buffer. HRESULT: " + std::to_string(hr)).c_str());
        return hr;
    }

    // ===================================================================================
    // CÓDIGO ACTUALIZADO: Conexión del callback con normalización de escala.
    // ===================================================================================
    m_userInterface.onImportModel = [this](const std::wstring& modelPath, const std::wstring& texturePath) {
        ModelLoader fbxLoader;
        // 1. Cargar el modelo FBX (sin cambios)
        std::string modelPathStr(modelPath.begin(), modelPath.end());
        if (!fbxLoader.LoadFBXModel(modelPathStr)) {
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

        // 4.1 Calcular la base (minY) del modelo ya normalizado para ubicarlo sobre el piso
        float minYOrig = XMVectorGetY(minPoint);
        float centerY = XMVectorGetY(center);
        float minYNormalized = (minYOrig - centerY) * scaleFactor; // base del modelo en espacio normalizado
        const float floorY = -5.0f;
        const float epsilon = 0.01f; // pequeño desplazamiento sobre el piso
        float placeY = floorY - minYNormalized + epsilon;

        // 5. Crear y configurar el Actor con la malla normalizada
        EU::TSharedPointer<Actor> newActor = EU::MakeShared<Actor>(m_device);
        if (newActor.isNull()) {
            ERROR("BaseApp", "onImportModel", "Failed to create new Actor.");
            return;
        }

        std::vector<Texture> textures;
        if (!texturePath.empty()) {
            Texture newTexture;
            if (SUCCEEDED(newTexture.init(m_device, texturePath, DDS))) {
                textures.push_back(newTexture);
            } else {
                std::string texturePathStr(texturePath.begin(), texturePath.end());
                ERROR("BaseApp", "onImportModel", ("FALLO AL INICIALIZAR LA TEXTURA DDS: " + texturePathStr).c_str());
            }
        } else {
            // Textura por defecto si el usuario no selecciona una
            Texture defaultTex;
            if (SUCCEEDED(defaultTex.init(m_device, L"seafloor.dds", DDS))) {
                textures.push_back(defaultTex);
            } else {
                ERROR("BaseApp", "onImportModel", "No se pudo cargar la textura por defecto seafloor.dds");
            }
        }

        newActor->setMesh(m_device, normalizedMeshes); // Usar las mallas normalizadas
        newActor->setTextures(textures);

        // La escala ahora es (1, 1, 1) porque el modelo base ya está en el tamaño correcto
        newActor->getComponent<Transform>()->setTransform(
            EU::Vector3(0.0f, placeY, 0.0f), // Base del modelo ligeramente sobre el piso
            EU::Vector3(0.0f, 0.0f, 0.0f), // Rotación
            EU::Vector3(1.0f, 1.0f, 1.0f)); // Escala

        newActor->setCastShadow(true);
        newActor->setReceiveShadow(false); // Evitar auto-sombra en el propio FBX
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

    // Light controls UI
    float lightPos[3] = { m_LightPos.x, m_LightPos.y, m_LightPos.z };
    m_userInterface.lightControlPanel(lightPos);
    m_LightPos = XMFLOAT4(lightPos[0], lightPos[1], lightPos[2], 1.0f);

    // Camera controls UI
    m_userInterface.cameraControlPanel(&m_camYawDeg, &m_camPitchDeg, &m_camDistance);

    static float t = 0.0f;
    static DWORD dwTimeStart = 0;
    DWORD dwTimeCur = GetTickCount();
    if (dwTimeStart == 0)
        dwTimeStart = dwTimeCur;
    t = (dwTimeCur - dwTimeStart) / 1000.0f;

    // Compute orbit camera view from yaw/pitch/distance around floor center
    {
        float yawRad = m_camYawDeg * (XM_PI / 180.0f);
        float pitchRad = m_camPitchDeg * (XM_PI / 180.0f);
        float cy = cosf(yawRad);
        float sy = sinf(yawRad);
        float cp = cosf(pitchRad);
        float sp = sinf(pitchRad);
        float ox = sy * cp * m_camDistance;
        float oy = sp * m_camDistance;
        float oz = cy * cp * m_camDistance;
        XMVECTOR Eye = XMVectorSet(m_camTarget.x + ox, m_camTarget.y + oy, m_camTarget.z + oz, 1.0f);
        XMVECTOR At = XMVectorSet(m_camTarget.x, m_camTarget.y, m_camTarget.z, 1.0f);
        XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
        m_View = XMMatrixLookAtLH(Eye, At, Up);
    }

    // Update camera cbuffers
    cbNeverChanges.mView = XMMatrixTranspose(m_View);
    m_neverChanges.update(m_deviceContext, nullptr, 0, nullptr, &cbNeverChanges, 0, 0);
    cbChangesOnResize.mProjection = XMMatrixTranspose(m_Projection);
    m_changeOnResize.update(m_deviceContext, nullptr, 0, nullptr, &cbChangesOnResize, 0, 0);

    // Compute and update light matrices buffer
    XMVECTOR lightEye = XMVectorSet(m_LightPos.x, m_LightPos.y, m_LightPos.z, 1.0f);
    XMVECTOR lightAt = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
    XMVECTOR lightUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    XMMATRIX lightView = XMMatrixLookAtLH(lightEye, lightAt, lightUp);
    XMMATRIX lightProj = XMMatrixPerspectiveFovLH(XM_PIDIV4, 1.0f, 0.1f, 100.0f);

    cbLight.LightView = XMMatrixTranspose(lightView);
    cbLight.LightProjection = XMMatrixTranspose(lightProj);
    cbLight.LightPos = m_LightPos;
    m_lightBuffer.update(m_deviceContext, nullptr, 0, nullptr, &cbLight, 0, 0);

    for (auto& actor : m_actors) {
        if (!actor.isNull()) {
            actor->update(t, m_deviceContext);
        }
    }
}

void
BaseApp::render() {
    // ------------------ Shadow pass ------------------
    // Set shadow viewport and DSV only
    m_shadowViewport.render(m_deviceContext);
    m_deviceContext.OMSetRenderTargets(0, nullptr, m_shadowDSV.m_depthStencilView);
    // Clear shadow depth
    m_deviceContext.ClearDepthStencilView(m_shadowDSV.m_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    // Backup camera matrices
    XMMATRIX viewBackup = cbNeverChanges.mView;
    XMMATRIX projBackup = cbChangesOnResize.mProjection;

    // Use light matrices in camera constant buffers for the shadow pass
    cbNeverChanges.mView = cbLight.LightView; // already transposed
    m_neverChanges.update(m_deviceContext, nullptr, 0, nullptr, &cbNeverChanges, 0, 0);
    cbChangesOnResize.mProjection = cbLight.LightProjection; // already transposed
    m_changeOnResize.update(m_deviceContext, nullptr, 0, nullptr, &cbChangesOnResize, 0, 0);
    // Bind VS constant buffers for shadow pass
    m_neverChanges.render(m_deviceContext, 0, 1);
    m_changeOnResize.render(m_deviceContext, 1, 1);

    // Set vertex shader, but null pixel shader (depth-only)
    m_shaderProgram.render(m_deviceContext, VERTEX_SHADER);
    m_deviceContext.PSSetShader(nullptr, nullptr, 0);

    // Draw only shadow casters
    for (auto& actor : m_actors) {
        if (!actor.isNull() && actor->canCastShadow()) {
            actor->render(m_deviceContext);
        }
    }

    // Restore camera matrices in constant buffers
    cbNeverChanges.mView = viewBackup;
    m_neverChanges.update(m_deviceContext, nullptr, 0, nullptr, &cbNeverChanges, 0, 0);
    cbChangesOnResize.mProjection = projBackup;
    m_changeOnResize.update(m_deviceContext, nullptr, 0, nullptr, &cbChangesOnResize, 0, 0);

    // ------------------ Main pass ------------------
    m_renderTargetView.render(m_deviceContext, m_depthStencilView, 1, ClearColor);
    m_viewport.render(m_deviceContext);
    m_depthStencilView.render(m_deviceContext);

    // Set full shader program
    m_shaderProgram.render(m_deviceContext);

    // Bind camera constant buffers
    m_neverChanges.render(m_deviceContext, 0, 1);
    m_changeOnResize.render(m_deviceContext, 1, 1);

    // Bind light cbuffer to both VS and PS and shadow map SRV to t1
    m_lightBuffer.render(m_deviceContext, 3, 1, true);
    m_shadowSRVTexture.render(m_deviceContext, 1, 1);

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

    // Destroy shadow resources
    m_shadowSRVTexture.destroy();
    m_shadowDSV.destroy();
    m_shadowTexture.destroy();
    m_lightBuffer.destroy();

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

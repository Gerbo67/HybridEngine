#pragma once
#include "Prerequisites.h"
#include "imgui.h"
#include <imgui_internal.h>
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <commdlg.h>  // Para el diálogo de archivo
#include <functional>

class Window;
class SwapChain;
class Texture;
class Actor;
class ModelComponent;
class Transform;

class 
UserInterface {
public:
    UserInterface();
    ~UserInterface();

    void 
    init(void* window, ID3D11Device* device, ID3D11DeviceContext* deviceContext);

    void 
    update();
  
    void 
    render();
  
    void
    vec3Control(const std::string& label,
                float* values,
                float resetValue = 0.0f,
                float columnWidth = 100.0f);

    void
    floatControl(const std::string& label,
                 float* value,
                 float resetValue = 0.0f,
                 float columnWidth = 100.0f);

    void 
    menuBar(Window window, SwapChain swapChain, Texture& backBuffer);

    void
    Renderer(Window window, ID3D11ShaderResourceView* renderTexture);

    void
    inspectorGeneral(EU::TSharedPointer<Actor> actor);

    void 
    inspectorContainer(EU::TSharedPointer<Actor> actor);

    void
    output();

    // Nuevas funciones para la interfaz mejorada
    void
    mainMenuBar();

    void
    objectControlPanel(EU::TSharedPointer<Actor> actor);

    void
    transformControls(EU::TSharedPointer<Actor> actor);

    void
    scaleControls(EU::TSharedPointer<Actor> actor);

    void
    rotationControls(EU::TSharedPointer<Actor> actor);

    std::wstring
    openFileDialog(const wchar_t* filter);

    void
    showImportDialog();

    void
    darkStyle();
  
    void
    greyStyle();

    void
    GameMakerStyle();

    void
    visualStudioStyle();

    void
    ToolTip(std::string icon, std::string tip);
    void
    ToolTip( std::string tip);

    void
    toolTipData();

    void 
    ToolBar();

    void 
    closeApp();

    void
    RenderFullScreenTransparentWindow();

    void
    outliner(const std::vector<EU::TSharedPointer<Actor>>& actors);

    void
    lightControlPanel(float position[3]);

    void
    cameraControlPanel(float* yawDeg, float* pitchDeg, float* distance);

private:
    bool checkboxValue = true;
    bool checkboxValue2 = false;
    std::vector<const char*> m_objectsNames;
    std::vector<const char*> m_tooltips;

    bool show_exit_popup = false; // Variable de estado para el popup
    // Nuevas variables para la interfaz
    bool show_import_dialog = false;
    std::wstring selectedFilePath = L"";
    bool showObjectControls = true;
    HWND m_windowHandle = nullptr;

public:
    int selectedActorIndex = -1;
    // El callback ahora necesita dos rutas: una para el modelo y otra para la textura
    std::function<void(const std::wstring&, const std::wstring&)> onImportModel;
    std::function<void()> onExitApplication;
};
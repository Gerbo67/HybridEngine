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

/**
 * @brief Sistema de interfaz de usuario que integra ImGui para controles de editor y herramientas de desarrollo.
 * 
 * UserInterface proporciona una interfaz gráfica completa para el motor, incluyendo inspectores de propiedades,
 * menús de aplicación, controles de transformación y herramientas de desarrollo. Utiliza ImGui como backend
 * para renderizado inmediato de elementos UI y se integra seamlessly con el pipeline de renderizado DirectX.
 */
class
UserInterface {
public:
    UserInterface();
    ~UserInterface();

    /**
     * @brief Inicializa el sistema de interfaz de usuario con contextos DirectX.
     * @param window Handle de la ventana Win32
     * @param device Dispositivo DirectX 11 para renderizado UI
     * @param deviceContext Contexto del dispositivo para comandos UI
     */
    void
    init(void* window, ID3D11Device* device, ID3D11DeviceContext* deviceContext);

    void
    update();

    void
    render();

    /**
     * @brief Crea controles de interfaz para vectores de 3 componentes (posición, rotación, escala).
     * @param label Etiqueta del control
     * @param values Array de 3 valores float (X, Y, Z)
     * @param resetValue Valor por defecto para resetear
     * @param columnWidth Ancho de las columnas de entrada
     */
    void
    vec3Control(const std::string& label,
                float* values,
                float resetValue = 0.0f,
                float columnWidth = 100.0f);

    /**
     * @brief Crea un control de entrada para valores float individuales.
     * @param label Etiqueta del control
     * @param value Puntero al valor float a modificar
     * @param resetValue Valor por defecto para resetear
     * @param columnWidth Ancho de la columna de entrada
     */
    void
    floatControl(const std::string& label,
                 float* value,
                 float resetValue = 0.0f,
                 float columnWidth = 100.0f);

    /**
     * @brief Renderiza la barra de menú principal con opciones de archivo y herramientas.
     * @param window Ventana de la aplicación
     * @param swapChain Cadena de intercambio para operaciones de captura
     * @param backBuffer Buffer trasero para funcionalidades de screenshot
     */
    void
    menuBar(Window window, SwapChain swapChain, Texture& backBuffer);

    /**
     * @brief Renderiza la ventana del renderer con preview de texturas.
     * @param window Ventana de la aplicación
     * @param renderTexture Textura de renderizado para mostrar en preview
     */
    void
    Renderer(Window window, ID3D11ShaderResourceView* renderTexture);

    /**
     * @brief Panel inspector general para propiedades básicas de actores.
     * @param actor Actor seleccionado para inspección
     */
    void
    inspectorGeneral(EU::TSharedPointer<Actor> actor);

    /**
     * @brief Panel contenedor del inspector con múltiples secciones de propiedades.
     * @param actor Actor seleccionado para inspección detallada
     */
    void
    inspectorContainer(EU::TSharedPointer<Actor> actor);

    /**
     * @brief Ventana de salida para logs, errores y información de depuración.
     */
    void
    output();

    /**
     * @brief Renderiza la barra de menú principal mejorada con nuevas funcionalidades.
     */
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
    ToolTip(std::string tip);

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

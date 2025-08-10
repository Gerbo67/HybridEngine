#include "UserInterface.h"

#include <algorithm>

#include "ECS/Actor.h"
#include "ECS/Transform.h"
#include "EngineUtilities/Vectors/Vector3.h"

class Transform;

UserInterface::UserInterface() {
    selectedActorIndex = 0;
}

UserInterface::~UserInterface() {
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void UserInterface::init(void* window, ID3D11Device* device, ID3D11DeviceContext* deviceContext) {
    m_windowHandle = (HWND)window;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    ImGui::StyleColorsDark();
    ImGui_ImplWin32_Init(window);
    ImGui_ImplDX11_Init(device, deviceContext);
}

void UserInterface::update() {
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

void UserInterface::render() {
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void UserInterface::vec3Control(const std::string& label, float* values, float resetValue, float columnWidth) {
    ImGui::PushID(label.c_str());

    ImGui::Columns(2);
    ImGui::SetColumnWidth(0, columnWidth);
    ImGui::Text(label.c_str());
    ImGui::NextColumn();

    ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0, 0});

    float lineHeight = GImGui->Font->LegacySize + GImGui->Style.FramePadding.y * 2.0f;
    ImVec2 buttonSize = {lineHeight + 3.0f, lineHeight};

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.9f, 0.2f, 0.2f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
    if (ImGui::Button("X", buttonSize))
        values[0] = resetValue;
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat("##X", &values[0], 0.1f, 0.0f, 0.0f, "%.2f");
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.3f, 0.8f, 0.3f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
    if (ImGui::Button("Y", buttonSize))
        values[1] = resetValue;
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat("##Y", &values[1], 0.1f, 0.0f, 0.0f, "%.2f");
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.1f, 0.25f, 0.8f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.2f, 0.35f, 0.9f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.1f, 0.25f, 0.8f, 1.0f});
    if (ImGui::Button("Z", buttonSize))
        values[2] = resetValue;
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat("##Z", &values[2], 0.1f, 0.0f, 0.0f, "%.2f");
    ImGui::PopItemWidth();

    ImGui::PopStyleVar();
    ImGui::Columns(1);
    ImGui::PopID();
}

void UserInterface::floatControl(const std::string& label, float* value, float resetValue, float columnWidth) {
    ImGui::PushID(label.c_str());

    ImGui::Columns(2);
    ImGui::SetColumnWidth(0, columnWidth);
    ImGui::Text(label.c_str());
    ImGui::NextColumn();

    ImGui::PushItemWidth(-1.0f);
    ImGui::DragFloat("##value", value, 0.001f, 0.0f, 0.0f, "%.3f");
    ImGui::PopItemWidth();

    ImGui::Columns(1);
    ImGui::PopID();
}

void UserInterface::mainMenuBar() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Import Model", "Ctrl+I")) {
                showImportDialog();
            }
            ToolTip("Importar modelos FBX u OBJ");

            ImGui::Separator();

            if (ImGui::MenuItem("Exit", "Alt+F4")) {
                show_exit_popup = true;
            }
            ToolTip("Salir de la aplicación");

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Options")) {
            if (ImGui::MenuItem("Show Object Controls", nullptr, &showObjectControls)) {
                // Toggle object controls visibility
            }
            ToolTip("Mostrar/ocultar panel de controles del objeto");

            ImGui::Separator();

            if (ImGui::BeginMenu("UI Style")) {
                if (ImGui::MenuItem("Dark")) {
                    darkStyle();
                }
                if (ImGui::MenuItem("Grey")) {
                    greyStyle();
                }
                if (ImGui::MenuItem("Game Maker")) {
                    GameMakerStyle();
                }
                if (ImGui::MenuItem("Visual Studio")) {
                    visualStudioStyle();
                }
                ImGui::EndMenu();
            }
            ToolTip("Cambiar el estilo de la interfaz");

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    // Mostrar diálogo de salida si está activo
    if (show_exit_popup) {
        closeApp();
    }
}

void UserInterface::objectControlPanel(EU::TSharedPointer<Actor> actor) {
    if (!showObjectControls || actor.isNull()) {
        return;
    }

    ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - 350, 25), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(330, 500), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("Object Controls", &showObjectControls)) {
        ImGui::Text("Selected Object: Actor %d", selectedActorIndex);
        ImGui::Separator();

        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
            transformControls(actor);
        }

        if (ImGui::CollapsingHeader("Scale", ImGuiTreeNodeFlags_DefaultOpen)) {
            scaleControls(actor);
        }

        if (ImGui::CollapsingHeader("Rotation", ImGuiTreeNodeFlags_DefaultOpen)) {
            rotationControls(actor);
        }

        ImGui::Separator();

        if (selectedActorIndex != 0) {
            if (ImGui::Button("Reset Transform", ImVec2(-1, 0))) {
                auto transform = actor->getComponent<Transform>();
                if (transform) {
                    transform->setTransform(EU::Vector3(0.0f, 0.0f, 0.0f),
                                            EU::Vector3(0.0f, 0.0f, 0.0f),
                                            EU::Vector3(1.0f, 1.0f, 1.0f));
                }
            }
            ToolTip("Resetear todas las transformaciones del objeto");
        } else {
            ImGui::Text("Reset Transform: Locked (Floor)");
            ToolTip("El piso no se puede resetear desde la UI");
        }
    }
    ImGui::End();
}

void UserInterface::transformControls(EU::TSharedPointer<Actor> actor) {
    if (selectedActorIndex == 0) {
        ImGui::Text("Position: Locked (Floor)");
        ToolTip("El piso no se puede mover");
        return;
    }
    auto transform = actor->getComponent<Transform>();
    if (!transform)
        return;

    EU::Vector3 position = transform->getPosition();
    float positionArray[3] = {position.x, position.y, position.z};

    vec3Control("Position", positionArray, 0.0f, 80.0f);

    // Actualizar la posición si cambió
    if (positionArray[0] != position.x || positionArray[1] != position.y || positionArray[2] != position.z) {
        transform->setPosition(EU::Vector3(positionArray[0], positionArray[1], positionArray[2]));
    }

    if (ImGui::IsItemHovered()) {
        ToolTip("Controla la posición del objeto en el espacio 3D");
    }
}

void UserInterface::scaleControls(EU::TSharedPointer<Actor> actor) {
    if (selectedActorIndex == 0) {
        ImGui::Text("Scale: Locked (Floor)");
        ToolTip("La escala del piso está bloqueada");
        return;
    }
    auto transform = actor->getComponent<Transform>();
    if (!transform)
        return;

    EU::Vector3 scale = transform->getScale();

    static bool uniformScale = true;
    ImGui::Checkbox("Uniform Scale", &uniformScale);
    ToolTip("Mantener proporciones al escalar");

    if (uniformScale) {
        float uniformValue = scale.x;

        ImGui::PushItemWidth(-1);
        if (ImGui::DragFloat("Uniform", &uniformValue, 0.01f, 0.0f, FLT_MAX, "%.2f")) {
            transform->setScale(EU::Vector3(uniformValue, uniformValue, uniformValue));
        }
        ImGui::PopItemWidth();
    } else {
        float scaleArray[3] = {scale.x, scale.y, scale.z};
        vec3Control("Scale", scaleArray, 1.0f, 80.0f);

        if (scaleArray[0] != scale.x || scaleArray[1] != scale.y || scaleArray[2] != scale.z) {
            transform->setScale(EU::Vector3(scaleArray[0], scaleArray[1], scaleArray[2]));
        }
    }
}

void UserInterface::rotationControls(EU::TSharedPointer<Actor> actor) {
    if (selectedActorIndex == 0) {
        ImGui::Text("Rotation: Locked (Floor)");
        ToolTip("La rotación del piso está bloqueada");
        return;
    }
    auto transform = actor->getComponent<Transform>();
    if (!transform)
        return;

    EU::Vector3 rotation = transform->getRotation();
    float rotationArray[3] = {rotation.x, rotation.y, rotation.z};

    vec3Control("Rotation", rotationArray, 0.0f, 80.0f);

    // Actualizar la rotación si cambió
    if (rotationArray[0] != rotation.x || rotationArray[1] != rotation.y || rotationArray[2] != rotation.z) {
        transform->setRotation(EU::Vector3(rotationArray[0], rotationArray[1], rotationArray[2]));
    }

    if (ImGui::IsItemHovered()) {
        ToolTip("Controla la rotación del objeto en grados");
    }

    // Botones de rotación rápida
    ImGui::Text("Quick Rotation:");
    if (ImGui::Button("90° X")) {
        rotation.x += 90.0f;
        transform->setRotation(rotation);
    }
    ImGui::SameLine();
    if (ImGui::Button("90° Y")) {
        rotation.y += 90.0f;
        transform->setRotation(rotation);
    }
    ImGui::SameLine();
    if (ImGui::Button("90° Z")) {
        rotation.z += 90.0f;
        transform->setRotation(rotation);
    }
    ToolTip("Rotar 90 grados en el eje seleccionado");
}

std::wstring UserInterface::openFileDialog(const wchar_t* filter) {
    OPENFILENAMEW ofn; // Usamos la versión 'W' (Wide/Unicode)
    wchar_t szFile[260] = {0};

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = m_windowHandle;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = filter;
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

    if (GetOpenFileNameW(&ofn)) {
        return std::wstring(szFile);
    }

    return L"";
}


void UserInterface::showImportDialog() {
    // CAMBIO 2: Usar wstring y literales wide (L"...")
    std::wstring modelPath = openFileDialog(L"FBX Models (*.fbx)\0*.fbx\0All Files\0*.*\0");

    if (!modelPath.empty()) {
        std::replace(modelPath.begin(), modelPath.end(), L'\\', L'/');

        std::wstring texturePath = openFileDialog(L"DDS Textures (*.dds)\0*.dds\0All Files\0*.*\0");

        if (!texturePath.empty()) {
            std::replace(texturePath.begin(), texturePath.end(), L'\\', L'/');
        }

        if (onImportModel) {
            onImportModel(modelPath, texturePath);
        }

        selectedFilePath = modelPath;
        ImGui::OpenPopup("Import Success");
    }

    if (ImGui::BeginPopupModal("Import Success", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Model imported successfully!");

        // CAMBIO 3: Convertir wstring a string solo para mostrar en ImGui
        std::string displayPath(selectedFilePath.begin(), selectedFilePath.end());
        ImGui::Text("File: %s", displayPath.c_str());

        ImGui::Separator();

        if (ImGui::Button("OK", ImVec2(120, 0))) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}

void UserInterface::closeApp() {
    if (ImGui::BeginPopupModal("Exit Application", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("¿Estás seguro de que quieres salir?");
        ImGui::Separator();

        if (ImGui::Button("Sí", ImVec2(120, 0))) {
            if (onExitApplication) {
                onExitApplication();
            }
            PostQuitMessage(0);
        }
        ImGui::SetItemDefaultFocus();
        ImGui::SameLine();
        if (ImGui::Button("No", ImVec2(120, 0))) {
            show_exit_popup = false;
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }

    if (show_exit_popup) {
        ImGui::OpenPopup("Exit Application");
    }
}

void UserInterface::ToolTip(std::string icon, std::string tip) {
    if (ImGui::IsItemHovered()) {
        ImGui::BeginTooltip();
        ImGui::Text("%s %s", icon.c_str(), tip.c_str());
        ImGui::EndTooltip();
    }
}

void UserInterface::ToolTip(std::string tip) {
    if (ImGui::IsItemHovered()) {
        ImGui::BeginTooltip();
        ImGui::Text("%s", tip.c_str());
        ImGui::EndTooltip();
    }
}

void UserInterface::darkStyle() {
    ImGui::StyleColorsDark();
}

void UserInterface::greyStyle() {
    ImGuiStyle& style = ImGui::GetStyle();
    style.Colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
    style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
    style.Colors[ImGuiCol_ChildBg] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
    style.Colors[ImGuiCol_PopupBg] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
    style.Colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
    style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
    style.Colors[ImGuiCol_TitleBg] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
    style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
    style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
    style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
    style.Colors[ImGuiCol_CheckMark] = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
    style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.08f, 0.50f, 0.72f, 1.00f);
    style.Colors[ImGuiCol_Button] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
    style.Colors[ImGuiCol_Header] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
}

void UserInterface::GameMakerStyle() {
    ImGuiStyle& style = ImGui::GetStyle();
    style.Colors[ImGuiCol_Text] = ImVec4(0.92f, 0.92f, 0.92f, 1.00f);
    style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.44f, 0.44f, 0.44f, 1.00f);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
    style.Colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
    style.Colors[ImGuiCol_Border] = ImVec4(0.51f, 0.36f, 0.15f, 1.00f);
    style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.51f, 0.36f, 0.15f, 1.00f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.78f, 0.55f, 0.21f, 1.00f);
    style.Colors[ImGuiCol_TitleBg] = ImVec4(0.51f, 0.36f, 0.15f, 1.00f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
    style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
    style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.53f);
    style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.21f, 0.21f, 0.21f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.47f, 0.47f, 0.47f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.81f, 0.83f, 0.81f, 1.00f);
    style.Colors[ImGuiCol_CheckMark] = ImVec4(0.78f, 0.55f, 0.21f, 1.00f);
    style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
    style.Colors[ImGuiCol_Button] = ImVec4(0.51f, 0.36f, 0.15f, 1.00f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.78f, 0.55f, 0.21f, 1.00f);
    style.Colors[ImGuiCol_Header] = ImVec4(0.51f, 0.36f, 0.15f, 1.00f);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.93f, 0.65f, 0.14f, 1.00f);
}

void UserInterface::visualStudioStyle() {
    ImGuiStyle& style = ImGui::GetStyle();
    style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
    style.Colors[ImGuiCol_ChildBg] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
    style.Colors[ImGuiCol_PopupBg] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
    style.Colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
    style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
    style.Colors[ImGuiCol_TitleBg] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
    style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
    style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
    style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
    style.Colors[ImGuiCol_CheckMark] = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
    style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.08f, 0.50f, 0.72f, 1.00f);
    style.Colors[ImGuiCol_Button] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
    style.Colors[ImGuiCol_Header] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
}

void UserInterface::outliner(const std::vector<EU::TSharedPointer<Actor>>& actors) {
    ImGui::SetNextWindowPos(ImVec2(10, 25), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(250, 400), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("Scene Outliner")) {
        ImGui::Text("Scene Objects (%zu)", actors.size());
        ImGui::Separator();

        for (size_t i = 0; i < actors.size(); ++i) {
            if (actors[i].isNull())
                continue;

            std::string actorName = "Actor " + std::to_string(i);

            // Verificar si este actor está seleccionado
            bool isSelected = (selectedActorIndex == static_cast<int>(i));

            if (ImGui::Selectable(actorName.c_str(), isSelected)) {
                selectedActorIndex = static_cast<int>(i);
            }

            if (ImGui::IsItemHovered()) {
                ToolTip("Click para seleccionar este objeto");
            }

            // Mostrar información adicional del actor
            if (isSelected) {
                ImGui::Indent();
                auto transform = actors[i]->getComponent<Transform>();
                if (transform) {
                    EU::Vector3 pos = transform->getPosition();
                    ImGui::Text("Position: (%.2f, %.2f, %.2f)", pos.x, pos.y, pos.z);
                }
                ImGui::Unindent();
            }
        }
    }
    ImGui::End();
}

void UserInterface::lightControlPanel(float position[3]) {
    ImGui::SetNextWindowPos(ImVec2(10, 440), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(260, 120), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Light Controls")) {
        ImGui::Text("Light Position");
        vec3Control("Position", position, 0.0f, 75.0f);
        if (ImGui::IsItemHovered()) {
            ToolTip("Mueve la luz para ver sombras en tiempo real");
        }
    }
    ImGui::End();
}

void UserInterface::cameraControlPanel(float* yawDeg, float* pitchDeg, float* distance) {
    ImGui::SetNextWindowPos(ImVec2(280, 440), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(280, 140), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Camera")) {
        ImGui::Text("Orbit Camera");
        ImGui::SliderFloat("Yaw (deg)", yawDeg, -180.0f, 180.0f, "%.1f");
        ImGui::SliderFloat("Pitch (deg)", pitchDeg, -89.0f, 89.0f, "%.1f");
        ImGui::SliderFloat("Distance", distance, 1.0f, 50.0f, "%.2f");
        ToolTip("Camara orbital: Yaw/Pitch/Distancia alrededor del piso");
    }
    ImGui::End();
}

#pragma once
#include "Prerequisites.h"

class Device;
class DeviceContext;

/**
 * @brief Define el formato y organización de los datos de entrada de vértices para el pipeline gráfico.
 * 
 * InputLayout especifica cómo los datos de vértices están organizados en memoria y cómo deben ser
 * interpretados por el vertex shader. Define elementos como posición, normales, coordenadas de textura
 * y otros atributos de vértice, asegurando la correcta comunicación entre la aplicación y los shaders.
 */
class
InputLayout {
public:
    InputLayout() = default;
    ~InputLayout() = default;

    /**
     * @brief Inicializa el layout de entrada con elementos específicos y datos del vertex shader.
     * @param device Dispositivo DirectX para crear el layout
     * @param Layout Vector de descriptores de elementos de entrada
     * @param VertexShaderData Datos compilados del vertex shader para validación
     * @return Resultado HRESULT de la operación
     */
    HRESULT
    init(Device& device,
         std::vector<D3D11_INPUT_ELEMENT_DESC>& Layout,
         ID3DBlob* VertexShaderData);

    void
    update();

    void
    render(DeviceContext& deviceContext);

    void
    destroy();

public:
    ID3D11InputLayout* m_inputLayout = nullptr; ///< Layout de entrada DirectX 11 nativo
};

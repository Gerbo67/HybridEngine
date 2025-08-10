#pragma once
#include "Prerequisites.h"
#include "InputLayout.h"

class Device;
class DeviceContext;

/**
 * @brief Programa de shaders completo que gestiona vertex y pixel shaders junto con su layout de entrada.
 * 
 * ShaderProgram encapsula la funcionalidad completa de shaders DirectX incluyendo compilación desde archivo,
 * creación de shaders específicos y gestión del layout de entrada asociado. Proporciona una interfaz
 * unificada para trabajar con programas de shaders complejos y su configuración en el pipeline gráfico.
 */
class
ShaderProgram {
public:
    ShaderProgram() = default;
    ~ShaderProgram() = default;

    /**
     * @brief Inicializa el programa de shaders completo desde un archivo.
     * @param device Dispositivo DirectX para crear los shaders
     * @param fileName Nombre del archivo que contiene el código de shaders
     * @param Layout Vector de descriptores para el layout de entrada
     * @return Resultado HRESULT de la operación
     */
    HRESULT
    init(Device& device,
         const std::string& fileName,
         std::vector<D3D11_INPUT_ELEMENT_DESC> Layout);

    void
    update();

    void
    render(DeviceContext& deviceContext);

    /**
     * @brief Renderiza aplicando un tipo específico de shader.
     * @param deviceContext Contexto del dispositivo para aplicar shaders
     * @param type Tipo de shader a aplicar (vertex, pixel, etc.)
     */
    void
    render(DeviceContext& deviceContext, ShaderType type);

    void
    destroy();

    /**
     * @brief Crea el layout de entrada para los shaders.
     * @param device Dispositivo DirectX para crear el layout
     * @param Layout Vector de descriptores de elementos de entrada
     * @return Resultado HRESULT de la operación
     */
    HRESULT
    CreateInputLayout(Device& device, std::vector<D3D11_INPUT_ELEMENT_DESC> Layout);

    /**
     * @brief Crea un shader específico del tipo indicado.
     * @param device Dispositivo DirectX para crear el shader
     * @param type Tipo de shader a crear
     * @return Resultado HRESULT de la operación
     */
    HRESULT
    CreateShader(Device& device, ShaderType type);

    /**
     * @brief Crea un shader desde un archivo específico.
     * @param device Dispositivo DirectX para crear el shader
     * @param type Tipo de shader a crear
     * @param fileName Nombre del archivo fuente del shader
     * @return Resultado HRESULT de la operación
     */
    HRESULT
    CreateShader(Device& device, ShaderType type, const std::string& fileName);

    /**
     * @brief Compila código de shader desde un archivo fuente.
     * @param szFileName Nombre del archivo de shader
     * @param szEntryPoint Punto de entrada del shader
     * @param szShaderModel Modelo de shader a utilizar
     * @param ppBlobOut Puntero de salida para el bytecode compilado
     * @return Resultado HRESULT de la compilación
     */
    HRESULT
    CompileShaderFromFile(char* szFileName,
                          LPCSTR szEntryPoint,
                          LPCSTR szShaderModel,
                          ID3DBlob** ppBlobOut);

public:
    ID3D11VertexShader* m_VertexShader = nullptr; ///< Vertex shader compilado
    ID3D11PixelShader* m_PixelShader = nullptr; ///< Pixel shader compilado
    InputLayout m_inputLayout; ///< Layout de entrada asociado

private:
    std::string m_shaderFileName; ///< Nombre del archivo de shader
    ID3DBlob* m_vertexShaderData = nullptr; ///< Bytecode del vertex shader
    ID3DBlob* m_pixelShaderData = nullptr; ///< Bytecode del pixel shader
};

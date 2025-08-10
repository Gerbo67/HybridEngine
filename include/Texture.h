#pragma once
#include "Prerequisites.h"

class Device;
class DeviceContext;

/**
 * @brief Gestión completa de texturas 2D para renderizado, incluyendo carga desde archivos y creación programática.
 * 
 * Texture encapsula la funcionalidad de texturas DirectX proporcionando métodos para cargar imágenes desde archivos,
 * crear texturas en tiempo de ejecución con dimensiones específicas y gestionar vistas de recursos para shaders.
 * Soporta múltiples formatos de imagen y configuraciones avanzadas como MSAA para texturas de alta calidad.
 */
class
Texture {
public:
    Texture() = default;
    ~Texture() = default;

    /**
     * @brief Inicializa una textura cargando desde un archivo de imagen.
     * @param device Dispositivo DirectX para crear la textura
     * @param textureName Nombre del archivo de textura a cargar
     * @param extensionType Tipo de extensión del archivo (DDS, PNG, JPG, etc.)
     * @return Resultado HRESULT de la operación
     */
    HRESULT
    init(Device device, const std::wstring& textureName, ExtensionType extensionType);

    /**
     * @brief Inicializa una textura con dimensiones y formato específicos.
     * @param device Dispositivo DirectX para crear la textura
     * @param width Ancho de la textura en píxeles
     * @param height Alto de la textura en píxeles
     * @param Format Formato de píxeles de la textura
     * @param BindFlags Flags de vinculación (render target, shader resource, etc.)
     * @param sampleCount Número de muestras para MSAA
     * @param qualityLevels Niveles de calidad para MSAA
     * @return Resultado HRESULT de la operación
     */
    HRESULT
    init(Device device,
         unsigned int width,
         unsigned int height,
         DXGI_FORMAT Format,
         unsigned int BindFlags,
         unsigned int sampleCount = 1,
         unsigned int qualityLevels = 0);

    /**
     * @brief Inicializa una textura basada en otra textura existente con formato diferente.
     * @param device Dispositivo DirectX para crear la textura
     * @param textureRef Textura de referencia como base
     * @param format Nuevo formato para la textura
     * @return Resultado HRESULT de la operación
     */
    HRESULT
    init(Device& device, Texture& textureRef, DXGI_FORMAT format);

    void
    update();

    /**
     * @brief Vincula la textura al pipeline de shaders para renderizado.
     * @param deviceContext Contexto del dispositivo para vincular la textura
     * @param StartSlot Slot inicial para vincular la textura
     * @param NumViews Número de vistas de textura a vincular
     */
    void
    render(DeviceContext& deviceContext, unsigned int StartSlot, unsigned int NumViews);

    void
    destroy();

public:
    ID3D11Texture2D* m_texture = nullptr; ///< Recurso de textura 2D como datos en memoria
    ID3D11ShaderResourceView* m_textureFromImg; ///< Vista de recurso de shader para acceso desde shaders
    std::wstring m_textureName; ///< Nombre del archivo de textura cargado
};

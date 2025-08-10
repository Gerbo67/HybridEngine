//--------------------------------------------------------------------------------------
// File: ShadowTester.fx
//
// Se han definido dos pixel shaders: uno para el renderizado normal y otro para la sombra.
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Variables de los constant buffers
//--------------------------------------------------------------------------------------
Texture2D txDiffuse : register(t0);
Texture2D txShadow : register(t1);
SamplerState samLinear : register(s0);

cbuffer cbNeverChanges : register(b0)
{
    matrix View;
};

cbuffer cbChangeOnResize : register(b1)
{
    matrix Projection;
};

cbuffer cbChangesEveryFrame : register(b2)
{
    matrix World;
    float4 vMeshColor;
};

cbuffer cbLight : register(b3)
{
    matrix LightView;
    matrix LightProjection;
    float4 LightPos;
};

//--------------------------------------------------------------------------------------
// Estructuras de entrada y salida para los shaders
//--------------------------------------------------------------------------------------
struct VS_INPUT
{
    float4 Pos : POSITION;
    float2 Tex : TEXCOORD0;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float2 Tex : TEXCOORD0;
    float4 LightPos : TEXCOORD1;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS(VS_INPUT input)
{
    PS_INPUT output = (PS_INPUT) 0;
    float4 worldPos = mul(input.Pos, World);
    output.Pos = mul(worldPos, View);
    output.Pos = mul(output.Pos, Projection);
    output.Tex = input.Tex;

    // Position from light for shadow mapping
    float4 lightClip = mul(worldPos, LightView);
    lightClip = mul(lightClip, LightProjection);
    output.LightPos = lightClip;
    
    return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader para renderizado normal
//--------------------------------------------------------------------------------------
float4 PS(PS_INPUT input) : SV_Target
{
    // Multiplicar solo RGB por el color del mesh y mantener alpha opaco
    float4 baseCol = txDiffuse.Sample(samLinear, input.Tex) * float4(vMeshColor.rgb, 1.0f);

    // Si este mesh no debe recibir sombras, omitir el muestreo del shadow map
    if (vMeshColor.a < 0.5f)
    {
        baseCol.a = 1.0f;
        return baseCol;
    }

    // Proyección desde el espacio de clip de la luz a coordenadas de textura
    float3 projCoords = input.LightPos.xyz / input.LightPos.w;
    float2 shadowUV = projCoords.xy * 0.5f + 0.5f;
    float currentDepth = projCoords.z * 0.5f + 0.5f; // map from [-1,1] to [0,1]

    // Si está fuera del shadow map, considerar iluminado
    if (shadowUV.x < 0.0f || shadowUV.x > 1.0f || shadowUV.y < 0.0f || shadowUV.y > 1.0f)
    {
        baseCol.a = 1.0f;
        return baseCol;
    }

    float shadowDepth = txShadow.Sample(samLinear, shadowUV).r;
    float bias = 0.003f;
    float visibility = (currentDepth - bias) > shadowDepth ? 0.35f : 1.0f;

    baseCol.rgb *= visibility;
    baseCol.a = 1.0f;
    return baseCol;
}

//--------------------------------------------------------------------------------------
// Pixel Shader para la sombra (color oscuro y semitransparente)
//--------------------------------------------------------------------------------------
float4 ShadowPS(PS_INPUT input) : SV_Target
{
    return float4(0.0f, 0.0f, 0.0f, 0.5f);
}
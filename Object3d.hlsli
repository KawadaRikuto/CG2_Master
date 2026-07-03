struct VertexShaderOutput {
    float4 position : POSITION;
    float2 texcoord : TEXCOORD0;
};

struct VertexShaderInput
{
    float4 position : POSITION;
    float2 texcoord : TEXCOORD;
};

struct Material
{
    float32_t4 color;
    int32_t enableLighting;
};

struct TransformationMatrix
{
    float32_t4x4 WVP;
    float32_t4x4 World;
};

struct DirectionalLight
{
    float32_t3 direction;
    float32_t4 color;
    float intensity;
};

struct TransformationMatrix
{
    float32_t4x4 WVP;
    float32_t4x4 World;
};

struct DirectionalLight
{
    float32_t3 direction;
    float32_t4 color;
    float intensity;
};
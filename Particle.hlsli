#ifndef PARTICLE_HLSLI
#define PARTICLE_HLSLI

struct TransformationMatrix
{
    float32_t4x4 WVP;
    float32_t4x4 World;
};

struct VertexShaderInput
{
    float32_t4 position : POSITION;
    float32_t2 texcoord : TEXCOORD0;
    float32_t3 normal : NORMAL0;
};

struct VertexShaderOutput
{
    float32_t4 position : SV_POSITION;
    float32_t2 texcoord : TEXCOORD0;
    float32_t3 normal : NORMAL0;
};

#endif

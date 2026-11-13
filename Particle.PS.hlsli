#include "Particle.hlsli"
struct Material
{
    float32_t4 color;
    int32_t enableLighting;
    float32_t3 padding;
    float32_t4x4 uvTransform;
};
ConstantBuffer<Material> gMaterial : register(b0);
Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);
float32_t4 main(VertexShaderOutput input) : SV_TARGET0
{
    float32_t2 uv = input.texcoord * 2.0f - 1.0f;
    float d = length(uv);
    float glow = saturate(1.0f - d);
    float core = pow(glow, 2.3f);
    float alpha = core * input.color.a;
    if (alpha < 0.012f)
        discard;
    float3 rgb = input.color.rgb * (0.7f + 0.3f * pow(glow, 0.5f));
    return float32_t4(rgb, alpha);
}

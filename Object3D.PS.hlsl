struct Material
{
    float32_t4 color;
    int32_t enableLighting;
};

struct DirectionalLight
{
    float32_t4 color;
    float32_t3 direction;
    float intensity;
};

ConstantBuffer<Material> gMaterial : register(b0);
ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1);

Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);

struct PixelShaderInput
{
    float32_t4 position : SV_POSITION;
    float32_t2 texcoord : TEXCOORD0;
    float32_t3 normal : NORMAL0;
};

struct PixelShaderOutput
{
    float32_t4 color : SV_TARGET0;
};

PixelShaderOutput main(PixelShaderInput input)
{
    PixelShaderOutput output;

    float32_t4 textureColor = gTexture.Sample(gSampler, input.texcoord);

    if (gMaterial.enableLighting != 0)
    {
        float cos = saturate(dot(normalize(input.normal), -normalize(gDirectionalLight.direction)));
        output.color = gMaterial.color * textureColor * gDirectionalLight.color * cos * gDirectionalLight.intensity;
    }
    else
    {
        output.color = gMaterial.color * textureColor;
    }

    return output;
}

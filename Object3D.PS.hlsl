struct Material
{
    float32_t4 color;
    int32_t enableLighting;
    float32_t3 padding;
    float32_t4x4 uvTransform;
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

    float32_t4 transformedUV =
        mul(float32_t4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);

    float32_t4 textureColor =
        gTexture.Sample(gSampler, transformedUV.xy);

    if (gMaterial.enableLighting != 0)
    {
        float NdotL = dot(normalize(input.normal), -normalize(gDirectionalLight.direction));
        float cosLight = pow(NdotL * 0.5f + 0.5f, 2.0f);

        output.color =
            textureColor * gMaterial.color *
            gDirectionalLight.color *
            gDirectionalLight.intensity *
            cosLight;
    }
    else
    {
        output.color = textureColor * gMaterial.color;
    }

    return output;
}
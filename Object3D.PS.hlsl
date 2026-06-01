struct Material {
    float4 color;
};
ConstantBuffer<Material> gMaterial : register(b0);

Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);


struct PSInput {
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
};

struct PixelShaderOutput {
    float4 color : SV_TARGET0;
};

PixelShaderOutput main(PSInput input) {
    PixelShaderOutput output;
    
    // テクスチャから色をサンプリング
    output.color = gTexture.Sample(gSampler, input.texcoord);
   
    return output;
}
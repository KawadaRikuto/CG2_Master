struct TransformationMatrix {
    float4x4 WVP;
};
ConstantBuffer<TransformationMatrix> gTransformationMatrix : register(b1);


struct VSInput {
    float4 position : POSITION;
    float2 texcoord : TEXCOORD;
};


struct VSOutput {
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
};

VSOutput main(VSInput input) {
    VSOutput output;

    // 行列の乗算
    output.position = mul(input.position, gTransformationMatrix.WVP);
    output.texcoord = input.texcoord;

    return output;
}
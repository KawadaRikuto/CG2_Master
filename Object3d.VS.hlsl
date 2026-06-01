struct TransformationMatrix
{
    float4x4 WVP;
};

ConstantBuffer<TransformationMatrix>
gTransformationMatrix
    : register(b0);

struct VSInput
{
    float4 position : POSITION;
};

struct VSOutput
{
    float4 position : SV_POSITION;
};

VSOutput main(VSInput input)
{
    VSOutput output;

    output.position =
        mul(
            input.position,
            gTransformationMatrix.WVP);

    return output;
}
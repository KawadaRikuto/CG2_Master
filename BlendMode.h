#pragma once
#include <d3d12.h>

enum BlendMode {
    kBlendModeNone,
    kBlendModeNormal,
    kBlendModeAdd,
    kBlendModeSubtract,
    kBlendModeMultiply,
    kBlendModeScreen,
    kCountOfBlendMode
};

inline D3D12_BLEND_DESC CreateBlendDesc(BlendMode mode) {
    D3D12_BLEND_DESC desc{};
    auto& rt = desc.RenderTarget[0];
    rt.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
    rt.BlendEnable = TRUE;
    rt.BlendOp = D3D12_BLEND_OP_ADD;
    rt.SrcBlendAlpha = D3D12_BLEND_ONE;
    rt.BlendOpAlpha = D3D12_BLEND_OP_ADD;
    rt.DestBlendAlpha = D3D12_BLEND_ZERO;
    switch (mode) {
    case kBlendModeNone:
        rt.BlendEnable = FALSE;
        rt.SrcBlend = D3D12_BLEND_ONE;
        rt.DestBlend = D3D12_BLEND_ZERO;
        break;
    case kBlendModeNormal:
        rt.SrcBlend = D3D12_BLEND_SRC_ALPHA;
        rt.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
        break;
    case kBlendModeAdd:
        rt.SrcBlend = D3D12_BLEND_SRC_ALPHA;
        rt.DestBlend = D3D12_BLEND_ONE;
        break;
    case kBlendModeSubtract:
        rt.SrcBlend = D3D12_BLEND_SRC_ALPHA;
        rt.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
        rt.DestBlend = D3D12_BLEND_ONE;
        break;
    case kBlendModeMultiply:
        rt.SrcBlend = D3D12_BLEND_ZERO;
        rt.DestBlend = D3D12_BLEND_SRC_COLOR;
        break;
    case kBlendModeScreen:
        rt.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;
        rt.DestBlend = D3D12_BLEND_ONE;
        break;
    default:
        rt.BlendEnable = FALSE;
        rt.SrcBlend = D3D12_BLEND_ONE;
        rt.DestBlend = D3D12_BLEND_ZERO;
        break;
    }
    return desc;
}

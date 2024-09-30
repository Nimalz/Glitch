#pragma once
#ifndef VERTEX_H
#define VERTEX_H

#include <d3d11_1.h>
#include <DirectXMath.h>

struct VertexPosTex
{
    VertexPosTex() = default;

    VertexPosTex(const VertexPosTex&) = default;
    VertexPosTex& operator=(const VertexPosTex&) = default;

    VertexPosTex(VertexPosTex&&) = default;
    VertexPosTex& operator=(VertexPosTex&&) = default;

    constexpr VertexPosTex(const DirectX::XMFLOAT3& _pos, const DirectX::XMFLOAT2& _tex) :
        pos(_pos), tex(_tex) {}

    DirectX::XMFLOAT3 pos;
    DirectX::XMFLOAT2 tex;
    static const D3D11_INPUT_ELEMENT_DESC inputLayout[2];
};

#endif
#pragma once
#include "EffectWrapper.h"
#include <vector>

struct Vertex;
struct dae::Matrix;


class Mesh final
{
public:

	Mesh() = default;
	~Mesh();

	Mesh(const Mesh&) = delete;
	Mesh(Mesh&&) noexcept = delete;
	Mesh& operator=(const Mesh&) = delete;
	Mesh& operator=(Mesh&&) noexcept = delete;

	void Initialize(ID3D11Device* pDevice, std::vector<Vertex> vertices, std::vector<uint32_t> indices);

	void Render(ID3D11DeviceContext* pDeviceContext) const;
	void SetMatrix(const dae::Matrix& worldViewProjMatrix) const;
	void SetDiffuseMap(const Texture* pTexture) const;
	void SetSampler(ID3D11Device* pDevice, D3D11_FILTER filter);

private:
	EffectWrapper* m_pEffect;
	ID3D11InputLayout* m_pInputLayout;
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;
	ID3D11SamplerState* m_pSamplerState;
	uint32_t m_NumIndices;
};

struct Vertex final
{
	float position[3]{};
	float color[3]{};
	float UV[2]{};
};
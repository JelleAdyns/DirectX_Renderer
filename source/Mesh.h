#pragma once
#include "EffectWrapperOpaque.h"
#include "EffectWrapperTransparancy.h"
#include <vector>


class Texture;
struct Vertex;
struct dae::Matrix;


class Mesh final
{
public:

	Mesh() = default;
	Mesh(const dae::Matrix& worldMatrix);
	~Mesh();

	Mesh(const Mesh&) = delete;
	Mesh(Mesh&&) noexcept = delete;
	Mesh& operator=(const Mesh&) = delete;
	Mesh& operator=(Mesh&&) noexcept = delete;

	void Initialize(ID3D11Device* pDevice, const std::wstring& assetFile, std::vector<Vertex> vertices, std::vector<uint32_t> indices, bool needsTransparancy = false);

	void Render(ID3D11DeviceContext* pDeviceContext) const;
	void SetEffectMatrices(const dae::Matrix& worldViewProjMatrix, const dae::Matrix& worldMatrix) const;
	void SetCameraPosition(const dae::Vector3& newCameraPosition)const;
	void SetSampler(ID3D11Device* pDevice, D3D11_FILTER filter);

	void SetTextures(Texture* pDiffuse, Texture* pNormalMap = nullptr, Texture* pSpecular = nullptr, Texture* pGlossiness = nullptr);

	const dae::Matrix& GetWorldMatrix() const { return m_WorldMatrix; }

private:
	EffectWrapper* m_pEffect{ nullptr };
	ID3D11InputLayout* m_pInputLayout{ nullptr };
	ID3D11Buffer* m_pVertexBuffer{ nullptr };
	ID3D11Buffer* m_pIndexBuffer{ nullptr };
	ID3D11SamplerState* m_pSamplerState{ nullptr };
	uint32_t m_NumIndices{ };

	dae::Matrix m_WorldMatrix{};
	Texture* m_pDiffuseTexture{ nullptr };
	Texture* m_pNormalTexture{ nullptr };
	Texture* m_pSpecularTexture{ nullptr };
	Texture* m_pGlossTexture{ nullptr };
};




#pragma once
#include "EffectWrapper.h"

class EffectWrapperOpaque final : public EffectWrapper
{
public:
	explicit EffectWrapperOpaque(ID3D11Device* pDevice, const std::wstring& assetFile);
	~EffectWrapperOpaque();

	EffectWrapperOpaque(const EffectWrapperOpaque&) = delete;
	EffectWrapperOpaque(EffectWrapperOpaque&&) noexcept = delete;
	EffectWrapperOpaque& operator=(const EffectWrapperOpaque&) = delete;
	EffectWrapperOpaque& operator=(EffectWrapperOpaque&&) noexcept = delete;

	ID3DX11EffectVectorVariable* GetCameraVariable() const;
	virtual void SetCameraPosition(const dae::Vector3& newCameraPosition) override;
	virtual void SetTextures(const Texture* pDiffuse, const Texture* pNormalMap = nullptr, const Texture* pSpecular = nullptr, const Texture* pGlossiness = nullptr) override;
private:
	ID3DX11EffectVectorVariable* m_pCameraPosition;
	ID3DX11EffectShaderResourceVariable* m_pNormalMapVariable;
	ID3DX11EffectShaderResourceVariable* m_pSpecularMapVariable;
	ID3DX11EffectShaderResourceVariable* m_pGlossMapVariable;
	ID3DX11EffectShaderResourceVariable* m_pDiffuseMapVariable;
};


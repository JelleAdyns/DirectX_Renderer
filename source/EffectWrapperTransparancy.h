#pragma once
#include "EffectWrapper.h"
class EffectWrapperTransparancy final : public EffectWrapper
{
public:
	explicit EffectWrapperTransparancy(ID3D11Device* pDevice, const std::wstring& assetFile);
	~EffectWrapperTransparancy();

	EffectWrapperTransparancy(const EffectWrapperTransparancy&) = delete;
	EffectWrapperTransparancy(EffectWrapperTransparancy&&) noexcept = delete;
	EffectWrapperTransparancy& operator=(const EffectWrapperTransparancy&) = delete;
	EffectWrapperTransparancy& operator=(EffectWrapperTransparancy&&) noexcept = delete;

	virtual void SetCameraPosition(const dae::Vector3& newCameraPosition) override {};
	virtual void SetTextures(const Texture* pDiffuse, const Texture* pNormalMap = nullptr, const Texture* pSpecular = nullptr, const Texture* pGlossiness = nullptr) override;

private:
	ID3DX11EffectShaderResourceVariable* m_pDiffuseMapVariable;
};


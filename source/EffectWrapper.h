#pragma once

#include "Texture.h"

struct dae::Matrix;
struct dae::Vector3;
class EffectWrapper
{
public:
	explicit EffectWrapper(ID3D11Device* pDevice, const std::wstring& assetFile);
	~EffectWrapper();

	EffectWrapper(const EffectWrapper&) = delete;
	EffectWrapper(EffectWrapper&&) noexcept = delete;
	EffectWrapper& operator=(const EffectWrapper&) = delete;
	EffectWrapper& operator=(EffectWrapper&&) noexcept = delete;


	ID3DX11Effect* GetEffect() const;
	ID3DX11EffectTechnique* GetTechinque() const;
	ID3DX11EffectMatrixVariable* GetWVPMatrix() const;
	ID3DX11EffectSamplerVariable* GetSamplerVariable() const;

	void SetMatrices(const dae::Matrix& worldViewProjMatrix, const dae::Matrix& worldMatrix);
	void SetSamplerVariable(ID3D11SamplerState* pSamplerState);

	//void SetDiffuseTexture(const Texture* pDiffuse);
	virtual void SetCameraPosition(const dae::Vector3& newCameraPosition) = 0;
	virtual void SetTextures(const Texture* pDiffuse, const Texture* pNormalMap = nullptr, const Texture* pSpecular = nullptr, const Texture* pGlossiness = nullptr) = 0;
	void SetNormalBoolean(bool useNormalMaps) { m_pBooleanUseNormalMaps->SetBool(useNormalMaps); };

protected:

	ID3DX11Effect* m_pEffect;
	ID3DX11EffectTechnique* m_pTechinque;
	ID3DX11EffectMatrixVariable* m_pWorldViewProjVariable;
	ID3DX11EffectMatrixVariable* m_pWorldVariable;
	
	ID3DX11EffectSamplerVariable* m_pSamplerVariable;
	ID3DX11EffectScalarVariable* m_pBooleanUseNormalMaps;

private:

	static ID3DX11Effect* LoadEffect(ID3D11Device* pDevice, const std::wstring& assetFile);
	
	
};


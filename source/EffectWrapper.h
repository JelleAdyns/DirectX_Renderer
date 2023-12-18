#pragma once

class Texture;
struct dae::Matrix;
class EffectWrapper final
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
	void SetMatrix(const dae::Matrix& worldViewProjMatrix);
	void SetDiffuseMap(const Texture* pDiffuseTexture);

private:
	static ID3DX11Effect* LoadEffect(ID3D11Device* pDevice, const std::wstring& assetFile);

	ID3DX11Effect* m_pEffect;
	ID3DX11EffectTechnique* m_pTechinque;
	ID3DX11EffectMatrixVariable* m_pMatWorldViewProjVariable;
	ID3DX11EffectShaderResourceVariable* m_pDiffuseMapVariable;
	//ID3D11SamplerState* m_pSamplerState;
	
};


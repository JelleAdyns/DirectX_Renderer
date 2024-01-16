#include "pch.h"
#include "EffectWrapperTransparancy.h"


EffectWrapperTransparancy::EffectWrapperTransparancy(ID3D11Device* pDevice, const std::wstring& assetFile):
	EffectWrapper{pDevice, assetFile }
{
	m_pTechinque = m_pEffect->GetTechniqueByName("TransparentTechnique");
	if (!m_pTechinque->IsValid()) std::wcout << L"Transparent Technique not valid\n";

	m_pDiffuseMapVariable = m_pEffect->GetVariableByName("gDiffuseMapTransparancy")->AsShaderResource();
	if (!m_pDiffuseMapVariable->IsValid()) std::wcout << L"Diffuse map not valid\n";
}

void EffectWrapperTransparancy::SetTextures(const Texture* pDiffuse, const Texture* pNormalMap, const Texture* pSpecular, const Texture* pGlossiness)
{
	if (m_pDiffuseMapVariable) m_pDiffuseMapVariable->SetResource(pDiffuse->GetShaderResourceView());
}

EffectWrapperTransparancy::~EffectWrapperTransparancy()
{
	m_pDiffuseMapVariable->Release();
}
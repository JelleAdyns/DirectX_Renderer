#include "pch.h"
#include "EffectWrapperOpaque.h"


EffectWrapperOpaque::EffectWrapperOpaque(ID3D11Device* pDevice, const std::wstring& assetFile) :
	EffectWrapper{ pDevice, assetFile }
{
	m_pCameraPosition = m_pEffect->GetVariableByName("gCameraPosition")->AsVector();
	if (!m_pCameraPosition->IsValid()) std::wcout << L"MatWorldVariable not valid\n";

	m_pDiffuseMapVariable = m_pEffect->GetVariableByName("gDiffuseMapOpaque")->AsShaderResource();
	if (!m_pDiffuseMapVariable->IsValid()) std::wcout << L"Diffuse map not valid\n";

	m_pNormalMapVariable = m_pEffect->GetVariableByName("gNormalMap")->AsShaderResource();
	if (!m_pNormalMapVariable->IsValid()) std::wcout << L"Normal map not valid\n";

	m_pSpecularMapVariable = m_pEffect->GetVariableByName("gSpecularMap")->AsShaderResource();
	if (!m_pSpecularMapVariable->IsValid()) std::wcout << L"Specular map not valid\n";

	m_pGlossMapVariable = m_pEffect->GetVariableByName("gGlossinessMap")->AsShaderResource();
	if (!m_pGlossMapVariable->IsValid()) std::wcout << L"Glossiness map not valid\n";



}
EffectWrapperOpaque::~EffectWrapperOpaque()
{
	
	m_pGlossMapVariable->Release();
	m_pSpecularMapVariable->Release();
	m_pNormalMapVariable->Release();
	m_pDiffuseMapVariable->Release();
	m_pCameraPosition->Release();
}

ID3DX11EffectVectorVariable* EffectWrapperOpaque::GetCameraVariable() const
{
	return m_pCameraPosition;
}
void EffectWrapperOpaque::SetCameraPosition(const dae::Vector3& newCameraPosition)
{
	float vectorPosition[3]{ newCameraPosition.x, newCameraPosition.y, newCameraPosition.z };
	m_pCameraPosition->SetFloatVector(vectorPosition);
}
void EffectWrapperOpaque::SetTextures(const Texture* pDiffuse, const Texture* pNormalMap, const Texture* pSpecular, const Texture* pGlossiness)
{
	if (m_pDiffuseMapVariable) m_pDiffuseMapVariable->SetResource(pDiffuse->GetShaderResourceView());
	if (m_pNormalMapVariable) m_pNormalMapVariable->SetResource(pNormalMap->GetShaderResourceView());
	if (m_pSpecularMapVariable) m_pSpecularMapVariable->SetResource(pSpecular->GetShaderResourceView());
	if (m_pGlossMapVariable) m_pGlossMapVariable->SetResource(pGlossiness->GetShaderResourceView());
}

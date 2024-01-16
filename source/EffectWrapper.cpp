#include "pch.h"
#include "EffectWrapper.h"


EffectWrapper::EffectWrapper(ID3D11Device* pDevice, const std::wstring& assetFile)
{
	m_pEffect = LoadEffect(pDevice, assetFile);



	m_pWorldViewProjVariable = m_pEffect->GetVariableByName("gWorldViewProj")->AsMatrix();
	if (!m_pWorldViewProjVariable->IsValid()) std::wcout << L"MatWorldViewProjVariable not valid\n";

	m_pWorldVariable = m_pEffect->GetVariableByName("gWorldMatrix")->AsMatrix();
	if (!m_pWorldVariable->IsValid()) std::wcout << L"MatWorldVariable not valid\n";


	m_pSamplerVariable = m_pEffect->GetVariableByName("gSamplerState")->AsSampler();
	if (!m_pSamplerVariable->IsValid()) std::wcout << L"Sampler variable not valid\n";


}

EffectWrapper::~EffectWrapper()
{

	m_pSamplerVariable->Release();
	m_pWorldVariable->Release();
	m_pWorldViewProjVariable->Release();
	m_pTechinque->Release();
	m_pEffect->Release();
}




//GETTERS
ID3DX11Effect* EffectWrapper::GetEffect() const
{
	return m_pEffect;
}
ID3DX11EffectTechnique* EffectWrapper::GetTechinque() const
{
	return m_pTechinque;
}
ID3DX11EffectMatrixVariable* EffectWrapper::GetWVPMatrix() const
{
	return m_pWorldViewProjVariable;
}
ID3DX11EffectSamplerVariable* EffectWrapper::GetSamplerVariable() const
{
	return m_pSamplerVariable;
}



//SETTERS
void EffectWrapper::SetMatrices(const dae::Matrix& worldViewProjMatrix, const dae::Matrix& worldMatrix)
{
	constexpr int numOfRows{ 4 };
	constexpr int numOfCols{ 4 };
	float arrayMatrix[numOfCols * numOfRows]{};
	for (int row = 0; row < numOfRows; ++row)
	{
		for (int col = 0; col < numOfRows; ++col)
		{
			arrayMatrix[col + row * numOfCols] = worldViewProjMatrix[row][col];
		}
	}
	m_pWorldViewProjVariable->SetMatrix(arrayMatrix);

	for (int row = 0; row < numOfRows; ++row)
	{
		for (int col = 0; col < numOfRows; ++col)
		{
			arrayMatrix[col + row * numOfCols] = worldMatrix[row][col];
		}
	}
	m_pWorldVariable->SetMatrix(arrayMatrix);


}

//void EffectWrapper::SetDiffuseTexture(const Texture* pDiffuse)
//{
//	if (m_pDiffuseMapVariable) m_pDiffuseMapVariable->SetResource(pDiffuse->GetShaderResourceView());
//}
void EffectWrapper::SetSamplerVariable(ID3D11SamplerState* pSamplerState)
{
	m_pSamplerVariable->SetSampler(0, pSamplerState);
}




//LOADEFFECT
ID3DX11Effect* EffectWrapper::LoadEffect(ID3D11Device* pDevice, const std::wstring& assetFile)
{
	HRESULT result;
	ID3D10Blob* pErrorBlob{ nullptr };
	ID3DX11Effect* pEffect{};

	DWORD shaderFlags{ 0 };

#if defined(DEBUG) || defined(_DEBUG)
	shaderFlags |= D3DCOMPILE_DEBUG;
	shaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	result = D3DX11CompileEffectFromFile(assetFile.c_str(), nullptr, nullptr, shaderFlags, 0, pDevice, &pEffect, &pErrorBlob);

	if (FAILED(result))
	{
		if (pErrorBlob != nullptr)
		{
			const char* pErrors = static_cast<char*>(pErrorBlob->GetBufferPointer());

			std::wstringstream ss;
			for (unsigned int i = 0; i < pErrorBlob->GetBufferSize(); i++)
			{
				ss << pErrors[i];
			}

			OutputDebugStringW(ss.str().c_str());
			pErrorBlob->Release();
			pErrorBlob = nullptr;

			std::wcout << ss.str() << std::endl;
		}
		else
		{
			std::wstringstream ss;
			ss << "EffectLoader: Failed to CreateEffectFromFile!\nPath: " << assetFile;
			std::wcout << ss.str() << std::endl;
			return nullptr;
		}
	}

	return pEffect;
}


#include "pch.h"
#include "EffectWrapper.h"

EffectWrapper::EffectWrapper(ID3D11Device* pDevice, const std::wstring& assetFile)
{
	m_pEffect = LoadEffect(pDevice, assetFile);

	m_pTechinque = m_pEffect->GetTechniqueByName("DefaultTechnique");
	if (!m_pTechinque->IsValid())
	{
		std::wcout << L"Technique not valid\n";
	}

	m_pMatWorldViewProjVariable = m_pEffect->GetVariableByName("gWorldViewProj")->AsMatrix();
	if (!m_pMatWorldViewProjVariable->IsValid())
	{
		std::wcout << L"MatWorldViewProjVariable not valid\n";
	}
}

EffectWrapper::~EffectWrapper()
{
	m_pTechinque->Release();
	m_pEffect->Release();
}

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
	return m_pMatWorldViewProjVariable;
}
void EffectWrapper::SetMatrix(const dae::Matrix& worldViewProjMatrix)
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
	m_pMatWorldViewProjVariable->SetMatrix(arrayMatrix);
}

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
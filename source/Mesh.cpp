#include "pch.h"
#include "Mesh.h"
#include "Vertex.h"
#include "Texture.h"

Mesh::Mesh(const dae::Matrix& worldMatrix) :
	m_WorldMatrix{ worldMatrix }
{

}

void Mesh::Initialize(ID3D11Device* pDevice, const std::wstring& assetFile, std::vector<Vertex> vertices, std::vector<uint32_t> indices, bool needsTransparancy)
{
	//Create Vertex Shader
	if (needsTransparancy) m_pEffect = new EffectWrapperTransparancy{ pDevice, assetFile };
	else m_pEffect = new EffectWrapperOpaque{ pDevice,assetFile };

	static constexpr uint32_t numElements{ 4 };
	D3D11_INPUT_ELEMENT_DESC vertexDesc[numElements]{};

	vertexDesc[0].SemanticName = "POSITION";
	vertexDesc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	vertexDesc[0].AlignedByteOffset = 0;
	vertexDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

	vertexDesc[1].SemanticName = "TEXTCOORD";
	vertexDesc[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	vertexDesc[1].AlignedByteOffset = 12;
	vertexDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

	vertexDesc[2].SemanticName = "NORMAL";
	vertexDesc[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	vertexDesc[2].AlignedByteOffset = 20;
	vertexDesc[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

	vertexDesc[3].SemanticName = "TANGENT";
	vertexDesc[3].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	vertexDesc[3].AlignedByteOffset = 32;
	vertexDesc[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	

	//Create Input Layout
	D3DX11_PASS_DESC passDesc{};
	m_pEffect->GetTechinque()->GetPassByIndex(0)->GetDesc(&passDesc);

	HRESULT result = pDevice->CreateInputLayout(vertexDesc, numElements, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &m_pInputLayout);
	if (FAILED(result)) return;


	//Create vertex buffers
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = sizeof(Vertex) * static_cast<uint32_t>(vertices.size());
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData{};
	initData.pSysMem = vertices.data();

	result = pDevice->CreateBuffer(&bd, &initData, &m_pVertexBuffer);
	if (FAILED(result)) return;


	//Create index buffer
	m_NumIndices = static_cast<uint32_t>(indices.size());
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = sizeof(uint32_t) * m_NumIndices;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	initData.pSysMem = indices.data();
	result = pDevice->CreateBuffer(&bd, &initData, &m_pIndexBuffer);
	if (FAILED(result)) return;


	result = m_pEffect->GetSamplerVariable()->GetSampler(0, &m_pSamplerState);
	if (FAILED(result)) return;

}
Mesh::~Mesh()
{
	delete m_pEffect;
	m_pSamplerState->Release();
	m_pIndexBuffer->Release();
	m_pVertexBuffer->Release();
	m_pInputLayout->Release();
	if(m_pGlossTexture != nullptr) delete m_pGlossTexture;
	if(m_pSpecularTexture != nullptr) delete m_pSpecularTexture;
	if(m_pNormalTexture != nullptr) delete m_pNormalTexture;
	if(m_pDiffuseTexture != nullptr) delete m_pDiffuseTexture;
}

void Mesh::Render(ID3D11DeviceContext* pDeviceContext) const
{
	
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	pDeviceContext->IASetInputLayout(m_pInputLayout);

	constexpr UINT stride = sizeof(Vertex);
	constexpr UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	
	D3DX11_TECHNIQUE_DESC techDesc{};
	m_pEffect->GetTechinque()->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		m_pEffect->GetTechinque()->GetPassByIndex(p)->Apply(0, pDeviceContext);
		pDeviceContext->DrawIndexed(m_NumIndices, 0, 0);
	}

}




//SETTERS
void Mesh::SetEffectMatrices(const dae::Matrix& worldViewProjMatrix, const dae::Matrix& worldMatrix) const
{
	m_pEffect->SetMatrices(worldViewProjMatrix, worldMatrix);
}
void Mesh::SetCameraPosition(const dae::Vector3& newCameraPosition) const
{
	m_pEffect->SetCameraPosition(newCameraPosition);
}

void Mesh::SetSampler(ID3D11Device* pDevice, D3D11_FILTER filter)
{
	D3D11_SAMPLER_DESC samplerDesc{};
	m_pSamplerState->GetDesc(&samplerDesc);

	samplerDesc.Filter = filter;

	m_pSamplerState->Release();
	HRESULT result = pDevice->CreateSamplerState(&samplerDesc, &m_pSamplerState);
	if (result != S_OK)
	{
		std::wcout << L"Sampler not correctly created in Mesh class" << std::endl;
		return;
	}

	m_pEffect->SetSamplerVariable(m_pSamplerState);
}
void Mesh::SetTextures(Texture* pDiffuse, Texture* pNormalMap, Texture* pSpecular, Texture* pGlossiness)
{
	m_pDiffuseTexture = pDiffuse;
	m_pNormalTexture = pNormalMap;
	m_pSpecularTexture = pSpecular;
	m_pGlossTexture = pGlossiness;
	m_pEffect->SetTextures(pDiffuse, pNormalMap, pSpecular, pGlossiness);
	
}
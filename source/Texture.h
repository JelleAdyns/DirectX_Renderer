#pragma once

#include <string>


struct Vector2;

class Texture final
{
public:
	Texture() = default;
	~Texture();

	Texture(const Texture&) = delete;
	Texture(Texture&&) noexcept = delete;
	Texture& operator=(const Texture&) = delete;
	Texture& operator=(Texture&&) noexcept = delete;

	HRESULT LoadFromFile(const std::string& path, ID3D11Device* pDevice);

	ID3D11ShaderResourceView* GetShaderResourceView() const { return m_pSRV; };
private:

	ID3D11Texture2D* m_pResource{nullptr};
	ID3D11ShaderResourceView* m_pSRV{ nullptr };
};

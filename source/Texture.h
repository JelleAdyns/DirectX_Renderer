#pragma once

#include <string>


struct Vector2;

class Texture final
{
public:
	explicit Texture(const std::string& path, ID3D11Device* pDevice);
	~Texture();

	Texture(const Texture&) = delete;
	Texture(Texture&&) noexcept = delete;
	Texture& operator=(const Texture&) = delete;
	Texture& operator=(Texture&&) noexcept = delete;
	ID3D11ShaderResourceView* GetShaderResourceView() const { return m_pSRV; };
private:

	ID3D11Texture2D* m_pResource;
	ID3D11ShaderResourceView* m_pSRV;
};

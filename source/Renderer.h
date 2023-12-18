#pragma once
#include "Mesh.h"
#include "Camera.h"
struct SDL_Window;
struct SDL_Surface;
class Texture;

namespace dae
{
	class Renderer final
	{
	public:
		Renderer(SDL_Window* pWindow);
		~Renderer();

		Renderer(const Renderer&) = delete;
		Renderer(Renderer&&) noexcept = delete;
		Renderer& operator=(const Renderer&) = delete;
		Renderer& operator=(Renderer&&) noexcept = delete;

		void Update(const Timer* pTimer);
		void Render() const;

		void SwitchFilterMethod()
		{

			switch (m_FilterMode)
			{
			case Filtermode::Point:

				m_FilterMode = Filtermode::Linear;
				break;
			case Filtermode::Linear:
				
				m_FilterMode = Filtermode::Anisotropic;
				break;
			case Filtermode::Anisotropic:
				m_FilterMode = Filtermode::Point;
				break;
			}
		}

		void SetSamplerState()
		{

			switch (m_FilterMode)
			{
			case Filtermode::Point:
				std::cout << "FILTERMODE: Point" << std::endl;
				m_Mesh.SetSampler(m_pDevice, D3D11_FILTER_MIN_MAG_MIP_POINT);
				break;
			case Filtermode::Linear:
				std::cout << "FILTERMODE: Linear" << std::endl;
				m_Mesh.SetSampler(m_pDevice, D3D11_FILTER_MIN_MAG_MIP_LINEAR);
				break;
			case Filtermode::Anisotropic:
				std::cout << "FILTERMODE: Anisotropic" << std::endl;
				m_Mesh.SetSampler(m_pDevice, D3D11_FILTER_ANISOTROPIC);
				break;
			}
		
		}

	private:

		enum class Filtermode
		{
			Point,
			Linear,
			Anisotropic
		};
		Filtermode m_FilterMode{ Filtermode::Point };

		SDL_Window* m_pWindow{};
		

		Camera m_Camera{};
		Mesh m_Mesh{};

		int m_Width{};
		int m_Height{};

		bool m_IsInitialized{ false };

		Texture* m_pTexture{ nullptr };
		//DIRECTX
		HRESULT InitializeDirectX();

		ID3D11Device* m_pDevice{};
		ID3D11DeviceContext* m_pDeviceContext{};
		IDXGISwapChain* m_pSwapChain{};
		ID3D11Texture2D* m_pDepthStencilBuffer{};
		ID3D11DepthStencilView* m_pDepthStencilView{};
		ID3D11Resource* m_pRenderTargetBuffer{};
		ID3D11RenderTargetView* m_pRenderTargetView{};
		//...
	};
}

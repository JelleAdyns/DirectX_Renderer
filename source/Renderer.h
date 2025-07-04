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

		void ToggleRotation() 
		{ 
			m_Rotate = !m_Rotate;
			std::cout << std::boolalpha << "ROTATION: " << m_Rotate << std::endl;
		}
		void ToggleNormals()
		{ 
			m_UseNormalMaps = !m_UseNormalMaps; 
			m_pVehicle->SetUseNormalMaps(m_UseNormalMaps);
			std::cout << std::boolalpha << "NORMAL MAPS: " << m_UseNormalMaps << std::endl;
		}
		void ToggleFire() 
		{
			m_ShowFire = !m_ShowFire; 
			std::cout << std::boolalpha << "FIRE SHOWN: " << m_ShowFire << std::endl;
		}
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
				m_pVehicle->SetSampler(m_pDevice, D3D11_FILTER_MIN_MAG_MIP_POINT);
				m_pFire->SetSampler(m_pDevice, D3D11_FILTER_MIN_MAG_MIP_POINT);
				std::cout << "FILTERMODE: Point" << std::endl;
				break;

			case Filtermode::Linear:
				m_pVehicle->SetSampler(m_pDevice, D3D11_FILTER_MIN_MAG_MIP_LINEAR);
				m_pFire->SetSampler(m_pDevice, D3D11_FILTER_MIN_MAG_MIP_LINEAR);
				std::cout << "FILTERMODE: Linear" << std::endl;
				break;

			case Filtermode::Anisotropic:
				m_pVehicle->SetSampler(m_pDevice, D3D11_FILTER_ANISOTROPIC);
				m_pFire->SetSampler(m_pDevice, D3D11_FILTER_ANISOTROPIC);
				std::cout << "FILTERMODE: Anisotropic" << std::endl;
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
		//std::vector<Mesh*> m_pVecMeshes{};
		Mesh* m_pVehicle;
		Mesh* m_pFire;
		int m_Width{};
		int m_Height{};

		bool m_IsInitialized{ false };

		float m_MeshRotation{};
		Matrix m_MeshWorldMatrix{};

		bool m_Rotate{ true };
		bool m_UseNormalMaps{ true };
		bool m_ShowFire{ true };

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

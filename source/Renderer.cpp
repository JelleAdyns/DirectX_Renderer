#include "pch.h"
#include "Renderer.h"
#include "Texture.h"
#include "Utils.h"

namespace dae {

	Renderer::Renderer(SDL_Window* pWindow) :
		m_pWindow{pWindow},
		m_MeshWorldMatrix{}
	{
		
		//Initialize
		SDL_GetWindowSize(pWindow, &m_Width, &m_Height);

		//Initialize DirectX pipeline
		const HRESULT result = InitializeDirectX();
		if (result == S_OK)
		{
			m_IsInitialized = true;
			std::wcout << L"DirectX is initialized and ready!\n";
		}
		else
		{
			std::wcout << L"DirectX initialization failed!\n";
		}
		/*std::vector<Vertex> vertices
		{
			{{0.f,0.5f,0.5f},{1.f,0.f,0.f}},
			{{0.5f,-0.5f,0.5f},{0.f, 0.f, 1.f}},
			{{-0.5f,-0.5f,0.5f},{0.f,1.f,0.f}}
		};*/

		/*std::vector<Vertex> vertices
		{
			{{0,3,2},{1.f,0.f,0.f}},
			{{3, -3, 2},{0.f, 0.f, 1.f}},
			{{-3, -3, 2},{0.f,1.f,0.f}}
		};*/
		/*std::vector<Vertex> vertices
		{
			Vertex{{-3,  3, -2},{1.f,1.f,1.f},{0.f,0.f}},
				Vertex{{ 0,  3, -2},{1.f,1.f,1.f},{ 0.5f, 0.f}},
				Vertex{{ 3,  3, -2},{1.f,1.f,1.f},{ 1, 0.f}},
				Vertex{{-3,  0, -2},{1.f,1.f,1.f},{0.f, 0.5f}},
				Vertex{{ 0,  0, -2},{1.f,1.f,1.f},{0.5f, 0.5f}},
				Vertex{{ 3,  0, -2},{1.f,1.f,1.f},{1, 0.5f}},
				Vertex{{-3, -3, -2},{1.f,1.f,1.f},{0.f, 1.f}},
				Vertex{{ 0, -3, -2},{1.f,1.f,1.f},{0.5f, 1.f}},
				Vertex{{ 3, -3, -2},{1.f,1.f,1.f},{1.f,1.f}}
		};
		std::vector<uint32_t> indices{ 3, 0, 1,    1, 4, 3,    4, 1, 2,
				2, 5, 4,    6, 3, 4,    4, 7, 6,
				7, 4, 5,    5, 8, 7 };*/


		

		//VEHICLE
		
		Mesh* vehicle{ new Mesh{ m_MeshWorldMatrix } };
		
		std::vector<Vertex> vertices{};
		std::vector<uint32_t> indices{};
		Utils::ParseOBJ("Resources/vehicle.obj", vertices, indices);
		vehicle->Initialize(m_pDevice, L"Resources/OpaqueShader.fx", vertices, indices);

		Texture* pDiffuse{ new Texture{} };
		Texture* pNormals{ new Texture{} };
		Texture* pSpecular{ new Texture{} };
		Texture* pGlossiness{ new Texture{} };
		pDiffuse->LoadFromFile("Resources/vehicle_diffuse.png", m_pDevice);
		pNormals->LoadFromFile("Resources/vehicle_normal.png", m_pDevice);
		pSpecular->LoadFromFile("Resources/vehicle_specular.png", m_pDevice);
		pGlossiness->LoadFromFile("Resources/vehicle_gloss.png", m_pDevice);

		vehicle->SetTextures(pDiffuse, pNormals, pSpecular, pGlossiness);

		m_pVehicle = vehicle;

		//FIRE
		Mesh* fire{ new Mesh{ m_MeshWorldMatrix} };
		
		vertices.clear();
		indices.clear();
		Utils::ParseOBJ("Resources/fireFX.obj", vertices, indices);
		fire->Initialize(m_pDevice, L"Resources/TransparancyShader.fx", vertices, indices, true);

		Texture* pDiffuseFlame{ new Texture{} };
		pDiffuseFlame->LoadFromFile("Resources/fireFX_diffuse.png", m_pDevice);
		fire->SetTextures(pDiffuseFlame);
		
		m_pFire = fire;

		m_Camera.Initialize(m_Width, m_Height, 45.f, { 0.f,0.f,-50.f });
		//m_pDeviceContext->GenerateMips(m_pTexture->GetShaderResourceView());
	}

	Renderer::~Renderer()
	{
		m_pRenderTargetView->Release();
		m_pRenderTargetBuffer->Release();
		m_pDepthStencilView->Release();
		m_pDepthStencilBuffer->Release();
		m_pSwapChain->Release();
		
		if (m_pDeviceContext)
		{
			m_pDeviceContext->ClearState();
			m_pDeviceContext->Flush();
			m_pDeviceContext->Release();
		}

		m_pDevice->Release();

		delete m_pVehicle;
		delete m_pFire;

	}

	void Renderer::Update(const Timer* pTimer)
	{
		m_Camera.Update(pTimer);
		if (m_Rotate)
		{
			m_MeshRotation += 45 * pTimer->GetElapsed() * TO_RADIANS;
			m_MeshWorldMatrix = Matrix::CreateRotationY(m_MeshRotation);
			m_pVehicle->SetWorldMatrix(m_MeshWorldMatrix);
			m_pFire->SetWorldMatrix(m_MeshWorldMatrix);
		}
	
	}


	void Renderer::Render() const
	{
		if (!m_IsInitialized)
			return;

		//1. Clear RTV & DSV

		static constexpr float color[4] = { 0.39f,0.59f,0.93f,1.f };
		m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, color);
		m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

		//2. Set Pipeline and Invoke Draw Calls
		Matrix worldViewProjectionMatrix{ m_MeshWorldMatrix * m_Camera.GetViewMatrix() * m_Camera.GetProjectionMatrix()};

		m_pVehicle->SetCameraPosition(m_Camera.GetCameraOrigin());
		m_pVehicle->SetEffectMatrices(worldViewProjectionMatrix, m_pVehicle->GetWorldMatrix());
		m_pVehicle->Render(m_pDeviceContext);

		if (m_ShowFire)
		{
			m_pFire->SetCameraPosition(m_Camera.GetCameraOrigin());
			m_pFire->SetEffectMatrices(worldViewProjectionMatrix, m_pFire->GetWorldMatrix());
			m_pFire->Render(m_pDeviceContext);
		}
		
		
		//3. Present Backbuffer
		m_pSwapChain->Present(0, 0);
	}

	HRESULT Renderer::InitializeDirectX()
	{
		//1. Create Device and DeviceContext
		D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_1;
		uint32_t createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
		HRESULT result = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, 0, createDeviceFlags, &featureLevel, 1, D3D11_SDK_VERSION, &m_pDevice, nullptr, &m_pDeviceContext);
		if (FAILED(result)) return result;

		//Create DXGI Factory
		IDXGIFactory1* pDxgiFactory{};
		result = CreateDXGIFactory1(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&pDxgiFactory));
		if (FAILED(result)) return result;

		//2. Create Swapchain
		DXGI_SWAP_CHAIN_DESC swapChainDesc{};
		swapChainDesc.BufferDesc.Width = m_Width;
		swapChainDesc.BufferDesc.Height = m_Height;
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 1;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 60;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = 1;
		swapChainDesc.Windowed = true;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		swapChainDesc.Flags = 0;


		// Get the handle to the Window from the SDL backbuffer
		SDL_SysWMinfo sysWMInfo{};
		SDL_GetVersion(&sysWMInfo.version);
		SDL_GetWindowWMInfo(m_pWindow, &sysWMInfo);
		swapChainDesc.OutputWindow = sysWMInfo.info.win.window;

		//Create Swapchain
		result = pDxgiFactory->CreateSwapChain(m_pDevice, &swapChainDesc, &m_pSwapChain);
		pDxgiFactory->Release();
		if (FAILED(result)) return result;


		//3. Create DepthStencil (DS) & DepthStencilView (DSV)

		//Resource
		D3D11_TEXTURE2D_DESC depthStencilDesc{};
		depthStencilDesc.Width = m_Width;
		depthStencilDesc.Height = m_Height;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.CPUAccessFlags = 0;
		depthStencilDesc.MiscFlags = 0;

		//View
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc{};
		depthStencilViewDesc.Format = depthStencilDesc.Format;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		result = m_pDevice->CreateTexture2D(&depthStencilDesc, nullptr, &m_pDepthStencilBuffer);
		if (FAILED(result)) return result;

		result = m_pDevice->CreateDepthStencilView(m_pDepthStencilBuffer, &depthStencilViewDesc, &m_pDepthStencilView);
		if (FAILED(result)) return result;


		//4. Create RanderTarget (RT) & RenerTargetView (RTV)

		//Resource
		result = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&m_pRenderTargetBuffer));
		if (FAILED(result)) return result;

		//View
		result = m_pDevice->CreateRenderTargetView(m_pRenderTargetBuffer,nullptr, &m_pRenderTargetView);
		if (FAILED(result)) return result;


		//5. Bind RTV & DSV to Output Merger Stage
		m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);


		//6. Set Viewport
		D3D11_VIEWPORT viewport{};
		viewport.Width = static_cast<float>(m_Width);
		viewport.Height = static_cast<float>(m_Height);
		viewport.TopLeftX = 0.f;
		viewport.TopLeftY = 0.f;
		viewport.MinDepth = 0.f;
		viewport.MaxDepth = 1.f;
		m_pDeviceContext->RSSetViewports(1, &viewport);


		
		return result;
	}
}

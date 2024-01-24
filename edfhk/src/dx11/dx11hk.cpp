#include <dx11/dx11hk.h>
#include <dx11/shaders/compile.h>

#include <game/game_structs.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

void drawMenu();

DX11Hk::DX11Hk()
{
}

DX11Hk::~DX11Hk()
{
	Shutdown();
}

void DX11Hk::Run(HMODULE hModule, HWND hWnd)
{
	this->hModule = hModule;
	this->hWnd = hWnd;

	if (InitializeHooks() != HookInitResult::Success)
	{
		MessageBox(hWnd, L"Failed to initialize hooks.", L"Fatal Error", MB_ICONERROR);
		return;
	}

	std::cout << "Hooks initialized." << std::endl;

	do
	{
		Sleep(100);
		if (GetAsyncKeyState(VK_INSERT) & 1)
		{
			ToggleMenu();
		}
	} while (!(GetAsyncKeyState(VK_END) & 1));
}

void DX11Hk::Shutdown()
{
	if (pDevice != nullptr)
	{
		pDevice->Release();
	}

	if (pDeviceContext != nullptr)
	{
		pDeviceContext->Release();
	}

	if (pSwapChain != nullptr)
	{
		pSwapChain->Release();
	}

	MH_DisableHook(MH_ALL_HOOKS);
	MH_Uninitialize();
}

HookInitResult DX11Hk::InitializeHooks()
{
	D3D_FEATURE_LEVEL levels[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1 };
	D3D_FEATURE_LEVEL obtainedLevel;
	DXGI_SWAP_CHAIN_DESC sd;
	{
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferCount = 1;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

		sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		sd.OutputWindow = hWnd;
		sd.SampleDesc.Count = 1;
		sd.Windowed = ((GetWindowLongPtr(hWnd, GWL_STYLE) & WS_POPUP) != 0) ? false : true;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		sd.BufferDesc.Width = 1;
		sd.BufferDesc.Height = 1;
		sd.BufferDesc.RefreshRate.Numerator = 0;
		sd.BufferDesc.RefreshRate.Denominator = 1;
	}

	HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, levels, sizeof(levels) / sizeof(D3D_FEATURE_LEVEL), D3D11_SDK_VERSION, &sd, &pSwapChain, &pDevice, &obtainedLevel, &pDeviceContext);
	if (FAILED(hr))
	{
		MessageBox(hWnd, L"Failed to create device and swapchain.", L"Fatal Error", MB_ICONERROR);
		throw std::runtime_error("Failed to create device and swapchain");
	}

	pSwapChainVTable = (DWORD_PTR*)(pSwapChain);
	pSwapChainVTable = (DWORD_PTR*)(pSwapChainVTable[0]);

	pDeviceVTable = (DWORD_PTR*)(pDevice);
	pDeviceVTable = (DWORD_PTR*)pDeviceVTable[0];

	pDeviceContextVTable = (DWORD_PTR*)(pDeviceContext);
	pDeviceContextVTable = (DWORD_PTR*)(pDeviceContextVTable[0]);

	if (MH_Initialize() != MH_OK)
	{
		return HookInitResult::HookCreationFailed;
	}

	if (auto res = MH_CreateHook((DWORD_PTR*)pSwapChainVTable[8], (LPVOID)PresentHk, reinterpret_cast<void**>(&pPresentHk)); res != MH_OK)
	{
		const char* errorString = MH_StatusToString(res);
		return HookInitResult::HookCreationFailed;
	}

	if (MH_EnableHook((DWORD_PTR*)pSwapChainVTable[8]) != MH_OK)
	{
		return HookInitResult::EnableHookFailed;
	}

	if (auto res = MH_CreateHook((DWORD_PTR*)pDeviceContextVTable[12], (LPVOID)DrawIndexedHk, reinterpret_cast<void**>(&pDrawIndexedHk)); res != MH_OK) {
		return HookInitResult::HookCreationFailed;
	}

	if (MH_EnableHook((DWORD_PTR*)pDeviceContextVTable[12]) != MH_OK) {
		return HookInitResult::EnableHookFailed;
	}

	//if (MH_CreateHook((DWORD_PTR*)pDeviceVTable[24], hookD3D11CreateQuery, reinterpret_cast<void**>(&phookD3D11CreateQuery)) != MH_OK) {
	//	return MH_ERROR;
	//}

	//if (MH_EnableHook((DWORD_PTR*)pDeviceVTable[24]) != MH_OK) {
	//	return MH_ERROR;
	//}

	//if (MH_CreateHook((DWORD_PTR*)pDeviceContextVTable[8], hookD3D11PSSetShaderResources, reinterpret_cast<void**>(&phookD3D11PSSetShaderResources)) != MH_OK) {
	//	return MH_ERROR;
	//}

	//if (MH_EnableHook((DWORD_PTR*)pDeviceContextVTable[8]) != MH_OK) {
	//	return MH_ERROR;
	//}

	//if (MH_CreateHook((DWORD_PTR*)pSwapChainVTable[50], DX11Hk::pClearRenderTargetViewHk, reinterpret_cast<void**>(&pClearRenderTargetViewHk)) != MH_OK) {
	//	return false;
	//}

	//if (MH_EnableHook((DWORD_PTR*)pSwapChainVTable[50]) != MH_OK) {
	//	return false;
	//}

	DWORD old_protect;
	VirtualProtect(pPresentHk, 2, PAGE_EXECUTE_READWRITE, &old_protect);

	return HookInitResult::Success;
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT APIENTRY WndProcHk(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	auto& dx11hk = DX11Hk::GetInstance();

	if (dx11hk.IsMenuEnabled())
	{
		ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
		return TRUE;
	}

	return CallWindowProc(dx11hk.oWndProc, hWnd, uMsg, wParam, lParam);
}

HRESULT __stdcall PresentHk(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	auto& hook = DX11Hk::GetInstance();

	std::call_once(hook.isInitialized, [&]() {
		// get device
		pSwapChain->GetDevice(__uuidof(hook.pDevice), reinterpret_cast<void**>(&hook.pDevice));

		// get context
		hook.pDevice->GetImmediateContext(&hook.pDeviceContext);

		// create a render target view using the first back buffer of the swap chain
		ID3D11Texture2D* backBuffer = nullptr;
		pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
		hook.pDevice->CreateRenderTargetView(backBuffer, nullptr, &hook.pRenderTargetView);
		backBuffer->Release();

		// set our wndproc
		hook.oWndProc = (WNDPROC)SetWindowLongPtr(hook.hWnd, GWLP_WNDPROC, (__int3264)(LONG_PTR)WndProcHk);

		// initialize imgui
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantTextInput || ImGui::GetIO().WantCaptureKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

		ImGui_ImplWin32_Init(hook.hWnd);
		ImGui_ImplDX11_Init(hook.pDevice, hook.pDeviceContext);
		ImGui_ImplDX11_CreateDeviceObjects();

		std::cout << "Imgui initialized." << std::endl;

		ImGui::StyleColorsDark();

		});

	if (hook.IsMenuEnabled())
	{
		drawMenu();
	}

	return hook.pPresentHk(pSwapChain, SyncInterval, Flags);
}

ID3D11PixelShader* LoadCustomToonShader(ID3D11Device* pDevice)
{
	ID3D11PixelShader* customShader = nullptr;

	// Load the compiled shader object (CSO)
	std::ifstream shaderFile("toon.cso", std::ios::binary | std::ios::ate);

	if (!shaderFile.is_open()) {
		// Handle the error (e.g., shader file not found)
		return nullptr;
	}

	// Get the file size
	std::streamsize fileSize = shaderFile.tellg();
	shaderFile.seekg(0);

	// Read the entire CSO into a buffer
	std::vector<char> shaderBytes(static_cast<size_t>(fileSize));
	shaderFile.read(shaderBytes.data(), fileSize);

	// Create the pixel shader from the CSO
	HRESULT hr = pDevice->CreatePixelShader(shaderBytes.data(), shaderBytes.size(), nullptr, &customShader);

	if (FAILED(hr)) {
		// Handle shader creation error
		return nullptr;
	}

	return customShader;
}

//#include <charconv>
//
//ID3D11PixelShader* LoadCustomToonShader(ID3D11Device* pDevice)
//{
//	ID3D11PixelShader* customShader = nullptr;
//
//	try {
//		// Open and read the shader file
//		std::ifstream shaderFile("toon.hlsl", std::ios::ate | std::ios::binary);
//		if (!shaderFile.is_open()) {
//			std::cerr << "Failed to open the file: toon.hlsl" << std::endl;
//			return nullptr;
//		}
//
//		// Get the file size
//		std::streamsize fileSize = shaderFile.tellg();
//		shaderFile.seekg(0);
//
//		// Read the entire file into a string
//		std::string shaderHlsl(static_cast<std::size_t>(fileSize), ' ');
//		shaderFile.read(shaderHlsl.data(), fileSize);
//
//		// Rest of your code for shader compilation
//	}
//	catch (const std::exception& e) {
//		std::cerr << "Exception: " << e.what() << std::endl;
//		return nullptr;
//	}
//
//	return customShader;
//}

//ID3D11PixelShader* LoadCustomToonShader(ID3D11Device* pDevice)
//{
//	ID3D11PixelShader* customShader = nullptr;
//
//	// read shader file and compile bytecode
//	std::ifstream shaderFile("toon.hlsl");
//	std::string shaderHlsl((std::istreambuf_iterator<char>(shaderFile)),
//		std::istreambuf_iterator<char>());
//
//	auto toonShader = shaders::compile::CompileShader(pDevice, shaderHlsl.c_str());
//
//	if (toonShader == nullptr)
//	{
//		return nullptr;
//	}
//
//	return customShader;
//}

void __stdcall DrawIndexedHk(ID3D11DeviceContext* pContext, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation)
{
	auto& hook = DX11Hk::GetInstance();

	// TODO: remember to release this when done with it
	static ID3D11PixelShader* customToonShader = nullptr; // LoadCustomToonShader(hook.pDevice);

	if (customToonShader)
	{
		pContext->PSSetShader(customToonShader, nullptr, 0);

		hook.pDrawIndexedHk(pContext, IndexCount, StartIndexLocation, BaseVertexLocation);

		pContext->PSSetShader(nullptr, nullptr, 0);
	}
	else
	{
		hook.pDrawIndexedHk(pContext, IndexCount, StartIndexLocation, BaseVertexLocation);
	}
}

void DX11Hk::ToggleMenu()
{
	showMenu = !showMenu;
}

void drawMenu()
{
	static auto& io = ImGui::GetIO();

	io.DisplaySize.x = 1920.0f;
	io.DisplaySize.y = 1080.0f;

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();

	// HP
	auto base = (_Start*)((uintptr_t)DX11Hk::GetInstance().hModule + 0x125ab80);
	auto player = base->TeamManager_ptr->UnkGame3_ptr->UnkGame4_ptr->UnkGame5_ptr->UnkGame6_ptr->UnkGame7_ptr->GameObject_ptr;
	auto weapon = player->ChildrenGameObjs->PlayerWeapon1_ptr;

	ImGui::NewFrame();

	ImGui::SetWindowSize(ImVec2(400, 300), ImGuiCond_Always);

	ImGui::Begin("EDF, EDF!");

	ImGui::Text("hp:\t\t\t%f\nelapsed:\t\t%f\nx: %f\ty: %f\tz: %f\nammo:\t\t%d\n", player->HealthPoints, player->SecondsElapsed, player->PlayerX, player->PlayerY, player->PlayerZ, weapon->CurrentAmmo);

	ImGui::NewLine();

	static bool freezeBooster = false;
	ImGui::Checkbox("freeze booster charge", &freezeBooster);

	if (freezeBooster)
	{
		player->BoosterCharge = player->MaxBoosterCharge;
	}

	ImGui::End();

	ImGui::EndFrame();

	ImGui::Render();

	auto& hook = DX11Hk::GetInstance();

	hook.pDeviceContext->OMSetRenderTargets(1, &hook.pRenderTargetView, nullptr);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}


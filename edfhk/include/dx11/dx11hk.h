#pragma once

using D3D11PresentHk = HRESULT(__stdcall*)(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
using D3D11DrawIndexedHk = void(__stdcall*) (ID3D11DeviceContext* pContext, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation);
using D3D11CreateQueryHk = void(__stdcall*)(ID3D11Device* pDevice, const D3D11_QUERY_DESC* pQueryDesc, ID3D11Query** ppQuery);
using D3D11PSSetShaderResourcesHk = void(__stdcall*)(ID3D11DeviceContext* pContext, UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView* const* ppShaderResourceViews);
using D3D11ClearRenderTargetViewHk = void(__stdcall*) (ID3D11DeviceContext* pContext, ID3D11RenderTargetView* pRenderTargetView, const FLOAT ColorRGBA[4]);

HRESULT __stdcall PresentHk(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
void __stdcall DrawIndexedHk(ID3D11DeviceContext *pContext, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation);

LRESULT APIENTRY WndProcHk(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

enum class HookInitResult {
	Success,
	HookCreationFailed,
	EnableHookFailed
};

class DX11Hk
{
public:
	static DX11Hk& GetInstance()
	{
		static DX11Hk instance;
		return instance;
	}

	DX11Hk(const DX11Hk&) = delete; // disable copy constructor
	DX11Hk& operator=(const DX11Hk&) = delete; // disable copy assignment

	void Run(HMODULE hModule, HWND hWnd);
	bool IsMenuEnabled()
	{
		return showMenu;
	}

	HMODULE hModule;
	HWND hWnd;
	WNDPROC oWndProc;

	ID3D11Device* pDevice;
	ID3D11DeviceContext* pDeviceContext;
	IDXGISwapChain* pSwapChain;
	ID3D11RenderTargetView* pRenderTargetView;
	D3D11PresentHk pPresentHk;
	D3D11DrawIndexedHk pDrawIndexedHk;
	D3D11CreateQueryHk pCreateQueryHk;
	D3D11PSSetShaderResourcesHk	pPSSetShaderResourcesHk;
	D3D11ClearRenderTargetViewHk pClearRenderTargetViewHk;

	std::once_flag isInitialized;

private:
	DX11Hk();
	~DX11Hk();

	void Shutdown();

	DWORD_PTR* pSwapChainVTable;
	DWORD_PTR* pDeviceVTable;
	DWORD_PTR* pDeviceContextVTable;

	bool showMenu;

	HookInitResult InitializeHooks();
	void ToggleMenu();
};


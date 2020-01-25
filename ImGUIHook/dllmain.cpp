#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <iostream>
#include <fstream>
#include <detours.h>
#include "Hook.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx9.h"
#include "ImGui/imgui_impl_win32.h"
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "d3d9.lib")
using namespace std;

const char* windowName = "Zula"; // Process Title or Game Title || Ex: Counter-Strike: Global Offensive - Zula - ImGui DirectX9 Example - Test D3D9 window - D3D TEST ENVIROMENT

void InputHandler()// Mouse Ayarları Falan Fiso
{
	for (int i = 0; i < 5; i++) ImGui::GetIO().MouseDown[i] = false;

	int button = -1;
	if (GetAsyncKeyState(VK_LBUTTON)) button = 0;

	if (button != -1) ImGui::GetIO().MouseDown[button] = true;
}

HRESULT WINAPI hkPresent(LPDIRECT3DDEVICE9 pDevice, const RECT *a, const RECT *b, HWND c, const RGNDATA *d)
{
	__asm PUSHAD;
	if (GetAsyncKeyState(VK_INSERT) & 1) Menu = !Menu;
	static bool init = true;
	if (init)
	{
		init = false;
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		ImGui_ImplWin32_Init(FindWindowA(NULL, windowName));
		ImGui_ImplDX9_Init(pDevice);
		TargetWnd = FindWindow(0, windowName);
		HDC_Desktop = GetDC(TargetWnd);
	}

	static float f = 0.0f;
	static int counter = 0;
	static int switchTabs;

	static float zulaespcolor = RGB(0, 0, -255);
	static float gladioespcolor = RGB(0, 0, 0);

	static const char* key[]{ "Ctrl","Shift","Right Click" };
	static const char* aimp[]{ "Head","Body" };
	static const char* team[]{ "Zula", "Gladio" };

	static int selectedK = 0;
	static int selectedP = 0;
	static int selectedT = 0;

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	if (Menu == true)
	{
		InputHandler();
		ImGui::SetNextWindowSize(ImVec2(320.0f, 220.0f)); // Genislik, Yükseklik
		ImGui::Begin("Lithe's Zula Vip Hack", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings);

		if (ImGui::Button("Visual", ImVec2(100.0f, 0.0f)))
			switchTabs = 0;
		ImGui::SameLine(0.0, 2.0f);
		if (ImGui::Button("Misc", ImVec2(100.0f, 0.0f)))
			switchTabs = 1;
		ImGui::SameLine(0.0, 2.0f);
		if (ImGui::Button("Aimbot###Button", ImVec2(100.0f, 0.0f)))
			switchTabs = 2;

		switch (switchTabs) {
		case 0:
			ImGui::Checkbox("Chams", &chams);
			ImGui::Text("   Zula Color: Blue", &zulaespcolor);
			ImGui::Text("   Gladio Color: Red", &gladioespcolor);
			ImGui::Checkbox("Esp Box", &espbox);
			ImGui::Text("   Zula Color: Blue", &zulaespcolor);
			ImGui::Text("   Gladio Color: Red", &gladioespcolor);
			ImGui::Checkbox("Esp Line", &espline);
			break;
		case 1:
			ImGui::Checkbox("No Smoke", &nosmoke);
			ImGui::Checkbox("No Flash", &noflash);
			ImGui::Checkbox("No Recoil", &norecoil);
			ImGui::Checkbox("No Spread", &nospread);
			ImGui::Checkbox("Fast Reload", &fastreload);
			ImGui::Checkbox("Fast Switch", &fastswitch);
			ImGui::Checkbox("Rapid Fire", &rapidfire);
			break;
		case 2:
			ImGui::Checkbox("Aimbot", &aimbot);
			ImGui::Combo("Aim Key", &selectedK, key, IM_ARRAYSIZE(key));
			ImGui::Combo("Aim Position", &selectedP, aimp, IM_ARRAYSIZE(aimp));
			ImGui::Combo("Aim Team", &selectedT, team, IM_ARRAYSIZE(team));
			break;
		}
		ImGui::End();
	}
	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
	__asm POPAD;
	return oPresent(pDevice, a, b, c, d);
}


HRESULT WINAPI hkEndScene(LPDIRECT3DDEVICE9 pDevice)
{
	//hacks:
	return oEndScene(pDevice);
}

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

HRESULT WINAPI hkDrawIndexedPrimitive(LPDIRECT3DDEVICE9 pDevice, D3DPRIMITIVETYPE PrimType, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT PrimitiveCount)
{
	__asm nop
	HRESULT hRet = oDrawIndexedPrimitive(pDevice, PrimType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, PrimitiveCount);
	//visual hacks: uiStride
	return hRet;
}

HRESULT WINAPI hkSetStreamSource(LPDIRECT3DDEVICE9 pDevice, UINT StreamNumber, IDirect3DVertexBuffer9* pStreamData, UINT OffsetInBytes, UINT Stride)
{
	__asm nop
	if (StreamNumber == 0){
		uiStride = Stride;
	}
	return oSetStreamSource(pDevice, StreamNumber, pStreamData, OffsetInBytes, Stride);
}

LRESULT CALLBACK MsgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){ return DefWindowProc(hwnd, uMsg, wParam, lParam); }
void DX_Init(DWORD* table)
{
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, "DX", NULL };
	RegisterClassEx(&wc);
	HWND hWnd = CreateWindow("DX", NULL, WS_OVERLAPPEDWINDOW, 100, 100, 300, 300, GetDesktopWindow(), NULL, wc.hInstance, NULL);
	LPDIRECT3D9 pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	LPDIRECT3DDEVICE9 pd3dDevice;
	pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pd3dDevice);
	DWORD* pVTable = (DWORD*)pd3dDevice;
	pVTable = (DWORD*)pVTable[0];
	table[ES] = pVTable[42];
	table[DIP] = pVTable[82];
	table[SSS] = pVTable[100];
	DestroyWindow(hWnd);
}

DWORD WINAPI Thread(LPVOID)
{
	DWORD VTable[3] = { 0 };
	while (GetModuleHandle("d3d9.dll") == NULL){
		Sleep(1000);
	}
	HWND  window = FindWindowA(NULL, windowName);
	oWndProc = (WNDPROC)SetWindowLongPtr(window, GWL_WNDPROC, (LONG_PTR)WndProc);
	DWORD dwVTable[2] = { 0 };
	CreateDevice(dwVTable);
	oPresent = (tPresent)DetourCreate((PBYTE) dwVTable[1], (PBYTE) &hkPresent, 5);
	DX_Init(VTable);
	HOOK(EndScene, VTable[ES]);
	HOOK(DrawIndexedPrimitive, VTable[DIP]);
	HOOK(SetStreamSource, VTable[SSS]);
	return 0;
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpvReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH){
		CreateThread(0, 0, Thread, 0, 0, 0);
	}
	return TRUE;
}

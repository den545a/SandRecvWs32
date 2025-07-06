#include "modules/other/consts.h"

#include "modules/modules.h"
#include "modules/other/misc.h"
#include "../src/modules/exploit/FakeLagLowPoint.h"
#include "../xorstr.hpp"




LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_KEYUP:
		g_ShowMenu = (wParam == VK_INSERT) ? !g_ShowMenu : g_ShowMenu;
		break;
	}

	// Передаём сообщение в ImGui
	if (g_ShowMenu and ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return -1;

	// Передаём сообщение окну игры
	return CallWindowProcW(g_WndProc, hWnd, msg, wParam, lParam);
}


bool onHook = false;




BOOL __stdcall hk_GetWindow(HWND hwnd, DWORD* pdw)
{

	if (g_WindowAffinity && g_Window == hwnd)
	{
		*pdw = WDA_NONE;
		return TRUE;
	}
	else {
		fn_GetWindowAffinity(hwnd, pdw);
	}

}

//BOOL hk_SetWindowAffnity(HWND hwnd, DWORD pdw)
//{
//	if (pdw == 0x2E)
//	{
//		return fn_SetWindowAffinity(hwnd, WDA_EXCLUDEFROMCAPTURE);
//	}
//
//	return fn_SetWindowAffinity(hwnd, WDA_NONE);
//}


//void APIENTRY Hooked_glUseProgram(GLuint program) {
//
//	if (AntiEffect && program == 33) {
//		return;
//	}
//
//	fn_UseProgram(program);

//}
bool setupOpenGLHooks()
{



	/*MH_CreateHook(SetWindowDisplayAffinity, &hk_SetWindowAffnity,
		reinterpret_cast<void**>(&fn_SetWindowAffinity));*/

	// Для подмены результата проверки видимости
	MH_CreateHook(glGetQueryObjectuiv, &hk_glGetQueryObjectuiv,
		reinterpret_cast<void**>(&glGetQueryObjectuiv));

	MH_CreateHook(glDrawElements, &hk_glDrawElements,
		reinterpret_cast<void**>(&glDrawElements));

	// Для обнаружения аномалий
	MH_CreateHook(glDrawArraysInstanced, &hk_glDrawArraysInstanced,
		reinterpret_cast<void**>(&glDrawArraysInstanced));

	MH_CreateHook(GetWindowDisplayAffinity, &hk_GetWindow,
		reinterpret_cast<void**>(&fn_GetWindowAffinity));

	return MH_EnableHook(MH_ALL_HOOKS) == MH_OK;
}

void BlockScreenshots() {
	
	if (g_WndProc) {
		// Устанавливаем свойство, чтобы заблокировать создание скриншотов
		SetWindowDisplayAffinity(g_Window, WDA_MONITOR);
		//MessageBox(NULL, L"Скриншоты заблокированы.", L"Информация", MB_OK);
	}
	else {
		//MessageBox(NULL, E(L"Не удалось получить активное окно."), E(L"Ошибка"), MB_OK);
	}
}

BOOL WINAPI hk_SwapBuffers(HDC hDC)
{
	const static int version = gladLoaderLoadGL();

	if (!version)
		return fn_SwapBuffers(hDC);

	const static bool hooked = setupOpenGLHooks();

	if (!hooked)
		return fn_SwapBuffers(hDC);

	if (!ImGui::GetCurrentContext())
	{
		IMGUI_CHECKVERSION();
		g_Window = WindowFromDC(hDC);
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		ImGui_ImplWin32_InitForOpenGL(g_Window);
		ImGui_ImplOpenGL3_Init();


		g_WndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtrW(g_Window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&WndProc)));
		
	
	}


	//BlockScreenshots();
	
	/*if (g_WindowAffinity)
	{
		SetWindowDisplayAffinity(g_Window, 0x2E);
	}
	else {
		SetWindowDisplayAffinity(g_Window, WDA_MONITOR);
	}*/
	
	if (g_ShowMenu) {
		GUI::runModule();
	}

	runModules();

	return fn_SwapBuffers(hDC);
}




BOOL APIENTRY DllMain(HINSTANCE hInstance, DWORD fdwReason, LPVOID lpReserved)
{
	
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hInstance);

		if (MH_Initialize() != MH_OK) {
			return FALSE;
		}

		RealWSASendPtr = reinterpret_cast<RealWSASend>(GetProcAddress(GetModuleHandle(E(TEXT("Ws2_32.dll"))), E("WSASend")));
		if (RealWSASendPtr == nullptr) {
			MH_Uninitialize();
			return FALSE;
		}

		if (MH_CreateHook(reinterpret_cast<LPVOID>(RealWSASendPtr), reinterpret_cast<LPVOID>(FakeLag::MyWSASendNoAllowPacket), reinterpret_cast<LPVOID*>(&RealWSASendPtr)) != MH_OK ||
			MH_CreateHook(&SwapBuffers, &hk_SwapBuffers, reinterpret_cast<void**>(&fn_SwapBuffers)) != MH_OK) {
			MH_Uninitialize();
			return FALSE;
		}

	/*	MH_CreateHook(pglUseProgram, &Hooked_glUseProgram, reinterpret_cast<void**>(&pglUseProgram));*/

		/*if (pUseProgram && MH_CreateHook(pUseProgram, &Hooked_glUseProgram, reinterpret_cast<void**>(&fn_UseProgram)) == MH_OK) {
			MH_EnableHook(pUseProgram);
		}
		else {
			MessageBoxA(NULL, "Failed to hook glUseProgram", "Error", MB_OK);
		}*/


		if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK) {
			MH_Uninitialize();
			return FALSE;
		}
		break;

	case DLL_PROCESS_DETACH:
		MH_DisableHook(MH_ALL_HOOKS);
		MH_Uninitialize();
		break;
	}

	return TRUE;
}

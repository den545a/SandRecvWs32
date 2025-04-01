#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <gl/GL.h>
#include <memory>
#include "src/object/Counts.h"
#include "MinHook.h"
#include "modules/other/consts.h"
#include <unordered_set>
#include "modules/modules.h"
#include "modules/exploit/FakeLagLowPoint.h"
#include "modules/xorstr.hpp"
#include <regex>
#include "globals.h"
#include <iostream>


LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_KEYUP:
        g_ShowMenu = (wParam == VK_INSERT) ? !g_ShowMenu : g_ShowMenu;
        break;
    }

    if (g_ShowMenu and ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return -1;

    return CallWindowProcW(g_WndProc, hWnd, msg, wParam, lParam);
}


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


bool setupOpenGLHooks()
{
    static bool initialized = false;
    if (initialized)
        return true; 

   
    MH_CreateHook(GetWindowDisplayAffinity, &hk_GetWindow,
        reinterpret_cast<void**>(&fn_GetWindowAffinity));

    bool result = MH_EnableHook(MH_ALL_HOOKS) == MH_OK;

    initialized = result; 
    return result;
}

BOOL WINAPI hk_SwapBuffers(HDC hDC)
{
    static bool openGLInitialized = false;
    static bool hooksInitialized = false;


    if (!openGLInitialized)
    {
        const int version = gladLoaderLoadGL();
        if (!version)
            return fn_SwapBuffers(hDC);
        openGLInitialized = true; 
    }


    if (!hooksInitialized)
    {
        hooksInitialized = setupOpenGLHooks();
        if (!hooksInitialized)
            return fn_SwapBuffers(hDC); 
    }


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


    if (g_ShowMenu || Reportcounter || NitifGold)
    {
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


        if (MH_CreateHook(&SwapBuffers, &hk_SwapBuffers, reinterpret_cast<void**>(&fn_SwapBuffers)) != MH_OK) {
            MH_Uninitialize();
            return FALSE;
        }

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
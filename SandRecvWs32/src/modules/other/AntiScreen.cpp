#include "AntiScreen.h"
#include <gl/GL.h>
#include <cstring>
#include "MinHook.h"
#include <mutex>  
#include <thread>
#include <chrono>

typedef void(APIENTRY* glReadPixels_t)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void* pixels);
glReadPixels_t original_glReadPixels = nullptr;


struct StateFlags {
    bool CircleESP, Bags, DrawBox2d, Case, AntiTerrain, AntiEffect, Reportcounter, g_ShowMenu, NitifGold;
};

std::mutex stateMutex;
StateFlags currentFlags, originalFlags;

void restoreStatesAfterDelay() {
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    std::lock_guard<std::mutex> lock(stateMutex);
    g_CircleESP = originalFlags.CircleESP;
    g_Bags = originalFlags.Bags;
    g_DrawBox2d = originalFlags.DrawBox2d;
    g_Case = originalFlags.Case;
    AntiTerrain = originalFlags.AntiTerrain;
    AntiEffect = originalFlags.AntiEffect;
    Reportcounter = originalFlags.Reportcounter;
    g_ShowMenu = originalFlags.g_ShowMenu;
    NitifGold = originalFlags.NitifGold;
}

void handleAntiScreen() {
    if (Antiscreen && (g_CircleESP || g_Bags || g_DrawBox2d || g_Case || AntiTerrain || AntiEffect || g_ShowMenu || NitifGold)) {
        std::lock_guard<std::mutex> lock(stateMutex);
        originalFlags = { g_CircleESP, g_Bags, g_DrawBox2d, g_Case, AntiTerrain, AntiEffect, Reportcounter, g_ShowMenu, NitifGold};
        ReportCounter++;

        g_CircleESP = false;
        g_Bags = false;
        g_DrawBox2d = false;
        g_Case = false;
        AntiTerrain = false;
        AntiEffect = false;
        Reportcounter = false;
        g_ShowMenu = false;
        NitifGold = false;

        std::thread(restoreStatesAfterDelay).detach();
    }
}

void APIENTRY hooked_glReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void* pixels) {
    handleAntiScreen();
    original_glReadPixels(x, y, width, height, format, type, pixels);
}



// Функция для работы с модулями
void AntiScreen::runModule() {
  
    HMODULE hOpenGL = GetModuleHandleA("Opengl32.dll");
    if (hOpenGL) {
        void* pglReadPixels = GetProcAddress(hOpenGL, "glReadPixels");
        if (pglReadPixels) {
            if (MH_CreateHook(pglReadPixels, &hooked_glReadPixels, reinterpret_cast<void**>(&original_glReadPixels)) == MH_OK) {
                MH_EnableHook(pglReadPixels);
            }
        }
    }
}

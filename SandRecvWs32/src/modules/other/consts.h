#pragma once

#include "includes.h"

inline LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

inline decltype(&SwapBuffers) fn_SwapBuffers = &SwapBuffers;

inline PFNGLBINDTEXTUREPROC pglBindTexture = nullptr;

typedef BOOL(__stdcall* a_GetWindow)(HWND, DWORD*);

typedef BOOL(__stdcall* a_SetWindow)(HWND, DWORD);

inline a_GetWindow fn_GetWindowAffinity;

inline a_SetWindow fn_SetWindowAffinity;





inline WNDPROC g_WndProc = nullptr;

// Õåíä?îêíà èãðû
inline HWND g_Window = nullptr;

inline bool g_ShowMenu = true;

inline int tabb = 0;

inline bool Rapidfire = false;
inline int Dcount = 0;

inline bool SandBlink = false;
inline bool RecvBlink = false;
inline bool AntiEffect = true;

static int packetCounter = 0;
inline int preactDelay = 0;
inline int postactDelay = 0;
inline bool mscount = false;
inline int timerblinkac = 0;
inline bool PulseRecv = false;
inline bool PulseSand = false;

inline bool AntiTerrain = false;

inline int ReportCounter = 0;

inline bool Reportcounter = false;

inline bool g_ESP = false;

inline bool Antiscreen = true;

inline bool g_Bags = false;

inline bool g_CircleESP = true;
inline float CircleRadius = 0.20f;
inline int CircleSegments = 30;

inline bool speedhack = false;

inline double mouseSensitivity = 0.0;

inline bool g_DrawBox2d = false;
inline float width2d = 0.7f;
inline float height2d = 1.8f;

inline int dalay = 0;

inline float crosshairSize = 20.0f;
inline float crosshairWidth = 2.0f;

inline bool g_CheckVisibility = false;
inline bool qPressed = false;
inline bool isCooldownActive = false;


inline bool Fly = false;
inline bool Fly2 = false;
inline int interval = 0;
inline int interval2 = 0;

inline bool g_3DBoxESP = false;

inline bool g_Case = false;

inline bool g_Barter = false;

inline bool NitifGold = false;
inline bool visible = false;
inline float windowHeight = 0.0f;
inline float speed = 1.0f;


inline glm::vec4 PlayerColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
inline glm::vec4 EntityColor = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
inline glm::vec4 CaseColor = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
inline glm::vec4 BarterColor = glm::vec4(0.0f, 0.5f, 0.31f, 1.0f);
inline glm::vec4 InfectedPlayer = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);
inline glm::vec4 BagsColor = glm::vec4(0.5f, 1.0f, 0.3f, 1.0f);
inline float g_MinDistance = 5.0f;
inline float g_FOV = 60.0f;
inline float aimHeight = 0.00f;
inline float angleThreshold = 0.0f;
inline bool g_FovRender = false;
inline bool g_MutantAimEnabled = false;
inline bool g_AimEnabled = true;
inline bool g_WindowAffinity = true;
inline float Y_Aim = 0;
inline int area_size = 250;
inline const float width_ratio = 1.0f;
inline const float height_ratio = 150.0f / 250.0f;
inline int area_width = static_cast<int>(area_size * width_ratio);
inline int area_height = static_cast<int>(area_size * height_ratio);

struct WSASendData {
	int totalLength;
	std::vector<std::string> hexData;
};

inline std::vector<WSASendData> g_WSASendData;
inline std::mutex g_WSASendMutex;

inline float aimHeights[] = { 0.85f, 0.60f, 0.0f };
inline int Part = 1;
inline const char* BodyPart[]{ "Head", "Neck", "Body" };


inline bool AimKeybind = false;
inline glm::vec4 viewport{};
inline bool Fakelag = false;

inline bool Headñrack = false;
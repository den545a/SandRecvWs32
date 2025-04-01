#include "modules/other/consts.h"

#include "modules/modules.h"
#include "modules/other/misc.h"
#include "../src/modules/exploit/FakeLagLowPoint.h"
#include "../xorstr.hpp"


//typedef void (APIENTRY* glUseProgram_t)(GLuint program);
//glUseProgram_t pglUseProgram = nullptr;
//
//void APIENTRY Hooked_glUseProgram(GLuint program) {
//
//
//
//	if (AntiEffect && program == 33) {
//		return;
//	}
//
//	pglUseProgram(program);
//}

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
// Перехваченная функция glGetQueryObjectuiv
// Вызывается перед отрисовкой объекта для проверки видимости
void GLAD_API_PTR hk_glGetQueryObjectuiv(GLuint id, GLenum pname, GLuint* params)
{
	glGetQueryObjectuiv(id, pname, params);

	// Подмена результата проверки видимости
	// Игроки начнут отрисовываться за преградами
	// Значительно влияет на производительность
	// https://www.khronos.org/opengl/wiki/Query_Object
	if (pname == GL_QUERY_RESULT)
		*params = 1;
}

bool onHook = false;

// Перехваченная функция glDrawElements
// Вызывается для рисования сущностей/предметов/артефактов
void WINAPI hk_glDrawElements(GLenum mode, GLsizei count, GLenum type, const void* indices)
{
	// Получаем идентификатор текущего шейдера
	GLint currentProgram = 0;
	glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);

	if (!currentProgram)
		return glDrawElements(mode, count, type, indices);

	// Получаем индекс uniform-блока persistentBlockif (keyState & 0x8000)
	// Содержит матрицы view и projection
	// Возвращает GL_INVALID_INDEX если ничего не найдено
	const GLuint blockIndex =
		glGetUniformBlockIndex(currentProgram, E("persistentBlock"));

	// Получаем местоположения матрицы модели
	// Равно -1 если ничего не найдено
	const GLint modelMatrixLocation =
		glGetUniformLocation(currentProgram, E("modelMatrix"));

	// У сущностей/предметов/артефактов они всегда присутствуют
	if (blockIndex == GL_INVALID_INDEX or modelMatrixLocation == -1)
		return glDrawElements(mode, count, type, indices);

	// У рюкзаков на земле и артефактов эта униформа отсутствует
	// Используем её для косвенного определения
	const GLint animated = glGetUniformLocation(currentProgram, E("isAnimated"));

	Object::Type objType = Object::Unknown;

	// Пытаемся определить тип объекта по count и animated
	// Равно -1 если ничего не найдено
	if (animated != -1)
	{
		// Список count которые присутствуют у игроков с разной бронёй
		static std::list<GLsizei> playerCounts
		{
			15432, 15186, 18198, 16218, 14136, 14004, //Не редкие (Одежда)
            17544, 16944, 8292, 16491, 29130, 32724, 35112, 22170, 32484, 32400, 25044, 25797, 23835, 43584, 36756, 25176, 30450, 30846, //Не редкие (Боевые)
            21138, 32262, 37728, 43083, 34089, 57834, 73656, 69486, 29028, 32607, 61398, 30972, 26346, 47232, 50592, 32214, 42909, //редкие (боевые)
            60063, 32277, 32253, 48840, 25848, 59424, 65982, 56748, 41586, 31236, //Супер редкие (боевые)
            8016, 23691, 22584, 34719, 40209, 29109, 19656, 48012, 16656, 35445, 25812, 29379, 33720, 42003, 24477, 33732, 29379, 45732, 18054, 33690, //фулл научка
            14718, 21330, 25758, 18132, 20277, 24771, 25038, 27534, 32934, 20808, 25356, 29709, //не редкие (комбинированные) 
            48930, 58368, 52116, 35886, 22584, 40287, 36534, 34431, 55167, 33558, 31080, 39777, 40161, 27450, 59433, 54072,52608,22632,52470,48435,5364,60777,48579,32400,40995,32874,34800,5123,//редкие (комбинированные)
			56430, 58245, 53673, 51066, 79590, 16393, 32715, 30780, 65535, 32937, 32874, 40996, 66396, 38628, 48579, 47040, 54552, 44418, 24147, 90996, 9258, 36543, 40287
		};

		static std::list<GLsizei> ArtefactsCounts
		{
			744, 8022, 2304, 7098, 3600, 3192, 1200, 600, 96, 3249, //электрофизические
			534, 4752, 4830, 14520, 7482, 1464, 3402, 5376, 7656, 1488, 1586, //Термические
			2376, 1338, 2268, 3012, 18036, 15822, 4920, 3174, 1674, 7608, //Гравитационные
			666, 930, 4680, 2388, 2376, 1440, 4116, 2754, //Биохимические
			9300, 7078 //Прочее
		};

		static std::list<GLsizei> entityCounts
		{
			28428,	// Снорк
			24516,	// Слепой пес
			19038,	// Тушкан
			72114,	// Псевдособака
			38058,	// Плоть
			37410,	// Кровосос
			23202,	// Кабан
			72114,	// Пси-собака
			42246,	// Химера
			64734  // Псевдогигант
			
			
			
		};

		// Проверяем, есть ли число count в списке playerCounts
		if (std::find(playerCounts.begin(), playerCounts.end(), count) != playerCounts.end())
			objType = Object::Player;
		else // Проверяем, есть ли число count в списке entityCounts
			if (std::find(entityCounts.begin(), entityCounts.end(), count) != entityCounts.end())
				objType = Object::Entity;
			/*else
				if (std::find(ArtefactsCounts.begin(), ArtefactsCounts.end(), count) != ArtefactsCounts.end())
					objType = Object::Artefacts;*/

	}

	// Если так и не получилось определить что это
	if (objType == Object::Unknown)
		return glDrawElements(mode, count, type, indices);

	// Получаем индекс привязки uniform-блока persistentBlock
	GLint bindIndex = 0;
	glGetActiveUniformBlockiv(currentProgram, blockIndex,
		GL_UNIFORM_BLOCK_BINDING, &bindIndex);

	// Получаем индекс буфера, к которому привязан uniform-блок
	GLint bufferIndex = 0;
	glGetIntegeri_v(GL_UNIFORM_BUFFER_BINDING, bindIndex, &bufferIndex);

	constexpr const char* names[]{ "viewMatrix", "projectionMatrix" };

	// Сохраняем текущий привязанный буфер
	int currentBuffer = 0;
	glGetIntegerv(GL_UNIFORM_BUFFER, &currentBuffer);

	// Привязываем буфер с persistentBlock
	glBindBuffer(GL_UNIFORM_BUFFER, bufferIndex);

	// Создаём временные переменные для хранения данных
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	// Получаем содержимое матриц
	// Обрати внимание на glGetBufferSubData, 2 и 3 значения заданы вручную
	// Их можно узнавать динамически, но они не менялись уже давно
	glGetUniformfv(currentProgram, modelMatrixLocation, glm::value_ptr(model));
	glGetBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(view));
	glGetBufferSubData(GL_UNIFORM_BUFFER, 64, sizeof(glm::mat4), glm::value_ptr(projection));

	// Восстанавливаем предыдущий привязанный буфер
	glBindBuffer(GL_UNIFORM_BUFFER, currentBuffer);

	// Добавляем объект в список
	g_ObjectList.emplace_back(model, view, projection, objType);

	MH_DisableHook(&glDrawElements);

	glDrawElements(mode, count, type, indices);
}

// Перехваченная функция glDrawArraysInstanced
// Вызывается для рисования аномалий
void GLAD_API_PTR hk_glDrawArraysInstanced(GLenum mode, GLint first, GLsizei count, GLsizei instancecount)
{
	glDrawArraysInstanced(mode, first, count, instancecount);
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

#include "gui.h"
#include "imgui.h"
#include <vector>
#include <string>
#include <mutex>
#include <sstream>
#include <modules/exploit/FakeLagLowPoint.h>

extern int minSendPacketSize;
extern int maxSendPacketSize;
extern int minRecvPacketSize;
extern int maxRecvPacketSize;
extern std::vector<unsigned char> sendByteFilter;
extern std::vector<unsigned char> recvByteFilter;
void CopyToClipboard(const std::string& str) {
    // Открываем буфер обмена
    if (OpenClipboard(NULL)) {
        // Очищаем буфер обмена
        EmptyClipboard();

        // Создаем глобальный объект памяти для строки
        HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, str.size() + 1);
        if (hMem) {
            // Копируем строку в память
            memcpy(GlobalLock(hMem), str.c_str(), str.size() + 1);
            GlobalUnlock(hMem);

            // Копируем данные в буфер обмена
            SetClipboardData(CF_TEXT, hMem);
        }

        // Закрываем буфер обмена
        CloseClipboard();
    }
}
void GUI::displaySendFilterMenu() {
    ImGui::Begin("Send Packet Filter", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);

    // Send Filtering controls
    ImGui::InputInt("Min Send Packet Size", &minSendPacketSize);
    ImGui::InputInt("Max Send Packet Size", &maxSendPacketSize);

    // Byte filter for Send
    static char sendByteFilterInput[128] = "";
    if (ImGui::InputText("Send Byte Filter (hex)", sendByteFilterInput, sizeof(sendByteFilterInput), ImGuiInputTextFlags_CharsHexadecimal)) {
        sendByteFilter.clear();
        for (size_t i = 0; i < strlen(sendByteFilterInput); i += 2) {
            if (i + 1 < strlen(sendByteFilterInput)) {
                unsigned char byte = (unsigned char)strtol(sendByteFilterInput + i, nullptr, 16);
                sendByteFilter.push_back(byte);
            }
        }
    }

    ImGui::End();
}

void GUI::displayCopiedPacketsMenu() {
    ImGui::Begin("Copied Packets", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);

    std::lock_guard<std::mutex> lock(logMutex);
    for (size_t i = 0; i < copiedPacketLogs.size(); i++) {
        ImGui::TextUnformatted(copiedPacketLogs[i].c_str());
    }

    ImGui::End();
}

void GUI::displayRecvFilterMenu() {
    ImGui::Begin("Recv Packet Filter", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);

    // Recv Filtering controls
    ImGui::InputInt("Min Recv Packet Size", &minRecvPacketSize);
    ImGui::InputInt("Max Recv Packet Size", &maxRecvPacketSize);

    // Byte filter for Recv
    static char recvByteFilterInput[128] = "";
    if (ImGui::InputText("Recv Byte Filter (hex)", recvByteFilterInput, sizeof(recvByteFilterInput), ImGuiInputTextFlags_CharsHexadecimal)) {
        recvByteFilter.clear();
        for (size_t i = 0; i < strlen(recvByteFilterInput); i += 2) {
            if (i + 1 < strlen(recvByteFilterInput)) {
                unsigned char byte = (unsigned char)strtol(recvByteFilterInput + i, nullptr, 16);
                recvByteFilter.push_back(byte);
            }
        }
    }

    ImGui::End();
}

void GUI::displaySendMenu() {
    ImGui::Begin("Packet Send Log", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);

    std::lock_guard<std::mutex> lock(logMutex);
    for (size_t i = 0; i < packetLogs.size(); i++) {
        if (packetLogs[i].find("[OUT]") != std::string::npos) {
            ImGui::TextUnformatted(packetLogs[i].c_str());

            // Добавляем кнопку для копирования пакета
            if (ImGui::Button(("Copy Packet " + std::to_string(i)).c_str())) {
                // Копируем данный пакет в буфер обмена
                CopyToClipboard(packetLogs[i]);
            }
        }
    }

    ImGui::End();
}

void GUI::displayRecvMenu() {
    ImGui::Begin("Packet Receive Log", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);

    std::lock_guard<std::mutex> lock(logMutex);
    for (size_t i = 0; i < packetLogs.size(); i++) {
        if (packetLogs[i].find("[IN]") != std::string::npos) {
            ImGui::TextUnformatted(packetLogs[i].c_str());

            if (ImGui::Button(("Copy Packet " + std::to_string(i)).c_str())) {
                CopyToClipboard(packetLogs[i]);
            }
        }
    }

    ImGui::End();
}

void GUI::runModule() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    if (g_ShowMenu) { // Убедитесь, что g_ShowMenu определен, если это необходимо
        // Показываем фильтры и логи
        displaySendFilterMenu();  // Send filter menu
        displayRecvFilterMenu();  // Recv filter menu

        // Показываем меню пакетов и меню скопированных пакетов
        displaySendMenu();    // Send menu
        displayRecvMenu();    // Receive menu
        displayCopiedPacketsMenu(); // Меню для отображения скопированных пакетов
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

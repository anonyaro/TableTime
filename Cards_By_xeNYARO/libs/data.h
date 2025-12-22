#pragma once
#include "libs.h"

bool isDragging = false;
POINT dragStart;
RECT clientRect;
WNDPROC OldButtonProc;
HBITMAP hBackground = NULL;
HICON bICON;
static bool vsyncEnabled = true;


wchar_t* db1Path = _wcsdup(L"main.db");
wchar_t* db2Path = _wcsdup(L"sub.db");
wchar_t currentDir[MAX_PATH];
wchar_t path[MAX_PATH] = {};

HWND dono;
HWND play;
HWND upd;
wchar_t g_ButtonText[64] = L"НАЧАТЬ-ИГРУ";
wchar_t savedTexts[BUTTON_COUNT][256] = {};  
bool areCleared = false;               
struct ButtonInfo {
    HWND hwnd;
    int x, y, w, h;
};


ButtonInfo buttonInfos[BUTTON_COUNT];
unsigned int currentRefreshRate = 60;
//...

struct buttons {
    //AUTHOR
    HWND card1;
    HWND card2;
    HWND card3;
    HWND card4;
    HWND card5;

    //PLAYERS
    HWND card6;
    HWND card7;
    HWND card8;
    HWND card9;
    HWND card10;

    //MAIN
    HWND card11;
    HWND card12;
    HWND card13;
    HWND card14;
    HWND card15;

	//Descriptors ctext
    const wchar_t* descriptor_btn_style[10] = { L"BUTTON", L"TableTime by xeNYARO", L"https://www.donationalerts.com/r/xenyaro", L"https://t.me/t3plc6x", L"Ожидание...", L"Поддержать меNYARO", L"♣\n  ♥\n  ♦", L"TableTime by xeNYARO {FPS: %d | Frequency -> %.2f m/s | CPU: %.2f%% | RAM: %zu MB}" };

    const wchar_t* null = nullptr;
    //valstogen


}datas;
HWND author[5] = { nullptr };
HWND player[5] = { nullptr }; 

buttons* p2data = &datas;

unsigned int btncollision()
{
    unsigned static int count{};
    unsigned static counter = ++count + 50;
    unsigned int total_collision = MAINCARD_Y + STATICARD_Y / 2;
    return counter + total_collision;
}

void st2b(const std::vector<HWND>& buttons, const wchar_t* text) {
    for (HWND hwnd : buttons) {
        SetWindowTextW(hwnd, text);
    }
}
void MakeButtonRounded(HWND hwnd, int radius)
{
    RECT rect;
    GetWindowRect(hwnd, &rect);
    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;

    HRGN hRgn = CreateRoundRectRgn(0, 0, width, height, radius, radius);
    SetWindowRgn(hwnd, hRgn, TRUE);
}
unsigned int screenWidth = GetSystemMetrics(SM_CXSCREEN);
unsigned int screenHeight = GetSystemMetrics(SM_CYSCREEN);

unsigned int x = (screenWidth - FHX) / 2;
unsigned int y = (screenHeight - FHY) / 2;

RECT wr = { 0, 0, FHX, FHY };
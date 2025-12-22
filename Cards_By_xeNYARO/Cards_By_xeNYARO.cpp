// Cards_By_xeNYARO.cpp 
// by anonyaro aka xenyaro pet-project game for cards collection and duels with friends simply guess the next word by meaning read readme.md for more info
// hiding whole screen when showing cards and hiding them when needed from window capturing programs and applications such as OBS studio, Zoom, Discord and others

#include "pch.h" //precompiled header
#include "data.h" //data struct and global variables
#include "Cards_By_xeNYARO.h" //main header nothing there but resource includes
#include "words.h" //words and everything related to them

#include <psapi.h>      //  GetProcessMemoryInfo / RAM
#include <tlhelp32.h>   //  CPU usage

#define MAX_LOADSTRING 100
HINSTANCE hInst;                              
WCHAR szTitle[MAX_LOADSTRING];                 
WCHAR szWindowClass[MAX_LOADSTRING];            

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    AboutDlgProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK ChildDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_CARDSBYXENYARO, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CARDSBYXENYARO));

    MSG msg;

    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}


INT_PTR CALLBACK ChildDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    static HWND hwnd = nullptr;
    switch (message) {
    case WM_INITDIALOG:
        hwnd = (HWND)lParam;
   
        SetDlgItemText(hDlg, ID_DB1_PATH, db1Path);
        SetDlgItemText(hDlg, ID_DB2_PATH, db2Path);     
        InvalidateRect(GetDlgItem(hDlg, ID_INDICATOR_VSYNC), NULL, TRUE); // обновим индикатор
        return TRUE;

    case WM_DRAWITEM: {
        LPDRAWITEMSTRUCT pDIS = (LPDRAWITEMSTRUCT)lParam;
        HBRUSH brush = nullptr;

        switch (wParam) {
        case ID_INDICATOR_VSYNC:
            brush = CreateSolidBrush(vsyncEnabled ? RGB(0, 255, 0) : RGB(128, 128, 128));
            break;
        case ID_INDICATOR_DB1:
            brush = CreateSolidBrush(GetFileAttributesW(db1Path) != INVALID_FILE_ATTRIBUTES ? RGB(0, 255, 0) : RGB(255, 0, 0));
            break;
        case ID_INDICATOR_DB2:
            brush = CreateSolidBrush(GetFileAttributesW(db2Path) != INVALID_FILE_ATTRIBUTES ? RGB(0, 255, 0) : RGB(255, 0, 0));
            break;
        }

        if (brush) {
            FillRect(pDIS->hDC, &pDIS->rcItem, brush);
            DeleteObject(brush);
            return TRUE;
        }
        break;
    }

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case ID_TOGGLE_VSYNC:
            vsyncEnabled = !vsyncEnabled;
            if (vsyncEnabled) {
                SetTimer(hwnd, FPS_TIMER_ID, 1000 / currentRefreshRate, NULL);
            }
            else {
                KillTimer(hwnd, FPS_TIMER_ID);
                SetWindowText(hwnd, p2data->descriptor_btn_style[1]); 
            }
            InvalidateRect(GetDlgItem(hDlg, ID_INDICATOR_VSYNC), NULL, TRUE);

            break;

        case ID_SELECT_DB1: {
            GetCurrentDirectoryW(MAX_PATH, currentDir);  
            OPENFILENAME ofn = { sizeof(ofn) };
            ofn.hwndOwner = hDlg;
            ofn.lpstrFilter = L"DB Files (*.db)\0*.db\0";
            ofn.lpstrFile = path;
            ofn.nMaxFile = MAX_PATH;
            ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

            if (GetOpenFileName(&ofn)) {
                if (db1Path) free(db1Path);             
                db1Path = _wcsdup(path);                
                SetDlgItemText(hDlg, ID_DB1_PATH, db1Path);
                InvalidateRect(GetDlgItem(hDlg, ID_INDICATOR_DB1), NULL, TRUE);
            }
            SetCurrentDirectoryW(currentDir);  
            break;
        }

        case ID_SELECT_DB2: {
            GetCurrentDirectoryW(MAX_PATH, currentDir); 

            OPENFILENAME ofn = { sizeof(ofn) };
            ofn.hwndOwner = hDlg;
            ofn.lpstrFilter = L"DB Files (*.db)\0*.db\0";
            ofn.lpstrFile = path;
            ofn.nMaxFile = MAX_PATH;
            ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

            if (GetOpenFileName(&ofn)) {
                if (db2Path) free(db2Path);             
                db2Path = _wcsdup(path);               
                SetDlgItemText(hDlg, ID_DB2_PATH, db2Path);
                InvalidateRect(GetDlgItem(hDlg, ID_INDICATOR_DB2), NULL, TRUE);
            }
            SetCurrentDirectoryW(currentDir); 
            break;
        }

        case SAVE:
			// we can also save the state here e.g (vsyncEnabled, db1Path, db2Path) currently not needed

            EndDialog(hDlg, SAVE);
            return TRUE;

        case IDCANCEL:
            EndDialog(hDlg, 0);
            return TRUE;
        }
        break;

    case WM_CLOSE:
        EndDialog(hDlg, 0);
        return TRUE;
    }
    return FALSE;
}
    

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(MAIN_ICO));
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(MAIN_ICO));
    wcex.hCursor        = LoadCursor(nullptr, IDC_HAND);

    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_CARDSBYXENYARO);
    wcex.lpszClassName  = szWindowClass;
    wcex.hbrBackground = CreatePatternBrush(LoadBitmap(hInstance, MAKEINTRESOURCE(MBG)));
    wcex.style |= CS_DBLCLKS;


    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; 

  
   AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

   HWND hWnd = CreateWindowW(szWindowClass, szTitle,
       WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, x, y,
       wr.right - wr.left,
       wr.bottom - wr.top,
       NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}


LRESULT CALLBACK ButtonProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_LBUTTONDOWN: {
        isDragging = true;
        SetCapture(hwnd);

        dragStart.x = LOWORD(lParam);
        dragStart.y = HIWORD(lParam);
        return 0;
    }
    case WM_MOUSEMOVE:
        if (isDragging) {
            HWND hParent = GetParent(hwnd);

			// 1. saving old btn pos. in client coords
            RECT oldRect;
            GetWindowRect(hwnd, &oldRect);
            ScreenToClient(hParent, (LPPOINT)&oldRect.left);
            ScreenToClient(hParent, (LPPOINT)&oldRect.right);

            // 2. getting mouse pos.
            POINT pt;
            GetCursorPos(&pt);
            ScreenToClient(hParent, &pt);

            int newX = pt.x - dragStart.x;
            int newY = pt.y - dragStart.y;

            RECT client;
            GetClientRect(hParent, &client);

            int width = oldRect.right - oldRect.left;
            int height = oldRect.bottom - oldRect.top;

			// window restrictions
            if (newX < 0) newX = 0;
            if (newY < 0) newY = 0;
            if (newX + width > client.right) newX = client.right - width;
            if (newY + height > client.bottom) newY = client.bottom - height;

			// 3. btn movement
            MoveWindow(hwnd, newX, newY, width, height, TRUE);

            // 4. btn redrawing only
            InvalidateRect(hwnd, NULL, TRUE);

            return 0;
        }
        break;

    case WM_LBUTTONUP:
        if (isDragging) {
            isDragging = false;
            ReleaseCapture();
            return 0;
        }
        break;
    }


    return CallWindowProc(OldButtonProc, hwnd, msg, wParam, lParam);
}

//CREATING BUTTONS FOR DRAGGING
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE: {
        bICON = (HICON)LoadImage(
            hInst,
            MAKEINTRESOURCE(BTN_ICO),
            IMAGE_ICON,
            250, 150,             // preferred 32x32, 64x64, but depends on ico size
            LR_DEFAULTCOLOR | LR_CREATEDIBSECTION
        );

        if (!bICON) {
            MessageBox(hwnd, L"Не удалось загрузить иконку", L"Ошибка", MB_ICONERROR);
        }
        hBackground = (HBITMAP)LoadImage(
            hInst,
            MAKEINTRESOURCE(MBG),
            IMAGE_BITMAP,
            0, 0,
            LR_CREATEDIBSECTION
        );
        if (!hBackground) {
            MessageBox(hwnd, L"Не удалось загрузить фоновый ресурс", L"Ошибка", MB_ICONERROR);
        }
        vsyncEnabled = true;
		SetTimer(hwnd, FPS_TIMER_ID, 1000 / currentRefreshRate, NULL);  // freq already set

        //CUSTOMS
        dono = CreateWindow(p2data->descriptor_btn_style[0], nullptr,
            WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_OWNERDRAW,
            525,910, MAINCARD_X/2+100, MAINCARD_Y/2+10,
            hwnd, (HMENU)CD, ((LPCREATESTRUCT)lParam)->hInstance, nullptr);
        ShowWindow(dono, SW_SHOW); // dono btn
        MakeButtonRounded(dono, 60);
        SetWindowRgn(dono,
            CreateRoundRectRgn(0, 0, MAINCARD_X / 2 + 20, MAINCARD_Y / 2 + 20, 60, 60),
            TRUE
        );

        play = CreateWindow(p2data->descriptor_btn_style[0], nullptr,
            WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_OWNERDRAW,
            780, 950, MAINCARD_X / 2 + 250, MAINCARD_Y / 3,
            hwnd, (HMENU)PLAY, ((LPCREATESTRUCT)lParam)->hInstance, nullptr);

        upd = CreateWindow(p2data->descriptor_btn_style[0], p2data->descriptor_btn_style[6],
            WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            1180, 910, MAINCARD_X / 4, MAINCARD_Y / 2 + 10,
            hwnd, (HMENU)UPD, ((LPCREATESTRUCT)lParam)->hInstance, nullptr);


        //MAINCARDS_STATIC
        p2data->card11 = CreateWindow(p2data->descriptor_btn_style[0], p2data->descriptor_btn_style[4],
            WS_VISIBLE | WS_CHILD | BS_CENTER | BS_VCENTER,
            STATICARD_X, STATICARD_Y, MAINCARD_X, MAINCARD_Y,
            hwnd, (HMENU)CARD11, ((LPCREATESTRUCT)lParam)->hInstance, nullptr);

        //MAINS
        p2data->card12 = CreateWindow(p2data->descriptor_btn_style[0], p2data->descriptor_btn_style[4],
            WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            STATICARD_X, btncollision(), MAINCARD_X, MAINCARD_Y,
            hwnd, (HMENU)CARD12, ((LPCREATESTRUCT)lParam)->hInstance, nullptr);


        p2data->card13 = CreateWindow(p2data->descriptor_btn_style[0], p2data->descriptor_btn_style[4],
            WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            STATICARD_X, btncollision() + (STATICARD_Y * 3) + STATICARD_Y / 2, MAINCARD_X, MAINCARD_Y,
            hwnd, (HMENU)CARD13, ((LPCREATESTRUCT)lParam)->hInstance, nullptr);


        p2data->card14 = CreateWindow(p2data->descriptor_btn_style[0], p2data->descriptor_btn_style[4],
            WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            STATICARD_X, btncollision() + (STATICARD_Y * 7), MAINCARD_X, MAINCARD_Y,
            hwnd, (HMENU)CARD14, ((LPCREATESTRUCT)lParam)->hInstance, nullptr);


        p2data->card15 = CreateWindow(p2data->descriptor_btn_style[0], p2data->descriptor_btn_style[4],
            WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            STATICARD_X, btncollision() + (STATICARD_Y * 8) + (STATICARD_Y * 5) / 2, MAINCARD_X, MAINCARD_Y,
            hwnd, (HMENU)CARD15, ((LPCREATESTRUCT)lParam)->hInstance, nullptr);


        //AUTHORCARDS -> PLAYERCARDS
        p2data->card1 = CreateWindow(p2data->descriptor_btn_style[0], p2data->descriptor_btn_style[4],
            WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            PLAYERCARD_X, PLAYERCARD_Y, MAINCARD_X, MAINCARD_Y,
            hwnd, (HMENU)nullptr, ((LPCREATESTRUCT)lParam)->hInstance, nullptr);

        GetClientRect(hwnd, &clientRect);

        OldButtonProc = (WNDPROC)SetWindowLongPtr(p2data->card1, GWLP_WNDPROC, (LONG_PTR)ButtonProc);

        p2data->card2 = CreateWindow(p2data->descriptor_btn_style[0], p2data->descriptor_btn_style[4],
            WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            PLAYERCARD_X, PLAYERCARD_Y, MAINCARD_X, MAINCARD_Y,
            hwnd, (HMENU)nullptr, ((LPCREATESTRUCT)lParam)->hInstance, nullptr);

        GetClientRect(hwnd, &clientRect);

        OldButtonProc = (WNDPROC)SetWindowLongPtr(p2data->card2, GWLP_WNDPROC, (LONG_PTR)ButtonProc);

        p2data->card3 = CreateWindow(p2data->descriptor_btn_style[0], p2data->descriptor_btn_style[4],
            WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            PLAYERCARD_X, PLAYERCARD_Y, MAINCARD_X, MAINCARD_Y,
            hwnd, (HMENU)nullptr, ((LPCREATESTRUCT)lParam)->hInstance, nullptr);

        GetClientRect(hwnd, &clientRect);

        OldButtonProc = (WNDPROC)SetWindowLongPtr(p2data->card3, GWLP_WNDPROC, (LONG_PTR)ButtonProc);

        p2data->card4 = CreateWindow(p2data->descriptor_btn_style[0], p2data->descriptor_btn_style[4],
            WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            PLAYERCARD_X, PLAYERCARD_Y, MAINCARD_X, MAINCARD_Y,
            hwnd, (HMENU)nullptr, ((LPCREATESTRUCT)lParam)->hInstance, nullptr);

        GetClientRect(hwnd, &clientRect);

        OldButtonProc = (WNDPROC)SetWindowLongPtr(p2data->card4, GWLP_WNDPROC, (LONG_PTR)ButtonProc);


        p2data->card5 = CreateWindow(p2data->descriptor_btn_style[0], p2data->descriptor_btn_style[4],
            WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            PLAYERCARD_X, PLAYERCARD_Y, MAINCARD_X, MAINCARD_Y,
            hwnd, (HMENU)nullptr, ((LPCREATESTRUCT)lParam)->hInstance, nullptr);

        GetClientRect(hwnd, &clientRect);

        OldButtonProc = (WNDPROC)SetWindowLongPtr(p2data->card5, GWLP_WNDPROC, (LONG_PTR)ButtonProc);


        //PLAYERCARDS -> AUTHORCARDS
        p2data->card6 = CreateWindow(p2data->descriptor_btn_style[0], p2data->descriptor_btn_style[4],
            WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            AUTHORCARD_X, AUTHOR_Y, MAINCARD_X, MAINCARD_Y,
            hwnd, (HMENU)BASE_BUTTON_ID, ((LPCREATESTRUCT)lParam)->hInstance, nullptr);

        GetClientRect(hwnd, &clientRect);

        OldButtonProc = (WNDPROC)SetWindowLongPtr(p2data->card6, GWLP_WNDPROC, (LONG_PTR)ButtonProc);

        p2data->card7 = CreateWindow(p2data->descriptor_btn_style[0], p2data->descriptor_btn_style[4],
            WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            AUTHORCARD_X, AUTHOR_Y, MAINCARD_X, MAINCARD_Y,
            hwnd, (HMENU)BUTTON2_ID, ((LPCREATESTRUCT)lParam)->hInstance, nullptr);

        GetClientRect(hwnd, &clientRect);

        OldButtonProc = (WNDPROC)SetWindowLongPtr(p2data->card7, GWLP_WNDPROC, (LONG_PTR)ButtonProc);

        p2data->card8 = CreateWindow(p2data->descriptor_btn_style[0], p2data->descriptor_btn_style[4],
            WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            AUTHORCARD_X, AUTHOR_Y, MAINCARD_X, MAINCARD_Y,
            hwnd, (HMENU)BUTTON3_ID, ((LPCREATESTRUCT)lParam)->hInstance, nullptr);

        GetClientRect(hwnd, &clientRect);

        OldButtonProc = (WNDPROC)SetWindowLongPtr(p2data->card8, GWLP_WNDPROC, (LONG_PTR)ButtonProc);

        p2data->card9 = CreateWindow(p2data->descriptor_btn_style[0], p2data->descriptor_btn_style[4],
            WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            AUTHORCARD_X, AUTHOR_Y, MAINCARD_X, MAINCARD_Y,
            hwnd, (HMENU)BUTTON4_ID, ((LPCREATESTRUCT)lParam)->hInstance, nullptr);

        GetClientRect(hwnd, &clientRect);

        OldButtonProc = (WNDPROC)SetWindowLongPtr(p2data->card9, GWLP_WNDPROC, (LONG_PTR)ButtonProc);

        //btn creation
        p2data->card10 = CreateWindow(p2data->descriptor_btn_style[0], p2data->descriptor_btn_style[4],
            WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            AUTHORCARD_X, AUTHOR_Y, MAINCARD_X, MAINCARD_Y,
            hwnd, (HMENU)BUTTON5_ID, ((LPCREATESTRUCT)lParam)->hInstance, nullptr);

		// sizeof client rect
        GetClientRect(hwnd, &clientRect);

        // new window process for btn
        OldButtonProc = (WNDPROC)SetWindowLongPtr(p2data->card10, GWLP_WNDPROC, (LONG_PTR)ButtonProc);

        break;
    }

case WM_TIMER:
if (vsyncEnabled && wParam == FPS_TIMER_ID) {
    // ===== FPS and frequency =====
    LARGE_INTEGER freq, start, end;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);
    Sleep(1);
    QueryPerformanceCounter(&end);
    double responseMs = (end.QuadPart - start.QuadPart) * 1000.0 / freq.QuadPart;

    DEVMODE devMode = { 0 };
    devMode.dmSize = sizeof(DEVMODE);
    EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devMode);
    int refreshRate = devMode.dmDisplayFrequency ? devMode.dmDisplayFrequency : 60;

    // ===== CPU Usage =====
    static ULONGLONG lastSysTotal = 0;
    static ULONGLONG lastProcTotal = 0;

    FILETIME fSysIdle, fSysKernel, fSysUser;
    FILETIME fProcCreation, fProcExit, fProcKernel, fProcUser;

    HANDLE hProcess = GetCurrentProcess();

    // getting current time
    GetSystemTimes(&fSysIdle, &fSysKernel, &fSysUser);
    GetProcessTimes(hProcess, &fProcCreation, &fProcExit, &fProcKernel, &fProcUser);

    // Conversion FILETIME to ULARGE_INTEGER
    ULARGE_INTEGER sysIdle, sysKernel, sysUser;
    ULARGE_INTEGER procKernel, procUser;

    memcpy(&sysIdle, &fSysIdle, sizeof(FILETIME));
    memcpy(&sysKernel, &fSysKernel, sizeof(FILETIME));
    memcpy(&sysUser, &fSysUser, sizeof(FILETIME));

    memcpy(&procKernel, &fProcKernel, sizeof(FILETIME));
    memcpy(&procUser, &fProcUser, sizeof(FILETIME));

	// counting sum system and process times
    ULONGLONG sysTotal = (sysKernel.QuadPart + sysUser.QuadPart) - sysIdle.QuadPart;
    ULONGLONG procTotal = procKernel.QuadPart + procUser.QuadPart;

    // calculating delta and CPU load
    ULONGLONG sysDelta = sysTotal - lastSysTotal;
    ULONGLONG procDelta = procTotal - lastProcTotal;

    double cpuPercent = 0.0;
    if (sysDelta != 0)
        cpuPercent = (double)procDelta * 100.0 / sysDelta;

    lastSysTotal = sysTotal;
    lastProcTotal = procTotal;

    // ===== RAM Usage =====
    PROCESS_MEMORY_COUNTERS_EX pmc = {};
    SIZE_T memUsageMB = 0;

    if (GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc))) {
        memUsageMB = pmc.PrivateUsage / (1024 * 1024);  // private mem in MB
    }

    // ===== ServerPanel update =====
    wchar_t title[256];
    swprintf(title, 256, p2data->descriptor_btn_style[7], refreshRate, responseMs, cpuPercent, memUsageMB);
    SetWindowText(hwnd, title);
}
break;
case WM_DRAWITEM:
{
    LPDRAWITEMSTRUCT pDIS = (LPDRAWITEMSTRUCT)lParam;

    if (pDIS->CtlID == CD) // ID 
    {
        // custom bg
        HBRUSH hBrush = CreateSolidBrush(RGB(24, 22, 32)); //  bg
        FillRgn(pDIS->hDC, CreateRoundRectRgn(0, 0, pDIS->rcItem.right, pDIS->rcItem.bottom, 20, 20), hBrush);
        DeleteObject(hBrush);

		// custom text
        SetBkMode(pDIS->hDC, TRANSPARENT);
        SetTextColor(pDIS->hDC, RGB(255, 32, 255)); //  text
        DrawTextW(pDIS->hDC, p2data->descriptor_btn_style[5], -1, &pDIS->rcItem, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

        return TRUE;
    }
    if (pDIS->CtlID == PLAY) // ID 
    {
        // custom bg
        HBRUSH hBrush = CreateSolidBrush(RGB(24, 22, 32)); //  bg
        FillRgn(pDIS->hDC, CreateRoundRectRgn(0, 0, pDIS->rcItem.right, pDIS->rcItem.bottom, 20, 20), hBrush);
        DeleteObject(hBrush);

        // custom text
        SetBkMode(pDIS->hDC, TRANSPARENT);
		SetTextColor(pDIS->hDC, RGB(255, 0, 0)); // red text
        DrawTextW(pDIS->hDC, g_ButtonText, -1, &pDIS->rcItem, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

        return TRUE;
    }
}
break;

    case WM_SETCURSOR:
        SetCursor(LoadCursor(NULL, IDC_HAND)); // cursor styling
        return TRUE;
    //Redraw check
    case WM_SIZE:
        GetClientRect(hwnd, &clientRect);
        break;

    case WM_DESTROY:
        if (hBackground) {
            DeleteObject(hBackground);
        }
        DestroyIcon(bICON);
        KillTimer(hwnd, FPS_TIMER_ID);
        PostQuitMessage(0);
        break;

        //MENU
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case CD:
        {
            ShellExecute(NULL, L"open", p2data->descriptor_btn_style[2], NULL, NULL, SW_SHOWNORMAL);
        }
        break;
        case PLAY:
        {
            // manually clearing previous words 
			FreeWordArrays(); 
      
            HWND playerCards[] = {
            p2data->card1,
            p2data->card2,
            p2data->card3,
            p2data->card4,
            p2data->card5
            };

            HWND authorCards[] = {
            p2data->card6,
            p2data->card7,
            p2data->card8,
            p2data->card9,
            p2data->card10
            };

            for (HWND h : playerCards) {
            SetWindowPos(h, nullptr,
            PLAYERCARD_X, PLAYERCARD_Y,
            MAINCARD_X, MAINCARD_Y,
            SWP_NOZORDER | SWP_NOACTIVATE);
            }
    
            for (HWND h : authorCards) {
            SetWindowPos(h, nullptr,
            AUTHORCARD_X, AUTHOR_Y,
            MAINCARD_X, MAINCARD_Y,
            SWP_NOZORDER | SWP_NOACTIVATE);
            }
            
			/* above is cleaner but whatever use this if having issues they are about the same
            SetWindowPos(p2data->card1, nullptr, PLAYERCARD_X, PLAYERCARD_Y, MAINCARD_X, MAINCARD_Y, SWP_NOZORDER | SWP_NOACTIVATE);
            SetWindowPos(p2data->card2, nullptr, PLAYERCARD_X, PLAYERCARD_Y, MAINCARD_X, MAINCARD_Y, SWP_NOZORDER | SWP_NOACTIVATE);
            SetWindowPos(p2data->card3, nullptr, PLAYERCARD_X, PLAYERCARD_Y, MAINCARD_X, MAINCARD_Y, SWP_NOZORDER | SWP_NOACTIVATE);
            SetWindowPos(p2data->card4, nullptr, PLAYERCARD_X, PLAYERCARD_Y, MAINCARD_X, MAINCARD_Y, SWP_NOZORDER | SWP_NOACTIVATE);
            SetWindowPos(p2data->card5, nullptr, PLAYERCARD_X, PLAYERCARD_Y, MAINCARD_X, MAINCARD_Y, SWP_NOZORDER | SWP_NOACTIVATE);

            SetWindowPos(p2data->card6, nullptr, AUTHORCARD_X, AUTHOR_Y, MAINCARD_X, MAINCARD_Y, SWP_NOZORDER | SWP_NOACTIVATE);
            SetWindowPos(p2data->card7, nullptr, AUTHORCARD_X, AUTHOR_Y, MAINCARD_X, MAINCARD_Y, SWP_NOZORDER | SWP_NOACTIVATE);
            SetWindowPos(p2data->card8, nullptr, AUTHORCARD_X, AUTHOR_Y, MAINCARD_X, MAINCARD_Y, SWP_NOZORDER | SWP_NOACTIVATE);
            SetWindowPos(p2data->card9, nullptr, AUTHORCARD_X, AUTHOR_Y, MAINCARD_X, MAINCARD_Y, SWP_NOZORDER | SWP_NOACTIVATE);
            SetWindowPos(p2data->card10, nullptr, AUTHORCARD_X, AUTHOR_Y, MAINCARD_X, MAINCARD_Y, SWP_NOZORDER | SWP_NOACTIVATE);
            */


            if (LoadWordsFromTwoFiles(db1Path,db2Path))
            {
				//shuffling words 
                //better sample below
               /*
                std::array<HWND, 5> playerCards = {
                    p2data->card1, p2data->card2, p2data->card3,
                    p2data->card4, p2data->card5
                };

                for (auto h : playerCards) {
                    SetWindowPos(h, nullptr,
                        PLAYERCARD_X, PLAYERCARD_Y,
                        MAINCARD_X, MAINCARD_Y,
                        SWP_NOZORDER | SWP_NOACTIVATE);
                }
                */           
                /* above is cleaner but whatever use this if having issues they are about the same */   
            
                //A&P
                st2b({ p2data->card1, p2data->card6 }, shuffledWords[0]);
                st2b({ p2data->card2, p2data->card7 }, shuffledWords[1]);
                st2b({ p2data->card3, p2data->card8 }, shuffledWords[2]);
                st2b({ p2data->card4, p2data->card9 }, shuffledWords[3]);
                st2b({ p2data->card5, p2data->card10 }, shuffledWords[4]);

                //MN
                SetWindowTextW(p2data->card11, originalWords[0]);
                SetWindowTextW(p2data->card12, originalWords[1]);
                SetWindowTextW(p2data->card13, originalWords[2]);
                SetWindowTextW(p2data->card14, originalWords[3]);
                SetWindowTextW(p2data->card15, originalWords[4]);
                */
                for (int i = 0; i < BUTTON_COUNT; i++)
                {
                    HWND hBtn = GetDlgItem(hwnd, BASE_BUTTON_ID + i);
                    GetWindowTextW(hBtn, savedTexts[i], 64);
                }
                areCleared = false; // active texts nothing to hide/hidden
            }
            wcscpy_s(g_ButtonText, L"ПЕРЕЗАПУСТИТЬ"); //restart game btn 
            InvalidateRect(play, NULL, TRUE); // hwndButton 
            UpdateWindow(play);
        }      
        SetWindowDisplayAffinity(hwnd, WDA_EXCLUDEFROMCAPTURE);
        break;
        case UPD:
        {
            SetWindowDisplayAffinity(hwnd, WDA_NONE);
            for (int i = 0; i < BUTTON_COUNT; i++)
            {
                HWND hBtn = GetDlgItem(hwnd, BASE_BUTTON_ID + i);

                if (!areCleared)
                {
                    // saving current visible state
                    wchar_t buf[64] = {};
                    GetWindowTextW(hBtn, buf, 64);

                    if (wcslen(buf) > 0)
                        wcscpy_s(savedTexts[i], 64, buf);

                    SetWindowTextW(hBtn, L""); // clearing text
                }
                else
                {
                    SetWindowTextW(hBtn, savedTexts[i]); // restoring
                }
            }

            areCleared = !areCleared; // changing flag in the end
            return 0;
        }   
        break;
        case ID_ACCEL_HIDE:
            SetWindowDisplayAffinity(hwnd, WDA_EXCLUDEFROMCAPTURE);
            break;
        case ID_ACCEL_SHOW:
            SetWindowDisplayAffinity(hwnd, WDA_NONE);
            break;
        case IDM_EXIT:
			FreeWordArrays(); // manually deleting previous words
            memset(savedTexts, 0, sizeof(savedTexts));
			PostQuitMessage(0); // sending post quit message to exit app
            return 0;
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hwnd, AboutDlgProc);
            return 0;
    case ID_OPTIONS:
        DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(ID_CHILD_WINDOW), hwnd, ChildDlgProc, (LPARAM)hwnd);
        break;

        }
        break;


        //CUSTOM REPAINTING/REDRAWING
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        if (hBackground) {
            HDC hdcMem = CreateCompatibleDC(hdc);
            HBITMAP hOld = (HBITMAP)SelectObject(hdcMem, hBackground);

            BITMAP bm;
            GetObject(hBackground, sizeof(bm), &bm);
            BitBlt(hdc, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);

            SelectObject(hdcMem, hOld);
            DeleteDC(hdcMem);
        }

        EndPaint(hwnd, &ps);
        return 0;
    }
    case WM_ERASEBKGND:
		return 1; // forcing no background erase to reduce flicker
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    return 0;
}

INT_PTR CALLBACK AboutDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_INITDIALOG:
    {
        HBITMAP hBmp = (HBITMAP)LoadImage(
            hInst,
            MAKEINTRESOURCE(IDB_ICON_LARGE),
            IMAGE_BITMAP,
			0, 0, // 0 means use actual size == NULL
            LR_DEFAULTCOLOR
        );
        if (hBmp)
            SendDlgItemMessage(hDlg, IDC_ME, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBmp);
        else
            MessageBox(hDlg, L"Ошибка загрузки BMP", L"Ошибка", MB_OK | MB_ICONERROR);


        return TRUE;
    }
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDOK:
        case IDCANCEL:
            EndDialog(hDlg, LOWORD(wParam));
            return TRUE;
        case IDC_ME:
            ShellExecute(NULL, L"open", p2data->descriptor_btn_style[3], NULL, NULL, SW_SHOWNORMAL);
            return TRUE;

        }
        break;

    case WM_NOTIFY:
    {
        LPNMHDR pnmh = (LPNMHDR)lParam;
        if (pnmh->idFrom == IDC_LINK && pnmh->code == NM_CLICK) {
            ShellExecute(NULL, L"open", L"https://example.com", NULL, NULL, SW_SHOWNORMAL);
            return TRUE;
        }
    }
    break;
    }
    return FALSE;
}
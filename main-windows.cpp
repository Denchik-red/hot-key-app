#include <iostream>

int main() {
    std::cout << "Hello, World!" << std::endl;
}

// // global_hotkey_win.cpp
// #include <iostream>
// #include <windows.h>

// LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
//     switch (uMsg) {
//         case WM_HOTKEY:
//             if (wParam == 1001) {
//                 std::cout << "Hotkey Ctrl+Shift+H pressed!\n";
//             }
//             break;
//         case WM_DESTROY:
//             PostQuitMessage(0);
//             return 0;
//     }
//     return DefWindowProcW(hwnd, uMsg, wParam, lParam);
// }

// int main() {
//     const char CLASS_NAME[] = "HotkeyConsoleWindow";

//     WNDCLASSA wc = {};
//     wc.lpfnWndProc   = WindowProc;
//     wc.hInstance     = GetModuleHandleA(nullptr);
//     wc.lpszClassName = CLASS_NAME;

//     if (!RegisterClassA(&wc)) {
//         std::cerr << "RegisterClass failed\n";
//         return 1;
//     }

//     HWND hwnd = CreateWindowExA(
//         0,
//         CLASS_NAME,
//         "Hotkey Handler",
//         0, 0, 0, 0, 0,
//         HWND_MESSAGE,
//         nullptr,
//         wc.hInstance,
//         nullptr
//     );

//     if (!hwnd) {
//         std::cerr << "CreateWindow failed\n";
//         return 1;
//     }

//     // Регистрируем Ctrl+Shift+H (id = 1001)
//     if (!RegisterHotKey(hwnd, 1001, MOD_CONTROL | MOD_SHIFT, 'H')) {
//         std::cerr << "RegisterHotKey failed\n";
//         return 1;
//     }

//     std::cout << "Global hotkey Ctrl+Shift+H registered. Press Ctrl+C to exit.\n";

//     MSG msg = {};
//     while (GetMessage(&msg, nullptr, 0, 0)) {
//         TranslateMessage(&msg);
//         DispatchMessage(&msg);
//     }

//     UnregisterHotKey(hwnd, 1001);
//     return 0;
// }
#include <windows.h>

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CREATE:
            // Create a ListBox control
            CreateWindow(L"LISTBOX", nullptr, WS_CHILD | WS_VISIBLE | WS_BORDER | LBS_HASSTRINGS,
                10, 10, 300, 200, hWnd, nullptr, GetModuleHandle(nullptr), nullptr);
            break;

        case WM_DROPFILES:
            // Handle dropped files
            HDROP hDrop = (HDROP)wParam;
            int numFiles = DragQueryFile(hDrop, 0xFFFFFFFF, nullptr, 0);
            for (int i = 0; i < numFiles; ++i) {
                wchar_t filePath[MAX_PATH];
                DragQueryFile(hDrop, i, filePath, MAX_PATH);
                SendMessage(GetDlgItem(hWnd, 1), LB_ADDSTRING, 0, (LPARAM)filePath);
            }
            DragFinish(hDrop);
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Create the main window
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"MyWin32App";
    RegisterClass(&wc);

    HWND hWnd = CreateWindow(L"MyWin32App", L"File Drop Location",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 400, 300,
        nullptr, nullptr, hInstance, nullptr);

    DragAcceptFiles(hWnd, TRUE); // Enable drag-and-drop

    ShowWindow(hWnd, nCmdShow);

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

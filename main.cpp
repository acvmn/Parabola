#include <windows.h>
#pragma warning(disable: 4244)

static int empty(HWND window)
{
    HDC hdc{};
    PAINTSTRUCT ps{};
    HDC hdcmem{};
    HGDIOBJ oldbitmap{};
    HANDLE image = LoadImage(NULL, L"Assets\\empty.bmp", IMAGE_BITMAP, 250, 250, LR_LOADFROMFILE);

    hdc = BeginPaint(window, &ps);
    hdcmem = CreateCompatibleDC(hdc);
    oldbitmap = SelectObject(hdcmem, image);
    StretchBlt(hdc, 0, 0, 250, 250, hdcmem, 0, 0, 250, 250, SRCCOPY);
    SelectObject(hdcmem, oldbitmap);
    DeleteDC(hdcmem);
    EndPaint(window, &ps);

    return 0;
}

static int parabola(HWND window, long double a, long double b, long double c)
{
    HDC hdc{};
    HPEN parabola = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
    HPEN frame = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
    HBRUSH background = CreateSolidBrush(RGB(255, 255, 255));

    long double x = -4.5;
    long double y{};

    empty(window);

    hdc = GetDC(window);
    SelectObject(hdc, parabola);
    y = a * (x * x);
    MoveToEx(hdc, (x + b * (1 / 1.5)) * (long double(400 / 25) / long double(400 / 250)) + long double(250 / 2), -(y + c * (1 / 1.5)) * (long double(400 / 25) / long double(400 / 250)) + long double(250 / 2), NULL);
    x += 0.005;
    while (x <= 4.5)
    {
        y = a * (x * x);
        LineTo(hdc, (x + b * (1 / 1.5)) * (long double(400 / 25) / long double(400 / 250)) + long double(250 / 2), -(y + c * (1 / 1.5)) * (long double(400 / 25) / long double(400 / 250)) + long double(250 / 2));
        x += 0.005;
    }
    ReleaseDC(window, hdc);

    return 0;
}

static LRESULT CALLBACK WndProc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    static HWND hwnd_a{};
    static HWND hwnd_b{};
    static HWND hwnd_c{};
    static wchar_t text_a[1024]{};
    static wchar_t text_b[1024]{};
    static wchar_t text_c[1024]{};
    static wchar_t* end_a;
    static wchar_t* end_b;
    static wchar_t* end_c;
    static long double a{};
    static long double b{};
    static long double c{};

    switch (message)
    {
    case WM_CREATE:
        CreateWindow(L"STATIC", L"a: ", WS_CHILD | WS_VISIBLE, 251, 1, 230, 20, window, NULL, NULL, NULL);
        hwnd_a = CreateWindow(L"EDIT", NULL, WS_CHILD | WS_VISIBLE, 271, 1, 210, 20, window, NULL, NULL, NULL);
        CreateWindow(L"STATIC", L"b: ", WS_CHILD | WS_VISIBLE, 251, 21, 230, 20, window, NULL, NULL, NULL);
        hwnd_b = CreateWindow(L"EDIT", NULL, WS_CHILD | WS_VISIBLE, 271, 21, 210, 20, window, NULL, NULL, NULL);
        CreateWindow(L"STATIC", L"c: ", WS_CHILD | WS_VISIBLE, 251, 41, 230, 20, window, NULL, NULL, NULL);
        hwnd_c = CreateWindow(L"EDIT", NULL, WS_CHILD | WS_VISIBLE, 271, 41, 210, 20, window, NULL, NULL, NULL);
        CreateWindow(L"BUTTON", L"Build a graph", WS_CHILD | WS_VISIBLE, 251, 61, 231, 30, window, (HMENU)1, NULL, NULL);
        break;
    case WM_PAINT:
        empty(window);
        break;
    case WM_COMMAND:
        if (LOWORD(wparam) == 1)
        {
            GetWindowText((HWND)hwnd_a, text_a, 1024);
            GetWindowText((HWND)hwnd_b, text_b, 1024);
            GetWindowText((HWND)hwnd_c, text_c, 1024);
            a = wcstold(text_a, &end_a);
            b = wcstold(text_b, &end_b);
            c = wcstold(text_c, &end_c);
            parabola(window, a, b, c);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(NULL);
        break;
    }

    return DefWindowProc(window, message, wparam, lparam);
}

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int cmd)
{
    WNDCLASS Class{};
    Class.style = NULL;
    Class.lpfnWndProc = WndProc;
    Class.cbClsExtra = NULL;
    Class.cbWndExtra = NULL;
    Class.hInstance = NULL;
    Class.hIcon = NULL;
    Class.hCursor = LoadCursor(NULL, IDC_ARROW);
    Class.hbrBackground = NULL;
    Class.lpszMenuName = NULL;
    Class.lpszClassName = L"Class";
    RegisterClass(&Class);

    HWND window = CreateWindowEx(NULL, L"Class", L"Parabola", WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, 500, 290, NULL, NULL, NULL, NULL);
    ShowWindow(window, cmd);

    MSG message;
    while (GetMessage(&message, NULL, NULL, NULL))
    {
        DispatchMessage(&message);
        TranslateMessage(&message);
    }

    return 0;
}
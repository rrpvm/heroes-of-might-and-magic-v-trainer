
#include "framework.h"
#include "HOMM5Trainer.h"
#include "GlobalVariables.h"
#include "RenderHelper.h"
#include "GameHackEngine.h"
#include <thread>
#define MAX_LOADSTRING 100
#define LABEL "HOMM5 Trainer By Rome0Coder"


// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
BOOL Create3DDevice(IDirect3DDevice9** pDevice, IDirect3D9** d3d_object, HWND hwnd, int resolutionX, int resolutionY)
{
    *d3d_object = Direct3DCreate9(D3D_SDK_VERSION);
    //assert(!d3d_object);
    D3DPRESENT_PARAMETERS d3dpp{};
    d3dpp.BackBufferWidth = resolutionX;
    d3dpp.BackBufferHeight = resolutionY;
    d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
    d3dpp.BackBufferCount = 1;
    d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
    d3dpp.MultiSampleQuality = 0;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.hDeviceWindow = hwnd;
    d3dpp.Windowed = true;
    d3dpp.EnableAutoDepthStencil = false;
    d3dpp.AutoDepthStencilFormat = D3DFMT_A8R8G8B8;
    d3dpp.Flags = 0;
    d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
    d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
    IDirect3D9* buf = *d3d_object;//crash fix
    HRESULT hr = buf->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, pDevice);
    return SUCCEEDED(hr);
}
bool app_still_work = true;

void second_thread() {
    while (app_still_work)
    {
        hack_engine.sync_data_with_game();
        Sleep(650);
    }
}
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, global_vars.szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_HOMM5TRAINER, global_vars.szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }
    if (!Create3DDevice(render.GetPDevice(), render.GetDirectxObject(), global_vars.hWnd, 1280,360))
    {
        MessageBox(NULL, "Failed init d3d9", "Fail", MB_OK);
    }
    render.Init();
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_HOMM5TRAINER));

    MSG msg;
    std::thread t(second_thread);
    t.detach();
    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        hack_engine.update_mouse();
        render.main_frame();
        render.end_frame();
        Sleep(1000 / 60);

    }
    app_still_work = false;
   if(t.joinable()) t.join();
    return (int)msg.wParam;
}
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_HOMM5TRAINER));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_HOMM5TRAINER);
    wcex.lpszClassName = global_vars.szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    global_vars.hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной
    
    global_vars.hWnd = CreateWindowW(global_vars.szWindowClass, global_vars.szTitle, WS_OVERLAPPED  |WS_SYSMENU| WS_MINIMIZEBOX|WS_TILED,
        0, 0, 1280 + 12, 360 + 52 , nullptr, nullptr, hInstance, nullptr);
    RECT rc = { 0,0,1280,360 };
    SendMessage(global_vars.hWnd, WM_SETTEXT, 0, (LPARAM)"HOMM5 Trainer By Rome0Coder");
    AdjustWindowRect(&rc, WS_OVERLAPPED, false);
    if (!global_vars.hWnd)
    {
        return FALSE;
    }

    ShowWindow(global_vars.hWnd, nCmdShow);
    UpdateWindow(global_vars.hWnd);
    SendMessage(global_vars.hWnd, WM_SETTEXT, 0, (LPARAM)"HOMM5 Trainer By Rome0Coder");
    return TRUE;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Разобрать выбор в меню:
        switch (wmId)
        {
      /*  case IDM_ABOUT:
            DialogBox(global_vars.hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;*/
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

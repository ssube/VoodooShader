// Testbed.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Testbed.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
TCHAR szTitle[MAX_LOADSTRING];                    // The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

IDirect3D9 * d3d_Object;
IDirect3DDevice9 * d3d_Device;

//VoodooShader::Core * core;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void DirectXDraw();
bool DirectXInit();
void DirectXQuit();
void DrawGDI(HWND hWnd);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

     // TODO: Place code here.
    MSG msg;
    HACCEL hAccelTable;

    d3d_Object = Direct3DCreate9(D3D_SDK_VERSION);
    if ( !d3d_Object )
    {
        return FALSE;
    }

    // Initialize global strings
    LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadString(hInstance, IDC_TESTBED, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TESTBED));

    // Main message loop:
    while ( true )
    {
        // Draw DX...
        if ( d3d_Device )
        {
            DirectXDraw();
        }

        if (GetMessage(&msg, NULL, 0, 0))
        {

            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style            = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra        = 0;
    wcex.cbWndExtra        = 0;
    wcex.hInstance        = hInstance;
    wcex.hIcon            = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TESTBED));
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground    = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName    = MAKEINTRESOURCE(IDC_TESTBED);
    wcex.lpszClassName    = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   // Start DX
   D3DPRESENT_PARAMETERS pp;
   ZeroMemory(&pp, sizeof(pp));
   pp.BackBufferWidth = 800;
   pp.BackBufferHeight = 600;
   pp.BackBufferFormat = D3DFMT_X8R8G8B8;
   pp.Windowed = TRUE;
   pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
   pp.MultiSampleType = D3DMULTISAMPLE_NONE;

   HRESULT device = d3d_Object->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, 
       hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &pp, &d3d_Device);

   if ( FAILED(device) )
   {
       return FALSE;
   }

   if ( !DirectXInit() )
   {
       return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND    - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY    - post a quit message and return
//
//
DWORD lastTicks = 0, ticks = 0, draws = 0;
float fps = 0;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int wmId, wmEvent;

    switch (message)
    {
    case WM_COMMAND:
        wmId    = LOWORD(wParam);
        wmEvent = HIWORD(wParam);
        // Parse the menu selections:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        case IDM_NEW:
            MessageBoxA(hWnd, "New", "Menu", MB_ICONINFORMATION);
            break;
        case IDM_OPEN:
            MessageBoxA(hWnd, "Open", "Menu", MB_ICONINFORMATION);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        break;
    case WM_DESTROY:
        DirectXQuit();
        PostQuitMessage(0);
        break;
    case WM_PAINT:
    default:
        //break;
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

// Message handler for about box.
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

struct Vert
{
    FLOAT x,y,z,w;
    DWORD color;
};
#define CVERT (D3DFVF_XYZRHW|D3DFVF_DIFFUSE)

IDirect3DVertexBuffer9 * verts;
ID3DXFont * font;
RECT rect;
char msg[64];

bool DirectXInit()
{
    LONG left = 5;//rand() % 200;
    LONG top = 5;//rand() % 150;
    rect.left = left; rect.right = left + 250;
    rect.top = top; rect.bottom = top + 150;

    Vert vertices[] =
    {
        { 150.0f,  50.0f, 0.5f, 1.0f, 0xffff0000, }, // x, y, z, rhw, color
        { 250.0f, 250.0f, 0.5f, 1.0f, 0xff00ff00, },
        {  50.0f, 250.0f, 0.5f, 1.0f, 0xff2266ff, },
    };

    HRESULT hr = d3d_Device->CreateVertexBuffer(sizeof(vertices), D3DUSAGE_WRITEONLY, CVERT, D3DPOOL_DEFAULT, &verts, NULL);

    if ( FAILED(hr) ) return FALSE;

    void * data;
    
    if ( FAILED(verts->Lock(0, sizeof(vertices), &data, 0) )) return FALSE;

    memcpy(data, vertices, sizeof(vertices));

    verts->Unlock();

    D3DXCreateFontA(d3d_Device, 12, 8, 1, 0, FALSE, NULL, NULL, NULL, NULL, "Arial", &font);

    return TRUE;
}

DWORD update;

void DirectXDraw()
{
    ++draws;
    ticks = GetTickCount();
    if ( ticks > ( lastTicks + 100 ) )
    {
        DWORD span = ticks - lastTicks;
        fps = ( draws * 10 ) / ((float)span / 100);
        lastTicks = ticks;

        if ( ++update == 10 )
        {
            LONG left = rand() % 200;
            LONG top = rand() % 150;
            rect.left = left; rect.right = left + 250;
            rect.top = top; rect.bottom = top + 150;
            update = 0;
        }
    }
    sprintf_s(msg, 64, "FPS: %f\0", fps);

    d3d_Device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0,0,128), 1.0f, 0);

    d3d_Device->BeginScene();

    d3d_Device->SetStreamSource(0, verts, 0, sizeof(Vert));
    d3d_Device->SetFVF(CVERT);

    d3d_Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);

    DWORD srcblend, destblend;
    d3d_Device->GetRenderState(D3DRS_SRCBLEND, &srcblend);
    d3d_Device->GetRenderState(D3DRS_DESTBLEND, &destblend);
    d3d_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
    d3d_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR);
    font->DrawTextA(NULL, msg, -1, &rect, DT_CENTER, D3DCOLOR_ARGB(0xFF, 0xFF, 0x88, 0x00));
    d3d_Device->SetRenderState(D3DRS_SRCBLEND, srcblend);
    d3d_Device->SetRenderState(D3DRS_DESTBLEND, destblend);

    d3d_Device->EndScene();

    d3d_Device->Present(NULL, NULL, NULL, NULL);
}

void DirectXQuit()
{
    if ( verts )
    {
        verts->Release();
        verts = NULL;
    }
    if ( d3d_Device )
    {
        d3d_Device->Release();
        d3d_Device = NULL;
    }
    if ( d3d_Object )
    {
        d3d_Object->Release();
        d3d_Object = NULL;
    }
}

void DrawGDI(HWND hWnd)
{
/*
    PAINTSTRUCT ps;
    HDC hdc;

    hdc = BeginPaint(hWnd, &ps);

    SetDCPenColor(hdc, 0x0000FFFF);
    SetROP2(hdc, R2_XORPEN);

    DrawTextA(hdc, msg, -1, &rect, DT_BOTTOM|DT_CENTER);

    EndPaint(hWnd, &ps);
*/
}
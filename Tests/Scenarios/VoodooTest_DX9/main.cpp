
#include <d3dx9.h>
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"winmm.lib")

LPDIRECT3D9 g_pD3D = NULL;
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer = NULL; // Buffer to hold vertices

struct MYVERTEX
{
    FLOAT x, y, z;
    DWORD color;
};

#define D3DFVF_MYVERTEX ( D3DFVF_XYZ | D3DFVF_DIFFUSE )

#define SafeRelease(pObject) if (pObject) { pObject->Release(); pObject = nullptr; }

HRESULT InitDX(HWND hWnd)
{
    g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
    if(g_pD3D == NULL)
    {
        return E_FAIL;
    }

    D3DDISPLAYMODE d3ddm;
    if(FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
    {
        return E_FAIL;
    }

    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory(&d3dpp, sizeof(d3dpp));

    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = d3ddm.Format;

    if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
        D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_pD3DDevice)))
    {
        return E_FAIL;
    }

    g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

    g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

    return S_OK;
}

HRESULT InitVertexBuffer()
{
    VOID * pVertices;

    MYVERTEX Vertices[] =
    {
        // Top Face
        {-5.0f, 5.0f, -5.0f, D3DCOLOR_XRGB(0, 0, 255),},    // Vertex 0 - Blue
        {-5.0f, 5.0f, 5.0f, D3DCOLOR_XRGB(255, 0, 0),},     // Vertex 1 - Red
        {5.0f, 5.0f, -5.0f, D3DCOLOR_XRGB(255, 0, 0),},     // Vertex 2 - Red
        {5.0f, 5.0f, 5.0f, D3DCOLOR_XRGB(0, 255, 0),},      // Vertex 3 - Green

        // Face 1
        {-5.0f, -5.0f, -5.0f, D3DCOLOR_XRGB(255, 0, 0),},   // Vertex 4 - Red
        {-5.0f, 5.0f, -5.0f, D3DCOLOR_XRGB(0, 0, 255),},    // Vertex 5 - Blue
        {5.0f, -5.0f, -5.0f, D3DCOLOR_XRGB(0, 255, 0),},    // Vertex 6 - Green
        {5.0f, 5.0f, -5.0f, D3DCOLOR_XRGB(255, 0, 0),},     // Vertex 7 - Red

        // Face 2
        {5.0f, -5.0f, 5.0f, D3DCOLOR_XRGB(0, 0, 255),},     // Vertex 8 - Blue
        {5.0f, 5.0f, 5.0f, D3DCOLOR_XRGB(0, 255, 0),},      // Vertex 9 - Green

        // Face 3
        {-5.0f, -5.0f, 5.0f, D3DCOLOR_XRGB(0, 255, 0),},    //Vertex 10 - Green
        {-5.0f, 5.0f, 5.0f, D3DCOLOR_XRGB(255, 0, 0),},     //Vertex 11 - Red

        // Face 4
        {-5.0f, -5.0f, -5.0f, D3DCOLOR_XRGB(255, 0, 0),},   //Vertex 12 - Red
        {-5.0f, 5.0f, -5.0f, D3DCOLOR_XRGB(0, 0, 255),},    //Vertex 13 - Blue

        // Bottom Face
        {5.0f, -5.0f, -5.0f, D3DCOLOR_XRGB(0, 255, 0),},    //Vertex 14 - Green
        {5.0f, -5.0f, 5.0f, D3DCOLOR_XRGB(0, 0, 255),},     //Vertex 15 - Blue
        {-5.0f, -5.0f, -5.0f, D3DCOLOR_XRGB(255, 0, 0),},   //Vertex 16 - Red
        {-5.0f, -5.0f, 5.0f, D3DCOLOR_XRGB(0, 255, 0),},    //Vertex 17 - Green
    };

    // Create the vertex buffer from our device.
    if(FAILED(g_pD3DDevice->CreateVertexBuffer(18 * sizeof(MYVERTEX),
        0, D3DFVF_MYVERTEX,
        D3DPOOL_DEFAULT, &g_pVertexBuffer, NULL)))
    {
        return E_FAIL;
    }

    // Get a pointer to the vertex buffer vertices and lock the vertex buffer
    if(FAILED(g_pVertexBuffer->Lock(0, sizeof(Vertices), &pVertices, 0)))
    {
        return E_FAIL;
    }

    // Copy our stored vertices values into the vertex buffer
    memcpy(pVertices, Vertices, sizeof(Vertices));

    // Unlock the vertex buffer
    g_pVertexBuffer->Unlock();

    return S_OK;
}


void SetRotation()
{
    // Here we will rotate our world around the x, y and z axis.
    D3DXMATRIX matWorld, matWorldX, matWorldY, matWorldZ;

    // Create the transformation matrices
    D3DXMatrixRotationX(&matWorldX, timeGetTime()/400.0f);
    D3DXMatrixRotationY(&matWorldY, timeGetTime()/400.0f);
    D3DXMatrixRotationZ(&matWorldZ, timeGetTime()/400.0f);

    // Combine the transformations by multiplying them together
    D3DXMatrixMultiply(&matWorld, &matWorldX, &matWorldY);
    D3DXMatrixMultiply(&matWorld, &matWorld, &matWorldZ);

    // Apply the tansformation
    g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
}

void SetCamera()
{
    D3DXMATRIX matView;

    D3DXMatrixLookAtLH
    (
        &matView, 
        &D3DXVECTOR3(0.0f, 0.0f,-30.0f), //Camera Position
        &D3DXVECTOR3(0.0f, 0.0f, 0.0f),  //Look At Position
        &D3DXVECTOR3(0.0f, 1.0f, 0.0f)   //Up Direction
    );

    g_pD3DDevice->SetTransform(D3DTS_VIEW, &matView);
}

void SetPerspective()
{
    // Here we specify the field of view, aspect ration and near and far clipping planes.
    D3DXMATRIX matProj;
    D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI/4, 1.0f, 1.0f, 500.0f);
    g_pD3DDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}


void Render()
{
    if(g_pD3DDevice == NULL)
    {
        return;
    }

    // Clear the backbuffer to black
    g_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

    // Begin the scene
    g_pD3DDevice->BeginScene();

    // Setup the rotation, camera, and perspective matrices
    SetRotation();
    SetCamera();
    SetPerspective();

    // Rendering our objects
    g_pD3DDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(MYVERTEX));
    g_pD3DDevice->SetFVF(D3DFVF_MYVERTEX);
    g_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);  // Top
    g_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4, 8);  // Sides
    g_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 14, 2); // Bottom

    // End the scene
    g_pD3DDevice->EndScene();

    // Flip the back and front buffers so that whatever has been rendered on the back buffer
    // will now be visible on screen (front buffer).
    g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

void CleanUp()
{
    SafeRelease(g_pVertexBuffer);
    SafeRelease(g_pD3DDevice);
    SafeRelease(g_pD3D);
}

void GameLoop()
{
    MSG msg;
    BOOL fMessage;

    PeekMessage(&msg, NULL, 0U, 0U, PM_NOREMOVE);

    while(msg.message != WM_QUIT)
    {
        // Check if any messages
        fMessage = PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE);

        if(fMessage)
        {
            // Process message
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            // No message to process, so render the current scene
            Render();
        }
    }
}

// The windows message handler
LRESULT WINAPI WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
        break;

    case WM_KEYUP:
        switch (wParam)
        {
        case VK_ESCAPE:
            // User has pressed the escape key, so quit
            DestroyWindow(hWnd);
            return 0;
            break;
        }
        break;
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

// Application entry point
INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, INT)
{
    // Register the window class
    WNDCLASSEX wc = {sizeof(WNDCLASSEX), CS_CLASSDC, WinProc, 0L, 0L,
        GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
        L"Tutorial 3", NULL};
    RegisterClassEx(&wc);

    // Create the application's window
    HWND hWnd = CreateWindow(L"Tutorial 3", L"www.dogonfire.dk :: Tutorial 3",
        WS_OVERLAPPEDWINDOW, 50, 50, 500, 500,
        GetDesktopWindow(), NULL, wc.hInstance, NULL);

    // Initialize Direct3D
    if(SUCCEEDED(InitDX(hWnd)))
    {
        // Show our window
        ShowWindow(hWnd, SW_SHOWDEFAULT);
        UpdateWindow(hWnd);

        // Initialize Vertex Buffer
        if(SUCCEEDED(InitVertexBuffer()))
        {
            // Start game running: Enter the game loop
            GameLoop();
        }
    }

    CleanUp();

    UnregisterClass(L"Tutorial 3", wc.hInstance);

    return 0;
}

#include <Windows.h>
#include "configuration.h"
#include "engine_types.h"
#include "debug.h"

#pragma comment (lib,"Gdiplus.lib")
#pragma comment (lib,"user32.lib")
#pragma comment (lib,"opengl32.lib")
#pragma comment (lib,"Gdi32.lib")
#pragma comment (lib,"Glu32.lib")



extern void ReportError(wchar_t* sMessage);

extern void ReportWin32Error(wchar_t* sMessage);

extern LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


/* Internal Engine functions */
extern void EngineDestroyWindow();

/* Engine Window functions */
extern INT EngineCreateWindow(
    const wchar_t* cTitle,
    const INT iWinWidth,
    const INT iWinHeight,
    const BOOL bFullScreen
);

extern BOOL EngineGetKeyState(const BYTE bKeyCode);

/*Engine draw Functions*/
extern void EngineDrawScene();

extern void EngineDrawPoint(
    const coordinate_t p,
    const color_t color
);

extern void EngineDrawLine(
    const coordinate_t p1,
    const coordinate_t p2,
    const color_t color
);

extern void EngineDrawTriangle(
    const coordinate_t top_p,
    const coordinate_t bottom_left_p,
    const coordinate_t bottom_right_p,
    const color_t color
);

extern void EngineDrawQuad(
    const coordinate_t top_left_p,
    const coordinate_t top_right_p,
    const coordinate_t bottom_right_p,
    const coordinate_t bottom_left_p,
    const color_t color
);



extern void EngineDrawSprite(
    const INT32 x,
    const INT32 y,
    const DWORD scale,
    const DWORD dwResourceId,
    const flip_t flip
);

#ifdef __cplusplus
extern "C"
{
#endif

    __declspec(dllexport) void HndlEngineDrawSprite(
        const INT32 x,
        const INT32 y,
        const DWORD scale,
        const DWORD dwResourceId,
        const flip_t flip
    )
    {
        EngineDrawSprite(x, y, scale, dwResourceId, flip);
    }


    __declspec(dllexport) void HndlEngineDrawQuad(
        const coordinate_t top_left_p,
        const coordinate_t top_right_p,
        const coordinate_t bottom_right_p,
        const coordinate_t bottom_left_p,
        const color_t color)
    {
        EngineDrawQuad(top_left_p, top_right_p, bottom_right_p, bottom_left_p, color);
    }

    __declspec(dllexport) void HndlEngineDrawTriangle(
        const coordinate_t top_p,
        const coordinate_t bottom_left_p,
        const coordinate_t bottom_right_p,
        const color_t color
    )
    {
        EngineDrawTriangle(top_p, bottom_left_p, bottom_right_p, color);
    }


    __declspec(dllexport) void HndlEngineDrawLine(
        const coordinate_t p1,
        const coordinate_t p2,
        const color_t color)
    {
        EngineDrawLine(p1, p2, color);
    }

    __declspec(dllexport) void HndlEngineDrawPoint(
        const coordinate_t p,
        const color_t color)
    {
        EngineDrawPoint(p, color);
    }

    __declspec(dllexport) BOOL HndlEngineGetKeyState(BYTE bKeyCode)
    {
        return EngineGetKeyState(bKeyCode);
    }

    __declspec(dllexport) INT HndlEngineCreateWindow()
    {
        EngineCreateWindow(
            L"Bear Engine",
            SCREEN_WIDTH,
            SCREEN_HEIGHT,
            FULL_SCREEN
        );
        return 0;
    }


    __declspec(dllexport) void HndlEngineRun(BOOL(*FctDraw)(void))
    {
        MSG msg;
        BOOL bDone = FALSE;
        EngineDrawScene();
        while (!bDone)
        {
            if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
            {
                if (msg.message == WM_QUIT)
                {
                    bDone = TRUE;
                }
                else
                {
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }
            }
            else
            {
                //Draw the scene
                if (FctDraw())
                {
                    EngineDrawScene();
                }
            }
        }
        EngineDestroyWindow();
    }

#ifdef __cplusplus
}
#endif /* __cplusplus */


BOOL APIENTRY DllMain(HINSTANCE hModule, DWORD ulReasonForCall, LPVOID lpReserved)
{
    switch (ulReasonForCall)
    {
    case DLL_PROCESS_ATTACH:

        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_PROCESS_DETACH:
        break;
    default:
        break;
    }
    return TRUE;
}
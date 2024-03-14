#include "pch.h"
#include "Engine.h"


#pragma comment (lib,"Gdiplus.lib")
#pragma comment (lib,"user32.lib")
#pragma comment (lib,"opengl32.lib")
#pragma comment (lib,"Gdi32.lib")
#pragma comment (lib,"Glu32.lib")
#pragma comment (lib,"Gdiplus.lib")

#pragma region imported_from_core



extern LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
extern int EngineCreateWindow(const std::wstring& cTitle);
extern int EngineInitializeGraphics();
extern void EngineDestroyWindow();

extern void EngineDrawScene();

extern BOOL EngineGetKeyState(const BYTE bKeyCode);

extern void EngineGetWindowSize(
    int32_t* iWindowWidth,
    int32_t* iWindowHeight
);
#pragma endregion

#pragma region engine_specific

static std::atomic<bool>bAtomicActive{ false };

static void EngineThreadHandler(bool(*FctDraw)(void));
#pragma endregion


void EngineThreadHandler(bool(*FctDraw)(void))
{
    
    if (EngineInitializeGraphics() != 0)
        return;
    EngineDrawScene();
    
    while (bAtomicActive) {
 
        
        if (FctDraw())
        {
            EngineDrawScene();
        }
        
  
  
    }
}

namespace bear
{

    void HndlEngineGetWindowSize(int32_t* iWindowWidth, int32_t* iWindowHeight)
    {
        EngineGetWindowSize(iWindowWidth, iWindowHeight);
    }

    void HndlEngineChangeLighting(
        bool bLightOn
    )
    {
        if (bLightOn == TRUE)
        {
            glEnable(GL_LIGHTING);
        }
        else
        {
            glDisable(GL_LIGHTING);
        }
    }



    bool HndlEngineGetKeyState(uint8_t bKeyCode)
    {
        return EngineGetKeyState(bKeyCode);
    }

    int32_t HndlEngineCreateWindow()
    {

        return EngineCreateWindow(
            L"Bear Engine"
        );

    }


    void HndlEngineRun(bool(*FctDraw)(void))
    {

        bAtomicActive = true;
        std::thread t=std::thread(EngineThreadHandler, FctDraw);

        /*
        while (GetMessage(&msg, NULL, 0, 0) > 0)
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }*/

        MSG msg;
        while(bAtomicActive){
            if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
                if (msg.message == WM_QUIT) {
                    bAtomicActive = false;
                }
                else {
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }
            }
        }
        t.join();
        
  

        
        EngineDestroyWindow();
    }
}

















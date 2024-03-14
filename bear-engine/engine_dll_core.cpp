#include "pch.h"
#include "framework.h"



#define NUMBER_OF_KEYS 256


/* Functions defined in this module. */
static HWND g_hWnd = NULL;

static HDC g_hDC = NULL;
static HGLRC g_hRC = NULL;
static ULONG_PTR g_ptrGdiplusToken = 0;

static BOOL g_baKeys[NUMBER_OF_KEYS] = { 0 };

static HINSTANCE g_hInstance = NULL;

static GLfloat g_faLightAmbient[] = { 0.5f, 0.5f, 0.5f, 1.0f };
static GLfloat g_faLightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
static GLfloat g_faLightPosition[] = { 0.0f, 0.0f, 2.0f, 1.0f };



extern volatile window_t g_sWindowConfiguration;



volatile window_t g_sWindowConfiguration = {
    .iWindowWidth= 800,
    .iWindowHeight = 600,
    .bIsFullScreen = FALSE,
    .bBitsPerColor = 32
};



static GLvoid InitGL(GLvoid);

static GLvoid ResizeGLScene(GLsizei width, GLsizei height);

static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

extern int EngineInitializeGraphics();

extern int EngineCreateWindow(const std::wstring & cTitle);

extern void EngineDestroyWindow();

extern void EngineDrawScene();

extern BOOL EngineGetKeyState(const BYTE bKeyCode);

extern void EngineGetWindowSize(
    int32_t* iWindowWidth, 
    int32_t *iWindowHeight
);


void EngineGetWindowSize(
    int32_t* iWindowWidth,
    int32_t *iWindowHeight
)
{
   
    *iWindowWidth = g_sWindowConfiguration.iWindowWidth;
    *iWindowHeight = g_sWindowConfiguration.iWindowHeight;
        
   
}

BOOL EngineGetKeyState(const BYTE bKeyCode)
{
    BOOL bKeyState = g_baKeys[bKeyCode];
    g_baKeys[bKeyCode] = FALSE;
    return bKeyState;
}



int EngineCreateWindow(const std::wstring &cTitle)
{
  
  WNDCLASS wc = {};

  RECT rWindowRect;


  DWORD dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
  DWORD dwStyle = WS_CAPTION | WS_SYSMENU | WS_VISIBLE | WS_THICKFRAME;

  g_hInstance = GetModuleHandle(NULL);

  if (g_hInstance == NULL)
  {
    return GetLastError();
  }
    

  wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
  wc.lpfnWndProc = (WNDPROC) WndProc;		
  wc.cbClsExtra = 0;				
  wc.cbWndExtra	= 0;				
  wc.hInstance	= g_hInstance;			
  wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);	
  wc.hbrBackground = NULL;			
  wc.lpszMenuName = NULL;			
  wc.lpszClassName = cTitle.c_str();			

  if (!RegisterClass(&wc))
  {
    utils::ReportError(L"Failed to register the Window Class.");
    return GetLastError();
  }

  /*if (g_sWindowConfiguration.bIsFullScreen)
  {
    DEVMODE dmScreenSettings;
    memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
    dmScreenSettings.dmSize = sizeof(dmScreenSettings);
    dmScreenSettings.dmPelsWidth = g_sWindowConfiguration.iWindowWidth;
    dmScreenSettings.dmPelsHeight = g_sWindowConfiguration.iWindowHeight;
    dmScreenSettings.dmBitsPerPel = g_sWindowConfiguration.bBitsPerColor;
    dmScreenSettings.dmFields = DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

    if (ChangeDisplaySettings (&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
    {
      utils::ReportError(L"The requested fullscreen mode is not supported!");
      g_sWindowConfiguration.bIsFullScreen = FALSE;
    }
      
  }*/

  if (g_sWindowConfiguration.bIsFullScreen)
  {
      dwExStyle = 0;
      dwStyle = WS_POPUP | WS_VISIBLE;
      HMONITOR hmon = MonitorFromWindow(g_hWnd, MONITOR_DEFAULTTONEAREST);
      MONITORINFO mi = { sizeof(mi) };
      if (!GetMonitorInfo(hmon, &mi))
          return GetLastError();
      g_sWindowConfiguration.iWindowWidth = mi.rcMonitor.right;
      g_sWindowConfiguration.iWindowHeight = mi.rcMonitor.bottom;
  
  
    ShowCursor(FALSE);
  }

  rWindowRect.left = (DWORDLONG)0;
  rWindowRect.top = (DWORDLONG)0;
  rWindowRect.right = (DWORDLONG)g_sWindowConfiguration.iWindowWidth;
  rWindowRect.bottom = (DWORDLONG)g_sWindowConfiguration.iWindowHeight;
  AdjustWindowRectEx(&rWindowRect, dwStyle, FALSE, dwExStyle);

  g_sWindowConfiguration.iWindowWidth = rWindowRect.right - rWindowRect.left;
  g_sWindowConfiguration.iWindowHeight = rWindowRect.bottom - rWindowRect.top;
  

  /*Create Window*/
  g_hWnd = CreateWindowEx(dwExStyle,
                          cTitle.c_str(),
                          cTitle.c_str(),
                          dwStyle | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
                          0,
                          0,
                          g_sWindowConfiguration.iWindowWidth,
                          g_sWindowConfiguration.iWindowHeight,
                          NULL,
                          NULL,
                          g_hInstance,
                          NULL);
  if (NULL == g_hWnd)
  {
    utils::ReportError(L"Could not create window!");
    return GetLastError();
  }
  return 0;
  
  
}

void EngineDestroyWindow()
{

  if (g_sWindowConfiguration.bIsFullScreen)
  {
    ChangeDisplaySettings(NULL,0);
    ShowCursor(TRUE);
  }
  if (g_ptrGdiplusToken)
  {
    Gdiplus::GdiplusShutdown(g_ptrGdiplusToken);
  }

  if (g_hRC)
  {
    if (!wglMakeCurrent(NULL, NULL))
    {
        utils::ReportError(L"Release of DC and RC failed");
    }
    if (!wglDeleteContext(g_hRC))
    {
        utils::ReportError(L"Render release failed");
    }
    g_hRC = NULL;
  }

  if (g_hDC)
  {
    if (0 == ReleaseDC(g_hWnd, g_hDC))
    {
        utils::ReportError(L"Release Device Context failed");
      g_hDC = NULL;
    }
  }
  
  
  if (g_hWnd && !DestroyWindow(g_hWnd))					// Are We Able To Destroy The Window?
  {
      utils::ReportError(L"Failed to destroy Window");
    g_hWnd=NULL;										// Set hWnd To NULL
  }

  if (!UnregisterClass(L"OpenGL",g_hInstance))			// Are We Able To Unregister Class
  {
      utils::ReportError(L"Could not unregister instance");
    g_hInstance=NULL;									// Set hInstance To NULL
  }
}

void EngineDrawScene()
{
  
  SwapBuffers(g_hDC);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
}

GLvoid InitGL(GLvoid)
{
  glEnable(GL_TEXTURE_2D);
  glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
  glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
  glClearDepth(1.0f);									// Depth Buffer Setup
  glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
  glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
 

  glLightfv(GL_LIGHT1, GL_AMBIENT, g_faLightAmbient);		// Setup The Ambient Light
  glLightfv(GL_LIGHT1, GL_DIFFUSE, g_faLightDiffuse);		// Setup The Diffuse Light
  glLightfv(GL_LIGHT1, GL_POSITION, g_faLightPosition);	// Position The Light
  glEnable(GL_LIGHT1);
}



GLvoid ResizeGLScene(GLsizei width, GLsizei height)
{
  if (height == 0)
  {
    height = 1;
   
  }

  
  g_sWindowConfiguration.iWindowWidth = width;
  g_sWindowConfiguration.iWindowHeight = height;
    
  
  GLfloat aspect = (GLfloat)width/(GLfloat)height;
  glViewport(0, 0, width, height);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluPerspective(45.0f, aspect, 0.1f, 100.0f);
  
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

}


int EngineInitializeGraphics()
{
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    GLuint PixelFormat = 0;
    // Initialize GDI+.
    if (Gdiplus::GdiplusStartup(&g_ptrGdiplusToken, &gdiplusStartupInput, NULL) != 0)
    {
        g_ptrGdiplusToken = NULL;
        return GetLastError();
    }

    PIXELFORMATDESCRIPTOR pfd =
    {
        sizeof(PIXELFORMATDESCRIPTOR), 1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA, 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        PFD_MAIN_PLANE, 0, 0, 0, 0
    };

    g_hDC = GetDC(g_hWnd);
    if (NULL == g_hDC)
    {
        utils::ReportError(L"Could not get Device Context");
        return GetLastError();
    }
    PixelFormat = ChoosePixelFormat(g_hDC, &pfd);
    if (0 == PixelFormat)
    {
        utils::ReportError(L"PixelFormat");
        return GetLastError();
    }

    if (!SetPixelFormat(g_hDC, PixelFormat, &pfd))
    {
        utils::ReportError(L"SetPixelFormat");
        return GetLastError();
    }

    g_hRC = wglCreateContext(g_hDC);
    if (NULL == g_hRC)
    {
        utils::ReportError(L"wglCreateContext");
        return GetLastError();
    }
    if (!wglMakeCurrent(g_hDC, g_hRC))
    {
        utils::ReportError(L"wglMakeCurrent");
        return GetLastError();
    }
    InitGL();
    ResizeGLScene(g_sWindowConfiguration.iWindowWidth, g_sWindowConfiguration.iWindowHeight);


    ShowWindow(g_hWnd, SW_SHOW);
    SetForegroundWindow(g_hWnd);
    SetFocus(g_hWnd);
    
}
    

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_ACTIVATE://should stop rendering in background
        break;
    case WM_SYSCOMMAND:
    {
        switch (wParam)
        {
        case SC_SCREENSAVE:
        case SC_MONITORPOWER:
            return 0;
        }
        break;
    }
    case WM_CLOSE:
        PostQuitMessage(0);
        break;
    case WM_KEYDOWN:
        g_baKeys[wParam] = TRUE;
        break;
    case WM_KEYUP:
        g_baKeys[wParam] = FALSE;
        break;
    case WM_SIZE:
        //ResizeGLScene(LOWORD(lParam), HIWORD(lParam));
        break;
    default:
        break;
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

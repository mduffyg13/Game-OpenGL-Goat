/* ==================================================================================
cWNDManager.cpp
================================================================================== */
#include "cWNDManager.h" 
#include <ctime> 
#include <iostream> 
#include <windows.h> 
#include <GL/glut.h> 
#include "wglext.h" 
#include "windowOGL.h"
#include "GameConstants.h"

cWNDManager* cWNDManager::pInstance = NULL;

typedef HGLRC(APIENTRYP PFNWGLCREATECONTEXTATTRIBSARBPROC)(HDC, HGLRC, const int*);
PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = NULL;

/* =================================================================================
Constructor
================================================================================= */

cWNDManager::cWNDManager() {
	m_isRunning = false;
	m_winOGL = NULL;
	m_hinstance = NULL;
	m_lastTime = 0;
}

/* =================================================================================
Singleton Design Pattern
================================================================================= */

cWNDManager* cWNDManager::getInstance() {

	if (pInstance == NULL) {

		pInstance = new cWNDManager();

	}

	return cWNDManager::pInstance;
}

bool cWNDManager::createWND(int width, int height, int bpp) {

	DWORD dwExStyle;					// Window Extended Style 
	DWORD dwStyle;						// Window Style

	m_windowRect.left = (long)0;		// Set Left Value To 0 
	m_windowRect.right = (long)width;	// Set Right Value To Requested Width 
	m_windowRect.top = (long)0;			// Set Top Value To 0 
	m_windowRect.bottom = (long)height; // Set Bottom Value To Requested Height

	// fill out the window class structure 
	m_windowClass.cbSize = sizeof(WNDCLASSEX);
	m_windowClass.style = CS_HREDRAW | CS_VREDRAW;
	m_windowClass.lpfnWndProc = (WNDPROC)cWNDManager::WndProc; //We set our static method as the event handler (WNDPROC) 
	m_windowClass.cbClsExtra = 0;
	m_windowClass.cbWndExtra = 0;
	m_windowClass.hInstance = m_hinstance;
	m_windowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);	// default icon 
	m_windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);	// default arrow 
	m_windowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);// NULL;						// don't need background 
	m_windowClass.lpszMenuName = NULL;						// no menu 
	m_windowClass.lpszClassName = "winOGL";
	m_windowClass.hIconSm = LoadIcon(NULL, IDI_WINLOGO);	// windows logo small icon

	// register the windows class 
	if (!RegisterClassEx(&m_windowClass)) {
		return false;
	}

	dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE; // Window Extended Style 
	dwStyle = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN; // Windows Style

	AdjustWindowRectEx(&m_windowRect, dwStyle, false, dwExStyle); // Adjust Window To True Requested Size

	// class registered, so now create our window 
	m_hwnd = CreateWindowEx(NULL, // extended style 
		"winOGL", // class name
		"My First OpenGL Window", // app name
		WS_OVERLAPPEDWINDOW, // the window style
		CW_USEDEFAULT, // the starting x coordinate
		CW_USEDEFAULT, // the starting y coordinate
		m_windowRect.right - m_windowRect.left, // the pixel width of the window
		m_windowRect.bottom - m_windowRect.top, // the pixel height of the window
		NULL, // the parent window; NULL for desktop
		NULL, // the menu for the application; NULL for none
		m_hinstance, // the handle to the application instance
		this); // no values passed to the window

	// check if window creation failed (hwnd would equal NULL)
	if (!m_hwnd)
		return 0;

	m_hdc = GetDC(m_hwnd);
	ShowWindow(m_hwnd, SW_SHOW); // display the window
	UpdateWindow(m_hwnd); // update the window

	m_lastTime = GetTickCount() / 1000.0f; //Initialize the time
	return true;
}

void cWNDManager::destroyWND()
{
	ShowCursor(true); // Show Mouse Pointer
}

void cWNDManager::attachOGLWnd(windowOGL* OGLWindow)
{
	m_winOGL = OGLWindow;
}

bool cWNDManager::isWNDRunning()
{
	return m_isRunning;
}

void cWNDManager::processWNDEvents()
{
	MSG msg;
	//While there are messages in the queue, store them in msg
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		//Process the messages one-by-one
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void cWNDManager::setupPixelFormat(void)
{
	HDC   g_hDC = GetDC(m_hwnd);
	
	PIXELFORMATDESCRIPTOR pfd;
	int pixelformat;

	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);			// Set the size of the structure
	pfd.nVersion = 1;									// Always set this to 1
	// Pass in the appropriate OpenGL flags
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.dwLayerMask = PFD_MAIN_PLANE;					// We want the standard mask (this is ignored anyway)
	pfd.iPixelType = PFD_TYPE_RGBA;						// We want RGB and Alpha pixel type
	pfd.cColorBits = 16;						// Here we use our #define for the color bits
	pfd.cDepthBits = 16;						// Depthbits is ignored for RGBA, but we do it anyway
	pfd.cAccumBits = 0;									// No special bitplanes needed
	pfd.cStencilBits = 0;								// We desire no stencil bits

	// This gets us a pixel format that best matches the one passed in from the device
	if ((pixelformat = ChoosePixelFormat(m_hdc, &pfd)) == FALSE)
	{
		MessageBox(NULL, "ChoosePixelFormat failed", "Error", MB_OK);
		//return FALSE;
	}

	// This sets the pixel format that we extracted from above
	if (SetPixelFormat(m_hdc, pixelformat, &pfd) == FALSE)
	{
		MessageBox(NULL, "SetPixelFormat failed", "Error", MB_OK);
		//return FALSE;
	}
}

LRESULT CALLBACK cWNDManager::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	cWNDManager* theWindow = NULL;
	switch (uMsg)
	{
	case WM_CREATE: // window creation
	{
		pInstance->m_hdc = GetDC(hWnd);
		pInstance->setupPixelFormat();

		//Set the version that we want, in this case 3.0
		int attribs[] = {
			WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
			WGL_CONTEXT_MINOR_VERSION_ARB, 0,
			0 }; //zero indicates the end of the array

		//Create temporary context so we can get a pointer to the function
		HGLRC tmpContext = wglCreateContext(pInstance->m_hdc);
		//Make it current
		wglMakeCurrent(pInstance->m_hdc, tmpContext);

		//Get the function pointer
		wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");

		//If this is NULL then OpenGL 3.0 is not supported
		if (!wglCreateContextAttribsARB)
		{
			std::cerr << "OpenGL 3.0 is not supported, falling back to GL 2.1" << std::endl;
			pInstance->m_hglrc = tmpContext;
		}
		else
		{
			// Create an OpenGL 3.0 context using the new function
			pInstance->m_hglrc = wglCreateContextAttribsARB(pInstance->m_hdc, 0, attribs);
			//Delete the temporary context
			wglDeleteContext(tmpContext);
		}
		//Make the GL3 context current
		wglMakeCurrent(pInstance->m_hdc, pInstance->m_hglrc);
		pInstance->m_isRunning = true; //Mark our window as running
	}
		break;

	case WM_DESTROY: // window destroy
	case WM_CLOSE: // windows is closing
		wglMakeCurrent(pInstance->m_hdc, NULL);
		wglDeleteContext(pInstance->m_hglrc);
		pInstance->m_isRunning = false; //Stop the main loop
		PostQuitMessage(0); //Send a WM_QUIT message
		return 0;
		break;
	case WM_SIZE:
	{
		int height = HIWORD(lParam); // retrieve width and height
		int width = LOWORD(lParam);
		pInstance->getAttachedWND()->onResize(width, height); //Call the example's resize method
	}
		break;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) //If the escape key was pressed
		{
			DestroyWindow(pInstance->m_hwnd); //Send a WM_DESTROY message
		}
		if (wParam == 98){
			txtypos--;
		}
		if (wParam == 80){
			/*if (!game){
				game = true;
			}
			else{ 
				game = false;
			}*/
		}
		if (wParam == 104){
			txtypos++;

		}if (wParam == 100){
			txtxpos--;
		}
		if (wParam == 102){
			txtxpos++;

		}
		if (wParam == 101){
			bool test = false;

		}
		if (wParam == 'o' || wParam == 'O'){
			if (!gameOver){
				gameOver = true;
				game = false;
			}
			else{
				gameOver = false;
				game = true;
			}
		}
		if (wParam == 'v' || wParam == 'V') //If d or D was pressed
		{
			drawMode = ++drawMode % 3;
			if (drawMode == 0) // fill mode
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glEnable(GL_DEPTH_TEST);
				glEnable(GL_CULL_FACE);

			}
			else if (drawMode == 1) // wireframe mode
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glDisable(GL_DEPTH_TEST);
				glDisable(GL_CULL_FACE);
			}
			else // point mode
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
				glDisable(GL_DEPTH_TEST);
				glDisable(GL_CULL_FACE);
			}
			break;
		}
		if (wParam == VK_LEFT) //If the Left Arrow key was pressed
		{

			if (!g_PlayerMoving){
				if (g_XPos < 4){
					g_XPos++;
					g_PlayerMoving = true;
				}


			}
		}
		if (wParam == VK_RIGHT) //If the Left Arrow key was pressed
		{
			if (!g_PlayerMoving){
				if (g_XPos > 0){
					g_XPos--;
					g_PlayerMoving = true;
				}
			}
		}
		if (wParam == VK_UP){
			

		}
		if (wParam == VK_TAB){
			if (debug){
				debug = false;
			}
			else{
				debug = true;
			}

		}
		if (wParam == VK_DOWN){
			
		}
		if (wParam == VK_SPACE){

			if (!game){ game = true; }
			else{
				//if player not firing and player not moving
				if (!g_PlayerFire && !g_PlayerMoving){
					g_PlayerFire = true;
				}
			}
		
		}
		if (wParam == 'C' || wParam == 'c'){
			if (camera == 0){
				camera = 1;
			}
			else{
				camera = 0;
			}
		}
		if (wParam == 'M' || wParam == 'm'){
			if (!mute){
				mute = true;
			}
			else{
				mute = false;
			}
		}
		break;
	default:
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
windowOGL* cWNDManager::getAttachedWND()
{
	return m_winOGL;
}
float cWNDManager::getElapsedSeconds()
{
	float currentTime = float(GetTickCount()) / 1000.0f;
	float seconds = float(currentTime - m_lastTime);
	m_lastTime = currentTime;
	return seconds;
}

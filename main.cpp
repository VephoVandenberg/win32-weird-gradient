#include <windows.h>

#define global static
#define internal static

global bool running;

internal LRESULT CALLBACK
window_procedure(HWND   window,
		 UINT   message,
		 WPARAM wParam,
		 LPARAM lParam );

int WINAPI
WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nShowCmd)
{

    WNDCLASSA window_class =  {};
    window_class.style = CS_VREDRAW|CS_HREDRAW;
    window_class.lpfnWndProc = window_procedure;
    window_class.hInstance = hInstance;
    window_class.lpszClassName = "WierdGradientClass";

    if (RegisterClassA(&window_class))
    {
	HWND window = CreateWindowExA(0,
				      window_class.lpszClassName,
				      "WierdGradient",
				      WS_OVERLAPPEDWINDOW|WS_VISIBLE,
				      CW_USEDEFAULT,
				      CW_USEDEFAULT,
				      CW_USEDEFAULT,
				      CW_USEDEFAULT,
				      0,
				      0,
				      hInstance,
				      0);

	
	if (window)
	{
	    running = true;
	    while (running)
	    {
		MSG message;
		while (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
		{
		    TranslateMessage(&message);
		    DispatchMessage(&message);
		}

	    }
	}
    }
    
    return 0;
}

LRESULT CALLBACK
window_procedure(HWND   window,
		 UINT   message,
		 WPARAM wParam,
		 LPARAM lParam )
{

    if (message == WM_CLOSE || message == WM_DESTROY)
    {
	running = false;
	return 0;
    }
    else
    {
	return  DefWindowProcA(window, message, wParam, lParam);
    }
}


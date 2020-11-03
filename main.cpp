#include <windows.h>

#define global static
#define internal static

global void *bitmap_memory;
global BITMAPINFO bitmap_info;
global bool running;
global int bitmap_width;
global int bitmap_height;

internal LRESULT CALLBACK
window_procedure(HWND   window,
		 UINT   message,
		 WPARAM wParam,
		 LPARAM lParam );

internal void
resize_dib_section(int width, int height, int XOffset, int YOffset);

internal void
update_window(HDC device_context,
	      RECT *window_rect);

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
    LRESULT result = 0;
    
    if (message == WM_CLOSE || message == WM_DESTROY)
    {
	running = false;
    }
    else if (message == WM_SIZE)
    {
	RECT client_rect;
	GetClientRect(window, &client_rect);
	int width = client_rect.right - client_rect.left;
	int height = client_rect.bottom - client_rect.top;
	resize_dib_section(width, height, 1, 1);
    }
    else if (message == WM_PAINT)
    {
	PAINTSTRUCT paint;
	HDC device_context = BeginPaint(window, &paint);
	
	RECT client_rect;
	GetClientRect(window, &client_rect);
	update_window(device_context, &client_rect);

	EndPaint(window, &paint);
	
    }
    else
    {
	result =  DefWindowProcA(window, message, wParam, lParam);
    }

    return result;
}



internal void
update_window(HDC device_context, RECT *client_rect)
{

    int window_width = client_rect->right - client_rect->left;
    int window_height = client_rect->bottom - client_rect->top;

    
    StretchDIBits(device_context,
		  0, 0, window_width, window_height,
		  0, 0, bitmap_width, bitmap_height,
		  bitmap_memory,
		  &bitmap_info,
		  DIB_RGB_COLORS, SRCCOPY);
}

internal void
resize_dib_section(int width, int height, int XOffset, int YOfset)
{

    bitmap_width = width;
    bitmap_height = height;

    if (bitmap_memory)
    {
	VirtualFree(bitmap_memory, 0, MEM_RELEASE);
    }

    bitmap_info = {};

    bitmap_info.bmiHeader.biSize = sizeof(bitmap_info.bmiHeader);
    bitmap_info.bmiHeader.biWidth = bitmap_width;
    bitmap_info.bmiHeader.biHeight = -bitmap_height;
    bitmap_info.bmiHeader.biPlanes = 1;
    bitmap_info.bmiHeader.biBitCount = 32;
    bitmap_info.bmiHeader.biCompression = BI_RGB;

  
    int bytes_per_pixel = 4;
    int bitmap_memory_size = bytes_per_pixel * bitmap_width * bitmap_height;

    bitmap_memory = VirtualAlloc(0, bitmap_memory_size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

    int pitch = width * bytes_per_pixel; 
    unsigned char *row = (unsigned char *)bitmap_memory;    
    for (int y = 0; y < bitmap_height; y++)
    {
	unsigned char *pixel = (unsigned char *)row;
	for (int x = 0; x < bitmap_width; x++)
	{
	    *pixel = x + XOffset;
	    pixel++;

	    *pixel = y;
	    pixel++;

	    *pixel = 0;
	    pixel++;

	    *pixel = 0;
	    pixel++;
	}

	row += pitch;
    }
}

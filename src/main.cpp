#include <iostream>

#if defined(_WIN32)

#include <windows.h>

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    MessageBoxA(NULL, "Hello from MyApp!", "MyApp", MB_OK | MB_ICONINFORMATION);
    return 0;
}

#elif defined(__APPLE__)

#include <TargetConditionals.h>
#if TARGET_OS_MAC
#include <ApplicationServices/ApplicationServices.h>

int main(int argc, char *argv[])
{
    std::cout << "Hello from MyApp (macOS)!" << std::endl;
    return 0;
}
#endif

#else

#include <X11/Xlib.h>
#include <X11/Xutil.h>

int main(int argc, char *argv[])
{
    Display *display = XOpenDisplay(NULL);
    if (!display)
    {
        std::cerr << "Cannot open display" << std::endl;
        return 1;
    }

    int screen = DefaultScreen(display);
    Window window = XCreateSimpleWindow(display, RootWindow(display, screen),
                                        0, 0, 800, 600, 1,
                                        BlackPixel(display, screen),
                                        WhitePixel(display, screen));

    XStoreName(display, window, "Empty Window");
    XSelectInput(display, window, ExposureMask | KeyPressMask);
    XMapWindow(display, window);

    XEvent event;
    while (1)
    {
        XNextEvent(display, &event);
        if (event.type == KeyPress)
        {
            break;
        }
    }

    XCloseDisplay(display);
    return 0;
}

#endif

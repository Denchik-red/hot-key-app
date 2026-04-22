#include <iostream>
#include <string>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <iostream>
#include <cstdlib>

int main() {
    Display *display = XOpenDisplay(NULL);
    if (!display) {
        std::cerr << "Не удалось открыть display" << std::endl;
        return 1;
    }

    Window root = XDefaultRootWindow(display);
    KeyCode keycode = XKeysymToKeycode(display, XStringToKeysym("L"));
    XGrabKey(display, keycode, ControlMask, root, False, GrabModeAsync, GrabModeAsync);
    XFlush(display);

    std::cout << "Глобальный hotkey Ctrl+Y зарегистрирован. Нажмите для теста." << std::endl;
    XEvent event;
    while (true) {
        XNextEvent(display, &event);
        if (event.type == KeyPress)
        {
            std::cout << "Hotkey Ctrl+Y нажат!" << std::endl;
            break;
        }
    }

    XUngrabKey(display, keycode, ControlMask, root);
    XCloseDisplay(display);
    return 0;
}

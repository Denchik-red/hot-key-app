#include <iostream>
#include <string>
#include <termios.h>
#include <unistd.h>
#include "capture_key_terminal.h"

std::string capture_key_terminal()
{
    termios old_t, new_t;
    tcgetattr(STDIN_FILENO, &old_t);
    new_t = old_t;
    cfmakeraw(&new_t);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_t);

    char c;
    read(STDIN_FILENO, &c, 1);

    tcsetattr(STDIN_FILENO, TCSANOW, &old_t);
    std::cout << "\n";
    std::cout << "input char: " << c << std::endl;
    if (c >= 0x01 && c <= 0x1A)
    {
        return "<Control>" + std::string(1, 'a' + (c - 0x01));
    }
    else if (c == 0x1B)
    { // ESC = начало Alt или спецпоследовательности
        char c2;
        read(STDIN_FILENO, &c2, 1);
        if (c2 >= 'a' && c2 <= 'z')
            return "<Alt>" + std::string(1, c2);
        if (c2 >= 'A' && c2 <= 'Z')
            return "<Alt>" + std::string(1, c2 + 32);
        return "<Alt>Key"; // fallback
    }
    else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
    {
        return std::string(1, c);
    }
    return "";
}
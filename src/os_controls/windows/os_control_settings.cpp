#include "os_control_settings.hpp"

#ifdef _WIN32
    #include <windows.h>
#else
    #include <cstdio>
    #include <unistd.h>
    #include <termios.h>
#endif

void biv::os::init_settings() {
#ifdef _WIN32
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(handle, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(handle, &cursorInfo);
#else
    // Linux: скрываем курсор
    std::printf("\033[?25l");
    std::fflush(stdout);
    
    // Настраиваем терминал
    struct termios t;
    tcgetattr(STDIN_FILENO, &t);
    t.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
#endif
}

void biv::os::set_cursor_start_position() {
#ifdef _WIN32
    COORD coord = {0, 0};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
#else
    // Linux: перемещаем курсор в (1,1)
    std::printf("\033[H");
    std::fflush(stdout);
#endif
}

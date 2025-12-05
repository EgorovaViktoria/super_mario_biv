#include "user_input.hpp"

#ifdef _WIN32
    #include <conio.h>
    #include <windows.h>
#else
    #include <unistd.h>
    #include <termios.h>
#endif

namespace biv::os {
    UserInput get_user_input() {
#ifdef _WIN32
        if (_kbhit()) {
            char ch = _getch();
            // Обработка стрелок в Windows
            if (ch == 0 || ch == 224) {
                ch = _getch();
                switch (ch) {
                    case 75: return UserInput::MAP_LEFT;   // ←
                    case 77: return UserInput::MAP_RIGHT;  // →
                    case 72: return UserInput::MARIO_JUMP; // ↑
                }
            }
            
            switch (ch) {
                case 'a': case 'A': return UserInput::MAP_RIGHT;
                case 'd': case 'D': return UserInput::MAP_LEFT;
                case 'w': case 'W': case ' ': return UserInput::MARIO_JUMP;
                case 'q': case 'Q': case 27: return UserInput::EXIT;
                default: return UserInput::NO_INPUT;
            }
        }
#else
        // Linux реализация
        char ch = 0;
        struct termios oldt, newt;
        
        // Сохраняем настройки терминала
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        
        // Отключаем канонический режим и эхо
        newt.c_lflag &= ~(ICANON | ECHO);
        newt.c_cc[VMIN] = 0;  // Не ждать ввода
        newt.c_cc[VTIME] = 0; // Нет таймаута
        
        // Применяем новые настройки
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        
        // Пытаемся прочитать символ
        if (read(STDIN_FILENO, &ch, 1) == 1) {
            // Обработка стрелок в Linux (ESC последовательности)
            if (ch == '\033') { // ESC
                char seq[3];
                if (read(STDIN_FILENO, &seq[0], 1) <= 0) {
                    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
                    return UserInput::EXIT;
                }
                if (read(STDIN_FILENO, &seq[1], 1) <= 0) {
                    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
                    return UserInput::NO_INPUT;
                }
                
                if (seq[0] == '[') {
                    switch (seq[1]) {
                        case 'A': return UserInput::MARIO_JUMP; // ↑
                        case 'B': // ↓ (если нужно)
                        case 'D': return UserInput::MAP_RIGHT;   // ←
                        case 'C': return UserInput::MAP_LEFT;  // →
                    }
                }
            }
            
            // Обработка обычных клавиш
            switch (ch) {
                case 'a': case 'A': return UserInput::MAP_RIGHT;
                case 'd': case 'D': return UserInput::MAP_LEFT;
                case 'w': case 'W': 
                case ' ':           return UserInput::MARIO_JUMP;
                case 'q': case 'Q': return UserInput::EXIT;
                default:            return UserInput::NO_INPUT;
            }
        }
        
        // Восстанавливаем настройки терминала
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
#endif
        return UserInput::NO_INPUT;
    }
}

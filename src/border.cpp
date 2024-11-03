#include "border.hpp"
#include <iostream>
#include <windows.h>
#include <conio.h>
#include <string>
#include <thread>
#include <vector>
#include <chrono>

constexpr int topLeft = 201;
constexpr int topRight = 187;
constexpr int bottomLeft = 200;
constexpr int bottomRight = 188;
constexpr int horizontal = 205;
constexpr int vertical = 186;

void setConsoleColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

int getConsoleWidth() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int width = 80;

    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    }

    return width;
}

void displayTitle() {
    setConsoleColor(9); // jasnoniebieski 

    std::cout << R"(  
     ____  _  __ _     _____               _
    |  _ \(_)/ _| |_  |_   _| __ __ _  ___| | _____ _ __ 
    | |_) | | |_| __|   | || '__/ _` |/ __| |/ / _ \ '__|
    |  _ <| |  _| |_    | || | | (_| | (__|   <  __/ |
    |_| \_\_|_|  \__|   |_||_|  \__,_|\___|_|\_\___|_|
)" << '\n';
    setConsoleColor(7); //domyslny kolor
}

void setCursorPosition(int x, int y) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD position = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
    SetConsoleCursorPosition(hConsole, position);
}

void drawFrame(int width, int height, int startX, int startY) {

    setCursorPosition(startX, startY);
    std::cout << static_cast<char>(topLeft);
    for (int i = 0; i < width - 2; ++i) {
        std::cout << static_cast<char>(horizontal);
    }
    std::cout << static_cast<char>(topRight) << '\n';

    for (int i = 0; i < height - 2; ++i) {
        setCursorPosition(startX, startY + i + 1);
        std::cout << static_cast<char>(vertical);
        for (int j = 0; j < width - 2; ++j) {
            std::cout << " ";
        }
        std::cout << static_cast<char>(vertical);
    }

    setCursorPosition(startX, startY + height - 1);
    std::cout << static_cast<char>(bottomLeft);
    for (int i = 0; i < width - 2; ++i) {
        std::cout << static_cast<char>(horizontal);
    }
    std::cout << static_cast<char>(bottomRight) << '\n';
}

void drawTextArea(int parentWidth, int parentHeight, int parentStartX, int parentStartY) {
    int textAreaWidth = static_cast<int>(parentWidth * 0.6);
    int textAreaHeight = 3;

    int startX = parentStartX + (parentWidth - textAreaWidth) / 2;
    int startY = parentStartY + 1;

    drawFrame(textAreaWidth, textAreaHeight, startX, startY);
}

std::string getInputText(int inputStartX, int inputStartY, int inputWidth) {
    setCursorPosition(inputStartX + 1, inputStartY + 1);
    std::string input;
    int cursorPos = 0;
    int displayOffset = 0;
    char ch;

    while (true) {
        setCursorPosition(inputStartX + 1, inputStartY + 1);
        std::string visibleText = input.substr(displayOffset, inputWidth - 2);
        std::cout << std::string(inputWidth - 2, ' ');
        setCursorPosition(inputStartX + 1, inputStartY + 1);
        std::cout << visibleText;

        setCursorPosition(inputStartX + 1 + (cursorPos - displayOffset), inputStartY + 1);

        ch = _getch();

        if (ch == 13) {
            break;
        }
        else if (ch == 8) {
            if (cursorPos > 0) {
                input.erase(cursorPos - 1, 1);
                cursorPos--;
                if (displayOffset > 0 && cursorPos < displayOffset) {
                    displayOffset--;
                }
            }
        }
        else if (ch == -32) {
            char arrowKey = _getch();
            switch (arrowKey) {
            case 75:
                if (cursorPos > 0) {
                    cursorPos--;
                    if (cursorPos < displayOffset) {
                        displayOffset--;
                    }
                }
                break;
            case 77:
                if (cursorPos < input.length()) {
                    cursorPos++;
                    if (cursorPos >= displayOffset + (inputWidth - 2)) {
                        displayOffset++;
                    }
                }
                break;
            default:
                break;
            }
        }
        else if (isprint(static_cast<unsigned char>(ch))) {
            input.insert(cursorPos, 1, ch);
            cursorPos++;
            if (cursorPos >= displayOffset + (inputWidth - 2)) {
                displayOffset++;
            }
        }
    }
    return input;
}

std::string cutText(const std::string& text, int maxLength) {
    if (text.length() > static_cast<size_t>(maxLength)) {
        return text.substr(0, maxLength - 3) + "...";
    }
    return text;
}

void moveCursorToBottom() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int consoleHeight = 25; 

    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        consoleHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    }

    COORD position = { 0, static_cast<SHORT>(consoleHeight - 1) };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
}


void setConsoleWindowSize(int width, int height) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    COORD bufferSize = { static_cast<SHORT>(width), static_cast<SHORT>(height) };
    SetConsoleScreenBufferSize(hConsole, bufferSize);

    SMALL_RECT windowSize = { 0, 0, static_cast<SHORT>(width - 1), static_cast<SHORT>(height - 1) };
    SetConsoleWindowInfo(hConsole, TRUE, &windowSize);
}


void playSwordAnimation(int startX, int startY) {
    using namespace std::chrono_literals;

    std::vector<std::string> symbol = {
    
        "        @@@@@@@@@@@@@                ",
        "         @@*++======@@@@@@@@           ",
        "           @@=+=====@*++++=:*@@@      ",
        "           @@=====+=@@@@@@@@@*=-@@@    ",
        "         @@@@=+++++=@%%%%%%%#@@@*=*@@  ",
        "        @@*@@=+++++=@%%%%%%%%%@#@@*=@@ ",
        "       @@*%@@=***++=@%%%%%%%%%%@@#@%+@@ ",
        "      @@+%@@@=***+*=@%%%%%%%%%%%%%#@%+@@",
        "     @@**@@@@=*****+@###%%%%%%#%%%%#@#+@@",
        "     @@#@@#@@+*****+@###**#%%###%%%%@@*@@",
        "     @@#@@%@@******+@#*##*+*###*#%%%@@#@@",
        "     @@#@@#@@+*****+@##***+**##**###@@#@@",
        "     @@##@#@@+*****+@*************#+@##@@",
        "      @@#@@@@++*+*++@**+++**++****+@@#@@ ",
        "       @%#@@@+***+++@++++++*+*+*++@@##@ ",
        "        @%%@@+++++++@+++++++***+%@@#%@  ",
        "         @@@@++++++=@@@@@@@@@@@@@@@@@@@@",
        "          @@-+++++++++++++=++++++=+*@@ ",
        "         @%-++======++=++========+*@@  ",
        "        @@@@@@@@@@@@@@@@@@@@@@@@@@@       "
    };


    const int fade_delay = 150;
    const int visible_pause = 1000;

    for (int fade_step = 0; fade_step < symbol.size(); ++fade_step) {
        for (int i = 0; i < symbol.size(); ++i) {
            setCursorPosition(startX, startY + i);
            if (i <= fade_step) {
                std::cout << std::string(symbol[i].size(), ' ');
            }
            else {
                std::cout << symbol[i];
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(fade_delay));
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(visible_pause));

    for (int fade_step = symbol.size() - 1; fade_step >= 0; --fade_step) {
        for (int i = 0; i < symbol.size(); ++i) {
            setCursorPosition(startX, startY + i);
            if (i < fade_step) {
                std::cout << std::string(symbol[i].size(), ' ');
            }
            else {
                std::cout << symbol[i];
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(fade_delay));
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(visible_pause));

    for (int i = 0; i < symbol.size(); ++i) {
        setCursorPosition(startX, startY + i);
        std::cout << std::string(symbol[i].size(), ' ');
    }
}

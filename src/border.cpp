#include "border.hpp"
#include <iostream>
#include <windows.h>
#include <conio.h>

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
    setConsoleColor(9); // Ustawienie koloru tekstu na jasnoniebieski

   // std::string title[] = {
    std::cout << R"(  
     ____  _  __ _     _____               _
    |  _ \(_)/ _| |_  |_   _| __ __ _  ___| | _____ _ __ 
    | |_) | | |_| __|   | || '__/ _` |/ __| |/ / _ \ '__|
    |  _ <| |  _| |_    | || | | (_| | (__|   <  __/ |
    |_| \_\_|_|  \__|   |_||_|  \__,_|\___|_|\_\___|_|
)" << '\n';
   // };

   // int consoleWidth = getConsoleWidth();
   // int titleWidth = title[0].length();

  /*  for (const auto& line : title) {
        // Obliczenie iloœci spacji potrzebnych do wyœrodkowania
        int padding = (consoleWidth - line.length()) / 2;
        if (padding < 0) {
            padding = 0;  // Zabezpieczenie przed ujemn¹ wartoœci¹ padding
        }
        std::cout << std::string(padding, ' ') << line << '\n';
    }
    */
    setConsoleColor(7); // Przywrócenie domyœlnego koloru
}

void setCursorPosition(int x, int y) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD position = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
    SetConsoleCursorPosition(hConsole, position);
}

void drawFrame(int width, int height, int startX, int startY) {
    if (width < 2 || height < 2) {
        std::cerr << "Width and height must be at least 2.\n";
        return;
    }

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



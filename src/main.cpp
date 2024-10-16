#include <iostream>
#include <string>
#include <windows.h> 
#include <conio.h> // obsługa getch()

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

    int topLeft = 201;
    int topRight = 187;
    int bottomLeft = 200;
    int bottomRight = 188;
    int horizontal = 205;
    int vertical = 186;

    setCursorPosition(startX, startY);

    
    std::cout << static_cast<char>(topLeft);
    for (int i = 0; i < width - 2; ++i) {
        std::cout << static_cast<char>(horizontal);
    }
    std::cout << static_cast<char>(topRight) << '\n';


    for (int i = 0; i < height - 2; ++i) {
        setCursorPosition(startX, startY + i + 1);  // Przejście do kolejnej linii
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
    // 60% szerokości bordera wysokość stała 3
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



int32_t main(int32_t _argc, char** _argv) {

    int width = 50; 
    int height = 20;
    int startX = 5;  
    int startY = 3; 

    int width2 = 50; 
    int height2 = 20; 
    int startX2 = startX + width + 5; 
    int startY2 = 3; 

    drawFrame(width, height, startX, startY);

    int nameWidth = static_cast<int>(width * 0.6); 
    int tagWidth = static_cast<int>(width * 0.3); 
    int fieldHeight = 3; 

    int nameStartX = startX + 2;
    int nameStartY = startY + 2;
    int tagStartX = nameStartX + nameWidth + 2;
    int tagStartY = nameStartY;

    drawFrame(nameWidth, fieldHeight, nameStartX, nameStartY);
    drawFrame(tagWidth, fieldHeight, tagStartX, tagStartY);

    std::string nameInput = getInputText(nameStartX, nameStartY, nameWidth);
    std::string cuttedName = cutText(nameInput, nameWidth - 2);

    std::string tagInput = getInputText(tagStartX, tagStartY, tagWidth);
    std::string cuttedTag = cutText(tagInput, tagWidth - 2);

    drawFrame(width2, height2, startX2, startY2);

   
    int NameWidth2 = nameWidth; 
    int TagWidth2 = tagWidth;  
    int NameHeight2 = 3;
    int NameStartX2 = startX2 + 2;
    int NameStartY2 = startY2 + 2;

   
    drawFrame(NameWidth2, NameHeight2, NameStartX2, NameStartY2);
    drawFrame(TagWidth2, NameHeight2, NameStartX2 + NameWidth2 + 2, NameStartY2);

   
    setCursorPosition(NameStartX2 + 1, NameStartY2 + 1);
    std::cout << cuttedName;

    setCursorPosition(NameStartX2 + NameWidth2 + 3, NameStartY2 + 1);
    std::cout << cuttedTag;

    
    setCursorPosition(0, startY + height + 1);

    return 0;
}

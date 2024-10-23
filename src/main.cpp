#include "border.hpp"
#include <iostream>

int32_t main(int32_t _argc, char** _argv) {
    int consoleWidth = 80;
    int titleHeight = 5;
    int padding = 3;

    displayTitle();


    for (int i = 0; i < padding; ++i) {
        std::cout << '\n';
    }

    int width = 50;
    int height = 20;
    int startX = 5;
    int startY = titleHeight + padding + 1;

    int width2 = 50;
    int height2 = 20;
    int startX2 = startX + width + 5;
    int startY2 = titleHeight + padding + 1;

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

    moveCursorToBottom();

    std::cout << "Nacisnij dowolny klawisz, aby zakonczyc.";

    std::cin.get();

    return 0;
}
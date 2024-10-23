#ifndef FRAME_HPP
#define FRAME_HPP

#include <string>

void setConsoleColor(int color);
int getConsoleWidth();
void displayTitle();
void setCursorPosition(int x, int y);
void drawFrame(int width, int height, int startX, int startY);
void drawTextArea(int parentWidth, int parentHeight, int parentStartX, int parentStartY);
std::string getInputText(int inputStartX, int inputStartY, int inputWidth);
std::string cutText(const std::string& text, int maxLength);
void moveCursorToBottom();

#endif // FRAME_HPP
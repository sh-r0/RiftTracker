﻿#include "display.hpp"
#include "border.hpp"
#include "networking.hpp"
#include <iostream>
#include <algorithm>
#include <string>
#define NOMINMAX
#include <windows.h>
#include <conio.h> 
#include <vector>
#include <iomanip>
#include <unordered_map>

struct CachedMatchInfo {
    std::vector<matchInfo_t> matches;
    std::unordered_map<std::string, gameInfo_t> gameDetails;
};

void displayPlayerStats(const playerInfo_t& player, int startX, int startY, int nameWidth, int tagWidth) {
    int frameWidth = nameWidth + tagWidth + 6;
    const char VERTICAL_LINE = (char)186; // kod ascii dla ║

    for (int i = 0; i < 6; i++) {
        setCursorPosition(startX, startY + i);
        std::cout << std::string(frameWidth, ' ');
    }

    try {
        summonerInfo_t summonerInfo = getSummonerInfo(player.puuid);

        for (int i = 0; i < 6; i++) {
            setCursorPosition(startX + frameWidth - 4, startY + i);
            std::cout << VERTICAL_LINE;
        }

        setCursorPosition(startX, startY);
        std::cout << "Nazwa: " << std::left << std::setw(nameWidth - 6) << player.gameName;
        setCursorPosition(startX + nameWidth - 4, startY);
        std::cout << "Tag: #" << player.tagLine;

        setCursorPosition(startX, startY + 1);
        std::cout << "Ranga: " << std::left << std::setw(nameWidth - 6) << summonerInfo.tier;
        setCursorPosition(startX + nameWidth - 4, startY + 1);
        std::cout << "Ilosc gier: " << (summonerInfo.wins + summonerInfo.losses);

        setCursorPosition(startX, startY + 2);
        float winRatio = (static_cast<float>(summonerInfo.wins) /
            (summonerInfo.wins + summonerInfo.losses)) * 100;
        std::cout << "Win ratio: " << std::fixed << std::setprecision(1) << winRatio << "%";

        setCursorPosition(startX + nameWidth - 4, startY + 2);
        std::cout << "Summoner level: " << summonerInfo.summonerLevel;
    }
    catch (const std::exception& e) {
        setCursorPosition(startX, startY + 1);
        std::cout << "Blad pobierania danych: " << e.what();
    }
}

void clearFrame(int startX, int startY, int width, int height) {
    for (int y = startY; y <= startY + height; ++y) {
        setCursorPosition(startX, y);
        std::cout << std::string(width + 1, ' ');
    }
}

void clearMatchesDisplay(int startX, int startY, int width, int height) {
    for (int i = 0; i < height; ++i) {
        setCursorPosition(startX, startY + i);
        std::cout << std::string(width, ' ');
    }
}

void clearMatchDetails(int startX, int startY, int width, int height) {
    width += 25; 
    height = 12; 

    for (int y = startY; y <= startY + height; ++y) {
        setCursorPosition(startX, y);
        std::cout << std::string(width + 1, ' ');
    }

    setCursorPosition(startX, startY + height + 1);
    std::cout << std::string(width + 1, ' ');
}

void displayMatches(const std::vector<std::string>& matchIds,
    const std::string& puuid,
    int startIndex,
    int selectedIndex,
    int startX,
    int startY,
    int frameWidth,
    CachedMatchInfo& cache) {

    frameWidth += 25;
    clearMatchesDisplay(startX, startY, frameWidth - 4, 20);

    if (cache.matches.empty()) {
        for (const auto& matchId : matchIds) {
            cache.matches.push_back(getMatchInfo(matchId, puuid));
        }
    }

    int displayCount = std::min(10, static_cast<int>(matchIds.size() - startIndex));

    setCursorPosition(startX, startY - 1);
    std::string navInfo = std::format("Strzalki: przewijanie, ENTER: szczegoly, ESC: wyjscie. Strona {}/{}",
        (startIndex / 10 + 1),
        ((matchIds.size() + 9) / 10));
    std::cout << navInfo;

    for (int i = 0; i < displayCount; ++i) {
        int currentIndex = startIndex + i;
        const auto& matchInfo = cache.matches[currentIndex];

        time_t timestamp = matchInfo.gameStartTimestamp / 1000;
        tm timeinfo;
        localtime_s(&timeinfo, &timestamp);
        char dateStr[20];
        strftime(dateStr, sizeof(dateStr), "%Y-%m-%d %H:%M", &timeinfo);

        std::string matchNumberStr = std::format("Mecz {}/{}", (currentIndex + 1), matchIds.size());
        std::string dateTimeStr = dateStr;
        std::string championStr = matchInfo.championName;
        std::string resultStr = (matchInfo.win ? "Wygrana" : "Przegrana");
        std::string kdaStr = std::format("KDA: {}/{}/{}",
            matchInfo.kills,
            matchInfo.deaths,
            matchInfo.assists);

        setCursorPosition(startX, startY + i * 2);
        if (currentIndex == selectedIndex) {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_BLUE | FOREGROUND_RED);
        }

        std::cout << std::left
            << std::setw(15) << matchNumberStr
            << std::setw(20) << dateTimeStr
            << std::setw(15) << championStr
            << std::setw(15) << resultStr
            << kdaStr;

        if (currentIndex == selectedIndex) {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
        }
    }
}

void displayMatchDetails(const std::string& matchId, int startX, int startY, int width, CachedMatchInfo& cache) {
    width += 25;
    drawFrame(width, 10, startX, startY);

    if (cache.gameDetails.find(matchId) == cache.gameDetails.end()) {
        cache.gameDetails[matchId] = getGameInfo(matchId);
    }
    const auto& gameInfo = cache.gameDetails[matchId];

    setCursorPosition(startX + 2, startY + 1);
    std::cout << "Szczegoly meczu (ESC - powrot do listy)";

    setCursorPosition(startX, startY + 2);
    std::cout << "+";
    for (int i = 0; i < width - 2; i++) {
        std::cout << "-";
    }
    std::cout << "+";

    int team1Y = startY + 3;
    setCursorPosition(startX + 2, team1Y);
    std::cout << "Druzyna 1:";

    int positions[] = { 2, 18, 38, 58, 78 };

    for (int i = 0; i < 5; i++) {
        setCursorPosition(startX + positions[i], team1Y + 1);
        std::cout << std::format("{} {}/{}/{}",
            gameInfo.blueTeam[i].champName.substr(0, 10),
            gameInfo.blueTeam[i].kills,
            gameInfo.blueTeam[i].deaths,
            gameInfo.blueTeam[i].assists);
    }

    int team2Y = startY + 6;
    setCursorPosition(startX + 2, team2Y);
    std::cout << "Druzyna 2:";

    for (int i = 0; i < 5; i++) {
        setCursorPosition(startX + positions[i], team2Y + 1);
        std::cout << std::format("{} {}/{}/{}",
            gameInfo.redTeam[i].champName.substr(0, 10),
            gameInfo.redTeam[i].kills,
            gameInfo.redTeam[i].deaths,
            gameInfo.redTeam[i].assists);
    }
}


void runApplication() {
    initClient();
    int consoleWidth = 80;
    int titleHeight = 5;
    int padding = 3;

    displayTitle();
    setConsoleWindowSize(100, 40);

    for (int i = 0; i < padding; ++i) {
        std::cout << '\n';
    }

    int width = 50;
    int height = 30;
    int startX = 5;
    int startY = titleHeight + padding + 1;

    std::cout << "         Wcisnij Enter aby rozpoczac...";
    std::cin.get();

    int symbolHeight = 20;
    height = std::max(height, symbolHeight);
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

    // druga ramka
    int rightStartX = startX + width + 5;
    drawFrame(width, height, startX + width + 5, startY);

    playSwordAnimation(rightStartX + 2, startY + 2);

    int NameWidth2 = nameWidth;
    int TagWidth2 = tagWidth;
    int NameHeight2 = 3;
    int NameStartX2 = rightStartX + 2;
    int NameStartY2 = startY + 2;

    for (int i = 0; i < NameHeight2; i++) {
        setCursorPosition(NameStartX2, NameStartY2 + i);
        std::cout << std::string(width - 4, ' ');
    }
    drawFrame(NameWidth2, NameHeight2, NameStartX2, NameStartY2);
    drawFrame(TagWidth2, NameHeight2, NameStartX2 + NameWidth2 + 2, NameStartY2);

    setCursorPosition(NameStartX2 + 1, NameStartY2 + 1);
    std::cout << cuttedName;
    setCursorPosition(NameStartX2 + NameWidth2 + 3, NameStartY2 + 1);
    std::cout << cuttedTag;

    try {
        playerInfo_t player = getPlayerInfo(cuttedName, cuttedTag);
        displayPlayerStats(player, nameStartX, nameStartY + 4, nameWidth, tagWidth);
        std::vector<std::string> matches = getMatchesList(player.puuid, "ranked");
        int matchesStartY = NameStartY2 + NameHeight2 + 2;

        if (matches.empty()) {
            setCursorPosition(NameStartX2, matchesStartY);
            std::cout << "Nie znaleziono zadnych meczy dla tego gracza.";
        }
        else {
            int currentStartIndex = 0;
            int selectedIndex = 0;
            bool running = true;
            bool showingMatchDetails = false;
            CachedMatchInfo cache; // Utworzenie cache'u

            drawFrame(width + 45, height, rightStartX, startY);
            displayMatches(matches, player.puuid, currentStartIndex, selectedIndex, NameStartX2, matchesStartY, width, cache);

            while (running) {
                if (_kbhit()) {
                    int key = _getch();
                    if (showingMatchDetails) {
                        if (key == 27) { // ESC
                            showingMatchDetails = false;
                            drawFrame(width + 45, height, rightStartX, startY);
                            displayMatches(matches, player.puuid, currentStartIndex, selectedIndex, NameStartX2, matchesStartY, width, cache);
                        }
                    }
                    else {
                        if (key == 224) { // strzałki
                            key = _getch();
                            switch (key) {
                            case 72: // góra
                                if (selectedIndex > 0) {
                                    selectedIndex--;
                                    if (selectedIndex < currentStartIndex) {
                                        currentStartIndex = (selectedIndex / 10) * 10;
                                    }
                                    displayMatches(matches, player.puuid, currentStartIndex, selectedIndex, NameStartX2, matchesStartY, width, cache);
                                }
                                break;
                            case 80: // dół
                                if (selectedIndex < matches.size() - 1) {
                                    selectedIndex++;
                                    if (selectedIndex >= currentStartIndex + 10) {
                                        currentStartIndex = (selectedIndex / 10) * 10;
                                    }
                                    displayMatches(matches, player.puuid, currentStartIndex, selectedIndex, NameStartX2, matchesStartY, width, cache);
                                }
                                break;
                            }
                        }
                        else if (key == 13) { // ENTER
                            showingMatchDetails = true;
                            clearFrame(rightStartX, startY, width + 45, height);
                            displayMatchDetails(matches[selectedIndex], rightStartX, startY, width + 20, cache);
                        }
                        else if (key == 27) { // ESC
                            running = false;
                        }
                    }
                }
            }
        }
    }
    catch (const std::exception& e) {
        setCursorPosition(NameStartX2, NameStartY2 + NameHeight2 + 2);
        std::cout << "Blad pobierania danych: " << e.what();
    }

   //  Cleanup
   //curl_easy_cleanup(curl_g);
   // curl_global_cleanup();
   // free(responseBuff);

    moveCursorToBottom();
}
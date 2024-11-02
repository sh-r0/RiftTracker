#include "networking.hpp"
#include "border.hpp"
#include <iostream>
#include <fstream>

int32_t main(int32_t _argc, char** _argv) {
    initClient();
    playerInfo_t p = getPlayerInfo("Agurin", "Euw");
    std::vector<std::string> matches = getMatchesList(p.puuid);
    for (auto match : matches) {
        matchInfo_t m = getMatchInfo(match, p.puuid);
        std::cout << std::format("{}\nkda: {}/{}/{}\n", m.win ? "victory" : "defeat", m.kills, m.deaths, m.assists);
    }

    return 0;
}
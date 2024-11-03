#include "networking.hpp"
#include "border.hpp"
#include <iostream>
#include <fstream>

int32_t main(int32_t _argc, char** _argv) {
    initClient();
    playerInfo_t p = getPlayerInfo("Agurin", "Euw");
    summonerInfo_t s = getSummonerInfo(p.puuid);
    auto matches = getMatchesList(p.puuid);
    auto gInfo = getGameInfo(matches[0]);

    //std::vector<std::string> matches = getMatchesList(p.puuid);
    //for (auto match : matches) {
    //    matchInfo_t m = getMatchInfo(match, p.puuid);
    //    std::cout << std::format("{} - {}:{}\n{}   kda: {}/{}/{}\n\n", 
    //        m.win ? "victory" : "defeat", m.gameDuration/60, m.gameDuration%60, 
    //        m.championName, m.kills, m.deaths, m.assists);
    //}

    return 0;
}
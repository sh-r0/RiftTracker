#include <fstream>
#include <iostream>
#include <format>

#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <Windows.h>
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #include <ws2def.h>
    #include <ws2ipdef.h>
    #include <wincrypt.h>
    #include <WinNls.h>
    #pragma comment (lib, "normaliz.lib")
    #pragma comment(lib, "crypt32.lib")
    #pragma comment (lib, "ws2_32.lib")
    #pragma comment (lib, "wldap32.lib")
    #pragma comment (lib, "mswsock.lib")
    #pragma comment (lib, "advapi32.lib")
#endif

#define CURL_STATICLIB
#include "curl/curl.h"
#include "json.hpp"

#include "networking.hpp"

CURL* curl_g = nullptr;
constexpr size_t rBuffSize_c = 8 * 1024 * 1024;
char* responseBuff = nullptr;
std::string apiKey_g;
size_t writtenToBuff = 0;
//                                      /riot/account/v1/accounts/by-riot-id/{gameName}/{tagLine}?api_key=${API CODE}
std::string urlBegin_c = "https://europe.api.riotgames.com";

inline void clearRBuff() {
    writtenToBuff = 0;
    ZeroMemory(responseBuff, rBuffSize_c);
    return;
}

inline nlohmann::json getResponseJson(void) {
    nlohmann::json res = nlohmann::json::parse(responseBuff);
    clearRBuff();
    return res;
}

static size_t write_data(void* ptr, size_t size, size_t nmemb, void* stream) {
    if (stream == nullptr)
        return size * nmemb;
    memcpy((char*)stream + writtenToBuff, ptr, size * nmemb);
    writtenToBuff += size * nmemb;
    return size * nmemb;
}


static void sendRequest(const std::string& _url) {
    curl_easy_setopt(curl_g, CURLOPT_URL, _url.c_str());
    curl_easy_perform(curl_g);
    return;
}

void loadApiKey(const std::string& _path) {
    std::fstream stream(_path);
    std::ostringstream ostream; ostream << stream.rdbuf();
    apiKey_g = ostream.str();
    return;
}

void initClient() {
    curl_g = curl_easy_init();
    curl_global_init(CURL_GLOBAL_ALL);
    responseBuff = (char*)malloc(rBuffSize_c);
    if (!responseBuff) {
        std::cerr << "Couldn't init response buff!\n";
        return;
    }

    ZeroMemory(responseBuff, rBuffSize_c);
    curl_easy_setopt(curl_g, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(curl_g, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curl_g, CURLOPT_HEADERDATA, nullptr);
    curl_easy_setopt(curl_g, CURLOPT_WRITEDATA, (void*)responseBuff);
    loadApiKey("api_key.txt");

    return;
}

playerInfo_t getPlayerInfo(stringCRef _name, stringCRef _tagline) {
    //https://europe.api.riotgames.com/riot/account/v1/accounts/by-riot-id/
    sendRequest(std::format("{}/riot/account/v1/accounts/by-riot-id/{}/{}?{}",
        urlBegin_c,
        _name,
        _tagline,
        std::format("api_key={}", apiKey_g)
    ));
    //sendRequest(urlBegin_c + _name + "/" + _tagline + "?api_key=" + apiKey_g);
    nlohmann::json responseJson = nlohmann::json::parse(responseBuff);
    playerInfo_t res{};
    res.puuid = responseJson.at("puuid");
    res.gameName = responseJson.at("gameName");
    res.tagLine = responseJson.at("tagLine");

    clearRBuff();
    return res;
}

std::vector<std::string> getMatchesList(stringCRef _puuid, stringCRef _queueType) {
    sendRequest(std::format("{}/lol/match/v5/matches/by-puuid/{}/ids?{}",
        urlBegin_c,
        _puuid,
        std::format("api_key={}&start=0&count=20&type=", apiKey_g, _queueType)
        ));
    nlohmann::json responseJson = nlohmann::json::parse(responseBuff);
    std::vector<std::string> res{};
    for (size_t i = 0; i < responseJson.size(); i++) {
        res.push_back(responseJson[i]);
    }

    clearRBuff();
    return res;
}

matchInfo_t getMatchInfo(stringCRef _matchId, stringCRef _puuid) {
    sendRequest(std::format("{}/lol/match/v5/matches/{}?{}",
        urlBegin_c,
        _matchId,
        std::format("api_key={}", apiKey_g)
    ));
    nlohmann::json responseJson = getResponseJson();
    size_t playerIndex = 0;
    for (; playerIndex < responseJson.at("metadata").at("participants").size(); playerIndex++) {
        if (responseJson.at("metadata").at("participants").at(playerIndex) == _puuid) break;
    }
    nlohmann::json& playerDto = responseJson.at("info").at("participants").at(playerIndex);
    matchInfo_t res{};

    //check for win
    for (auto& team : responseJson.at("info").at("teams"))
        if (team.at("teamId") == playerDto.at("teamId")) {
            res.win = team.at("win");
        }
        else continue;

    res.kills = playerDto.at("kills");
    res.deaths = playerDto.at("deaths");
    res.assists = playerDto.at("assists");

    return res;
}

/*
int32_t main35(int32_t _argc, char** _argv) {

    curl_g = curl_easy_init();
    curl_global_init(CURL_GLOBAL_ALL);
    char* responseBuff = (char*)malloc(rBuffSize_c);
    if (!responseBuff) return -1;

    ZeroMemory(responseBuff, rBuffSize_c);
    curl_easy_setopt(curl_g, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(curl_g, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curl_g, CURLOPT_HEADERDATA, nullptr);
    curl_easy_setopt(curl_g, CURLOPT_WRITEDATA, (void*)responseBuff);

    loadApiKey("api_key.txt");
    std::string gameName = "Agurin";
    std::string tagLine = "EUW";

    sendRequest(urlBegin_c + gameName + "/" + tagLine + "?api_key=" + apiKey_g);
    nlohmann::json responseJson;
    responseJson = responseBuff;
    std::cout << responseBuff;

    return 0;
}
*/
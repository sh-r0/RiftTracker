#pragma once
#include <string>
#include <vector>
#include <format>

extern std::string apiKey_g;

struct playerInfo_t {
	std::string puuid;
	std::string gameName;
	std::string tagLine;
};

struct summonerDTO_t {
	std::string accountId;
	int profileIconId;
	long revisionDate;
	std::string id; // summonerId;
	std::string puuid;
	int summonerLevel;
};

struct playerMatchDetails_t {
	std::string championName;
	int kills;
	int deaths;
	int assists;
	int teamId;
};

struct matchInfo_t {
	bool win;
	int kills, deaths, assists;
	std::string championName;
	long long gameStartTimestamp;
	std::vector<playerMatchDetails_t> teamPlayers;
};

using stringCRef = const std::string&;

void loadApiKey(stringCRef);
void initClient();
playerInfo_t getPlayerInfo(stringCRef, stringCRef);
std::vector<std::string> getMatchesList(stringCRef _puuid, stringCRef _queueType = "ranked");
matchInfo_t getMatchInfo(stringCRef _matchId, stringCRef _puuid);
//summonerDTO_t getSummonerDTO(stringCRef _puuid);




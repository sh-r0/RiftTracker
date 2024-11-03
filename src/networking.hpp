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

struct summonerInfo_t {
	std::string accountId;
	int profileIconId;
	long revisionDate;
	std::string summonerId; // summonerId;
	std::string puuid;
	int summonerLevel;
	std::string rank, tier;
	int lp;
	int32_t wins, losses;
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
	std::string matchId;
	int64_t gameDuration;
	int64_t gameStartTimestamp;
	std::string championName;
	int32_t kills, deaths, assists;
};

struct gameStat_t {
	std::string playerName;
	std::string champName;
	int32_t kills, deaths, assists;
};

struct gameInfo_t {
	std::vector<gameStat_t> blueTeam, redTeam;
};

using stringCRef = const std::string&;

void loadApiKey(stringCRef);
void initClient();
playerInfo_t getPlayerInfo(stringCRef, stringCRef);
std::vector<std::string> getMatchesList(stringCRef _puuid, stringCRef _queueType = "ranked");
matchInfo_t getMatchInfo(stringCRef _matchId, stringCRef _puuid);
gameInfo_t getGameInfo(stringCRef _matchId);
summonerInfo_t getSummonerInfo(stringCRef _puuid);

//summonerDTO_t getSummonerDTO(stringCRef _puuid);




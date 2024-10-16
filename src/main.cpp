#include <iostream>
#include <fstream>

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

CURL* curl_g = nullptr;
constexpr size_t rBuffSize_c = 8 * 1024 * 1024;
std::string apiKey_g;
size_t writtenToBuff = 0;

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

//                                      /riot/account/v1/accounts/by-riot-id/{gameName}/{tagLine}?api_key=${API CODE}
std::string urlBegin_c = "https://europe.api.riotgames.com/riot/account/v1/accounts/by-riot-id/";

int32_t main(int32_t _argc, char** _argv) {
    
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

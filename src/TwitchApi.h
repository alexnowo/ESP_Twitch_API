#ifndef TwitchApi_h
#define TwitchApi_h

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

#define ID_TWITCH   "https://id.twitch.tv/oauth2/token"
#define TWITCH_HOST "api.twitch.tv"
// Fingerprint correct as of February 12, 2021
#define TWITCH_FINGERPRINT "BC 73 A5 9C 6E EE 38 43 A6 37 FC 32 CF 08 16 DC CF F1 5A 66"
#define TWITCH_TIMEOUT 1500

struct UserData
{
    char *id;
    char *login;
    char *displayName;
    char *type;
    char *broadCasterType;
    char *description;
    char *profileImageUrl;
    char *offlineImageUrl;
    long viewCount;
    bool error;
};

struct FollowerData
{
    long total;
    char *fromId;
    char *fromName;
    char *toId;
    char *toName;
    char *followedAt;
    bool error;
};

struct StreamInfo
{
    char *id;
    char *userId;
    char *userName;
    char *gameId;
    char *type;
    char *title;
    long viewerCount;
    char *startedAt;
    char *language;
    char *thumbnailUrl;
    bool error;
};

class TwitchApi
{
  public:
    TwitchApi(char *clientId, char *secret);
    bool getAccessToken();
    bool makeGetRequestWithClientId(char *command);
    UserData getUserData(char *loginName);
    FollowerData getFollowerData(char *id);
    StreamInfo getStreamInfo(char *loginName);
    int portNumber = 443;
    //bool _checkFingerPrint = true; //Fail request if fingerprint doesnt match
    bool debug = false;
    Client *client;

  private:
    char *_clientId;
	char *_secret;
    char *_accessToken;
    HTTPClient http;
    void closeClient();
};

#endif
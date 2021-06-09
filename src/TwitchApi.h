#ifndef TwitchApi_h
#define TwitchApi_h

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

#define DEBUG

#define HOST_API_TWITCH "api.twitch.tv"
#define HOST_ID_TWITCH  "id.twitch.tv"
#define URI_ID_TWITCH   "/oauth2/token"
#define PORT 443

#define RESPONSE_ID_SIZE 192
#define ACCESS_TOKEN_SIZE 30

/* struct UserData
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
}; */

class TwitchApi
{
  public:
    TwitchApi(WiFiClientSecure &client, char *clientId, char *secret);
    bool getAccessToken();
    bool debug = false;

  private:
    WiFiClientSecure _client;
    char *_clientId;
	char *_secret;
    char _accessToken[ACCESS_TOKEN_SIZE + 1];
    HTTPClient _http;

};

#endif
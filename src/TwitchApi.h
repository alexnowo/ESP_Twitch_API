#ifndef TwitchApi_h
#define TwitchApi_h

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

#define DEBUG_TWITCH

#define HOST_API_TWITCH   "api.twitch.tv"
#define URI_SEARCH_TWITCH "/helix/streams"

#define HOST_ID_TWITCH    "id.twitch.tv"
#define URI_ID_TWITCH     "/oauth2/token"
#define PORT 443

#define RESPONSE_ID_SIZE 192
#define ACCESS_TOKEN_SIZE 30

class TwitchApi
{
  public:
    TwitchApi(WiFiClientSecure &client, char *clientId, char *secret);
    bool GetAccessToken();
    bool IsLive(char *streamerName);
    bool status = false;

  private:
    WiFiClientSecure _client;
    char *_clientId;
	char *_secret;
    char _accessToken[ACCESS_TOKEN_SIZE + 1];
    HTTPClient _http;

};

#endif
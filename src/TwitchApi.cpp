#include "TwitchApi.h"

TwitchApi::TwitchApi(WiFiClientSecure &client, char *clientId, char *secret)
{
    this->_client = client;
    this->_clientId = clientId;
	this->_secret = secret;
}

bool TwitchApi::GetAccessToken()
{    
    StaticJsonDocument<RESPONSE_ID_SIZE> response;

    String params = "client_id=";
    params += this->_clientId;
    params += "&client_secret=";
    params += this->_secret;
    params += "&grant_type=client_credentials";

    #ifdef DEBUG_TWITCH
        Serial.print(HOST_ID_TWITCH);
        Serial.print(URI_ID_TWITCH);
        Serial.print("?");
        Serial.println(params);
    #endif

    this->_http.begin(this->_client, HOST_ID_TWITCH, PORT, URI_ID_TWITCH);
    this->_http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    int16_t httpCode = this->_http.POST(params);
    
    #ifdef DEBUG_TWITCH
        Serial.print("HTTP Code: ");
        Serial.println(httpCode);
    #endif

    if(httpCode == HTTP_CODE_OK)
    {
        String payload = this->_http.getString();
        
        #ifdef DEBUG_TWITCH
            Serial.print("Payload: ");
            Serial.println(payload);
        #endif
        this->_http.end();

        DeserializationError error = deserializeJson(response, payload);
        if(error)
        {
            #ifdef DEBUG_TWITCH
                Serial.print(F("deserializeJson() failed: "));
                Serial.println(error.f_str());
            #endif
            return false;
        }
        strcpy(this->_accessToken, response["access_token"]);
        #ifdef DEBUG_TWITCH
            Serial.print("Access token: ");
            Serial.println(_accessToken);
        #endif
    }
    else
    {
        this->_http.end();
        return false;
    }
    return true;
}

bool TwitchApi::IsLive(char *streamerName)
{
    #ifdef DEBUG_TWITCH
        Serial.print("Streamer name: ");
        Serial.println(streamerName);
    #endif

    String headerAcessToken = "Bearer ";
    headerAcessToken += this->_accessToken;

    
    String path = URI_SEARCH_TWITCH;
    path += "?user_login=";
    path += streamerName;

    #ifdef DEBUG_TWITCH
        Serial.println(path);
        Serial.println("Header:");
        Serial.println(headerAcessToken);
    #endif

    this->_http.begin(this->_client, HOST_API_TWITCH, PORT, path);
    this->_http.addHeader("Authorization", headerAcessToken);
    this->_http.addHeader("Client-Id", this->_clientId);
    
    int16_t httpCode = this->_http.GET();
    #ifdef DEBUG_TWITCH
        Serial.print("HTTP Code: ");
        Serial.println(httpCode);
    #endif
    if(httpCode == HTTP_CODE_OK)
    {
        String payload = this->_http.getString();
        
        #ifdef DEBUG_TWITCH
            Serial.print("Payload: ");
            Serial.println(payload);
        #endif
        this->_http.end();

/*         DeserializationError error = deserializeJson(response, payload);
        if(error)
        {
            #ifdef DEBUG_TWITCH
                Serial.print(F("deserializeJson() failed: "));
                Serial.println(error.f_str());
            #endif
            return false;
        }
        strcpy(this->_accessToken, response["access_token"]);
        #ifdef DEBUG_TWITCH
            Serial.print("Access token: ");
            Serial.println(_accessToken);
        #endif */
    }
    else
    {
        this->_http.end();
        return false;
    }
    return true;
}
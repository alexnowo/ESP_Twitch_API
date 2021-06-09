#include "TwitchApi.h"

TwitchApi::TwitchApi(WiFiClientSecure &client, char *clientId, char *secret)
{
    this->_client = client;
    this->_clientId = clientId;
	this->_secret = secret;
}

bool TwitchApi::getAccessToken()
{    
    StaticJsonDocument<RESPONSE_ID_SIZE> response;

    String params = "client_id=";
    params += this->_clientId;
    params += "&client_secret=";
    params += this->_secret;
    params += "&grant_type=client_credentials";

    #ifdef DEBUG
        Serial.print(HOST_ID_TWITCH);
        Serial.print(URI_ID_TWITCH);
        Serial.println(params);
    #endif

    this->_http.begin(this->_client, HOST_ID_TWITCH, PORT, URI_ID_TWITCH);
    this->_http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    int16_t httpCode = this->_http.POST(params);
    
    #ifdef DEBUG
        Serial.print("HTTP Code: ");
        Serial.println(httpCode);
    #endif

    if(httpCode == HTTP_CODE_OK)
    {
        String payload = this->_http.getString();
        
        #ifdef DEBUG
            Serial.print("Payload: ");
            Serial.println(payload);
        #endif
        this->_http.end();

        DeserializationError error = deserializeJson(response, payload);
        if(error)
        {
            #ifdef DEBUG
                Serial.print(F("deserializeJson() failed: "));
                Serial.println(error.f_str());
            #endif
            return false;
        }
        strcpy(this->_accessToken, response["access_token"]);
        #ifdef DEBUG
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
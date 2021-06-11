#include "TwitchApi.h"

TwitchApi::TwitchApi(WiFiClientSecure &client, char *clientId, char *secret)
{
    this->_client = client;
    this->_clientId = clientId;
	this->_secret = secret;

    // TODO : Make secure connection with fingerprint
    this->_client.setInsecure();
    this->_client.setTimeout(5000);
}

bool TwitchApi::GetAccessToken()
{   
    bool error = false; 
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
    
    this->_http.begin(this->_client, HOST_ID_TWITCH, HTTPS_PORT, URI_ID_TWITCH);
    this->_http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    int16_t httpCode = this->_http.POST(params);
    
    #ifdef DEBUG_TWITCH
        Serial.print("HTTP Code: ");
        Serial.println(httpCode);
    #endif

    String payload = this->_http.getString();
    
    #ifdef DEBUG_TWITCH
        Serial.print("Payload: ");
        Serial.println(payload);
    #endif
    if(httpCode == HTTP_CODE_OK)
    {
        DeserializationError errorJson = deserializeJson(response, payload);
        if(errorJson)
        {
            Serial.print(F("deserializeJson() failed: "));
            Serial.println(errorJson.f_str());
            error = true;
        }
        strcpy(this->_accessToken, response["access_token"]);
        #ifdef DEBUG_TWITCH
            Serial.print("Access token: ");
            Serial.println(_accessToken);
        #endif
    }
    else
    {
        error = true;
    }

    this->_http.end();
    return error;
}

bool TwitchApi::IsLive(char *streamerName)
{
    bool error = false;

    #ifdef DEBUG_TWITCH
        Serial.print("Streamer name: ");
        Serial.println(streamerName);
    #endif

    String headerAcessToken = "Bearer ";
    headerAcessToken += this->_accessToken;

    String query = "/helix/streams?user_login=";
    query += streamerName;

    #ifdef DEBUG_TWITCH
        Serial.print(HOST_API_TWITCH);
        Serial.print(URI_SEARCH_TWITCH);
        Serial.print("?");
        Serial.println(query);
        Serial.println("Header:");
        Serial.println(headerAcessToken);
    #endif

    this->_client.connect(HOST_API_TWITCH, HTTPS_PORT);

    this->_client.print(F("GET "));
    this->_client.print(query);
    this->_client.println(F(" HTTP/1.1"));

    //Headers
    this->_client.print(F("Host: "));
    this->_client.println(HOST_API_TWITCH);

    this->_client.print(F("Client-ID: "));
    this->_client.println(_clientId);

    this->_client.print(F("Authorization: "));
    this->_client.println(headerAcessToken);

    if (this->_client.println() == 0)
    {
        Serial.println(F("Failed to send request"));
        error = true;
    }

    char status[32] = {0};
    this->_client.readBytesUntil('\r', status, sizeof(status));
    Serial.println(status);
    if (strcmp(status, "HTTP/1.1 200 OK") != 0)
    {
        Serial.print(F("Unexpected response: "));
        Serial.println(status);
        error = true;
    }

    char endOfHeaders[] = "\r\n\r\n";
    if (!this->_client.find(endOfHeaders))
    {
        Serial.println(F("Invalid response"));
        error = true;
    }
    else
    {
        StaticJsonDocument<STREAM_INFO_SIZE> root;

        // Parse JSON object
        DeserializationError errorJson = deserializeJson(root, this->_client);
        if(errorJson)
        {   
            Serial.print(F("getUserData deserializeJson() failed: "));
            Serial.println(errorJson.c_str());
            error = true;
        }
        else
        {
            JsonObject data0 = root["data"][0];
            if(data0.isNull())
            {
                this->status = false;
                #ifdef DEBUG_TWITCH
                    Serial.println(F("User offline!"));
                #endif
            }
            else
            {
                this->status = true;
                #ifdef DEBUG_TWITCH
                    Serial.println(F("User online!"));
                #endif
            }
        }
    }

    this->_client.stop();
    return error;
}
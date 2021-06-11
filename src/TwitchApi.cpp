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

    String params = "?client_id=";
    params += this->_clientId;
    params += "&client_secret=";
    params += this->_secret;
    params += "&grant_type=client_credentials";

    #ifdef DEBUG_TWITCH
        Serial.println(String("POST ") + URI_ID_TWITCH + params + String(" HTTP/1.1"));
    #endif
	
	if(client.connect(HOST_ID_TWITCH, HTTPS_PORT)) {
		this->_client.println(String("POST ") + URI_ID_TWITCH + params + String(" HTTP/1.1"));	
		
		this->_client->print(F("Host: "));
		this->_client->println(HOST_ID_TWITCH);

		this->_client->println(F("Cache-Control: no-cache"));
		this->_client->println(F("Content-Type", "application/x-www-form-urlencoded"));
	}
    
    
	while(client.available()) 
	{
		String line = client.readStringUntil('\r');
		#ifdef DEBUG_TWITCH
			Serial.print(line);
		#endif
	}
    

 /*   String payload = this->_http.getString();
    
    #ifdef DEBUG_TWITCH
        Serial.print("Payload: ");
        Serial.println(payload);
    #endif
    if(httpCode == HTTP_CODE_OK)
    {
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
        return false;
    }*/
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

    String query = "user_login=";
    query += streamerName;

    #ifdef DEBUG_TWITCH
        Serial.print(HOST_API_TWITCH);
        Serial.print(URI_SEARCH_TWITCH);
        Serial.print("?");
        Serial.println(query);
        Serial.println("Header:");
        Serial.println(headerAcessToken);
    #endif

    this->_http.begin(this->_client, HOST_API_TWITCH, PORT, URI_SEARCH_TWITCH);

    this->_http.addHeader("Authorization", headerAcessToken);
    this->_http.addHeader("Client-Id", this->_clientId);
    this->_http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    int16_t httpCode = this->_http.sendRequest("GET", query);
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
        if(payload.isEmpty())
        {
            this->status = false;
        }
        else
        {
            this->status = true;
        }
        this->_http.end();
    }
    else
    {
        this->_http.end();
        return false;
    }
    return true;
}
// dotDevice 
// A header file for sending commands from your TinyPico to your virtual dotDevice on the server
// This is a thin layer around the basic websockets stuff that adds some rate limiting on the
// messages being sent so that students don't _accidentally_ bring the server to its knees.
//
// .... of course they can still intentionally bring the server to its knees by editing this file.
// ..... but I trust them... sort of.
//
// author: stf

#include <ArduinoWebsockets.h>
#include <WiFi.h>

using namespace websockets;
WebsocketsClient client;

void onEventsCallback(WebsocketsEvent event, String data) {
    if(event == WebsocketsEvent::ConnectionOpened) {
        Serial.println("Connnection Opened");
    } else if(event == WebsocketsEvent::ConnectionClosed) {
        Serial.println("Connnection Closed");
    } else if(event == WebsocketsEvent::GotPing) {
        Serial.println("Got a Ping!");
    } else if(event == WebsocketsEvent::GotPong) {
        Serial.println("Got a Pong!");
    }
}

class dotDevice{

        public:
                dotDevice(const char* ssid, const char* password, const char* ws) {
                        _ssid = ssid;
                        _password = password;
                        _ws = ws;

                        _time_last_send = millis();
                }

                void connect() {
                        // Connect to the server
                        Serial.print("Connecting to the WiFi...\n");
                        WiFi.begin(_ssid, _password);

                        // Wait some time to connect to wifi
                        for(int i = 0; i < 10 && WiFi.status() != WL_CONNECTED; i++) {
                            Serial.print(".");
                            delay(1000);
                        }

                        Serial.print("Connected to WiFi successfully.\n");

                        // run callback when events are occuring
                        client.onEvent(onEventsCallback);

                        Serial.print("Attempting to connect to the AWS server.\n");
                        client.connect(_ws);

                        Serial.print("pinging the AWS server.\n");
                        client.ping();

                        Serial.print("Connection established\n");
                }

                void send(String s) {
                        while(_time_last_send + _rate_limit > millis()) { /* block */ }
                        client.send(s);
                }

        private: 
                const char *_ssid;
                const char *_password;
                const char *_ws;
                const char *_gid;

                unsigned long _time_last_send;
                const unsigned long _rate_limit = 50; // can't send a command less than 50ms apart. 
                
        
};

#include "dotDevice.h"


const char* ssid = "<YOUR WIFI SSID>";
const char* password = "<YOUR WIFI PASSWORD>";
const char* server = "ws://ec2-52-15-138-171.us-east-2.compute.amazonaws.com:1234"; 

const char* gid = "<YOUR GROUP GID>";

dotDevice ddev(ssid, password, server);

void setup() {
    Serial.begin(115200);
    ddev.connect();
}

String json_str;

void loop() {

    delay(3000);

    json_str = "{ \"device\" : \""+String(gid)+"\", \"cmd\" : \"SAY\", \"text\": \"Woohoo!\" }";
    Serial.println(json_str);
    ddev.send(json_str);

}

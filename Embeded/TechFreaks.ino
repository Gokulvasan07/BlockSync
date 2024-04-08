#define DEBUG_SW 1 // Make it 1 to see all debug messages in Serial Monitor
#include <IRremote.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <FirebaseESP32.h>
#include "DHT.h"

#define DHTPIN 27    // what pin we're connected to

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11 

// Initialize DHT sensor for normal 16mhz Arduino
DHT dht(DHTPIN, DHTTYPE);

// Firebase Credentials
#define FIREBASE_HOST "iot-project-2f20e-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "DCJqdn05xseUfXIve8SzxWu0wMEpkdtn1gE2WzRG"

// WiFi Credentials
#define WIFI_SSID "iQOO Z3 5G"
#define WIFI_PASSWORD "hemanth143"

// IR Receiver Pin

int IR_Recv = 12;

int status1 = 0, status2 = 0, status3 = 0, status4 = 0, status5 = 0, status6 = 0, status7 = 0, status8 = 0;

// Start Web Server

WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Decoding Results
IRrecv irrecv(IR_Recv);
decode_results results;

unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0;
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

// Function Declaration
void with_internet();
void without_internet();
void client_server();
void temperature();
// Pins of Switches
#define S5 32
#define S6 35
#define S7 34
#define S8 39

// Pins of Relay (Appliances Control)
#define R5 15
#define R6 2
#define R7 4
#define R8 22
#define R9 21

#define FAN 26
#define ALARM 25

#define RED 13
#define GREEN 12
#define BLUE 14


// Define FirebaseESP32 data object
FirebaseData firebaseData;
FirebaseJson json;

// Necessary Variables

int switch_ON_Flag1_previous_I = 0;
int switch_ON_Flag2_previous_I = 0;
int switch_ON_Flag3_previous_I = 0;
int switch_ON_Flag4_previous_I = 0;
int switch_ON_Flag5_previous_I = 0;

String Speed_Value;

void setup()
{
    // put your setup code here, to run once:
    pinMode(R5, OUTPUT);
    pinMode(R6, OUTPUT);
    pinMode(R7, OUTPUT);
    pinMode(R8, OUTPUT);
    pinMode(R9, OUTPUT);
    pinMode(FAN, OUTPUT);
    pinMode(ALARM, OUTPUT);
    pinMode(RED, OUTPUT);
    pinMode(BLUE, OUTPUT);
    pinMode(GREEN, OUTPUT);


    irrecv.enableIRIn(); // Enable IR Receiver

    Serial.begin(115200);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    dht.begin();
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
    Firebase.reconnectWiFi(true);
}

void loop()
{

    if (WiFi.status() != WL_CONNECTED)
    {
        if (DEBUG_SW)
            Serial.println("Not Connected");
        if (irrecv.decode(&results))
        {
            long int decCode = results.value;
            Serial.println(results.value);
            switch (results.value)
            {
            case 2160019894: // Press 1 to Turn on and Off
                if (status1 == 0)
                {
                    digitalWrite(R5, HIGH);
                    status1 = 1;
                }
                else if (status1 == 1)
                {
                    digitalWrite(R5, LOW);
                    status1 = 0;
                }
                break;
            case 2160052534:
                if (status2 == 0)
                {
                    digitalWrite(R6, HIGH);
                    status2 = 1;
                }
                else if (status2 == 1)
                {
                    digitalWrite(R6, LOW);
                    status2 = 0;
                }
                break;
            case 2160014284: // Press 2 to Turn on and Off
                if (status3 == 0)
                {
                    digitalWrite(R7, HIGH);
                    status3 = 1;
                }
                else if (status3 == 1)
                {
                    digitalWrite(R7, LOW);
                    status3 = 0;
                }
                break;

            case 2160030094: // Press 2 to Turn on and Off
                if (status4 == 0)
                {
                    digitalWrite(R8, HIGH);
                    status4 = 1;
                }
                else if (status4 == 1)
                {
                    digitalWrite(R8, LOW);
                    status4 = 0;
                }
                break;
            case 2160062734: // Press 2 to Turn on and Off
                if (status5 == 0)
                {
                    digitalWrite(R9, HIGH);
                    status5 = 1;
                }
                else if (status5 == 1)
                {
                    digitalWrite(R9, LOW);
                    status5 = 0;
                }
                break;
            case 2160006124: // Press 2 to Turn on and Off
                if (status6 == 0)
                {
                    digitalWrite(FAN, HIGH);
                    status6 = 1;
                }
                else if (status6 == 1)
                {
                    digitalWrite(FAN, LOW);
                    status6 = 0;
                }
                break;
            case 2160021934: // Press 2 to Turn on and Off
                if (status7 == 0)
                {
                    digitalWrite(ALARM, HIGH);
                    status7 = 1;
                }
                else if (status7 == 1)
                {
                    digitalWrite(ALARM, LOW);
                    status7 = 0;
                }
                break;
            case 2160016324: // Press 2 to Turn on and Off
                if (status8 == 0)
                {
                    digitalWrite(R5, HIGH);
                    digitalWrite(R6, HIGH);
                    digitalWrite(R7, HIGH);
                    digitalWrite(R8, HIGH);
                    digitalWrite(R9, HIGH);
                    digitalWrite(FAN, HIGH);
                    digitalWrite(ALARM, HIGH);
                    status8 = 1;
                }
                else if (status8 == 1)
                {
                    digitalWrite(R5, LOW);
                    digitalWrite(R6, LOW);
                    digitalWrite(R7, LOW);
                    digitalWrite(R8, LOW);
                    digitalWrite(R9, LOW);
                    digitalWrite(FAN, LOW);
                    digitalWrite(ALARM, LOW);
                    status8 = 0;
                }
                break;
            }
            irrecv.resume();
        }
        delay(10);
    }
    else
    {
        if (DEBUG_SW)
            Serial.println(" Connected");
        Data_from_firebase();
        temperature();
        // with_internet();
    }
}

void Data_from_firebase()
{
    if (Firebase.getString(firebaseData, "/Appliances/appliance1"))
    {
        if (DEBUG_SW)
            Serial.print("Relay1 - ");
        if (DEBUG_SW)
            Serial.println(firebaseData.stringData());

        if (firebaseData.stringData() == "1")
        {
            digitalWrite(R5, HIGH);
        }
        else
        {
            digitalWrite(R5, LOW);
        }
    }
    if (Firebase.getString(firebaseData, "/Appliances/appliance2"))
    {

        if (DEBUG_SW)
            Serial.print("Relay2 - ");
        if (DEBUG_SW)
            Serial.println(firebaseData.stringData());
        if (firebaseData.stringData() == "1")
        {
            digitalWrite(R6, HIGH);
        }
        else
        {
            digitalWrite(R6, LOW);
        }
    }

    if (Firebase.getString(firebaseData, "/Appliances/appliance3"))
    {
        if (DEBUG_SW)
            Serial.print("Relay3 - ");
        if (DEBUG_SW)
            Serial.println(firebaseData.stringData());

        if (firebaseData.stringData() == "1")
        {
            digitalWrite(R7, HIGH);
        }
        else
        {
            digitalWrite(R7, LOW);
        }
    }

    if (Firebase.getString(firebaseData, "/Appliances/appliance4"))
    {
        if (DEBUG_SW)
            Serial.print("Relay4 - ");
        if (DEBUG_SW)
            Serial.println(firebaseData.stringData());

        if (firebaseData.stringData() == "1")
        {
            digitalWrite(R8, HIGH);
        }
        else
        {
            digitalWrite(R8, LOW);
        }
    }

    if (Firebase.getString(firebaseData, "/Appliances/appliance5"))
    {
        if (DEBUG_SW)
            Serial.print("Relay5 - ");
        if (DEBUG_SW)
            Serial.println(firebaseData.stringData());

        if (firebaseData.stringData() == "1")
        {
            digitalWrite(R9, HIGH);
        }
        else
        {
            digitalWrite(R9, LOW);
        }
    }

    if (Firebase.getString(firebaseData, "/Appliances/appliance6"))
    {
        if (DEBUG_SW)
            Serial.print("Relay6 - ");
        if (DEBUG_SW)
            Serial.println(firebaseData.stringData());

        if (firebaseData.stringData() == "0")
        {
            digitalWrite(R9, HIGH);
        }
        else
        {
            digitalWrite(R9, LOW);
        }
    }

    if (Firebase.getString(firebaseData, "/Appliances/appliance7"))
    {
        if (DEBUG_SW)
            Serial.print("Relay7 - ");
        if (DEBUG_SW)
            Serial.println(firebaseData.stringData());

        if (firebaseData.stringData() == "0")
        {
            digitalWrite(R9, HIGH);
        }
        else
        {
            digitalWrite(R9, LOW);
        }
    }
}

void with_internet()
{
    // FOR SWITCH
    if (digitalRead(S5) == LOW)
    {
        if (switch_ON_Flag1_previous_I == 0)
        {
            digitalWrite(R5, HIGH);
            if (DEBUG_SW)
                Serial.println("Relay1- ON");
            String Value1 = "1";
            json.set("/appliance1", Value1);
            Firebase.updateNode(firebaseData, "/Appliances", json);
            switch_ON_Flag1_previous_I = 1;
        }
        if (DEBUG_SW)
            Serial.println("Switch1 -ON");
    }
    if (digitalRead(S5) == HIGH)
    {
        if (switch_ON_Flag1_previous_I == 1)
        {
            digitalWrite(R5, LOW);
            if (DEBUG_SW)
                Serial.println("Relay1 OFF");
            String Value1 = "0";
            json.set("/appliance1", Value1);
            Firebase.updateNode(firebaseData, "/Appliances", json);
            switch_ON_Flag1_previous_I = 0;
        }
        if (DEBUG_SW)
            Serial.println("Switch1 OFF");
    }

    if (digitalRead(S6) == LOW)
    {
        if (switch_ON_Flag2_previous_I == 0)
        {
            digitalWrite(R6, HIGH);
            if (DEBUG_SW)
                Serial.println("Relay2- ON");
            String Value2 = "1";
            json.set("/appliance2", Value2);
            Firebase.updateNode(firebaseData, "/Appliances", json);
            switch_ON_Flag2_previous_I = 1;
        }
        if (DEBUG_SW)
            Serial.println("Switch2 -ON");
    }
    if (digitalRead(S6) == HIGH)
    {
        if (switch_ON_Flag2_previous_I == 1)
        {
            digitalWrite(R6, LOW);
            if (DEBUG_SW)
                Serial.println("Relay2 OFF");
            String Value2 = "0";
            json.set("/appliance2", Value2);
            Firebase.updateNode(firebaseData, "/Appliances", json);
            switch_ON_Flag2_previous_I = 0;
        }
        if (DEBUG_SW)
            Serial.println("Switch2 OFF");
    }

    if (digitalRead(S7) == LOW)
    {
        if (switch_ON_Flag3_previous_I == 0)
        {
            digitalWrite(R7, HIGH);
            if (DEBUG_SW)
                Serial.println("Relay3- ON");
            String Value3 = "1";
            json.set("/appliance3", Value3);
            Firebase.updateNode(firebaseData, "/Appliances", json);
            switch_ON_Flag3_previous_I = 1;
        }
        if (DEBUG_SW)
            Serial.println("Switch3 -ON");
    }
    if (digitalRead(S7) == HIGH)
    {
        if (switch_ON_Flag3_previous_I == 1)
        {
            digitalWrite(R7, LOW);
            if (DEBUG_SW)
                Serial.println("Relay3 OFF");
            String Value3 = "0";
            json.set("/appliance3", Value3);
            Firebase.updateNode(firebaseData, "/Appliances", json);
            switch_ON_Flag3_previous_I = 0;
        }
        if (DEBUG_SW)
            Serial.println("Switch3 OFF");
    }

    if (digitalRead(S8) == LOW)
    {
        if (switch_ON_Flag4_previous_I == 0)
        {
            digitalWrite(R8, HIGH);
            if (DEBUG_SW)
                Serial.println("Relay4- ON");
            String Value4 = "1";
            json.set("/appliance4", Value4);
            Firebase.updateNode(firebaseData, "/Appliances", json);
            switch_ON_Flag4_previous_I = 1;
        }
        if (DEBUG_SW)
            Serial.println("Switch4 -ON");
    }
    if (digitalRead(S8) == HIGH)
    {
        if (switch_ON_Flag4_previous_I == 1)
        {
            digitalWrite(R8, LOW);
            if (DEBUG_SW)
                Serial.println("Relay4 OFF");
            String Value4 = "0";
            json.set("/appliance4", Value4);
            Firebase.updateNode(firebaseData, "/Appliances", json);
            switch_ON_Flag4_previous_I = 0;
        }
        if (DEBUG_SW)
            Serial.println("Switch4 OFF");
    }
    if (digitalRead(S8) == LOW)
    {
        if (switch_ON_Flag5_previous_I == 0)
        {
            digitalWrite(R8, HIGH);
            if (DEBUG_SW)
                Serial.println("Relay5- ON");
            String Value5 = "1";
            json.set("/appliance5", Value5);
            Firebase.updateNode(firebaseData, "/Appliances", json);
            switch_ON_Flag5_previous_I = 1;
        }
        if (DEBUG_SW)
            Serial.println("Switch5 -ON");
    }
    if (digitalRead(S8) == HIGH)
    {
        if (switch_ON_Flag5_previous_I == 1)
        {
            digitalWrite(R8, LOW);
            if (DEBUG_SW)
                Serial.println("Relay5 OFF");
            String Value5 = "0";
            json.set("/appliance5", Value5);
            Firebase.updateNode(firebaseData, "/Appliances", json);
            switch_ON_Flag5_previous_I = 0;
        }
        if (DEBUG_SW)
            Serial.println("Switch5 OFF");
    }
}

void temperature()
{
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    float f = dht.readTemperature(true);
    float heatIndex = dht.computeHeatIndex(f, h);
    json.set("/humidity", h);
    json.set("/heatIndex", heatIndex);
    json.set("/temperature", t);
    Firebase.updateNode(firebaseData, "/Appliances", json);
    if(t >= 30) 
    {
        digitalWrite(RED,LOW);
        digitalWrite(GREEN,LOW);
        digitalWrite(BLUE,HIGH);

    }
    else
    {
        digitalWrite(BLUE,LOW);
        digitalWrite(RED,HIGH);
        digitalWrite(GREEN,HIGH);

    }
}

void without_internet()
{

    if (irrecv.decode(&results))
    {
        long int decCode = results.value;
        Serial.println(results.value);
        switch (results.value)
        {
        case 2160019894: // Press 1 to Turn on and Off
            if (status1 == 0)
            {
                digitalWrite(R5, HIGH);
                status1 = 1;
            }
            else if (status1 == 1)
            {
                digitalWrite(R5, LOW);
                status1 = 0;
            }
            break;
        case 2160052534:
            if (status2 == 0)
            {
                digitalWrite(R6, HIGH);
                status2 = 1;
            }
            else if (status2 == 1)
            {
                digitalWrite(R6, LOW);
                status2 = 0;
            }
            break;
        case 2160014284: // Press 2 to Turn on and Off
            if (status3 == 0)
            {
                digitalWrite(R7, HIGH);
                status3 = 1;
            }
            else if (status3 == 1)
            {
                digitalWrite(R7, LOW);
                status3 = 0;
            }
            break;

        case 2160030094: // Press 2 to Turn on and Off
            if (status4 == 0)
            {
                digitalWrite(R8, HIGH);
                status4 = 1;
            }
            else if (status4 == 1)
            {
                digitalWrite(R8, LOW);
                status4 = 0;
            }
            break;
        case 2160062734: // Press 2 to Turn on and Off
            if (status5 == 0)
            {
                digitalWrite(R9, HIGH);
                status5 = 1;
            }
            else if (status5 == 1)
            {
                digitalWrite(R9, LOW);
                status5 = 0;
            }
            break;
        case 2160006124: // Press 2 to Turn on and Off
            if (status6 == 0)
            {
                digitalWrite(FAN, HIGH);
                status6 = 1;
            }
            else if (status6 == 1)
            {
                digitalWrite(FAN, LOW);
                status6 = 0;
            }
            break;
        case 2160021934: // Press 2 to Turn on and Off
            if (status7 == 0)
            {
                digitalWrite(ALARM, HIGH);
                status7 = 1;
            }
            else if (status7 == 1)
            {
                digitalWrite(ALARM, LOW);
                status7 = 0;
            }
            break;
        }
    }
}

void client_server()
{
    WiFiClient client = server.available(); // Listen for incoming clients
    if (client)
    { // If a new client connects,
        currentTime = millis();
        previousTime = currentTime;
        Serial.println("New Client."); // print a message out in the serial port
        String currentLine = "";       // make a String to hold incoming data from the client
        while (client.connected() && currentTime - previousTime <= timeoutTime)
        { // loop while the client's connected
            currentTime = millis();
            if (client.available())
            {                           // if there's bytes to read from the client,
                char c = client.read(); // read a byte, then
                Serial.write(c);        // print it out the serial monitor
                header += c;
                if (c == '\n')
                { // if the byte is a newline character
                    // if the current line is blank, you got two newline characters in a row.
                    // that's the end of the client HTTP request, so send a response:
                    if (currentLine.length() == 0)
                    {
                        // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
                        // and a content-type so the client knows what's coming, then a blank line:
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-type:text/html");
                        client.println("Connection: close");
                        client.println();

                        // turns the GPIOs on and off
                        if (header.indexOf("GET /1/on") >= 0)
                        {
                            Serial.println("GPIO 1 on");
                            // output26State = "on";
                            digitalWrite(R5, HIGH);
                            String Value1 = "1";
                            json.set("/appliance1", Value1);
                            Firebase.updateNode(firebaseData, "/Appliances", json);
                        }
                        else if (header.indexOf("GET /1/off") >= 0)
                        {
                            Serial.println("GPIO 1 off");
                            // output26State = "off";
                            digitalWrite(R5, LOW);
                        }
                        else if (header.indexOf("GET /2/on") >= 0)
                        {
                            Serial.println("GPIO 2 on");
                            // output27State = "on";
                            digitalWrite(R6, HIGH);
                        }
                        else if (header.indexOf("GET /2/off") >= 0)
                        {
                            Serial.println("GPIO R6 off");
                            // output27State = "off";
                            digitalWrite(R6, LOW);
                        }

                        // Display the HTML web page
                        // Break out of the while loop
                        break;
                    }
                    else
                    { // if you got a newline, then clear currentLine
                        currentLine = "";
                    }
                }
                else if (c != '\r')
                {                     // if you got anything else but a carriage return character,
                    currentLine += c; // add it to the end of the currentLine
                }
            }
        }
        // Clear the header variable
        header = "";
        // Close the connection
        client.stop();
        Serial.println("Client disconnected.");
        Serial.println("");
    }
}
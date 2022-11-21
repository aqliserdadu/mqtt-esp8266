#include <ESP8266WiFi.h>
#include <PubSubClient.h>


const char *ssid = "Hunter";
const char *password = "qwerty272";
const char *idboard = "S34R7"; 
const char *mqtt_server = "mqtt-dashboard.com";
const char *clientId = "H4rDy1Ndustri35";
const char *pubTopic = "H4rdy/S34R7_sub";
const char *subTopic = "H4rdy/S34R7_pub";
unsigned long lastMsg = 0;

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi()
{

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  String message;
  for (int i = 0; i < length; i++)
  {

    message = message + (char)payload[i]; // Conver *byte to String
  }
  Serial.print(message);
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if (message == "off")
  {

    digitalWrite(BUILTIN_LED, HIGH); // Turn the LED on (Note that LOW is the voltage level
    client.publish(pubTopic, "off|Dev01");
  }

  if (message == "on")
  {
    digitalWrite(BUILTIN_LED, LOW);
    client.publish(pubTopic, "on|Dev01");
    // Turn the LED on (Note that LOW is the voltage level
  }
}

void reconnect()
{
  // Loop until we're reconnected
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    if (client.connect(clientId))
    {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(pubTopic, "online");
      // ... and resubscribe
      client.subscribe(subTopic);
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup()
{
  pinMode(BUILTIN_LED, OUTPUT); // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop()
{

  if (!client.connected())
  {
    reconnect();
  }
  client.loop();

  unsigned long now = millis();
  if (now - lastMsg > 2000)
  {
    lastMsg = now;
    Serial.println("Publish.........");
    Serial.println("Check Device Online ");
    client.publish(pubTopic, "online");
  }
}


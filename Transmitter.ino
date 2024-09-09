#include <esp_now.h>
#include <WiFi.h>

/**Reciver Data**/
esp_now_peer_info_t slave;

/**Data That Is Going To Be Sent**/
uint8_t data = 1;


int buttonState1 = 0;
int buttonState2 = 0;
int buttonState3 = 0;

void setup() {
  Serial.begin(115200); //For Debugging

  pinMode(32, INPUT);
  pinMode(35, INPUT);
  pinMode(34, INPUT);

  WiFi.mode(WIFI_STA); //Sets MASTER(Transmitter) As A WiFi Station
  esp_now_init();
  esp_now_register_send_cb(OnDataSent);
  ScanForSlave(); //Finds The Other ESP32 Set As The Reciver
  esp_now_add_peer(&slave);

  Serial.println("Receiver Has Been Connected: Program Ready In 1 Second");
  delay(1000);
  Serial.println("Ready");
}

void loop() {
  buttonState1 = digitalRead(32);
  buttonState2 = digitalRead(35);
  buttonState3 = digitalRead(34);

  //Serial.println(buttonState1 + buttonState2 + buttonState3);

  if (buttonState1 == HIGH) {
    data = 1;
    esp_now_send(slave.peer_addr, &data, sizeof(data));
  }
  else if (buttonState2 == HIGH) {
    data = 2;
    esp_now_send(slave.peer_addr, &data, sizeof(data));
  }
  else if (buttonState3 == HIGH) {
    data = 3;
    esp_now_send(slave.peer_addr, &data, sizeof(data));
  }
  else {
    data = 0;
  }
  delay(150);
}

void ScanForSlave() {
  int8_t scanResults = WiFi.scanNetworks();

  for (int i = 0; i < scanResults; ++i) {
    String SSID = WiFi.SSID(i);
    String BSSIDstr = WiFi.BSSIDstr(i);

    if (SSID.indexOf("TARGET") == 0) {

      int mac[6];
      if (6 == sscanf(BSSIDstr.c_str(), "%x:%x:%x:%x:%x:%x", &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5])) {
        for (int j = 0; j < 6; ++j) {
          slave.peer_addr[j] = (uint8_t) mac[j];
        }
      }

      slave.channel = 1;
      slave.encrypt = 0;
      break;
    }
  }
}

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Data Has Been Sent: ");
  Serial.println(data);
}

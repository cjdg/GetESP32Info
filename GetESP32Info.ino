// Get board information for ESP32x


#define LED_PIN 2
#define BLINK_DELAY 1000

#include "esp_mac.h"

uint32_t chipId = 0;

void setup() {
  pinMode(LED_PIN,OUTPUT);
  Serial.begin(115200);
  printSeparator();

  for (int i = 0; i < 17; i = i + 8) {
    chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
  }
  Serial.printf("ESP32 Chip model = %s Rev %d\n", ESP.getChipModel(), ESP.getChipRevision());
  Serial.printf("This chip has %d cores\n", ESP.getChipCores());
  Serial.print("Chip ID: ");
  Serial.println(chipId);

  Serial.printf("Flash chip size: %d bytes \n",ESP.getFlashChipSize());
  Serial.println("Free memory: " + String(esp_get_free_heap_size()) + " bytes");
  if(psramInit())
  {
    Serial.printf("Total PSRAM: %d bytes \n", ESP.getPsramSize());
  } else {
    Serial.println("PSRAM not available");
  }

  Serial.print("Core Version: ");
  Serial.println(ESP.getCoreVersion());

  Serial.println("Interface\t\t\t\t\t\tMAC address (6 bytes, 4 universally administered, default)");

  Serial.print("Wi-Fi Station (using 'esp_efuse_mac_get_default')\t");
  Serial.println(getDefaultMacAddress());

  Serial.print("WiFi Station (using 'esp_read_mac')\t\t\t");
  Serial.println(getInterfaceMacAddress(ESP_MAC_WIFI_STA));

  Serial.print("WiFi Soft-AP (using 'esp_read_mac')\t\t\t");
  Serial.println(getInterfaceMacAddress(ESP_MAC_WIFI_SOFTAP));

  Serial.print("Bluetooth (using 'esp_read_mac')\t\t\t");
  Serial.println(getInterfaceMacAddress(ESP_MAC_BT));

  Serial.print("Ethernet (using 'esp_read_mac')\t\t\t\t");
  Serial.println(getInterfaceMacAddress(ESP_MAC_ETH));
  
  printSeparator();
  delay(1000);
}


void printSeparator()
{
  int print_chars = 160;
  for(int i=0; i < print_chars; i++)
  {
    Serial.print("=");
  }
  Serial.print('\n');

}

String getDefaultMacAddress() {

  String mac = "";

  unsigned char mac_base[6] = {0};

  if (esp_efuse_mac_get_default(mac_base) == ESP_OK) {
    char buffer[18];  // 6*2 characters for hex + 5 characters for colons + 1 character for null terminator
    sprintf(buffer, "%02X:%02X:%02X:%02X:%02X:%02X", mac_base[0], mac_base[1], mac_base[2], mac_base[3], mac_base[4], mac_base[5]);
    mac = buffer;
  }

  return mac;
}

String getInterfaceMacAddress(esp_mac_type_t interface) {

  String mac = "";

  unsigned char mac_base[6] = {0};

  if (esp_read_mac(mac_base, interface) == ESP_OK) {
    char buffer[18];  // 6*2 characters for hex + 5 characters for colons + 1 character for null terminator
    sprintf(buffer, "%02X:%02X:%02X:%02X:%02X:%02X", mac_base[0], mac_base[1], mac_base[2], mac_base[3], mac_base[4], mac_base[5]);
    mac = buffer;
  }

  return mac;
}

void loop() {
  digitalWrite(LED_PIN,HIGH);
  delay(BLINK_DELAY);
  digitalWrite(LED_PIN,LOW);
  delay(BLINK_DELAY);
  
}

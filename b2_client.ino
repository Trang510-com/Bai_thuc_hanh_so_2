#include <BLEDevice.h>

#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHAR_UUID    "beb5483e-36e1-4688-b7f5-ea07361b26a8"

static BLEUUID serviceUUID(SERVICE_UUID);
static BLEUUID charUUID(CHAR_UUID);

static bool doConnect = false;
static bool connected = false;
static BLEAdvertisedDevice *myDevice;
static BLERemoteCharacteristic *pRemoteChar;
BLEClient *pClient;
int counter = 0;

class MyClientCallback : public BLEClientCallbacks {
  void onConnect(BLEClient *c) {}
  void onDisconnect(BLEClient *c) {
    connected = false;
    Serial.println(">> MAT KET NOI voi server!");
  }
};

class MyScanCallbacks : public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice device) {
    Serial.print("Thay thiet bi: ");
    Serial.println(device.toString().c_str());

    if (device.haveServiceUUID() && device.isAdvertisingService(serviceUUID)) {
      Serial.println(">> Day la server can tim!");
      BLEDevice::getScan()->stop();
      myDevice = new BLEAdvertisedDevice(device);
      doConnect = true;
    }
  }
};

bool connectToServer() {
  Serial.println("Dang ket noi...");
  pClient = BLEDevice::createClient();
  pClient->setClientCallbacks(new MyClientCallback());

  if (!pClient->connect(myDevice)) return false;

  BLERemoteService *pService = pClient->getService(serviceUUID);
  if (pService == nullptr) {
    Serial.println("Khong tim thay Service");
    pClient->disconnect();
    return false;
  }

  pRemoteChar = pService->getCharacteristic(charUUID);
  if (pRemoteChar == nullptr) {
    Serial.println("Khong tim thay Characteristic");
    pClient->disconnect();
    return false;
  }

  Serial.println(">> KET NOI THANH CONG!");
  connected = true;
  return true;
}

void setup() {
  Serial.begin(115200);
  Serial.println("Khoi dong BLE Client...");
  BLEDevice::init("ESP32_CLIENT");
}

void loop() {
  if (doConnect) {
    connectToServer();
    doConnect = false;
  }

  if (connected) {
    String msg = "Hello lan " + String(counter++);
    pRemoteChar->writeValue(msg.c_str(), msg.length());
    Serial.print("Da ghi: ");
    Serial.println(msg);
    delay(3000);
  }
  else {
    Serial.println("Dang quet...");
    BLEScan *pScan = BLEDevice::getScan();
    pScan->setAdvertisedDeviceCallbacks(new MyScanCallbacks());
    pScan->setActiveScan(true);
    pScan->start(5, false);   
    delay(500);
  }
}
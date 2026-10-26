#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>

// UUID (có thể dùng nguyên, hoặc tạo mới tại uuidgenerator.net)
#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHAR_UUID    "beb5483e-36e1-4688-b7f5-ea07361b26a8"

BLECharacteristic *pCharacteristic;

// Báo khi có client kết nối / ngắt kết nối
class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer *pServer) {
    Serial.println(">> Client da KET NOI");
    BLEDevice::startAdvertising();
  }
  void onDisconnect(BLEServer *pServer) {
    Serial.println(">> Client MAT KET NOI -> mo lai advertising");
    delay(300);
    BLEDevice::startAdvertising();   // QUAN TRONG: mo lai advertising
  }
  void onDisconnect(BLEServer *pServer, esp_ble_gatts_cb_param_t *param) {
  Serial.printf(">> Ly do ngat (ma): 0x%02X\n", param->disconnect.reason);
  }
};

// Báo khi client GHI dữ liệu vào Characteristic
class MyCharCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pChar) {
    auto value = pChar->getValue();
    Serial.print(">> Gia tri moi nhan duoc: ");
    Serial.println(value.c_str());
  }
};

void setup() {
  Serial.begin(115200);
  Serial.println("Khoi dong BLE Server...");

  BLEDevice::init("THU_SERVER");              // tên hiển thị khi quét
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  BLEService *pService = pServer->createService(SERVICE_UUID);

  pCharacteristic = pService->createCharacteristic(
    CHAR_UUID,
    BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE
  );
  pCharacteristic->setValue("Xin chao");        // giá trị ban đầu
  pCharacteristic->setCallbacks(new MyCharCallbacks());

  pService->start();

  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  BLEDevice::startAdvertising();

  Serial.println("Server dang advertising, cho client...");
}

void loop() {
  delay(2000);
}
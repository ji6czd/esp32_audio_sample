#include <BackgroundAudio.h>
#include <ESP32I2SAudio.h>
#include <FS.h>
#include <SPIFFS.h>

const char* SoundFile[] = {
    "/cat.wav", "/dog.wav", "/cow.wav", "/sheep.wav", "/rooster.wav",
};

// ピン配置
#include <Arduino.h>
constexpr uint8_t I2S_BCLK = 41;
constexpr uint8_t I2S_DOUT = 42;
constexpr uint8_t I2S_LRCK = 43;

// 下記はグローバル変数として定義しなければならない。
ESP32I2SAudio audio(I2S_BCLK, I2S_LRCK, I2S_DOUT);  // BCLK, LRCLK, DOUT
BackgroundAudioWAV BMP(audio);

void fail() {
  while (1) {
    delay(1000);
    ESP.restart();
  }
}

void playWav(const char* filename) {
  File audioFile = SPIFFS.open(filename, "r");
  if (!audioFile) {
    Serial.println("Failed to open WAV file");
    fail();
  }
  constexpr size_t bufferSize = 4096;
  uint8_t* audioFileBuffer = new uint8_t[bufferSize];
  while (audioFile.peek() != -1) {
    int len = audioFile.read(audioFileBuffer, bufferSize);
    while (BMP.availableForWrite() < len) {
      delay(1);
    }
    BMP.write(audioFileBuffer, len);
  }
  while (!BMP.done()) {
    delay(1);
  }
  audioFile.close();
  delete audioFileBuffer;
}

void setup() {
  Serial.begin(115200);
  delay(200);  // Wait for serial monitor to open
  Serial.println("Hello!");

  // SPIFFS初期化
  if (!SPIFFS.begin(true)) {
    Serial.println("SPIFFS Mount Failed");
    return;
  }

  // WAVファイル再生の初期化
  BMP.begin();
}

void loop() {
  playWav("/dog.wav");
  delay(2000);
  playWav("/cat.wav");
  delay(2000);
  playWav("/cow.wav");
  delay(2000);
  playWav("/sheep.wav");
  delay(2000);
  playWav("/rooster.wav");
  delay(2000);
}
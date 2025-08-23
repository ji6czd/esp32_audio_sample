#include <Arduino.h>
#include <BackgroundAudio.h>
#include <ESP32I2SAudio.h>
#include <FS.h>
#include <SPIFFS.h>

char* SoundFile[] = {
    "/cat.wav", "/dog.wav", "/cow.wav", "/sheep.wav", "/rooster.wav",
};

// 下記はグローバル変数として定義しなければならない。
ESP32I2SAudio audio(D4, D5, D6);  // BCLK, LRCLK, DOUT
BackgroundAudioWAV BMP(audio);

void fail() {
  while (1) {
    delay(1000);
    ESP.restart();
  }
}

void playWav(const char* filename) {
  constexpr size_t bufferSize = 4096;
  File audioFile;
  uint8_t* audioFileBuffer = new uint8_t[bufferSize];
  audioFile = SPIFFS.open(filename, "r");
  if (!audioFile) {
    Serial.println("Failed to open WAV file");
    delete audioFileBuffer;
    fail();
  }

  while (audioFile && BMP.availableForWrite() > bufferSize) {
    int len = audioFile.read(audioFileBuffer, bufferSize);
    BMP.write(audioFileBuffer, len);
  }
  Serial.println("Close");
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
#include <M5Core2.h>  // M5Core2ライブラリをインクルード（Arduino関数も含む）

// setup() - 電源ONまたはリセット時に1回だけ実行される
// 初期化処理（ピン設定、通信開始、画面設定など）を記述
void setup() {
  M5.begin();  // M5Stack Core2を初期化
  pinMode(23, OUTPUT); // ピン7(G23)を出力に設定
}

// loop() - setup()完了後、無限ループで繰り返し実行される
// メインの処理をここに記述（自動的に繰り返されるのでwhile(true)は不要）
void loop() {
  M5.update();  // ボタン状態などを更新（今回は使わないが習慣的に入れる）
  digitalWrite(23, HIGH); // LED点灯
  delay(1000);            // 1秒待機
  digitalWrite(23, LOW);  // LED消灯
  delay(1000);            // 1秒待機
}
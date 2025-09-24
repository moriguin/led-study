#include <M5Core2.h>  // M5Core2ライブラリをインクルード（Arduino関数も含む）

// setup() - 電源ONまたはリセット時に1回だけ実行される
// 初期化処理（ピン設定、通信開始、画面設定など）を記述
void setup() {
  M5.begin();  // M5Stack Core2を初期化
  M5.Lcd.setTextSize(3);  // 文字サイズを3倍に設定
  M5.Lcd.setTextColor(WHITE, BLACK);  // 白文字、黒背景に設定
  M5.Lcd.clear();  // 画面をクリア
  M5.Lcd.setCursor(60, 100);  // 表示位置を(60, 100)に設定
  M5.Lcd.println("Hello World");  // "Hello World"を表示
}

// loop() - setup()完了後、無限ループで繰り返し実行される
// メインの処理をここに記述（自動的に繰り返されるのでwhile(true)は不要）
void loop() {
  M5.update();  // ボタン状態などを更新（今回は使わないが習慣的に入れる）
}
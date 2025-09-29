// M5Stack Core2 + Servo2モジュールでサーボを自動制御するプログラム
// 電源ONで自動的に一定間隔でサーボが動く

#include <M5Core2.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// === 基本設定（変更不要） ===
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40, Wire1);
#define SERVOMIN  102    // 0度の位置
#define SERVOMAX  512    // 180度の位置
#define SERVO_FREQ 50     // サーボの周波数

// === 使用するサーボの番号 ===
const int SERVO_CH = 0;  // 1番目のコネクタに接続（0～15の番号）

// === 便利な関数：角度を指定してサーボを動かす ===
// 使い方: moveServo(90); で90度に移動
void moveServo(int angle) {
  // 角度を0～180度の範囲に制限
  if (angle < 0) angle = 0;
  if (angle > 180) angle = 180;

  // 角度をサーボの制御値に変換して動かす
  int pulse = map(angle, 0, 180, SERVOMIN, SERVOMAX);
  pwm.setPWM(SERVO_CH, 0, pulse);
}

// === 初期設定（電源ON時に1回だけ実行） ===
void setup() {
  // M5Core2を起動
  M5.begin(true, true, true, true, kMBusModeInput);

  // 画面の準備
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.setTextSize(2);

  // タイトル表示
  M5.Lcd.setCursor(10, 10);
  M5.Lcd.println("Servo2 Auto Control");

  // Servo2モジュールを起動
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ);
  delay(100);

  // サーボを中央（90度）に移動
  moveServo(90);

  // 動作説明を表示
  M5.Lcd.setCursor(10, 50);
  M5.Lcd.println("Auto movement:");
  M5.Lcd.setCursor(10, 80);
  M5.Lcd.println("0 -> 90 -> 180 -> 90");

  delay(1000);
}

// === メインループ（繰り返し実行される） ===
void loop() {
  // 0度に移動
  moveServo(0);
  M5.Lcd.fillRect(10, 120, 200, 30, BLACK);
  M5.Lcd.setCursor(10, 120);
  M5.Lcd.println("Position: 0 degrees");
  delay(2000);  // 2秒待機

  // 90度に移動
  moveServo(90);
  M5.Lcd.fillRect(10, 120, 200, 30, BLACK);
  M5.Lcd.setCursor(10, 120);
  M5.Lcd.println("Position: 90 degrees");
  delay(2000);  // 2秒待機

  // 180度に移動
  moveServo(180);
  M5.Lcd.fillRect(10, 120, 200, 30, BLACK);
  M5.Lcd.setCursor(10, 120);
  M5.Lcd.println("Position: 180 degrees");
  delay(2000);  // 2秒待機

  // 90度に戻る
  moveServo(90);
  M5.Lcd.fillRect(10, 120, 200, 30, BLACK);
  M5.Lcd.setCursor(10, 120);
  M5.Lcd.println("Position: 90 degrees");
  delay(2000);  // 2秒待機
}

/* ===================================
   接続方法：
   1. Servo2モジュールとM5Core2を合体
   2. サーボをServo2の1番目のコネクタに接続
   3. Servo2モジュールに外部電源を接続（DC 5-6V）

   動作：
   - 起動時：サーボが90度（中央）に移動
   - ボタンA：0度（左端）に移動
   - ボタンB：90度（中央）に移動
   - ボタンC：180度（右端）に移動
   =================================== */
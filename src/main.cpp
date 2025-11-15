// =====================================================
// 四足歩行ロボット - 斜対歩（トロット歩容）プログラム
// =====================================================
// M5Stack Core2 for AWS + Servo2 モジュールを使用した
// 四足歩行ロボットの制御プログラムです。
//
// 【ハードウェア構成】
// - M5Stack Core2 for AWS: メインコントローラー
// - Servo2 モジュール: PCA9685チップ搭載、I2C制御
// - サーボモーター x 4個: 各足に1個ずつ
//
// 【サーボの配置】
// - CH0: 右前足（Right Front）
// - CH1: 右後ろ足（Right Rear）
// - CH2: 左前足（Left Front）
// - CH3: 左後ろ足（Left Rear）
//
// 【歩行方式：斜対歩（トロット歩容）】
// 対角線上にある足を同時に動かす歩き方です。
// - グループA: 右前 + 左後ろ を同時に動かす
// - グループB: 左前 + 右後ろ を同時に動かす
// これを交互に繰り返すことで前進します。
//
// 【動作の流れ】
// 1. グループAを持ち上げる → 前に振る → 下ろす
// 2. グループBを持ち上げる → 前に振る → 下ろす
// 3. 繰り返し
// =====================================================

#include <Arduino.h>
#include <M5Unified.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <Avatar.h>
#include "quadruped/quadruped.h"

// PWMドライバーのインスタンス作成
// アドレス0x40、I2C通信はWire1を使用
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40, Wire1);

// Stack-chan 顔インスタンス
using namespace m5avatar;
Avatar avatar;

void setup() {
  // M5Stack Core2の初期化
  auto cfg = M5.config();
  M5.begin(cfg);

  Serial.begin(115200);

  // I2C通信の初期化（GPIO32=SDA, GPIO33=SCL）
  Wire1.begin(32, 33);

  // 画面表示の設定
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(10, 10);
  M5.Lcd.println("Quadruped Robot");
  M5.Lcd.println("Initializing...");

  // PWMドライバー（PCA9685）の初期化
  // 50Hzでサーボモーターを制御
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);  // 内部クロック周波数
  pwm.setPWMFreq(50);  // サーボ用の周波数（50Hz = 20ms周期）
  initState();// サーボモーターの初期位置設定
  Serial.println("setup initstate done");
  delay(1000);  // 初期位置に移動するまで待機

  M5.Lcd.println("Ready!");
  Serial.println("Quadruped robot initialized");
  Serial.println("Starting diagonal gait...");

  avatar.init();  // アニメーション開始（瞬き自動）
  M5.Lcd.fillRect(240, 5, 80, 30, BLACK);
}

enum State { OFF, WALKING };
State robotState = OFF;
bool wasTouching = false; // タッチ検出時の連続トリガー防止

void loop() {
  M5.update();
  bool touching = (M5.Touch.getCount() > 0);
  if (touching) {
    // タッチ開始の瞬間を検出
    if (touching && !wasTouching) {
      robotState = (robotState == OFF) ? WALKING : OFF;
      if (robotState == OFF) {
        initState();
      }
      avatar.setExpression(Expression::Happy);
      avatar.setSpeechText(robotState == State::WALKING ? "WALKING!" : "STOP!");
      Serial.println(robotState == State::WALKING ? "START WALKING" : "STOPPED");
    }
  }
  wasTouching = touching;

  if (robotState == WALKING) {
    trotGaitStep();
  }

  // // タッチで Avatar 表情変更（おまけ）
  // if (M5.Touch.getCount() > 0) {
  //   avatar.setExpression(Expression::Happy);
  //   delay(500);
  //   avatar.setExpression(Expression::Neutral);
  // }
}
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

  // 全てのサーボを初期位置（90度、直立）にセット
  int channels[] = {0, 1, 2, 3};  // 4本の足に対応するチャンネル
  int midPulse = 300;  // 90度に相当するPWM値

  for(int i = 0; i < 4; i++) {
    pwm.setPWM(channels[i], 0, 250);
  }

  delay(2000);  // 初期位置に移動するまで待機

  for(int i = 0; i < 4; i++) {
    pwm.setPWM(channels[i], 0, midPulse);
  }

  delay(1000);  // 初期位置に移動するまで待機

  M5.Lcd.println("Ready!");
  Serial.println("Quadruped robot initialized");
  Serial.println("Starting diagonal gait...");

  avatar.init();  // アニメーション開始（瞬き自動）
}

enum State { OFF, WALKING }; // 名前付き
State robotState = OFF;
bool wasTouching = false; // タッチ検出時の連続トリガー防止

void loop() {
  M5.update();

  bool touching = (M5.Touch.getCount() > 0);
  if (touching) {
    // タッチ開始の瞬間を検出
    if (touching && !wasTouching) {
      robotState = (robotState == OFF) ? WALKING : OFF;
      avatar.setExpression(Expression::Happy);
      avatar.setSpeechText(robotState == State::WALKING ? "WALKING!" : "OFF!");
      Serial.println(robotState == State::WALKING ? "START WALKING" : "STOPPED");
    }
  }
  wasTouching = touching;

  if (robotState == WALKING) {
    // TODO 他のクラスに定義した歩行関数を呼び出す
  }

  // // タッチで Avatar 表情変更（おまけ）
  // if (M5.Touch.getCount() > 0) {
  //   avatar.setExpression(Expression::Happy);
  //   delay(500);
  //   avatar.setExpression(Expression::Neutral);
  // }

  // ==========================================
  // 四足歩行ロボット - 斜対歩（トロット歩容）
  // ==========================================

  // --- サーボの配置定義 ---
  // チャンネル番号と足の対応関係
  int ch_rightFront = 0;   // CH0: 右前足
  int ch_rightRear = 1;    // CH1: 右後ろ足
  int ch_leftFront = 2;    // CH2: 左前足
  int ch_leftRear = 3;     // CH3: 左後ろ足

  // --- サーボの角度（PWM値）定義 ---
  // ※お使いのサーボの特性に合わせて調整してください
  int midPulse = 300;    // 90度：直立位置（初期位置）
  int liftPulse = 250;   // 60度：足を持ち上げた位置（地面から離す）
  int forwardPulse = 350; // 120度：足を前に振った位置（前進）

  // --- 動作のタイミング設定 ---
  int stepDelay = 800;   // 各動作の間隔（ミリ秒）ゆっくり歩くための設定

  // ==========================================
  // ステップ1: グループA（右前 + 左後ろ）を動かす
  // ==========================================

  // 1-1: グループAの足を持ち上げる
  // 右前足と左後ろ足を同時に地面から離します
  M5.Lcd.fillRect(0, 80, 320, 20, BLACK);
  M5.Lcd.setCursor(10, 80);
  M5.Lcd.println("Step: RF+LR Lift");
  Serial.println("Step 1-1: Right Front + Left Rear - Lift");

  pwm.setPWM(ch_rightFront, 0, liftPulse);  // 右前足を持ち上げ
  pwm.setPWM(ch_leftRear, 0, liftPulse);    // 左後ろ足を持ち上げ
  delay(stepDelay);

  // 1-2: グループAの足を前に振る
  // 持ち上げた足を前方に移動させます
  M5.Lcd.fillRect(0, 80, 320, 20, BLACK);
  M5.Lcd.setCursor(10, 80);
  M5.Lcd.println("Step: RF+LR Forward");
  Serial.println("Step 1-2: Right Front + Left Rear - Forward");

  pwm.setPWM(ch_rightFront, 0, forwardPulse);  // 右前足を前へ
  pwm.setPWM(ch_leftRear, 0, forwardPulse);    // 左後ろ足を前へ
  delay(stepDelay);

  // 1-3: グループAの足を下ろす
  // 前に出した足を地面に着地させます
  M5.Lcd.fillRect(0, 80, 320, 20, BLACK);
  M5.Lcd.setCursor(10, 80);
  M5.Lcd.println("Step: RF+LR Down");
  Serial.println("Step 1-3: Right Front + Left Rear - Down");

  pwm.setPWM(ch_rightFront, 0, midPulse);  // 右前足を着地
  pwm.setPWM(ch_leftRear, 0, midPulse);    // 左後ろ足を着地
  delay(stepDelay);

  // ==========================================
  // ステップ2: グループB（左前 + 右後ろ）を動かす
  // ==========================================

  // 2-1: グループBの足を持ち上げる
  // 左前足と右後ろ足を同時に地面から離します
  M5.Lcd.fillRect(0, 80, 320, 20, BLACK);
  M5.Lcd.setCursor(10, 80);
  M5.Lcd.println("Step: LF+RR Lift");
  Serial.println("Step 2-1: Left Front + Right Rear - Lift");

  pwm.setPWM(ch_leftFront, 0, liftPulse);  // 左前足を持ち上げ
  pwm.setPWM(ch_rightRear, 0, liftPulse);  // 右後ろ足を持ち上げ
  delay(stepDelay);

  // 2-2: グループBの足を前に振る
  // 持ち上げた足を前方に移動させます
  M5.Lcd.fillRect(0, 80, 320, 20, BLACK);
  M5.Lcd.setCursor(10, 80);
  M5.Lcd.println("Step: LF+RR Forward");
  Serial.println("Step 2-2: Left Front + Right Rear - Forward");

  pwm.setPWM(ch_leftFront, 0, forwardPulse);  // 左前足を前へ
  pwm.setPWM(ch_rightRear, 0, forwardPulse);  // 右後ろ足を前へ
  delay(stepDelay);

  // 2-3: グループBの足を下ろす
  // 前に出した足を地面に着地させます
  M5.Lcd.fillRect(0, 80, 320, 20, BLACK);
  M5.Lcd.setCursor(10, 80);
  M5.Lcd.println("Step: LF+RR Down");
  Serial.println("Step 2-3: Left Front + Right Rear - Down");

  pwm.setPWM(ch_leftFront, 0, midPulse);   // 左前足を着地
  pwm.setPWM(ch_rightRear, 0, midPulse);   // 右後ろ足を着地
  delay(stepDelay);

  // このサイクルが繰り返されることで、ロボットが斜対歩で前進します
  // loop()関数なので、自動的に最初に戻って繰り返されます
}
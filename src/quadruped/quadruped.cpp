// src/quadruped/quadruped.cpp
#include "quadruped.h"
#include <M5Unified.h>

// ==========================================
// サーボの配置定義
// ==========================================
#define CH_RF 0  // 右前足（Right Front）
#define CH_RR 1  // 右後ろ足（Right Rear）
#define CH_LF 2  // 左前足（Left Front）
#define CH_LR 3  // 左後ろ足（Left Rear）

// ==========================================
// サーボの角度（PWM値）定義
// ※お使いのサーボの特性に合わせて調整してください
// ==========================================
const int MID_PULSE     = 300;  // 90度：直立位置（初期位置）
const int LIFT_PULSE    = 250;  // 60度：足を持ち上げた位置（地面から離す）
const int FORWARD_PULSE = 350;  // 120度：足を前に振った位置（前進）

// ==========================================
// 動作のタイミング設定
// ==========================================
const int STEP_DELAY = 100;  // 各動作の間隔（ミリ秒）ゆっくり歩くための設定

// ==========================================
// ステップ管理
// ==========================================
static int step = 0;  // 0〜5の6ステップで1サイクル

// ==========================================
// 四足歩行ロボット - 斜対歩（トロット歩容）
// ==========================================
void trotGaitStep() {
  // ==========================================
  // ステップ1: グループA（右前 + 左後ろ）を動かす
  // ==========================================
  if (step == 0) {
    // 1-1: グループAの足を後方(60度)に持ち上げる
    // 右前足と左後ろ足を同時に地面から離します
    Serial.println("Step 1-1: Right Front + Left Rear - Lift");
    pwm.setPWM(CH_RF, 0, LIFT_PULSE);
    pwm.setPWM(CH_LR, 0, LIFT_PULSE);
  }

  else if (step == 1) {
    // 1-2: グループAの足を前に振る
    // 持ち上げた足を前方に移動させます
    Serial.println("Step 1-2: Right Front + Left Rear - Forward");
    pwm.setPWM(CH_RF, 0, FORWARD_PULSE);
    pwm.setPWM(CH_LR, 0, FORWARD_PULSE);
  }

  else if (step == 2) {
    // 1-3: グループAの足を下ろす
    // 前に出した足を地面に着地させます
    Serial.println("Step 1-3: Right Front + Left Rear - Down");
    pwm.setPWM(CH_RF, 0, MID_PULSE);
    pwm.setPWM(CH_LR, 0, MID_PULSE);
  }

  // ==========================================
  // ステップ2: グループB（左前 + 右後ろ）を動かす
  // ==========================================
  else if (step == 3) {
    // 2-1: グループBの足を持ち上げる
    // 左前足と右後ろ足を同時に地面から離します
    Serial.println("Step 2-1: Left Front + Right Rear - Lift");
    pwm.setPWM(CH_LF, 0, LIFT_PULSE);
    pwm.setPWM(CH_RR, 0, LIFT_PULSE);
  }

  else if (step == 4) {
    // 2-2: グループBの足を前に振る
    // 持ち上げた足を前方に移動させます
    Serial.println("Step 2-2: Left Front + Right Rear - Forward");
    pwm.setPWM(CH_LF, 0, FORWARD_PULSE);
    pwm.setPWM(CH_RR, 0, FORWARD_PULSE);
  }

  else if (step == 5) {
    // 2-3: グループBの足を下ろす
    // 前に出した足を地面に着地させます
    Serial.println("Step 2-3: Left Front + Right Rear - Down");
    pwm.setPWM(CH_LF, 0, MID_PULSE);
    pwm.setPWM(CH_RR, 0, MID_PULSE);
  }

  // 次のステップへ
  delay(STEP_DELAY);
  step = (step + 1) % 6;  // 6ステップで1サイクル
}

void initState() {
  const int channels[] = {0, 1, 2, 3};
  step = 0;

  for(int i = 0; i < 4; i++) {
    pwm.setPWM(channels[i], 0, LIFT_PULSE);
  }

  delay(2000);  // 初期位置に移動するまで待機

  for(int i = 0; i < 4; i++) {
    pwm.setPWM(channels[i], 0, MID_PULSE);
  }
  delay(2000);  // 初期位置に移動するまで待機
}
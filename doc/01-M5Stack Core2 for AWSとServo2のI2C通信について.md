# M5Stack Core2 for AWSとServo2のI2C通信について

## 質問

M5Stack Core2 for AWSとServo2は、物理的に接続されると思うんだけど、二つのデバイスは必ずI2C通信するんですね？その通信のために、Wire.hライブラリ？（これはArduinoが提供？）を使用するの？

## 回答

はい、その理解で正解です！詳しく説明しますね。

### **物理接続とI2C通信の関係：**

#### **1. 物理接続（M-Busスタック方式）**
```
横から見た図：
    M5Core2 for AWS
┌─────────────────┐
│   [液晶画面]    │
└─────────────────┘
    ↓↑ M-Busコネクタで直接スタック
┌─────────────────┐
│ Servo2モジュール│
└─────────────────┘
```

M-Busコネクタには多数のピンがあり、その中に：
- VCC (電源)
- GND (グランド)
- GPIO32/SDA (I2Cデータ線)
- GPIO33/SCL (I2Cクロック線)
が含まれています。

#### **2. 通信方式**
- **GPIO32/33**（Wire1）でI2C通信を行う
- **M-Bus経由**で電源も供給
- **Groveケーブルは不要**（底面で直接接続）

### **I2C通信が必須な理由：**

Servo2モジュールは**PCA9685チップ**を搭載しており、このチップは**I2C通信専用**です。

```
M5Core2 → [I2C命令] → Servo2(PCA9685) → [PWM信号] → サーボモーター
```

- M5Core2からServo2への指令はI2C通信のみ
- Servo2からサーボへの制御はPWM信号

### **Wire.hライブラリについて：**

#### **提供元：Arduino**
```cpp
#include <Wire.h>  // Arduino標準ライブラリ
```

#### **役割：**
- **I2C通信の基本機能**を提供
- **マスター/スレーブ通信**の制御
- **データの送受信**

#### **使用例：**
```cpp
Wire1.begin();           // I2C初期化
Wire1.beginTransmission(0x40);  // アドレス0x40に送信開始
Wire1.write(データ);      // データ送信
Wire1.endTransmission(); // 送信終了
```

### **なぜAdafruit_PWMServoDriverも必要？**

```cpp
Wire.h              // I2C通信の「文法」
↓
Adafruit_PWMServoDriver.h  // PCA9685との「会話内容」
```

- **Wire.h**: I2C通信の基本機能
- **Adafruit_PWMServoDriver.h**: PCA9685チップ専用の制御コマンド

**結論**: Servo2モジュールとの通信は必ずI2C、そのためにArduino標準のWire.hライブラリを使用します！
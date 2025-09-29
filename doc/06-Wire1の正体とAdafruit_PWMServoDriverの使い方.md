# Wire1の正体とAdafruit_PWMServoDriverの使い方

## 質問
Wire1って、文字列でもない上にどこにも変数宣言されていないのに突然でてきたから驚くけど、これはどういうことでしょう？
Adafruit_PWMServoDriverを最初に宣言する必要があって、第一引数と第二引数には何を指定する必要があって、みたいな説明できる？

## 回答

素晴らしい疑問です！確かに`Wire1`が突然出てきて謎ですよね。

### **Wire1の正体**

`Wire1`は**事前に定義済みのグローバルオブジェクト**です。

```cpp
// 実はM5Core2.hの中で既に定義されている！
// （見えないけど存在する）
TwoWire Wire1 = TwoWire(1);  // これが既に作られている
```

#### **どこで定義されているか**

```cpp
#include <M5Core2.h>  // ← この中で Wire1 が用意される！
```

M5Core2.hをインクルードすると、自動的に：
1. `Wire` オブジェクト（内部I2C用）
2. `Wire1` オブジェクト（Port A用）← これ！

が使えるようになります。

#### **Javaで例えると**

```java
// Javaなら
public class System {
    public static final PrintStream out = new PrintStream();  // 既に存在
}
// だから System.out.println() が使える

// C++も同じ
// Wire1 は既に存在するから、いきなり使える
```

### **Adafruit_PWMServoDriverの詳細説明**

#### **コンストラクタ（作成時の設定）**

```cpp
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(アドレス, I2Cオブジェクト);
```

#### **引数の詳細**

| 引数 | 型 | 説明 | 例 |
|---|---|---|---|
| 第1引数 | `uint8_t` | I2Cアドレス（0x40～0x7F） | `0x40` |
| 第2引数 | `TwoWire&` | I2Cオブジェクトの参照 | `Wire1` |

#### **具体例で説明**

```cpp
// パターン1：デフォルト（引数なし）
Adafruit_PWMServoDriver pwm1 = Adafruit_PWMServoDriver();
// → アドレス0x40、Wire（デフォルト）を使用

// パターン2：アドレスのみ指定
Adafruit_PWMServoDriver pwm2 = Adafruit_PWMServoDriver(0x41);
// → アドレス0x41、Wire（デフォルト）を使用

// パターン3：アドレスとI2Cバス指定（今回のパターン）
Adafruit_PWMServoDriver pwm3 = Adafruit_PWMServoDriver(0x40, Wire1);
// → アドレス0x40、Wire1（Port A）を使用
```

### **なぜWire1を指定する必要があるか？**

#### **M5Core2には2つのI2Cバスがある**

```cpp
Wire   // 内部I2C（GPIO21/22）- タッチパネル等が使用中
Wire1  // 外部I2C（GPIO32/33）- Port A、M-Bus用 ← Servo2はこっち！
```

#### **間違えるとどうなる？**

```cpp
// 間違い例
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40, Wire);
// → 内部I2Cを使おうとする → Servo2と通信できない！

// 正しい
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40, Wire1);
// → 外部I2C（M-Bus）を使う → Servo2と通信OK！
```

### **第1引数（I2Cアドレス）について**

#### **0x40とは？**

- **16進数表記**のアドレス
- 10進数だと64
- I2Cデバイスの「住所」

#### **なぜアドレスが必要？**

```
I2Cバス上に複数のデバイスがある場合：

Wire1 ─┬─ Servo2モジュール（0x40）    ← これと話したい！
       ├─ センサーA（0x50）
       └─ センサーB（0x60）
```

アドレスを指定することで、特定のデバイスとだけ通信できます。

### **実際のコードの流れ**

```cpp
// 1. M5Core2.hをインクルード
#include <M5Core2.h>  // これでWire1が使える状態になる

// 2. Adafruitライブラリをインクルード
#include <Adafruit_PWMServoDriver.h>

// 3. PWMドライバーオブジェクトを作成
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40, Wire1);
//                                                      ↑     ↑
//                                                   アドレス  I2Cバス

// 4. 初期化して使用開始
void setup() {
    M5.begin();      // M5Core2とWire1の初期化
    pwm.begin();     // Servo2モジュールの初期化
}
```

### **よくある疑問と答え**

#### **Q: Wire1はどこで作られる？**
A: M5Core2.hの中で自動的に作られます（Arduino方式）

#### **Q: なぜ文字列じゃないの？**
A: Wire1は**オブジェクト名**（変数名）だから

#### **Q: 他の書き方は？**
```cpp
// アドレスを変数にする
uint8_t servo_address = 0x40;
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(servo_address, Wire1);

// ポインタで渡すことも可能
TwoWire* i2c_bus = &Wire1;
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40, *i2c_bus);
```

### **まとめ**

1. **Wire1**は M5Core2.h で既に定義済み（見えないけど存在）
2. **第1引数（0x40）**：通信相手のアドレス
3. **第2引数（Wire1）**：使うI2Cバス（通信路）
4. M5Core2では必ず**Wire1**を使う（Wire は内部用で使用中）

JavaのSystem.outのように、**最初から用意されているオブジェクト**だと思えばOKです！
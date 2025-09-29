# C++のライブラリとincludeの仕組み

## 質問
C++のライブラリ？今回使っているライブラリについて教えてくれる？Javaなら、import文のパッケージ名に組織名がつくけど、C++だと分かんないよね？

## 回答

その通りです！C++のライブラリの仕組みはJavaとは大きく異なります。

### **JavaとC++の違い**

#### **Java**
```java
import com.adafruit.drivers.PWMServoDriver;  // 組織名が明確
import org.m5stack.core.M5Core2;             // 階層構造が分かる
```

#### **C++**
```cpp
#include <Adafruit_PWMServoDriver.h>  // 誰が作った？
#include <M5Core2.h>                  // どこから来た？
#include <Wire.h>                     // これは何？
```

### **今回使っているライブラリの詳細**

| ライブラリ | 作成者 | 種類 | 実際の場所 |
|---|---|---|---|
| `M5Core2.h` | M5Stack社 | 外部ライブラリ | `.pio/libdeps/m5stack-core2/M5Core2/` |
| `Wire.h` | Arduino | 標準ライブラリ | Arduinoフレームワーク内蔵 |
| `Adafruit_PWMServoDriver.h` | Adafruit Industries | 外部ライブラリ | `.pio/libdeps/m5stack-core2/Adafruit PWM Servo Driver Library/` |

### **C++でライブラリの出所を知る方法**

#### **1. PlatformIOの場合（今回のプロジェクト）**

**platformio.ini**を見る：
```ini
lib_deps =
    m5stack/M5Core2@^0.1.9                              # 組織名/ライブラリ名@バージョン
    adafruit/Adafruit PWM Servo Driver Library@^3.0.2   # 明確に分かる！
```

#### **2. 実際のファイル構造**

```
led-study/
├── .pio/
│   └── libdeps/
│       └── m5stack-core2/
│           ├── M5Core2/
│           │   ├── src/
│           │   │   └── M5Core2.h     ← ここにある
│           │   └── library.json      ← 作者情報
│           └── Adafruit PWM Servo Driver Library/
│               ├── Adafruit_PWMServoDriver.h
│               └── library.properties ← 作者情報
```

#### **3. library.jsonで確認**

M5Core2の例：
```json
{
  "name": "M5Core2",
  "version": "0.1.9",
  "author": "M5Stack",
  "repository": {
    "type": "git",
    "url": "https://github.com/m5stack/M5Core2.git"
  }
}
```

### **ライブラリの種類と見分け方**

#### **1. システム標準ライブラリ**
```cpp
#include <Wire.h>      // Arduino標準
#include <SPI.h>       // Arduino標準
#include <Arduino.h>   // Arduino標準
```
- **特徴**：短い名前、基本機能
- **場所**：フレームワーク内蔵

#### **2. ハードウェアメーカーのライブラリ**
```cpp
#include <M5Core2.h>   // M5Stack社
#include <M5Stack.h>   // M5Stack社
```
- **特徴**：製品名がそのまま
- **場所**：platformio.iniで追加

#### **3. サードパーティライブラリ**
```cpp
#include <Adafruit_PWMServoDriver.h>  // Adafruit社
#include <ServoEasing.h>              // 個人開発者
```
- **特徴**：会社名や機能名が入る
- **場所**：platformio.iniで追加

### **なぜC++は分かりにくい？**

#### **歴史的理由**

1. **C++は古い**（1985年～）
   - パッケージ管理の概念がなかった時代
   - ファイル名だけで管理

2. **互換性重視**
   - 古いコードも動くように
   - シンプルな#includeを維持

3. **プリプロセッサ方式**
   ```cpp
   #include <ファイル名>  // ファイルをそのまま挿入
   ```
   - Javaのimportとは根本的に違う
   - 単純なテキスト置換

### **PlatformIOのおかげで改善**

昔のC++：
```cpp
// どこから持ってくる？誰が作った？バージョンは？
#include <Adafruit_PWMServoDriver.h>
```

PlatformIO使用時：
```ini
# 明確！組織名、バージョン、すべて分かる
lib_deps =
    adafruit/Adafruit PWM Servo Driver Library@^3.0.2
```

### **実用的なTips**

#### **ライブラリの詳細を調べる方法**

1. **PlatformIO Registry**で検索
   - https://registry.platformio.org/
   - ライブラリ名で検索すると詳細が分かる

2. **コマンドで確認**
   ```bash
   pio lib search "Adafruit PWM"
   ```

3. **インストール済みを確認**
   ```bash
   pio lib list
   ```

4. **VSCodeで確認**
   - ライブラリ名を右クリック → 「定義へ移動」
   - 実際のファイルが開く

### **まとめ**

- **C++の#include**：ファイル名だけで組織名は不明
- **PlatformIO**：platformio.iniで組織名とバージョンが明確
- **実ファイル**：`.pio/libdeps/`配下で確認可能
- **Javaより原始的**だが、PlatformIOが現代的に管理

C++のライブラリ管理は確かに分かりにくいですが、PlatformIOのようなツールで大幅に改善されています！
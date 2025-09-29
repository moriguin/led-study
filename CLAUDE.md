# Claude Code 設定・コンテキスト

## プロジェクト概要
- **プロジェクト名**: led-study
- **内容**: M5Stack Core2を使った電子工作の学習プロジェクト
- **主な使用機器**: M5Stack Core2 for AWS、Servo2モジュール、サーボモーター

## ユーザー情報
- **経験レベル**: 電子工作の完全な初心者
- **プログラミング**: 初心者レベル（詳細な説明が必要）
- **電子工学知識**: 基礎知識なし（I2C、PWM、GPIO等の用語説明が必要）

## 作業方針
- **説明スタイル**: 初心者向けに丁寧で詳細な日本語コメント
- **コード記述**: シンプルで理解しやすい構造を優先
- **専門用語**: 必ず解説を付ける
- **例え話**: 料理や日常生活の例えを使って説明

## 質問・回答のドキュメント化
質問があった場合は、以下の形式でドキュメント化する：

### ファイル命名規則
```
doc/番号-質問の要点.md
```

### ファイル内容フォーマット
```markdown
# 質問の要点

## 質問
（ユーザーの質問をそのまま記載）

## 回答
（Claudeの回答をそのまま記載）
```

### 例
- `doc/01-M5Stack Core2 for AWSとServo2のI2C通信について.md`

## プロジェクト構成
```
led-study/
├── doc/                    # 質問・回答のドキュメント
├── src/                    # ソースコード
│   ├── main.cpp           # メインプログラム
│   └── main_simple.cpp.example  # 参考例
├── platformio.ini         # PlatformIO設定
└── CLAUDE.md             # このファイル
```

## 技術情報
### ハードウェア
- **M5Stack Core2 for AWS**: メインコントローラー
- **Servo2モジュール**: PCA9685チップ搭載、I2C制御（アドレス0x40）
- **接続**: Port A（赤色ポート）でI2C通信

### 必要ライブラリ
```ini
lib_deps =
    m5stack/M5Core2@^0.1.9
    adafruit/Adafruit PWM Servo Driver Library@^3.0.2
```

### よく使う関数
```cpp
// サーボ制御の簡単な関数
void moveServo(int angle) {
    int pulse = map(angle, 0, 180, SERVOMIN, SERVOMAX);
    pwm.setPWM(SERVO_CH, 0, pulse);
}
```

## 作業時の注意点
1. **必ず初心者向けの説明を心がける**
2. **専門用語は必ず解説する**
3. **コードには詳細な日本語コメントを付ける**
4. **質問があれば即座にドキュメント化する**
5. **シンプルで分かりやすいコード構造を優先**

## 定期的に実行するコマンド
```bash
# ビルドテスト
pio run

# アップロード
pio run --target upload

# シリアルモニター
pio device monitor
```
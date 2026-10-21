# 統合ロボット制御基板 設計仕様書 (Design Specifications Rev.4.1)

> 重要 / 本書のステータスと位置づけ
> 本仕様書は、STM32H723ZGT6のデータシートDS13313、リファレンスマニュアルRM0468、およびNUCLEO-H723ZG回路図MB1364に基づくハードウェア詳細検証を反映した確定仕様案である。Rev.4.1では、外部LED制御用GPIO 3出力および制御用リミットスイッチ 4入力を追加し、全95ピンの割り当てを確定した。

---

## システム概要

NUCLEO-H723ZG（STM32H723ZGT6）を制御コアとして採用し、エンコーダ8系統を最優先で確保したうえで、モータPWM 16出力、サーボPWM 6出力、リミットスイッチ12入力、電磁弁10出力、外部LED 3出力、Ethernet、FDCAN、I2C、SPI、UARTを同一マイコン上で共存させる統合制御基板を設計する。

### 基準ドキュメント
* MCUデータシート: DS13313 (STM32H723xE/G)
* MCUリファレンスマニュアル: RM0468 (STM32H723/733, STM32H725/735, STM32H730)
* Nucleoボードマニュアル / 回路図: UM2407 / MB1364-H723ZG-E01 Schematic

---

## 要件定義と機能一覧

| カテゴリ | 用途 | 必要数 | 設計方針 / 使用ペリフェラル | 備考 |
| :--- | :--- | :---: | :--- | :--- |
| エンコーダ最優先 | 絶対角エンコーダ AMT203-V | 4 系統 | SPI1 (SCK/MISO/MOSI共用) + GPIO (CS1からCS4) | 5V駆動時はMISO信号電圧のレベル確認必須 |
| エンコーダ最優先 | 増分式AB相エンコーダ | 4 系統 | TIM2, TIM3, TIM4, TIM23 (Encoder Mode) | 32-bitタイマー2系統、16-bitタイマー2系統で構成 |
| 通信先行固定 | Ethernet 有線LAN | 1 系統 | ETH (RMII) + オンボードPHY (LAN8742A) | オンボードRJ-45使用。RMIIピンを最優先で保護 |
| PWM出力 | モータ制御用 PWM | 16 出力 | TIM1(4ch), TIM8(4ch), TIM24(4ch), TIM12(2ch), TIM15(2ch) | 目標周波数: 20kHz（タイマー単位で周期共有） |
| PWM出力 | サーボモータ制御用 PWM | 6 出力 | TIM5(2ch), TIM13(1ch), TIM14(1ch), TIM16(1ch), TIM17(1ch) | 目標周波数: 50Hz（周期20ms）。外部ICなしで内部完結 |
| 通信 | CAN 通信 | 2 系統 | FDCAN1, FDCAN2 | 外部5Vトランシーバ（TCAN1042V等）を接続 |
| 通信 | シリアル通信 UART | 3 系統 | USART3 (ST-LINK VCP), USART2, UART7 | デバッグ用および外部機器通信用 |
| 通信 | 拡張 SPI / I2C | 各 1から2 系統 | SPI3, I2C1, I2C2 | 外部センサ / 拡張モジュール用 |
| デジタル入出力 | リミットスイッチ | 12 入力 | GPIO_Input 12本（PG0からPG5, PG8からPG10, PG12, PG14, PG15） | Port Gで統一 |
| デジタル入出力 | 電磁弁駆動 | 10 出力 | GPIO_Output 10本 | 電磁弁はMOSFETアレイおよび還流ダイオード経由で駆動 |
| デジタル入出力 | 外部LED制御 | 3 出力 | GPIO_Output 3本（PD10, PD11, PE15） | 外部インジケータLED用 |

---

## タイマー割り当ての設計と検証結果

### 同一タイマー内の周期共有に関する原則
1つのタイマーに属する複数チャンネル（CH1からCH4）は、単一のカウンタ周期（ARRおよびプリスケーラ設定）を共有する。チャンネルごとにデューティ比（CCR）は独立して変更できるが、同一タイマー内でモータ用の20kHzとサーボ用の50Hzの異なる周波数を同時に出力することはできない。したがって、モータPWM用タイマーとサーボPWM用タイマーは完全に分離する。

### サーボPWM 6出力確定の経緯と検証結果
STM32H723ZGT6の通常タイマー群において、TIM2, TIM3, TIM4, TIM23 をエンコーダ4系統に使用し、さらにEthernet RMIIとピンが重複する TIM5_CH2 (PA1) および TIM5_CH3 (PA2) を除外した場合、残る通常タイマーのチャンネル数合計は理論上22出力となる。

* モータPWM（16出力）: TIM1(4ch) + TIM8(4ch) + TIM24(4ch) + TIM12(2ch) + TIM15(2ch) = 16出力
* サーボPWM（6出力）: TIM5(CH1, CH4の2ch) + TIM13(1ch) + TIM14(1ch) + TIM16(1ch) + TIM17(1ch) = 6出力
* 合計: 22出力

このため、サーボPWMの要求数を6出力と決定した。これによって外付けPWMドライバICを追加することなく、マイコン内部の通常タイマーのみでモータ16出力とサーボ6出力の計22出力を完全に両立させることができる。

---

## ピン配置一覧

### エンコーダ 8系統（SPI1 4系統 ＋ Timer Encoder Mode 4系統）

| 用途 | 方式 | 周辺機能 | 信号 / CH | GPIO | Alternate Function | 検証結果 / 注意事項 | コネクタ位置 |
| :--- | :--- | :--- | :--- | :---: | :---: | :--- | :---: |
| ENC1から4 | SPI | SPI1 | SCK | PA5 | AF5 | 競合なし | CN9 ピン9 |
| ENC1から4 | SPI | SPI1 | MISO | PA6 | AF5 | 5V入力時はレベル変換の要否を確認 | CN9 ピン13 |
| ENC1から4 | SPI | SPI1 | MOSI | PD7 | AF5 | PA7(ETH)およびPB5(TIM3)回避のためPD7を使用 | CN9 ピン4 |
| ENC1 | SPI | GPIO | CS1 | PD14 | - | GPIO_Output | CN7 ピン16 |
| ENC2 | SPI | GPIO | CS2 | PD15 | - | GPIO_Output | CN7 ピン18 |
| ENC3 | SPI | GPIO | CS3 | PE10 | - | GPIO_Output | CN10 ピン25 |
| ENC4 | SPI | GPIO | CS4 | PE12 | - | GPIO_Output | CN10 ピン27 |
| ENC5 | 増分式 | TIM2 (32-bit) | CH1 / CH2 | PA15 / PB3 | AF1 | Debug設定をSerial WireのみにしてPB3(SWO)を解放 | CN7 ピン17 / CN7 ピン7 |
| ENC6 | 増分式 | TIM3 (16-bit) | CH1 / CH2 | PB4 / PB5 | AF2 | 競合なし | CN7 ピン27 / CN7 ピン29 |
| ENC7 | 増分式 | TIM4 (16-bit) | CH1 / CH2 | PD12 / PD13 | AF2 | 競合なし | CN10 ピン19 / CN10 ピン21 |
| ENC8 | 増分式 | TIM23 (32-bit)| CH1 / CH2 | PF0 / PF1 | AF13 | 基板上のSB44/SB46の接続設定を確認 | CN11 ピン51 / CN11 ピン53 |

### Ethernet (RMII) オンボード固定ピン

| 用途 | 周辺機能 | 信号名 | GPIOピン | Alternate Function | 検証結果 / 注意事項 |
| :--- | :--- | :--- | :---: | :---: | :--- |
| Ethernet | ETH | RMII_REF_CLK / MDIO / MDC | PA1 / PA2 / PC1 | AF11 | オンボードLAN8742A接続。他用途への割り当て禁止 |
| Ethernet | ETH | RMII_CRS_DV / RXD0 / RXD1 | PA7 / PC4 / PC5 | AF11 | 同上 |
| Ethernet | ETH | RMII_TX_EN / TXD0 / TXD1 | PG11 / PG13 / PB13 | AF11 | 同上 |

### モータPWM 16出力（同一タイマー内周期共有: 20kHz）

| 用途 | タイマー | CH | GPIO | Alternate Function | 設定パラメータ | コネクタ位置 |
| :--- | :--- | :---: | :---: | :---: | :--- | :---: |
| M1_PWM1 / 2 | TIM1 (16-bit) | CH1 / CH2 | PE9 / PE11 | AF1 | 周波数20kHz / PSC=0, ARR=4799 | CN10 ピン21 / CN10 ピン23 |
| M2_PWM1 / 2 | TIM1 (16-bit) | CH3 / CH4 | PE13 / PE14 | AF1 | 同上 | CN10 ピン29 / CN10 ピン31 |
| M3_PWM1 / 2 | TIM8 (16-bit) | CH1 / CH2 | PC6 / PC7 | AF3 | 周波数20kHz / PSC=0, ARR=4799 | CN7 ピン1 / CN7 ピン2 |
| M4_PWM1 / 2 | TIM8 (16-bit) | CH3 / CH4 | PC8 / PC9 | AF3 | 同上 | CN8 ピン2 / CN8 ピン4 |
| M5_PWM1 / 2 | TIM24 (32-bit)| CH1 / CH2 | PF11 / PF12 | AF14 | 周波数20kHz / PSC=0, ARR=4799 | CN12 ピン42 / CN10 ピン5 |
| M6_PWM1 / 2 | TIM24 (32-bit)| CH3 / CH4 | PF13 / PF14 | AF14 | 同上 | CN10 ピン7 / CN10 ピン9 |
| M7_PWM1 / 2 | TIM12 (16-bit)| CH1 / CH2 | PB14 / PB15 | AF2 | PB14は赤LED(LD3)接続 | CN12 ピン36 / CN12 ピン38 |
| M8_PWM1 / 2 | TIM15 (16-bit)| CH1 / CH2 | PE5 / PE6 | AF4 | 周波数20kHz / PSC=0, ARR=4799 | CN9 ピン3 / CN9 ピン5 |

### サーボPWM 6出力（周期: 50Hz）

| 用途 | タイマー | CH | GPIO | Alternate Function | 設定パラメータ | コネクタ位置 |
| :--- | :--- | :---: | :---: | :---: | :--- | :---: |
| SERVO1 | TIM5 (32-bit) | CH1 | PA0 | AF2 | 周波数50Hz / PSC=95, ARR=19999 | CN8 ピン1 |
| SERVO2 | TIM5 (32-bit) | CH4 | PA3 | AF2 | 同上 (CH2/3はEthernet使用のため無効) | CN9 ピン1 |
| SERVO3 | TIM13 (16-bit)| CH1 | PF8 | AF9 | 周波数50Hz / PSC=95, ARR=19999 | CN9 ピン15 |
| SERVO4 | TIM14 (16-bit)| CH1 | PF9 | AF9 | 周波数50Hz / PSC=95, ARR=19999 | CN9 ピン11 |
| SERVO5 | TIM16 (16-bit)| CH1 | PF6 | AF1 | 周波数50Hz / PSC=95, ARR=19999 | CN11 ピン47 / CN9 ピン5 |
| SERVO6 | TIM17 (16-bit)| CH1 | PF7 | AF1 | 周波数50Hz / PSC=95, ARR=19999 | CN9 ピン9 |

### 通信インターフェースおよびGPIO

| 用途 | 周辺機能 | 信号 | GPIO | Alternate Function | 検証結果 / 注意事項 | コネクタ位置 |
| :--- | :--- | :--- | :---: | :---: | :--- | :--- |
| CAN 1 | FDCAN1 | RX / TX | PD0 / PD1 | AF9 | 外部5Vトランシーバ接続 | CN9 ピン10 / CN9 ピン8 |
| CAN 2 | FDCAN2 | RX / TX | PB12 / PB6 | AF9 | 外部5Vトランシーバ接続 | CN12 ピン16 / CN10 ピン17 |
| デバッグUART | USART3 | TX / RX | PD8 / PD9 | AF7 | オンボードST-LINK VCP接続 | CN10 ピン2 / CN10 ピン4 |
| 外部UART 1 | USART2 | TX / RX | PD5 / PD6 | AF7 | 外部通信用 | CN9 ピン6 / CN9 ピン2 |
| 外部UART 2 | UART7 | RX / TX | PE7 / PE8 | AF7 | 外部通信用 | CN11 ピン37 / CN11 ピン39 |
| 拡張SPI | SPI3 | SCK / MISO / MOSI | PC10 / PC11 / PC12 | AF6 | 拡張センサ / モジュール用 | CN7 ピン3 / CN7 ピン5 / CN7 ピン7 |
| I2C 1 | I2C1 | SCL / SDA | PB8 / PB9 | AF4 | 外部プルアップ抵抗配置 | CN9 ピン2 / CN9 ピン4 |
| I2C 2 | I2C2 | SCL / SDA | PB10 / PB11 | AF4 | 外部プルアップ抵抗配置 | CN12 ピン25 / CN12 ピン27 |
| リミットSW 1から6 | GPIO_Input | LIMIT_SW1からLIMIT_SW6 | PG0からPG5 | - | Port G | CN11 ピン49,51,53,55,57,59 |
| リミットSW 7 / 8 | GPIO_Input | LIMIT_SW7 / LIMIT_SW8 | PG8 / PG9 | - | USB回避 | CN11 ピン61 / CN12 ピン40 |
| リミットSW 9から12 | GPIO_Input | LIMIT_SW9からLIMIT_SW12 | PG10, PG12, PG14, PG15 | - | 新設リミットSW | CN11 ピン66,65 / CN12 ピン64,66 |
| 電磁弁 1から3 | GPIO_Output | SOL1からSOL3 | PF2からPF4 | - | 外部MOSFET経由 | CN11 ピン56,58,60 |
| 電磁弁 4 | GPIO_Output | SOL4 | PF15 | - | 同上 | CN11 ピン38 |
| 電磁弁 5から7 | GPIO_Output | SOL5からSOL7 | PD2からPD4 | - | 同上 | CN11 ピン42,44,46 |
| 電磁弁 8から10 | GPIO_Output | SOL8からSOL10 | PE2からPE4 | - | 同上 | CN11 ピン50,52,54 |
| 外部LED 1から3 | GPIO_Output | EXT_LED1からEXT_LED3 | PD10, PD11, PE15 | - | 新設LED制御 | CN10 ピン33,35 / CN10 ピン13 |

---

## ハードウェアおよびシステム設計上の確認事項

* 基板クロック供給の回路確認:
  NUCLEO-H723ZGの回路図（MB1364）に基づき、ST-LINKからのMCOクロック供給またはオンボード水晶発振子のどちらが有効かを確認してCubeMXのRCC設定を行う。
* AMT203-Vの信号レベル整合:
  AMT203-Vを5Vで駆動する場合、SPI1_MISO（PA6）の入力許容電圧範囲を確認し、必要に応じてレベルシフタ回路を挿入する。
* CANトランシーバの電圧整合:
  5VのCANバス規定に合わせるため、ロジック側3.3V、バス側5Vに対応するトランシーバ（TCAN1042V等）を使用する。
* 電磁弁駆動およびサーボ電源の分離:
  電磁弁はMOSFETアレイおよび還流ダイオードを介して駆動し、サーボ電源はマイコン電源系から独立したDC-DCコンバータより供給する。
* Ethernet DMAとCortex-M7 D-Cacheの整合性:
  使用するEthernetドライバに合わせ、DMAディスクリプタおよびパケットバッファ領域のMPU属性を設定してD-Cacheの無効化処理を行う。
* PB3のデバッグピン解放:
  CubeMXのSYS設定でDebugをSerial Wireのみに設定し、SWOトレースピンを解放してTIM2_CH2として利用可能にする。
* PB14のLED回路の負荷影響:
  PB14は赤LED（LD3）に直結しているため、PWM波形への影響を考慮して必要に応じて基板上の抵抗を取り外す。
* PF0およびPF1のソルダブリッジ設定:
  UM2407を参照し、SB44およびSB46を導通状態に設定してPF0およびPF1をCN11コネクタへ接続する。
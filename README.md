# <h1 align="center">🛠️ cpptestmap — Unreal Engine C++ Test Map</h1>

<p align="center">
&nbsp; <b>Unreal Engine・C++・プロトタイプ / 個人学習プロジェクト</b><br>
&nbsp; UE5 の C++ 機能、マップロジック、ゲームプレイシステムを検証するためのテストプロジェクト
</p>

<p align="center">
<img src="https://img.shields.io/badge/Engine-Unreal_Engine-000000?logo=unrealengine&style=for-the-badge">
<img src="https://img.shields.io/badge/Language-C%2B%2B-00599C?logo=c%2B%2B&style=for-the-badge">
<img src="https://img.shields.io/badge/Status-Active-success?style=for-the-badge">
</p>

---

## 🎮 プロジェクト概要 / Project Overview

cpptestmap は、Unreal Engine における C++ 実装のテスト・機能検証 を目的とした軽量プロジェクトです。  
基礎クラスの挙動確認、マップ内インタラクション、外部プラグインの統合テストなど、  
UE5 C++ 学習と検証に特化したプロトタイプ環境です。

---

## 🧩 主な特徴 / Key Features

### 🧱 C++ クラスの検証
- GameMode / PlayerController / Character の基本継承テスト  
- カスタム Actor / Component のプロトタイピング  
- Blueprint 連携（UFUNCTION / UPROPERTY）  

### 🗺️ マップロジックのテスト
- TriggerVolume によるイベント処理  
- SpawnPoint 制御  
- マップ内での C++ 主導インタラクションの検証  

### ⚙️ 外部プラグインの統合
- Plugins/FPSAnimab8ebc56ef7c4V7 の動作検証  
- アニメーション・物理挙動・サードパーティ機能のチェック  

---

## 🛠 使用技術 / Tech Stack

| 分類 | 内容 |
|------|------|
| ゲームエンジン | Unreal Engine 5.4 |
| 言語 | C++ |
| 開発環境 | Visual Studio / Rıder |
| バージョン管理 | Git / GitHub |
| ライセンス | MIT License |

---

## 🚀 実行方法 / How to Run

### 🔹 ① リポジトリのクローン
```bash
git clone https://github.com/HekSeon/cpptestmap.git
cd cpptestmap
```
🔹 ② プロジェクトファイル生成

cpptestmap.uproject を右クリック

"Generate Visual Studio Project Files" を選択

🔹 ③ ビルド & エディタ起動

生成された .sln を Visual Studio で開く

Build 設定を Development Editor に変更

ソリューションをビルド

Unreal Editor を起動

👤 作者 / Author

BAYAR SEMIH (HekSeon)
UE5 ＆ C++ を中心に学習中のゲームプログラマー
個人プロジェクトで実験・研究・プロトタイピングを継続中

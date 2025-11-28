# 🛠️ cpptestmap — Unreal Engine C++ テストマップ

# 

# <p align="center"> \&nbsp; <b>Unreal Engine・C++・プロトタイプ / 個人学習プロジェクト</b>

# 

# \&nbsp; UE5 の C++ 機能、マップ関連ロジック、ゲームプレイシステムの検証 </p>

# 

# <p align="center"> <img src="https://img.shields.io/badge/Engine-Unreal\_Engine-000000?logo=unrealengine\&style=for-the-badge"> <img src="https://img.shields.co/badge/Language-C%2B%2B-00599C?logo=c%2B%2B\&style=for-the-badge"> <img src="https://img.shields.io/badge/License-MIT-green?style=for-the-badge"> <img src="https://img.shields.io/badge/Status-Active-success?style=for-the-badge"> </p>

# 

# 🎮 プロジェクト概要 / Project Overview

# 

# cpptestmap は、Unreal Engine における C++ の実装や機能テスト を目的としたシンプルなマッププロジェクトです。

# 

# 特に、ゲームの核となる C++ クラスの作成、マップ内でのオブジェクトとのインタラクション、または新しいエンジンの機能検証のために構築されました。

# 

# 🧩 主な特徴と目的 / Key Features \& Objectives

# 

# このプロジェクトは、主に以下の目的で使用されます：

# 

# 🧱 C++ クラスの検証

# 

# &nbsp;   GameMode, PlayerController, Character クラスなど、基本的な UE C++ クラスの継承と動作テスト。

# 

# &nbsp;   カスタムコンポーネントやアクターの即時的なプロトタイピング。

# 

# 🗺️ マップロジックの実装テスト

# 

# &nbsp;   トリガーボリューム、スポーンポイントなどのマップベースのロジックを C++ で制御。

# 

# &nbsp;   UProperty や UFunction を使用した C++ とブループリント間の通信テスト。

# 

# ⚙️ プラグインと外部機能の統合

# 

# &nbsp;   Plugins/FPSAnimab8ebc56ef7c4V7 のような、外部プラグインの動作確認および統合テスト。

# 

# &nbsp;   (例: アニメーション、物理システム、サードパーティライブラリの利用)

# 

# 🛠 使用技術 / Tech Stack

# 

# 分類	内容

# ゲームエンジン	Unreal Engine 5.x (または最新の対応バージョン)

# 言語	C++ (主要)

# 開発環境	Visual Studio (Windows) / XCode (Mac)

# バージョン管理	Git / GitHub

# ライセンス	MIT License

# 

# 🚀 実行方法 / How to Run

# 

# 🔹 ステップ①：リポジトリのクローン

# 

# まず、プロジェクトをローカル環境にクローンします。

# '''Bash

# 

# git clone https://github.com/HekSeon/cpptestmap.git

# cd cpptestmap

# '''

# 🔹 ステップ②：プロジェクトファイルの生成

# 

# Unreal Engine の C++ プロジェクトを実行するには、Visual Studio (または XCode) のソリューションファイルを生成する必要があります。

# 

# &nbsp;   クローンしたフォルダ内の cpptestmap.uproject ファイルを右クリックします。

# 

# &nbsp;   「Generate Visual Studio Project Files (Visual Studio プロジェクトファイルを生成)」を選択します。

# 

# 🔹 ステップ③：ビルドとエディタ起動

# 

# &nbsp;   生成された .sln (ソリューション) ファイルを Visual Studio で開きます。

# 

# &nbsp;   ビルド構成を "Development Editor" に設定し、ソリューションをビルドします。

# 

# &nbsp;   ビルド成功後、Visual Studio から起動するか、.uproject ファイルをダブルクリックして Unreal Engine Editor を起動します。

# 

# 👤 作者 / Author

# 

# BAYAR SEMIH (HekSeon)

# 

# &nbsp;   学習中のゲームプログラマー。

# 

# &nbsp;   Unreal Engine と C++ を用いたゲーム開発を中心に学習とプロトタイピングを行っています。


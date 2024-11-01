# Poetics SDK (C++)

[[English](./README.md)]

- [Poetics SDK (C++)](#poetics-sdk-c)
- [1. 紹介](#1-紹介)
- [2. 主な機能](#2-主な機能)
- [3. ビルドガイド](#3-ビルドガイド)
- [4. APIの仕様](#4-apiの仕様)

# 1. 紹介

このリポジトリには `C++` ベースの Poetics SDK が含まれています。

現在公式にサポートしているのは `Windows`の `x64`プラットフォームのみとなります。

# 2. 主な機能

WebSocketストリームを経由して、音声データをPoetics Speech APIサーバーに送信し、Speech-to-textの結果をリアルタイムに受信することができます。

# 3. ビルドガイド

[BUILD.md](docs/ja-JP/BUILD.md)ページをご覧ください。

ビルドが終わったら、C++プロジェクトにライブラリを追加して使用することができます。

1. SDKの `include`フォルダに提供したヘッダーファイルをプロジェクトに追加
2. `.lib`ファイルがあるリンクディレクトリを追加して、入力に `poetics_sdk.lib` を追加
3. ビルドディレクトリにDLLファイルをコピーしてサンプルプログラムを実行

追加のライブラリディレクトリの経路はDebugとReleaseがそれぞれ異なることにご注意ください。

# 4. APIの仕様

本SDKライブラリで提供されるAPIについては、[API.md](docs/ja-JP/API.md)をご確認ください。

Speech Server APIの詳細については、[[poetics-api.readme.io](https://poetics-api.readme.io/docs/streaming-asr)]を参照してください。

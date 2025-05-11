# Alembic ファイル出力サンプル / Alembic File Export Sample

## 概要 / Overview

このコードはAlembicファイルを出力するための簡単なサンプルです。Alembicファイルフォーマットを使用して、アニメーションデータを保存・交換するための基本的な実装例を提供します。

This code is a simple sample for exporting Alembic files. It provides a basic implementation example for storing and exchanging animation data using the Alembic file format.

## 前提条件 / Assumptions

- オブジェクトのトポロジーは不変として、フレーム毎に頂点座標と頂点カラーが変化することを仮定しています。
- The object's topology is assumed to be invariant, with vertex coordinates and vertex colors changing per frame.

## 開発環境 / Development Environment

- このコードはVisual Studioで実行することを前提としています。
- These codes are intended to be run in Visual Studio.

## ライセンス / License

- AlembicライブラリはAlembicライセンスのもとにあります。詳細についてはLICENSEファイルを参照してください。
- The Alembic library is under the Alembic license. For more details, please refer to the LICENSE file.

## 注意事項 / Notes

- Alembicライブラリはインクルードのパス部分のみを改変しております。
- I have only modified the include path portion of the Alembic library.

## デモ / Demo

- 以下はAlembicファイル出力の例です：
- Below is an example of Alembic file output:

![result](https://github.com/J-Siberia/Alembic_Example/blob/main/output_animation.gif)

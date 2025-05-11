#include <iostream>
#include <vector>
#include "ExportAlembic.h"

int main()
{
	double timeStep = 1.0 / 60.0;	//!< タイムステップ幅
	int maxSimulationFrames = 1000;	//!< 最大シミュレーションフレーム数

    AlembicExporter alembicExporter("output.abc", timeStep);	//!< Alembicエクスポータの初期化

	//表面メッシュに関するデータを初期化（四面体メッシュ）
	std::vector<int> meshVertexIndices;
	meshVertexIndices.push_back(0);
	meshVertexIndices.push_back(1);
	meshVertexIndices.push_back(2);	//Face1
	meshVertexIndices.push_back(0);
	meshVertexIndices.push_back(3);
	meshVertexIndices.push_back(2);	//Face2
	meshVertexIndices.push_back(0);
	meshVertexIndices.push_back(1);
	meshVertexIndices.push_back(3);	//Face3
	meshVertexIndices.push_back(1);
	meshVertexIndices.push_back(2);
	meshVertexIndices.push_back(3);	//Face4

	// 頂点座標に関するデータを初期化
	std::vector<float> vertexPositions;
	vertexPositions.push_back(0.0f); vertexPositions.push_back(1.0f); vertexPositions.push_back(0.0f);	// 頂点0
	vertexPositions.push_back(-1.0f); vertexPositions.push_back(0.0f); vertexPositions.push_back(-1.0f);	// 頂点1
	vertexPositions.push_back(1.0f); vertexPositions.push_back(0.0f); vertexPositions.push_back(-1.0f);	// 頂点2
	vertexPositions.push_back(0.0f); vertexPositions.push_back(0.0f); vertexPositions.push_back(0.0f);	// 頂点3

	// 頂点色に関するデータを初期化
	std::vector<float> vertexColors;
	vertexColors.push_back(1.0f); vertexColors.push_back(0.0f); vertexColors.push_back(0.0f); vertexColors.push_back(1.0f);	// 頂点0
	vertexColors.push_back(0.0f); vertexColors.push_back(1.0f); vertexColors.push_back(0.0f); vertexColors.push_back(1.0f);	// 頂点1
	vertexColors.push_back(0.0f); vertexColors.push_back(0.0f); vertexColors.push_back(1.0f); vertexColors.push_back(1.0f);	// 頂点2
	vertexColors.push_back(1.0f); vertexColors.push_back(1.0f); vertexColors.push_back(0.0f); vertexColors.push_back(1.0f);	// 頂点3

	// エクスポータのメッシュデータを設定
	alembicExporter.InitializeMesh(vertexPositions, meshVertexIndices, vertexColors);


	// シミュレーションループ
	for (int i = 0; i < maxSimulationFrames; i++)
	{
		// 頂点座標を更新
		vertexPositions[1] = 0.5 * sinf(i * 0.11f) + 1.0f;	// 頂点0のy座標を更新
		vertexPositions[3] = -1.0f + 0.5 * sinf(i * 0.12f);	// 頂点1のx座標を更新
		vertexPositions[8] = -1.0f + 0.5 * sinf(i * 0.13f);	// 頂点2のz座標を更新
		vertexPositions[10] = 0.5 * sinf(i * 0.14f);	// 頂点3のy座標を更新

		// 頂点色を更新
		vertexColors[1] = 0.5f - 0.5f * sinf(i * 0.1f);	// 頂点0のg成分を更新
		vertexColors[4] = 0.5f - 0.5f * sinf(i * 0.2f);	// 頂点1のr成分を更新
		vertexColors[9] = 0.5f - 0.5f * sinf(i * 0.3f);	// 頂点2のg成分を更新
		vertexColors[14] = 0.5f - 0.5f * sinf(i * 0.4f);	// 頂点3のb成分を更新

		
		// フレームの更新
		alembicExporter.UpdateFrame(vertexPositions, vertexColors);
	}
	// Alembicファイルを閉じる
	alembicExporter.Close();

	std::cout << "Alembic file exported successfully." << std::endl;

	return 0;

}

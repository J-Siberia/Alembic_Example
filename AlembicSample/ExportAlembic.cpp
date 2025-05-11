/*!
  @file export_alembic.h

  @brief Alembicエクスポート用ソースファイル

  @author J-Siberia
  @date 2025
*/

#include "ExportAlembic.h"
#include <iostream>

using namespace Alembic::Abc;
using namespace Alembic::AbcGeom;

AlembicExporter::AlembicExporter(const std::string& filename, double timeStep)
    : m_timeStep(timeStep)
    , m_currentTime(0.0)
    , m_initialized(false)
    , m_numVertices(0)
    , m_numFaces(0)
{
    // タイムステップからfpsを計算
    double fps = 1.0 / timeStep;

    // Alembicファイルを作成
    auto archiveType = Alembic::AbcCoreOgawa::WriteArchive();
    m_archive = CreateArchiveWithInfo(
        archiveType,
        filename,
        "SimulationExport",  // アプリケーション名
        "",                  // ユーザー情報
        ErrorHandler::kThrowPolicy
    );

    // タイムサンプリング情報を設定
    TimeSampling ts(timeStep, 0.0);
    m_tsIndex = m_archive.addTimeSampling(ts);

    // トップレベルオブジェクトを作成
    m_topObj = m_archive.getTop();
}

AlembicExporter::~AlembicExporter() {
    Close();
}

void AlembicExporter::InitializeMesh(
    const std::vector<float>& vertices,
    const std::vector<int>& indices,
    const std::vector<float>& colors)
{
    if (m_initialized) {
        std::cerr << "Mesh is already initialized" << std::endl;
        return;
    }

    // 頂点数をチェック
    m_numVertices = vertices.size() / 3;  // xyz座標で3要素

    if (colors.size() != m_numVertices * 4) {  // rgba色で4要素
        std::cerr << "Color data size doesn't match vertex count" << std::endl;
        return;
    }

    // インデックス情報を保存（トポロジーは変化しない前提）
    m_indices = indices;

    // 三角形メッシュと仮定して面数を計算
    m_numFaces = indices.size() / 3;

    // メッシュオブジェクトを作成
    m_meshObj = OPolyMesh(m_topObj, "simulationMesh", m_tsIndex);
    m_meshSchema = m_meshObj.getSchema();

    // 色情報のパラメータを作成
    m_colorParam = OC4fGeomParam(
        m_meshSchema.getArbGeomParams(),  // パラメータコンテナを取得
        "Cs",                             // 属性名
        false,                            // isIndexed
        GeometryScope::kVertexScope,      // 頂点ごとの色
        1,                                // 各頂点に1つの色値
        m_tsIndex                         // タイムサンプリングインデックス
    );

    // 初期フレームを設定
    UpdateFrame(vertices, colors);

    m_initialized = true;
}

void AlembicExporter::UpdateFrame(
    const std::vector<float>& vertices,
    const std::vector<float>& colors)
{
    if (!m_initialized) {
        std::cerr << "Mesh is not initialized" << std::endl;
        return;
    }

    // 頂点データを変換
    std::vector<Imath::V3f> verts;
    verts.reserve(m_numVertices);

    for (size_t i = 0; i < m_numVertices; ++i) {
        size_t idx = i * 3;
        verts.push_back(Imath::V3f(
            vertices[idx],
            vertices[idx + 1],
            vertices[idx + 2]
        ));
    }

    // 色データを変換
    std::vector<Imath::C4f> colorVals;
    colorVals.reserve(m_numVertices);

    for (size_t i = 0; i < m_numVertices; ++i) {
        size_t idx = i * 4;
        colorVals.push_back(Imath::C4f(
            colors[idx],
            colors[idx + 1],
            colors[idx + 2],
            colors[idx + 3]
        ));
    }

    // インデックスとポリゴン頂点数の設定
    std::vector<int32_t> faceIndices(m_indices.begin(), m_indices.end());
    std::vector<int32_t> faceCounts(m_numFaces, 3);  // すべて三角形と仮定

    // メッシュサンプルの作成
    OPolyMeshSchema::Sample meshSample;
    meshSample.setPositions(V3fArraySample(verts));

    // 最初のフレームのみインデックス情報を設定
    if (m_currentTime == 0.0) {
        meshSample.setFaceIndices(Int32ArraySample(faceIndices));
        meshSample.setFaceCounts(Int32ArraySample(faceCounts));
    }

    // データをAlembicに書き込む
    m_meshSchema.set(meshSample);

    // 色情報のサンプルを作成と設定
    OC4fGeomParam::Sample colorSample;
    colorSample.setVals(C4fArraySample(colorVals));
    m_colorParam.set(colorSample);

    // 現在時刻を更新
    m_currentTime += m_timeStep;
}

void AlembicExporter::Close() {
    m_initialized = false;
}
/*!
  @file export_alembic.h

  @brief Alembicエクスポート用ヘッダファイル

  @author J-Siberia
  @date 2025
*/

#ifndef ALEMBIC_EXPORTER_H
#define ALEMBIC_EXPORTER_H

#include <Alembic/AbcGeom/All.h>
#include <Alembic/AbcCoreOgawa/All.h>
#include <string>
#include <vector>


class AlembicExporter {
public:
    // コンストラクタ - ファイル名とタイムステップ幅を受け取る
    AlembicExporter(const std::string& filename, double timeStep);

    // デストラクタ
    ~AlembicExporter();

    // メッシュの初期化 - 頂点、インデックス、色情報を設定
    void InitializeMesh(
        const std::vector<float>& vertices,  // 頂点座標 (x1,y1,z1,x2,y2,z2,...)
        const std::vector<int>& indices,     // 面のインデックス
        const std::vector<float>& colors     // 頂点色 (r1,g1,b1,a1,r2,g2,b2,a2,...)
    );

    // フレームの更新 - 新しい頂点座標と色情報を設定
    void UpdateFrame(
        const std::vector<float>& vertices,  // 新しい頂点座標
        const std::vector<float>& colors     // 新しい頂点色
    );

    // ファイルを閉じる
    void Close();

private:
    // Alembicアーカイブとオブジェクト
    Alembic::Abc::OArchive m_archive;
    Alembic::Abc::OObject m_topObj;
    Alembic::AbcGeom::OPolyMesh m_meshObj;
    Alembic::AbcGeom::OPolyMeshSchema m_meshSchema;
    Alembic::AbcGeom::OC4fGeomParam m_colorParam;

    // メッシュの情報
    std::vector<int> m_indices;
    size_t m_numVertices;
    size_t m_numFaces;

    // タイムステップ関連
    double m_timeStep;
    double m_currentTime;
    uint32_t m_tsIndex;  // タイムサンプリングインデックス

    // 初期化フラグ
    bool m_initialized;
};

#endif // ALEMBIC_EXPORTER_H
/*!
  @file export_alembic.h

  @brief Alembic�G�N�X�|�[�g�p�\�[�X�t�@�C��

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
    // �^�C���X�e�b�v����fps���v�Z
    double fps = 1.0 / timeStep;

    // Alembic�t�@�C�����쐬
    auto archiveType = Alembic::AbcCoreOgawa::WriteArchive();
    m_archive = CreateArchiveWithInfo(
        archiveType,
        filename,
        "SimulationExport",  // �A�v���P�[�V������
        "",                  // ���[�U�[���
        ErrorHandler::kThrowPolicy
    );

    // �^�C���T���v�����O����ݒ�
    TimeSampling ts(timeStep, 0.0);
    m_tsIndex = m_archive.addTimeSampling(ts);

    // �g�b�v���x���I�u�W�F�N�g���쐬
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

    // ���_�����`�F�b�N
    m_numVertices = vertices.size() / 3;  // xyz���W��3�v�f

    if (colors.size() != m_numVertices * 4) {  // rgba�F��4�v�f
        std::cerr << "Color data size doesn't match vertex count" << std::endl;
        return;
    }

    // �C���f�b�N�X����ۑ��i�g�|���W�[�͕ω����Ȃ��O��j
    m_indices = indices;

    // �O�p�`���b�V���Ɖ��肵�Ėʐ����v�Z
    m_numFaces = indices.size() / 3;

    // ���b�V���I�u�W�F�N�g���쐬
    m_meshObj = OPolyMesh(m_topObj, "simulationMesh", m_tsIndex);
    m_meshSchema = m_meshObj.getSchema();

    // �F���̃p�����[�^���쐬
    m_colorParam = OC4fGeomParam(
        m_meshSchema.getArbGeomParams(),  // �p�����[�^�R���e�i���擾
        "Cs",                             // ������
        false,                            // isIndexed
        GeometryScope::kVertexScope,      // ���_���Ƃ̐F
        1,                                // �e���_��1�̐F�l
        m_tsIndex                         // �^�C���T���v�����O�C���f�b�N�X
    );

    // �����t���[����ݒ�
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

    // ���_�f�[�^��ϊ�
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

    // �F�f�[�^��ϊ�
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

    // �C���f�b�N�X�ƃ|���S�����_���̐ݒ�
    std::vector<int32_t> faceIndices(m_indices.begin(), m_indices.end());
    std::vector<int32_t> faceCounts(m_numFaces, 3);  // ���ׂĎO�p�`�Ɖ���

    // ���b�V���T���v���̍쐬
    OPolyMeshSchema::Sample meshSample;
    meshSample.setPositions(V3fArraySample(verts));

    // �ŏ��̃t���[���̂݃C���f�b�N�X����ݒ�
    if (m_currentTime == 0.0) {
        meshSample.setFaceIndices(Int32ArraySample(faceIndices));
        meshSample.setFaceCounts(Int32ArraySample(faceCounts));
    }

    // �f�[�^��Alembic�ɏ�������
    m_meshSchema.set(meshSample);

    // �F���̃T���v�����쐬�Ɛݒ�
    OC4fGeomParam::Sample colorSample;
    colorSample.setVals(C4fArraySample(colorVals));
    m_colorParam.set(colorSample);

    // ���ݎ������X�V
    m_currentTime += m_timeStep;
}

void AlembicExporter::Close() {
    m_initialized = false;
}
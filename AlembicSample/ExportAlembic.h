/*!
  @file export_alembic.h

  @brief Alembic�G�N�X�|�[�g�p�w�b�_�t�@�C��

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
    // �R���X�g���N�^ - �t�@�C�����ƃ^�C���X�e�b�v�����󂯎��
    AlembicExporter(const std::string& filename, double timeStep);

    // �f�X�g���N�^
    ~AlembicExporter();

    // ���b�V���̏����� - ���_�A�C���f�b�N�X�A�F����ݒ�
    void InitializeMesh(
        const std::vector<float>& vertices,  // ���_���W (x1,y1,z1,x2,y2,z2,...)
        const std::vector<int>& indices,     // �ʂ̃C���f�b�N�X
        const std::vector<float>& colors     // ���_�F (r1,g1,b1,a1,r2,g2,b2,a2,...)
    );

    // �t���[���̍X�V - �V�������_���W�ƐF����ݒ�
    void UpdateFrame(
        const std::vector<float>& vertices,  // �V�������_���W
        const std::vector<float>& colors     // �V�������_�F
    );

    // �t�@�C�������
    void Close();

private:
    // Alembic�A�[�J�C�u�ƃI�u�W�F�N�g
    Alembic::Abc::OArchive m_archive;
    Alembic::Abc::OObject m_topObj;
    Alembic::AbcGeom::OPolyMesh m_meshObj;
    Alembic::AbcGeom::OPolyMeshSchema m_meshSchema;
    Alembic::AbcGeom::OC4fGeomParam m_colorParam;

    // ���b�V���̏��
    std::vector<int> m_indices;
    size_t m_numVertices;
    size_t m_numFaces;

    // �^�C���X�e�b�v�֘A
    double m_timeStep;
    double m_currentTime;
    uint32_t m_tsIndex;  // �^�C���T���v�����O�C���f�b�N�X

    // �������t���O
    bool m_initialized;
};

#endif // ALEMBIC_EXPORTER_H
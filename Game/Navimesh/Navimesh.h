#pragma once
#include "Physics/ICollider.h"
#include "physics/PhysicsStaticObject.h"
#include "physics/BoxCollider.h"

//������`����ƃi�r���b�V���̃f�[�^���č쐬�����B
//�w�i�f�[�^�ɕύX���������Ƃ��ɗL���ɂ��Ă��������B

//#define REBUILD_NAVIMESH_DATA		

//������`����ƁA�i�r���b�V���̃f�o�b�O�\�����L���ɂȂ�B
#ifdef _DEBUG
//#define USE_NAVIMESH_DEBUG	//Debug�B
#else
//#define USE_NAVIMESH_DEBUG	//Release.
#endif
class Game;
struct CellBinary {

	Vector3	vertexPos[3];	//�Z���̒��_�̃|�W�V����
	int			linkCellNo[3];	//�Z���ɗאڂ��Ă���Z��
	Vector3	centerPos;		//�Z���̐^�񒆂̃|�W�V����
};
//�Z���N���X
struct Cell {
	enum NaviState {
		State_NotResearch,
		State_Opened,
		State_Closed,
	};
	Vector3	vertexPos[3];	//�Z���̒��_�̃|�W�V�����B
	Cell*		linkCells[3];	//�Z���ɗאڂ��Ă���Z���B
	Vector3	centerPos;		//�Z���̐^�񒆂̃|�W�V�����B
	int			linkMax = 0;	//�אڂ��Ă���Z���̌��B
	float		costFromStart;
	float		costToGoal;
	Cell*		parentCell;
	NaviState	state = State_NotResearch;
};

class Floor;
class Navimesh 
{
public:
	Navimesh();
	~Navimesh();
	void Init();
	const std::vector<Cell*>& GetCell() const
	{
		return m_cells;
	}
private:
	/// <summary>
	/// �ۑ����ꂽ�o�C�i���f�[�^����i�r���b�V�����\�z�B
	/// </summary>
	void BuildNavimeshFromBinary();
private:
	//typedef�͊����̌^���ɐV�������O��t����B
	typedef std::vector<Vector3>					VertexBuffer;	//���_�o�b�t�@�B
	typedef std::vector<unsigned int>				IndexBuffer;	//�C���f�b�N�X�o�b�t�@�B
	//unique_ptr�̓R�s�[�s�\�ȃN���X�B
	typedef std::unique_ptr<VertexBuffer>			VertexBufferPtr;
	typedef std::unique_ptr<IndexBuffer>			IndexBufferPtr;
	std::vector<VertexBufferPtr>					m_vertexBufferArray;	//���_�o�b�t�@�̔z��B
	std::vector<IndexBufferPtr>						m_indexBufferArray;		//�C���f�b�N�X�o�b�t�@�̔z��B
	
	BoxCollider			m_collider;						//�Z���p�̃J�v�Z���R���C�_�[�B
	const float			COLLIDER_EXPANSION = 2.45f;		//�R���C�_�[�̊g��{���B���X��2.45f�B
	std::vector<Cell*>	m_cells;						//�Z���̔z��B
	
	//�f�o�b�O�֌W�B
	//SkinModelRender*				m_debugModel = nullptr;			//�f�o�b�O���f���B
	//std::vector<SkinModelRender*>	m_debugModelList;				//�f�o�b�O���f���̔z��B
	//bool							m_isDebug = false;				//�f�o�b�O���[�h�̃I���I�t�B
	//bool							m_isChangeActiveModel = false;	//�f�o�b�O���[�h�̃I���I�t�B
	//bool							m_isCreateModel = false;		//�f�o�b�O���f������������ǂ����B
};
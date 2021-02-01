#include "stdafx.h"
#include "Navimesh.h"
#include <thread>

/*!
	*@brief	�X�g�b�v�E�H�b�`�N���X�B
	*@details
	* C#��Stopwatch�̂悤�Ɏg����B</br>
	*@code
	�T���v���R�[�h
	Stopwatch sw;
	sw.Start();		//�v���J�n�B
	for(int i = 0; i < 100; i++ ){
	}
	sw.Stop();		//�v���I��
	printf("�o�ߎ��� = %lf(�P�ʁF�b)\n", sw.GetElapsed());
	printf("�o�ߎ��� = %lf(�P�ʁF�~��)\n", sw.GetElapsedMillisecond());
	*@endcode
	*
	*/
//class CStopwatch {
//public:
//	/*!
//		*@brief	�R���X�g���N�^�B
//		*/
//	CStopwatch()
//	{
//		freq = 0;
//		end = 0;
//		begin = 0;
//		elapsed = 0.0;
//		elapsedMill = 0.0;
//		elapsedMicro = 0.0;
//	}
//	/*!
//		*@brief	�f�X�g���N�^�B
//		*/
//	~CStopwatch()
//	{
//	}
//	/*!
//		*@brief	�v���J�n�B
//		*/
//	void Start()
//	{
//		::QueryPerformanceCounter((LARGE_INTEGER*)&begin);
//	}
//	/*!
//		*@brief	�v���I��
//		*/
//	void Stop()
//	{
//		::QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
//		::QueryPerformanceCounter((LARGE_INTEGER*)&end);
//		elapsed = double(end - begin) / freq;
//		elapsedMill = elapsed * 1000.0;
//		elapsedMicro = elapsedMill * 1000.0;
//	}
//	/*!
//		*@brief	�o�ߎ��Ԃ��擾(�P��:�b)
//		*/
//	double GetElapsed() const
//	{
//		return elapsed;
//	}
//	/*!
//		*@brief	�o�ߎ��Ԃ��擾(�P��:�~���b)
//		*/
//	double GetElapsedMillisecond() const
//	{
//		return elapsedMill;
//	}
//	/*!
//		*@brief	�o�ߎ��Ԃ��擾(�P��:�}�C�N���b)
//		*/
//	double GetElapsedMicrosecond() const
//	{
//		return elapsedMicro;
//	}
//private:
//	LONGLONG freq;
//	LONGLONG end;
//	LONGLONG begin;
//	double elapsed;			//�o�ߎ���(�P�ʁF�b)
//	double elapsedMill;		//�o�ߎ���(�P�ʁF�~���b)
//	double elapsedMicro;	//�o�ߎ���(�P�ʁF�}�C�N���b)
//
//};


struct NMCallBack : public btCollisionWorld::ConvexResultCallback
{
	/// <summary>
	/// ���l
		//CallBack�N���X�̃R���X�g���N�^
		//CallBack()
		//���������X�g
		//{}�̑O�ɏ��������s��
		//namespace�@btCollisionWorld
		//�\���́@ClosestConvexResultCallback
		//ClosestConvexResultCallback�̓R���X�g���N�^�Ɉ���������̂�
		//���������Ȃ��Ă͂Ȃ�Ȃ�
		//{}
	/// </summary>
	
	//��Q�������邩�Ȃ�������
	bool isHit = false;
	//�Փ˂����珟��ɌĂ�ł����
	virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		//��������
		isHit = true;
		return 0;
	}
};

Navimesh::Navimesh()
{
}

Navimesh::~Navimesh()
{
	//m_isCreateModel = false;
}
void Navimesh::BuildNavimeshFromBinary()
{
	//�A�Z�b�g�t�H���_����i�r���b�V���̃f�[�^�����[�h����B
	FILE* fp = fopen("stage.hnv", "rb");
	if (fp == nullptr) {
		//�t�@�C���I�[�v���Ɏ��s
		//MessageBox(nullptr, "�G���[", "stage.hnv���J���܂���ł����B", MB_OK);
	}
	int numCell;
	fread(&numCell, sizeof(int), 1, fp);
	//CellBinary�̔z��𓮓I�Ɋm��
	CellBinary* cellBinarys = new CellBinary[numCell];
	fread(cellBinarys, sizeof(CellBinary) * numCell, 1, fp);

	//CellBinary����Cell�̏����\�z����B
	for (int i = 0; i < numCell; i++) {
		//�Z���̐���
		Cell* cell = new Cell;

		for (int j = 0; j < 3; j++) {
			cell->vertexPos[j] = cellBinarys[i].vertexPos[j];
		}
		cell->centerPos = cellBinarys[i].centerPos;
		m_cells.push_back(cell);
	}

	for (int i = 0; i < numCell; i++) {
		for (int j = 0; j < 3; j++) {
			if (cellBinarys[i].linkCellNo[j] != -1) {
				m_cells[i]->linkCells[j] = m_cells[(cellBinarys[i].linkCellNo[j])];
			}
			else {
				m_cells[i]->linkCells[j] = NULL;
			}
		}
	}
	delete[] cellBinarys;
	fclose(fp);
}
void Navimesh::Init()
{
	//�Z���̓����蔻��p�̃J�v�Z���R���C�_�[
	//������
	//���a��1�ł���̂͌�X�ɎO�p�`�̈�Ԓ����d�S���_�ԂɊg�傷�邽��
	//	m_collider.Create(1.f, 30.f);
#ifdef REBUILD_NAVIMESH_DATA
#else
	//�ۑ����ꂽ�o�C�i���f�[�^����i�r���b�V�����\�z�B
	BuildNavimeshFromBinary();
#endif
}
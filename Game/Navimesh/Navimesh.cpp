#include "stdafx.h"
#include "Navimesh.h"
#include <thread>

Navimesh::Navimesh()
{
}

Navimesh::~Navimesh()
{
	Release();
}

void Navimesh::BuildNavimeshFromBinary()
{
	//�A�Z�b�g�t�H���_����i�r���b�V���̃f�[�^�����[�h����B
	FILE* fp = fopen("Assets/navmesh/map.nvm", "rb");
	if (fp == nullptr) {
		//�t�@�C���I�[�v���Ɏ��s
		//MessageBox(nullptr, "�G���[", "stage.hnv���J���܂���ł����B", MB_OK);
		return;
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
			cell->vertexPos[j] = cellBinarys[i].vertexPos[j] * 100.0f;
		}
		cell->centerPos = cellBinarys[i].centerPos * 100.0f;
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

void Navimesh::Release()
{
	for (int i = 0; i < m_cells.size(); i++)
	{
		delete m_cells[i];
	}
	m_cells.clear();
}

void Navimesh::Init()
{
	//�ۑ����ꂽ�o�C�i���f�[�^����i�r���b�V�����\�z�B
	BuildNavimeshFromBinary();
}
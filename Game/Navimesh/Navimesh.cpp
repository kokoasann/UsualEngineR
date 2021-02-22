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
	//アセットフォルダからナビメッシュのデータをロードする。
	FILE* fp = fopen("Assets/navmesh/map.nvm", "rb");
	if (fp == nullptr) {
		//ファイルオープンに失敗
		//MessageBox(nullptr, "エラー", "stage.hnvが開けませんでした。", MB_OK);
		return;
	}
	int numCell;
	fread(&numCell, sizeof(int), 1, fp);
	//CellBinaryの配列を動的に確保
	CellBinary* cellBinarys = new CellBinary[numCell];
	fread(cellBinarys, sizeof(CellBinary) * numCell, 1, fp);

	//CellBinaryからCellの情報を構築する。
	for (int i = 0; i < numCell; i++) {
		//セルの生成
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
	//保存されたバイナリデータからナビメッシュを構築。
	BuildNavimeshFromBinary();
}
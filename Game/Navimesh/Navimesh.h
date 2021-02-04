#pragma once
struct CellBinary {

	Vector3		vertexPos[3];	//セルの頂点のポジション
	int			linkCellNo[3];	//セルに隣接しているセル
	Vector3		centerPos;		//セルの真ん中のポジション
};
//セルクラス
struct Cell {
	Vector3		vertexPos[3];	//セルの頂点のポジション。
	Cell*		linkCells[3];	//セルに隣接しているセル。
	Vector3		centerPos;		//セルの真ん中のポジション。
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
	/// 保存されたバイナリデータからナビメッシュを構築。
	/// </summary>
	void BuildNavimeshFromBinary();
private:	
	std::vector<Cell*>	m_cells;						//セルの配列。
};
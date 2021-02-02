#pragma once
#include "Physics/ICollider.h"
#include "physics/PhysicsStaticObject.h"
#include "physics/BoxCollider.h"

//これを定義するとナビメッシュのデータが再作成される。
//背景データに変更が入ったときに有効にしてください。

//#define REBUILD_NAVIMESH_DATA		

//これを定義すると、ナビメッシュのデバッグ表示が有効になる。
#ifdef _DEBUG
//#define USE_NAVIMESH_DEBUG	//Debug。
#else
//#define USE_NAVIMESH_DEBUG	//Release.
#endif
class Game;
struct CellBinary {

	Vector3	vertexPos[3];	//セルの頂点のポジション
	int			linkCellNo[3];	//セルに隣接しているセル
	Vector3	centerPos;		//セルの真ん中のポジション
};
//セルクラス
struct Cell {
	enum NaviState {
		State_NotResearch,
		State_Opened,
		State_Closed,
	};
	Vector3	vertexPos[3];	//セルの頂点のポジション。
	Cell*		linkCells[3];	//セルに隣接しているセル。
	Vector3	centerPos;		//セルの真ん中のポジション。
	int			linkMax = 0;	//隣接しているセルの個数。
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
	/// 保存されたバイナリデータからナビメッシュを構築。
	/// </summary>
	void BuildNavimeshFromBinary();
private:
	//typedefは既存の型名に新しい名前を付ける。
	typedef std::vector<Vector3>					VertexBuffer;	//頂点バッファ。
	typedef std::vector<unsigned int>				IndexBuffer;	//インデックスバッファ。
	//unique_ptrはコピー不可能なクラス。
	typedef std::unique_ptr<VertexBuffer>			VertexBufferPtr;
	typedef std::unique_ptr<IndexBuffer>			IndexBufferPtr;
	std::vector<VertexBufferPtr>					m_vertexBufferArray;	//頂点バッファの配列。
	std::vector<IndexBufferPtr>						m_indexBufferArray;		//インデックスバッファの配列。
	
	BoxCollider			m_collider;						//セル用のカプセルコライダー。
	const float			COLLIDER_EXPANSION = 2.45f;		//コライダーの拡大倍率。元々は2.45f。
	std::vector<Cell*>	m_cells;						//セルの配列。
	
	//デバッグ関係。
	//SkinModelRender*				m_debugModel = nullptr;			//デバッグモデル。
	//std::vector<SkinModelRender*>	m_debugModelList;				//デバッグモデルの配列。
	//bool							m_isDebug = false;				//デバッグモードのオンオフ。
	//bool							m_isChangeActiveModel = false;	//デバッグモードのオンオフ。
	//bool							m_isCreateModel = false;		//デバッグモデルを作ったかどうか。
};
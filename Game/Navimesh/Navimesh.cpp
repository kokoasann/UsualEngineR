#include "stdafx.h"
#include "Navimesh.h"
#include <thread>

/*!
	*@brief	ストップウォッチクラス。
	*@details
	* C#のStopwatchのように使える。</br>
	*@code
	サンプルコード
	Stopwatch sw;
	sw.Start();		//計測開始。
	for(int i = 0; i < 100; i++ ){
	}
	sw.Stop();		//計測終了
	printf("経過時間 = %lf(単位：秒)\n", sw.GetElapsed());
	printf("経過時間 = %lf(単位：ミリ)\n", sw.GetElapsedMillisecond());
	*@endcode
	*
	*/
//class CStopwatch {
//public:
//	/*!
//		*@brief	コンストラクタ。
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
//		*@brief	デストラクタ。
//		*/
//	~CStopwatch()
//	{
//	}
//	/*!
//		*@brief	計測開始。
//		*/
//	void Start()
//	{
//		::QueryPerformanceCounter((LARGE_INTEGER*)&begin);
//	}
//	/*!
//		*@brief	計測終了
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
//		*@brief	経過時間を取得(単位:秒)
//		*/
//	double GetElapsed() const
//	{
//		return elapsed;
//	}
//	/*!
//		*@brief	経過時間を取得(単位:ミリ秒)
//		*/
//	double GetElapsedMillisecond() const
//	{
//		return elapsedMill;
//	}
//	/*!
//		*@brief	経過時間を取得(単位:マイクロ秒)
//		*/
//	double GetElapsedMicrosecond() const
//	{
//		return elapsedMicro;
//	}
//private:
//	LONGLONG freq;
//	LONGLONG end;
//	LONGLONG begin;
//	double elapsed;			//経過時間(単位：秒)
//	double elapsedMill;		//経過時間(単位：ミリ秒)
//	double elapsedMicro;	//経過時間(単位：マイクロ秒)
//
//};


struct NMCallBack : public btCollisionWorld::ConvexResultCallback
{
	/// <summary>
	/// 備考
		//CallBackクラスのコンストラクタ
		//CallBack()
		//初期化リスト
		//{}の前に初期化を行う
		//namespace　btCollisionWorld
		//構造体　ClosestConvexResultCallback
		//ClosestConvexResultCallbackはコンストラクタに引数があるので
		//初期化しなくてはならない
		//{}
	/// </summary>
	
	//障害物があるかないか判定
	bool isHit = false;
	//衝突したら勝手に呼んでくれる
	virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		//当たった
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
	//アセットフォルダからナビメッシュのデータをロードする。
	FILE* fp = fopen("stage.hnv", "rb");
	if (fp == nullptr) {
		//ファイルオープンに失敗
		//MessageBox(nullptr, "エラー", "stage.hnvが開けませんでした。", MB_OK);
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
	//セルの当たり判定用のカプセルコライダー
	//初期化
	//半径が1であるのは後々に三角形の一番長い重心頂点間に拡大するため
	//	m_collider.Create(1.f, 30.f);
#ifdef REBUILD_NAVIMESH_DATA
#else
	//保存されたバイナリデータからナビメッシュを構築。
	BuildNavimeshFromBinary();
#endif
}
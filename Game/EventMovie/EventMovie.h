#pragma once

/*

	struct Marker
	{
		float time
		int nameNum
		wchar_t name[nameNum]
	}
	struct Object
	{
		int id
		int nameNum
		wchar_t name[nameNum]
	}
	struct Frame
	{
		float time
		Matrix cameraMat
		Matrix objectMats[objectNum]
	}

	int markerNum
	Marker markers[markerNum]
	
	int objectNum
	Object objects[objectNum]

	int frameNum
	Frame frames[frameNum]

*/

struct SEventMovieMarker
{
	float time = 0.f;
	wchar_t* markerName = nullptr;
};

struct SEventMovieObject
{
	wchar_t* objName = nullptr;
	int id = -1;
};

struct SEventMovieFrame
{
	float time = 0.f;

};

/// <summary>
/// 
/// </summary>
class EventMovie :public GameObject
{
public:
	EventMovie();
	virtual ~EventMovie();

	/// <summary>
	/// 本開放。確保したものを開放するための関数。
	/// </summary>
	virtual void Release() override;

	/// <summary>
	/// このオブジェクトの所持するゲームオブジェクト等を消すための関数。
	/// 面倒くさければRelease関数と同じでもよい。
	/// </summary>
	virtual void OnDestroy() override;

	/// <summary>
	/// NewGO時に即座に呼ばれる関数。
	/// コンストラクタの代わりに使おう。
	/// </summary>
	void Awake() override;

	/// <summary>
	/// スタート関数。
	/// </summary>
	/// <returns>trueを返さない限り、ずっとStartを呼び続ける</returns>
	bool Start() override;

	/// <summary>
	/// 更新。の前に呼ばれる更新。
	/// </summary>
	void PreUpdate() override;
	/// <summary>
	/// 更新。
	/// </summary>
	void Update() override;
	/// <summary>
	/// 更新。の後に呼ばれる更新。
	/// </summary>
	void PostUpdate() override;

	/// <summary>
	/// 描画。
	/// </summary>
	void Render() override;

	/// <summary>
	/// 手前に表示するやつの描画。
	/// </summary>
	void PostRender() override;
private:

};

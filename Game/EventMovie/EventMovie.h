#pragma once

/*

	struct Marker
	{
		float time
		int nameNum
		char name[nameNum+1] // +1はNULL文字分。
	}
	struct Object
	{
		int id
		int nameNum
		char name[nameNum+1]
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


using ActorInitFunc = std::function<ModelRender* (const std::string&)>;
using EventListennerFunc = std::function<void(const std::string&)>;

/// <summary>
/// 
/// </summary>
class EventMovie :public GameObject
{
public:
	struct SEventMovieMarker
	{
		float time = 0.f;
		std::string markerName;
	};

	struct SEventMovieObject
	{
		std::string objName;
		int id = -1;
	};

	struct SEventMovieFrame
	{
		float time = 0.f;
		Matrix camMat;
		Matrix* objMats = nullptr;
	};

	struct Matrix4x3
	{
		Vector3 v[4];
	};


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
	/// 
	/// </summary>
	/// <param name="path"></param>
	/// /// <param name="camera"> メインカメラ </param>
	/// <param name="actorInitFunc"> アクターを初期化する関数 引数の文字列はアクターの名前　戻り値はアクターのモデル このモデルに行列をぶち込んでいく</param>
	/// <param name="eventListennerFunc"> animationの animetion event みたいなもの。これでアクターのアニメーションをさせたりフェードアウトさせたりする </param>
	void Init(const char* path,Camera* camera,const ActorInitFunc& actorInitFunc,const EventListennerFunc& eventListennerFunc);

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

	void SetOffset(const Vector3& v)
	{
		m_offset = v;
	}
private:
	Vector3 m_offset;
	Camera* m_camera = nullptr;
	EventListennerFunc m_eventListennerFunc;

	std::vector<SEventMovieMarker> m_eventMovieMarker;

	std::vector<ModelRender*> m_actors;
	std::vector<SEventMovieFrame> m_frames;
	Matrix* m_objMats = nullptr;
};

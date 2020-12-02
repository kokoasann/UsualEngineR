#pragma once
#include "State/IPlayerSstate.h"
#include "../../UsualEngineR/Character/CharacterController.h"
#include "Effect/ParticleEffect.h"

class IEnemy;
class GameCamera;
class Pod;
class JetPack;
class JetEffect;

/// <summary>
/// 
/// </summary>
class Player final : public GameObject
{
public:

	enum class EnState {
		enGround,
		enFlying,
		enFall,
		enDiving,
		enAttack,
		enGuard,
		enStun,
		enDead,
		enNumState
	};

	enum class EnAnimation {
		enIdle,
		enRun,
		enWalk,
		enFall,
		enHovering,
		enFlyMove,
		enFlyMoveFaster,
		//enDamage,
		enGuard,
		enKnockBack,
		enAttack,
		enPunch1,
		enPunch2,
		enPunch3,
		enKick,
		enDead,
		enNumAnim
	};

	enum class EnAttackType {
		enSlash,
		enRemoteAttackPreset,
		enMeleePreset,
		enNumAttackType,
		enNone,
	};

	enum class EnAttackPreset {
		enDefault,
		enRemoteAttackPreset,
		enMeleePreset,
		enExposivePreset,
		enNumPreset
	};

	enum class EnPlayerBone {
		enBack,
		enSOLE_L,
		enSOLE_R,
		enHand_R,
		enHand_L,
		enNumBoneType
	};

	Player();
	virtual ~Player();

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

	/// <summary>
	/// プレイヤーのステートクラスを取得する
	/// </summary>
	/// <param name="state">取得したいステートの種類</param>
	/// <returns></returns>
	IPlayerState* GetState(const EnState& state) const{
		return m_stateList[static_cast<int>(state)];
	};

	/// <summary>
	/// 現在のステートに遷移する前のステートを返す
	/// </summary>
	/// <returns>1つ前のステート</returns>
	IPlayerState* GetPreviousState() const {
		return m_previousState;
	};

	const IPlayerState* GetCurrentState() const {
		return m_currentState;
	}

	/// <summary>
	/// 座標を取得
	/// </summary>
	/// <returns>座標</returns>
	const Vector3& GetPosition() const {
		return m_position;
	}

	void SetPosition(const Vector3& pos) {
		m_position = pos;
		m_charaCon.SetPosition(pos);
	}

	void Respawn() {
		SetPosition(m_SpawnPos);
		SetRotation(m_SpawnRot);
		m_hp = m_HP_MAX;
		m_endurance = m_ENDURANCE_MAX;
		m_boost = m_BOOST_MAX;
	}

	void SetVelocity(const Vector3& velocity) {
		m_velocity = velocity;
	}

	const Vector3 GetVelocity() const {
		return m_velocity;
	}

	void SetLocalVelocity(const Vector3& velocity) {
		m_localVelocity = velocity;
	}

	const Vector3 GetLocalVelocity() const {
		return m_localVelocity;
	}

	void PlayAnimation(const EnAnimation& anim, const float interpolate = 0.2f){
		m_model->Play(static_cast<int>(anim), interpolate);
	}

	const bool IsPlayingAnimation(){
		return m_model->IsAnimPlaying();
	}

	const float GetCurrentHP() {
		return m_hp;
	}

	static const float GetMaxHP() {
		//static const float maxHp = 500.f;
		return m_HP_MAX;
	}

	const float GetCurrentEndurance() {
		return m_endurance;
	}

	const float GetMaxEndurance() {
		return m_ENDURANCE_MAX;
	}

	const bool IsOnGround() {
		return m_charaCon.IsOnGround();
	}

	const Vector3& GetForward() const {
		return m_forward;
	}

	/// <summary>
	/// プレイヤーにダメージを適用する
	/// </summary>
	/// <param name="damageAmount"> 攻撃力</param>
	void ApplyDamage(const float damageAmount, const bool stunFlag = false, const Vector3& vel= Vector3::Zero){
		auto finalDamage = damageAmount - (damageAmount * m_armorParam / 100.f);
		m_hp = max(0.f, m_hp - finalDamage);
		if(stunFlag){
			m_previousState = m_currentState;
			m_currentState->Exit(this);
			m_currentState = m_nextState = m_stateList[static_cast<int>(EnState::enStun)];
			m_currentState->Enter(this);
			m_knockBackImpulse = vel;
		}
	}

	/// <summary>
	/// プレイヤーのアーマーが何%ダメージを軽減できるか設定
	/// </summary>
	/// <param name="param">param%ダメージ軽減アーマーを着用</param>
	void SetArmorParam(const float param) {
		m_armorParam = param;
	}

	void Heal(const float amount) {
		m_hp = min(m_HP_MAX, m_hp + amount);
	}

	/*
	void SetVelocityGoal(Vector3& velocityGoal) {
		m_velocityGoal = velocityGoal;
	}

	const Vector3 GetVelocityGoal() const {
		return m_velocityGoal;
	}
	*/

	/// <summary>
	/// プレイヤーの回転を取得
	/// </summary>
	/// <returns>プレイヤーに適応されるクォータニオン</returns>
	const Quaternion& GetRotation() const {
		return m_rotation;
	}

	void SetRotation(const Quaternion& rot) {
		m_rotation = rot;
	}

	const float GetSpeed() const {
		return m_speed;
	}

	const float GetCurrentBoost() {
		return m_boost;
	}

	const float GetMaxBoost() {
		return m_BOOST_MAX;
	}

	void UseBoost(const float amount) {
		m_boost = max(0.f, m_boost - amount);
	}

	void ChargeBoost(const float amount) {
		m_boost = min(m_BOOST_MAX, m_boost + amount);
	}

	void  UseStamina(const float amount) {
		m_endurance = max(0.f, m_endurance - amount);
	}

	void ChargeEndurance(const float amount) {
		m_endurance = min(m_ENDURANCE_MAX, m_endurance + amount);
	}

	//void SetGameCamera(GameCamera* camera) {
		//mp_camera = camera;
	//}

	//GameCamera& GetGameCamera() {
		//return *mp_camera;
	//}

	const EnAttackPreset& GetCurrentAttackPreset() const{
		return m_currentAttackPreset;
	}

	void UnlockPreset(const EnAttackPreset& preset) {
		m_canUsePreset[TO_INT(preset)] = true;
	}

	Pod* GetPod() const {
		return m_pod;
	}

	Model& GetModel() {
		return m_model->GetModel();
	}

	ModelRender& GetModelRender() {
		return *m_model;
	}

	const Bone* GetBone(const EnPlayerBone& boneType) const{
		return m_playerBones.at(TO_INT(boneType));
	}

	void FireThrusters() {
		m_isUsingThrusters = true;
	}

	void StopThrusters() {
		m_isUsingThrusters = false;
	}

	const bool IsUsingThrusters() {
		return m_isUsingThrusters;
	}

	const bool ColCheck(const Player::EnPlayerBone&);

	//TODO : protect these member
	Vector3 m_velocity = Vector3::Zero;
	Vector3 m_localVelocity = Vector3::Zero;
	Vector3 m_knockBackImpulse = Vector3::Zero;
	//Vector3 m_velocityGoal = Vector3::Zero;
	
private:
	//func
	void SearchTarget();

	/// <summary>
	/// 攻撃の種類を選択
	/// </summary>
	void UpdateAttackType();


	void SetAnimation(const int no, const char* path, const bool loopFlag) {
		m_animationMap.insert(std::make_pair(no, std::make_unique<CAnimationClip>()));
		m_animationMap.at(no)->Load(path);
		m_animationMap.at(no)->BuildKeyFramesAndAnimationEvents();
		m_animationMap.at(no)->SetLoopFlag(loopFlag);
	}

	//Current Attack Type
	EnAttackPreset m_currentAttackPreset = EnAttackPreset::enDefault;
	bool m_canUsePreset[TO_INT(EnAttackPreset::enNumPreset)] = { true, true, true , true};

	//State
	IPlayerState* m_nextState = nullptr;
	IPlayerState* m_currentState = nullptr;
	IPlayerState* m_previousState = nullptr;
	std::vector<IPlayerState*> m_stateList;

	//Model
	ModelRender* m_model = nullptr;
	CAnimationClip m_animClip[1];
	//std::vector<CAnimationClipPtr> m_animlist;
	std::map<int, std::unique_ptr<CAnimationClip>> m_animationMap;

	const Vector3 m_SpawnPos = { 0,15,-100 };
	Vector3 m_position = m_SpawnPos;
	Vector3 m_forward = { 0,0,1 };
	Quaternion m_SpawnRot = Quaternion::Identity;
	Quaternion m_rotation = Quaternion::Identity;
	const Vector3 m_scale = { 0.5f,0.5f,0.5f };

	//Move
	CharacterController m_charaCon;
	float m_speed = 30.f;

	//Physics
	float m_charaConRadius = 1.f;
	float m_charaConHeight = 6.f;

	//Pod
	Pod* m_pod = nullptr;

	//Attachments
	std::vector<Bone*> m_playerBones;
	JetPack* m_jetPack = nullptr;


	//Ability
	//TODO : use struct like this
	/*
	struct StPlayerAbility {
		const float HP;
		float currentHp;
		const float STAMINA;
		float currentStamina;
		const float BOOST;
		float boost;
	};
	*/

	static const float  m_HP_MAX;
	const float m_ENDURANCE_MAX = 300.f;
	const float m_BOOST_MAX = 300.f;

	float m_hp = m_HP_MAX;
	//TODO : change the name endurance to stamina
	float m_endurance = m_ENDURANCE_MAX;
	float m_boost = m_BOOST_MAX;
	float m_armorParam = 0.f; //アーマーでダメージを何パーセント抑えるか. (初期状態 : 0%軽減).

	//Effecct
	const int RIGHT = 0;
	const int LEFT = 1;
	//void InitThrusterEffect();
	bool m_isUsingThrusters = false;
	//PlaneParticleEffectRender* m_thrusterEffects[2] = { nullptr };
	std::vector<JetEffect*> m_jetEffects;


	//Hands
	SphereCollider		m_sphereCollider;
	const float m_HandRadius = 3.f;
};
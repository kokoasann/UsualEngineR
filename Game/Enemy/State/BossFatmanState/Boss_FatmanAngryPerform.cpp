#include "stdafx.h"
#include "Boss_FatmanAngryPerform.h"
#include "Effect/Beam.h"
#include "Effect/MuzzleFlash.h"

Boss_FatmanAngryPerform::Boss_FatmanAngryPerform()
{
	BeamEffectInitData bid;
	for (int i = 0; i < 2; i++) {
		Beam* beam = NewGO<Beam>(0);
		beam->Init(bid);
		beam->SetSca(Vector3::One * 0.3f);
		m_beams.push_back(beam);
	}

	MuzzleFlashEffectInitData mfid;
	for (int i = 0; i < 2; i++) {
		auto muzzleFlash = NewGO<MuzzleFlash>(0);
		muzzleFlash->Init(mfid);
		muzzleFlash->SetSca(Vector3::One * 0.1);
		m_muzzleFlashes.push_back(muzzleFlash);
	}
}

Boss_FatmanAngryPerform::~Boss_FatmanAngryPerform()
{	
	//生成した数だけ削除。
	for (int i = 0; i < m_beams.size(); i++) {
		DeleteGO(m_beams.at(i));
	}
	for (int i = 0; i < m_muzzleFlashes.size(); i++) {
		DeleteGO(m_muzzleFlashes.at(i));
	}
}

void Boss_FatmanAngryPerform::Release()
{
}

void Boss_FatmanAngryPerform::OnDestroy()
{
	Release();
}


void Boss_FatmanAngryPerform::Awake()
{

}

bool Boss_FatmanAngryPerform::Start()
{
	return true;
}



void Boss_FatmanAngryPerform::Update()
{
	//ビーム。
	for (int ikNo = 0; ikNo < IK_NUM; ikNo++) {
		m_beams[ikNo]->SetPos(m_BeamIk[ikNo]->GetEffectorBone()->GetWorldMatrix().GetTransrate());
		m_beams[ikNo]->SetRot(m_BeamIk[ikNo]->GetEffectorBone()->GetWorldMatrix().GetRotate());
		m_beams[ikNo]->SetIsLoop(true);
		m_beams[ikNo]->Play();
	}

	//マズルフラッシュ。
	for (int ikNo = 0; ikNo < IK_NUM; ikNo++) {
		m_muzzleFlashes[ikNo]->SetPos(m_ShootIk[ikNo]->GetEffectorBone()->GetWorldMatrix().GetTransrate());
		m_muzzleFlashes[ikNo]->SetRot(m_ShootIk[ikNo]->GetEffectorBone()->GetWorldMatrix().GetRotate());
		m_muzzleFlashes[ikNo]->Play();
	}
}





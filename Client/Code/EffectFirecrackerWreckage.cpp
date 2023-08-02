#include "stdafx.h"
#include "..\Header\EffectFirecrackerWreckage.h"

CEffectFirecrackerWreckage::CEffectFirecrackerWreckage(LPDIRECT3DDEVICE9 pGraphicDev)
	: CTempParticle(pGraphicDev)
{
}

CEffectFirecrackerWreckage::~CEffectFirecrackerWreckage()
{
	Free();
}

HRESULT CEffectFirecrackerWreckage::Ready_Object(_vec3 vOriginPos, int numParticles)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	for (int i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	m_eObjectTag = OBJECTTAG::EFFECT;
	m_ParticleTag = PARTICLETAG::PARTICLE_FIRECRACKER;
	//M_PARTICLETAG
	m_pTransform->Translate(vOriginPos);

	ParticleBoundingBox EffectBox;
	EffectBox.vMin = { -100.f, -100.f, -100.f };
	EffectBox.vMax = { 100.f, 100.f, 100.f };

	m_vOrigin = vOriginPos;
	m_fSize = 0.05f;
	m_vbSize = 2048;
	m_vbOffset = 0;
	m_vbBatchSize = 512;
	m_BoundingBox = EffectBox;

	for (int i = 0; i < numParticles; i++)
		Add_Particle();

	_tchar* pPath = L"../Bin/SRSource/Effect/Square_effect/Square_effect_White.png";
	CTempParticle::Ready_Object(pPath);

	return S_OK;
}

void CEffectFirecrackerWreckage::Initial_Particle(ParticleAttribute* _attribute)
{
	_attribute->bAlive = true;

	_attribute->vPosition = _vec3(0.f, 0.f, 0.f);
	_vec3 min = _vec3(-1.0f, -1.0f, -1.f);
	_vec3 max = _vec3(1.0f, 1.0f, 1.f);

	Get_RandomVector(
		&_attribute->vVelocity,
		&min,
		&max
	);

	// 구체를 만들기 위한 정규화
	D3DXVec3Normalize(
		&_attribute->vVelocity,
		&_attribute->vVelocity);

	_attribute->vVelocity *= 10.0f;

	_attribute->Color = D3DXCOLOR(
		Get_RandomFloat(0.0f, 1.0f),
		Get_RandomFloat(0.0f, 1.0f),
		Get_RandomFloat(0.0f, 1.0f),
		1.0f
	);

	_attribute->fAge = 0.0f;
	_attribute->fLifeTime = 3.0f;
}

_int CEffectFirecrackerWreckage::Update_Object(const _float& fTimeDelta)
{
	m_fTime += 5.f * fTimeDelta;

	if (m_fTime > 3.f)
		Engine::EventManager()->DeleteObject(this);

	for (auto& iter : m_ParticleList)
	{
		if (iter.bAlive)
		{
			iter.vPosition += iter.vVelocity * fTimeDelta;

			iter.fAge += fTimeDelta;

			if (iter.fAge > iter.fLifeTime)
				iter.bAlive = false;

			// 경계 범위를 벗어났는가?
			if (m_BoundingBox.vMax.x < iter.vPosition.x || m_BoundingBox.vMin.x > iter.vPosition.x ||
				m_BoundingBox.vMax.y < iter.vPosition.y || m_BoundingBox.vMin.y > iter.vPosition.y ||
				m_BoundingBox.vMax.z < iter.vPosition.z || m_BoundingBox.vMin.z > iter.vPosition.z)
			{
				// 재활용
				Initial_Particle(&iter);
			}
		}
	}

	Engine::Renderer()->Add_RenderGroup(RENDER_EFFECT, this);

	return 0;
}

void CEffectFirecrackerWreckage::LateUpdate_Object(void)
{
	CTempParticle::LateUpdate_Object();
}

void CEffectFirecrackerWreckage::Render_Object()
{
	m_pGraphicDev->SetRenderState(D3DRS_FOGENABLE, FALSE);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());
	CTempParticle::Render_Object();

	m_pGraphicDev->SetRenderState(D3DRS_FOGENABLE, TRUE);
}

CEffectFirecrackerWreckage* CEffectFirecrackerWreckage::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vOriginPos, int numParticles)
{
	CEffectFirecrackerWreckage* pInstance = new CEffectFirecrackerWreckage(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(vOriginPos, numParticles)))
	{
		Safe_Release(pInstance);
		MSG_BOX("CEffectFirecrackerWreckage Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CEffectFirecrackerWreckage::Free()
{
	__super::Free();
}
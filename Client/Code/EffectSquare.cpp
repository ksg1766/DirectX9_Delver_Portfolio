#include "stdafx.h"
#include "..\Header\EffectSquare.h"

CEffectSquare::CEffectSquare(LPDIRECT3DDEVICE9 pGraphicDev)
	: CTempParticle(pGraphicDev)
{
}


CEffectSquare::~CEffectSquare()
{

}

HRESULT CEffectSquare::Ready_Object(_vec3 vOriginPos, int numParticles, ParticleBoundingBox* boundbox, const _tchar* pPath)
{
	m_eObjectTag = OBJECTTAG::EFFECT;

	m_vOrigin = vOriginPos;
	m_fSize = 0.00005f;
	m_vbSize = 2048;
	m_vbOffset = 0;
	m_vbBatchSize = 512;
	m_BoundingBox = *boundbox;

	for (int i = 0; i < numParticles; i++)
		Add_Particle();

	CTempParticle::Ready_Object(pPath);

	return S_OK;
}

void CEffectSquare::Initial_Particle(ParticleAttribute* _attribute)
{
	_attribute->bAlive = true;

	_attribute->vPosition = m_vOrigin;
	_vec3 min = _vec3(-.05f, -1.0f, -.05f);
	_vec3 max = _vec3(.05f, 0.0f, .05f);

	Get_RandomVector(
		&_attribute->vVelocity,
		&min,
		&max
	);

	// 구체를 만들기 위한 정규화
	D3DXVec3Normalize(
		&_attribute->vVelocity,
		&_attribute->vVelocity);

	_attribute->vVelocity *= 20.0f;

	_attribute->Color = D3DXCOLOR(
		Get_RandomFloat(0.0f, 1.0f),
		Get_RandomFloat(0.0f, 1.0f),
		Get_RandomFloat(0.0f, 1.0f),
		1.0f
	);

	_attribute->fAge = 0.0f;
	_attribute->fLifeTime = 3.0f;
}

_int CEffectSquare::Update_Object(const _float & fTimeDelta)
{
	m_fTime += 5.f * fTimeDelta;

	if(m_fTime > 5.f)
		Engine::EventManager()->DeleteObject(this);

	for (auto& iter : m_ParticleList)
	{
		if (iter.bAlive)
		{
			if (m_fTime < .5f)
			{
				iter.vPosition += iter.vVelocity * fTimeDelta;
			}
			else
			{
				iter.vPosition.y -= 10.f * fTimeDelta * 3.f;
				iter.vPosition += iter.vVelocity * fTimeDelta;
			}

			//iter.vPosition += iter.vVelocity * fTimeDelta;// *m_fSpeed;
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

void CEffectSquare::LateUpdate_Object(void)
{
	CTempParticle::LateUpdate_Object();
}

void CEffectSquare::Render_Object()
{
	//m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matPosWorld);
	
	CTempParticle::Render_Object();
}

void CEffectSquare::OnCollisionEnter(CCollider* _pOther)
{
}

void CEffectSquare::OnCollisionStay(CCollider* _pOther)
{
}

void CEffectSquare::OnCollisionExit(CCollider* _pOther)
{
}

CEffectSquare* CEffectSquare::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vOriginPos, int numParticles, ParticleBoundingBox* boundbox, const _tchar* pPath)
{
	CEffectSquare* pInstance = new CEffectSquare(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(vOriginPos, numParticles, boundbox, pPath)))
	{
		Safe_Release(pInstance);
		MSG_BOX("CEffectSquare Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CEffectSquare::Free()
{
}

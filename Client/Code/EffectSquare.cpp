#include "stdafx.h"
#include "..\Header\EffectSquare.h"

CEffectSquare::CEffectSquare(LPDIRECT3DDEVICE9 pGraphicDev)
	: CTempParticle(pGraphicDev)
{
}

CEffectSquare::~CEffectSquare()
{
	Free();
}

HRESULT CEffectSquare::Ready_Object(_vec3 vOriginPos, int numParticles, EFFECTCOLOR _Color)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	for (int i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	m_eObjectTag = OBJECTTAG::EFFECT;
	m_ParticleTag = PARTICLETAG::PARTICLE_SQUARE;
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

	m_ParticleColor = _Color;

	if (m_ParticleColor != EFFECTCOLOR::ECOLOR_NONE) {
		Read_Path(_Color);
	}

	return S_OK;
}

void CEffectSquare::Read_Path(EFFECTCOLOR _color)
{
	_tchar* pPath = nullptr;

	switch (m_ParticleColor)
	{
	case Engine::ECOLOR_PINK:
		pPath = L"../Bin/SRSource/Effect/Square_effect/Square_effect_Alien.png";
		break;
	case Engine::ECOLOR_RED:
		pPath = L"../Bin/SRSource/Effect/Square_effect/Square_effect_Red.png";
		break;
	case Engine::ECOLOR_ORANGE:
		break;
	case Engine::ECOLOR_YELLOW:
		pPath = L"../Bin/SRSource/Effect/Square_effect/Square_effect_Yellow.png";
		break;
	case Engine::ECOLOR_LIGHTGREEN:
		break;
	case Engine::ECOLOR_GREEN:
		pPath = L"../Bin/SRSource/Effect/Square_effect/Square_effect_Slime.png";
		break;
	case Engine::ECOLOR_SKYBLUE:
		break;
	case Engine::ECOLOR_BLUE:
		pPath = L"../Bin/SRSource/Effect/Square_effect/Square_effect_Blue.png";
		break;
	case Engine::ECOLOR_INDIGO:
		break;
	case Engine::ECOLOR_PURPLE:
		break;
	case Engine::ECOLOR_BROWN:
		pPath = L"../Bin/SRSource/Effect/Square_effect/Square_effect_Bat.png";
		break;
	case Engine::ECOLOR_APRICOT:
		pPath = L"../Bin/SRSource/Effect/Square_effect/Square_effect_BasicSkeleton.png";
		break;
	case Engine::ECOLOR_WHITE:
		pPath = L"../Bin/SRSource/Effect/Square_effect/Square_effect_White.png";
		break;
	case Engine::ECOLOR_RAINBOW:
		pPath = L"../Bin/SRSource/Effect/Square_effect/Square_effect_White.png";
		break;
	default:
		break;
	}

	CTempParticle::Ready_Object(pPath);
}

void CEffectSquare::Set_MonsterEffectColor(MONSTERTAG _MonsterTag)
{
	switch (_MonsterTag)
	{
	case MONSTERTAG::SPIDER:
		m_ParticleColor = EFFECTCOLOR::ECOLOR_BROWN;
		break;
	case MONSTERTAG::WARRIOR:
		m_ParticleColor = EFFECTCOLOR::ECOLOR_RED;
		break;
	case MONSTERTAG::BAT:
		m_ParticleColor = EFFECTCOLOR::ECOLOR_BROWN;
		break;
	case MONSTERTAG::WIZARD:
		m_ParticleColor = EFFECTCOLOR::ECOLOR_APRICOT;
		break;
	case MONSTERTAG::ALIEN:
		m_ParticleColor = EFFECTCOLOR::ECOLOR_PINK;
		break;
	case MONSTERTAG::SLIME:
		m_ParticleColor = EFFECTCOLOR::ECOLOR_GREEN;
		break;
	case MONSTERTAG::SKELETON:
		m_ParticleColor = EFFECTCOLOR::ECOLOR_APRICOT;
		break;
	case MONSTERTAG::SKULLGHOST:
		m_ParticleColor = EFFECTCOLOR::ECOLOR_WHITE;
		break;
	case MONSTERTAG::WORM:
		m_ParticleColor = EFFECTCOLOR::ECOLOR_RED;
		break;
	default:
		m_ParticleColor = EFFECTCOLOR::ECOLOR_RED;
		break;
	}

	Read_Path(m_ParticleColor);
}

void CEffectSquare::Initial_Particle(ParticleAttribute* _attribute)
{
	_attribute->bAlive = true;

	_attribute->vPosition = _vec3(0.f, 0.f, 0.f);
	_vec3 min = _vec3(-.05f, -.05f, -.05f);
	_vec3 max = _vec3(.05f, 0.05f, .05f);

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
		1.0f,
		1.0f,
		1.0f,
		1.0f
	);

	_attribute->fAge = 0.0f;
	_attribute->fLifeTime = 3.0f;
}

_int CEffectSquare::Update_Object(const _float& fTimeDelta)
{
	m_fTime += 5.f * fTimeDelta;

	if (m_fTime > 3.f)
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
				iter.vPosition.y -= 10.f * fTimeDelta;
				iter.vPosition += iter.vVelocity * fTimeDelta;
			}

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
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());

	CTempParticle::Render_Object();
}

CEffectSquare* CEffectSquare::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vOriginPos, int numParticles, EFFECTCOLOR _Color)
{
	CEffectSquare* pInstance = new CEffectSquare(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(vOriginPos, numParticles, _Color)))
	{
		Safe_Release(pInstance);
		MSG_BOX("CEffectSquare Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CEffectSquare::Free()
{
	__super::Free();
}
#include "..\..\Header\TempParticle.h"

CTempParticle::CTempParticle(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CTempParticle::CTempParticle(const CTempParticle& rhs)
	: CGameObject(rhs.m_pGraphicDev)
{
}

CTempParticle::~CTempParticle()
{
}

HRESULT CTempParticle::Ready_Object(const _tchar* pPath)
{
	TCHAR	szFileName[128] = L"";
	wsprintf(szFileName, pPath);

	FAILED_CHECK_RETURN(D3DXCreateTextureFromFile(m_pGraphicDev, szFileName, (LPDIRECT3DTEXTURE9*)&m_pTexture), E_FAIL);

	FAILED_CHECK_RETURN(m_pGraphicDev->CreateVertexBuffer(
		m_vbSize * sizeof(ParticleVTX),
		D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS | D3DUSAGE_WRITEONLY,
		FVF_Particle,
		D3DPOOL_DEFAULT,
		&m_pVB,
		0),
		E_FAIL);

	return S_OK;
}

_int CTempParticle::Update_Object(const _float& fTimeDelta)
{
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

	return 0;
}

void CTempParticle::LateUpdate_Object(void)
{
	Remove_DeadParticle();
}

void CTempParticle::Render_Object(void)
{
	if (!m_ParticleList.empty())
	{
		// 렌더 상태를 지정한다.
		PreRender_Object();

		m_pGraphicDev->SetTexture(0, m_pTexture);
		m_pGraphicDev->SetFVF(FVF_Particle);
		m_pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(ParticleVTX));

		// 버텍스 버퍼를 벗어날 경우 처음부터 시작한다.
		if (m_vbOffset >= m_vbSize)
			m_vbOffset = 0;

		ParticleVTX* v = 0;

		m_pVB->Lock(
			m_vbOffset * sizeof(ParticleVTX),
			m_vbBatchSize * sizeof(ParticleVTX),
			(void**)&v,
			m_vbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);

		DWORD numParticlesInBatch = 0;

		// 모든 파티클이 렌더링될 때까지
		for (auto& iter : m_ParticleList)
		{
			if (iter.bAlive)
			{
				// 한 단계의 생존한 파티클을 다음 버텍스 세그먼트로 복사한다.
				v->vPosition = iter.vPosition;
				v->Color = (D3DCOLOR)iter.Color;
				v++; // next element;
				numParticlesInBatch++; // 단계 카운터를 증가

				// 현재 단계가 모두 채워져 있는가?
				if (numParticlesInBatch == m_vbBatchSize)
				{
					// 버텍스 버퍼로 복사된 마지막 단계의 파티클들을 그린다
					m_pVB->Unlock();

					m_pGraphicDev->DrawPrimitive(D3DPT_POINTLIST, m_vbOffset, m_vbBatchSize);

					// 단계가 그려지는 동안 다음 단계를 파티클로 채운다.
					// 다음 단계의 처음 오프셋으로 이동한다.
					m_vbOffset += m_vbBatchSize;

					// 버텍스 버퍼의 경계를 넘는 메모리로 오프셋을 설정하지 않는다. 경계를 넘을 경우 처음부터 시작한다.
					if (m_vbOffset >= m_vbSize)
						m_vbOffset = 0;

					m_pVB->Lock(
						m_vbOffset * sizeof(ParticleVTX),
						m_vbBatchSize * sizeof(ParticleVTX),
						(void**)&v,
						m_vbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);

					numParticlesInBatch = 0; // 다음 단계를 위한 리셋
				}
			}
		}

		m_pVB->Unlock();

		// (numParticlesInBatch == vbBatchSize) 조건이 만족되지 못하여 마지막 단계가 렌더링되지 않는 경우가 발생할 수 있다.
		// 일부만 채워진 단계는 이곳에서 렌더링 된다.
		if (numParticlesInBatch)
		{
			m_pGraphicDev->DrawPrimitive(D3DPT_POINTLIST, m_vbOffset, numParticlesInBatch);
		}

		// 다음 블록
		m_vbOffset += m_vbBatchSize;

		PostRender_Object();
	}
}

void CTempParticle::PreRender_Object(void)
{
	_matrix WorldMatrix;
	D3DXMatrixIdentity(&WorldMatrix);

	WorldMatrix._41 = m_vOrigin.x;
	WorldMatrix._42 = m_vOrigin.y;
	WorldMatrix._43 = m_vOrigin.z;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &WorldMatrix);

	//m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, false);
	m_pGraphicDev->SetRenderState(D3DRS_POINTSPRITEENABLE, true);
	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALEENABLE, true);
	m_pGraphicDev->SetRenderState(D3DRS_POINTSIZE, *((DWORD*)&m_fSize));
	//m_pGraphicDev->SetRenderState(D3DRS_POINTSIZE_MIN, (DWORD)0.0f);

	// 거리에 따른 파티클의 크기를 제어.
	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALE_A, (DWORD)0.0f); // 해당 변수로 기본 크기 조정함.
	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALE_B, (DWORD)0.0f);
	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALE_C, *((DWORD*)&m_fSize));

	// 텍스처의 알파를 이용.
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//// z
	//m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
	//m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	//m_pGraphicDev->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
}

void CTempParticle::PostRender_Object(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_POINTSPRITEENABLE, false);
	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALEENABLE, false);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
}

void CTempParticle::Remove_DeadParticle() // 제거하기보다는 재활용하는 것이 효과적일 수도 있다.
{
	for (auto iter = m_ParticleList.begin(); iter != m_ParticleList.end();)
	{
		if ((iter)->bAlive == false)
		{
			iter = m_ParticleList.erase(iter);
		}
		else
			++iter;
	}
}

void CTempParticle::Free(void)
{
	Safe_Release(m_pVB);
	Safe_Release(m_pTexture);

	CGameObject::Free();
}
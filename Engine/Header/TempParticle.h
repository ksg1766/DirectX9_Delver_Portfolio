#pragma once
#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CTempParticle : public CGameObject
{
protected:
	explicit CTempParticle(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTempParticle(const CTempParticle& rhs);
	virtual ~CTempParticle();

public:
	bool isEmpty() { return m_ParticleList.empty(); }
	bool isDead() {
		// 파티클이 모두 죽은 경우 True를 리턴.
		for (const auto& attribute : m_ParticleList)
		{
			if (attribute.bAlive)
				return false;
		}
		return true;
	}

protected:
	virtual void    Initial_Particle(ParticleAttribute* _attribute)PURE; // 파티클마다 리셋되는 방식이 달라진다.
	virtual void    Add_Particle() {
		ParticleAttribute attribute;
		Initial_Particle(&attribute);
		m_ParticleList.push_back(attribute);
	}


protected:
	HRESULT			Ready_Object(const _tchar* pPath);
	_int			Update_Object(const _float& fTimeDelta);
	void		    LateUpdate_Object(void);
	void		    Render_Object(void);


protected:
	void Get_RandomVector(_vec3* out, _vec3* min, _vec3* max)
	{
		out->x = Get_RandomFloat(min->x, max->x);
		out->y = Get_RandomFloat(min->y, max->y);
		out->z = Get_RandomFloat(min->z, max->z);
	}

	float Get_RandomFloat(float lowBound, float hightBound)
	{
		if (lowBound >= hightBound)
			return lowBound;

		float f = (rand() % 10000) * 0.0001f;

		return (f * (hightBound - lowBound)) + lowBound;
	}

private:
	virtual void Reset_List()
	{
		// 리스트 내의 모든 파티클 시스템 리셋
		for (auto& iter : m_ParticleList) {
			Initial_Particle(&iter);
		}
	}
	virtual	void PreRender_Object();
	virtual	void PostRender_Object();
	virtual void Remove_DeadParticle();

protected:
	EFFECTCOLOR m_ParticleColor = EFFECTCOLOR::ECOLOR_END;

	LPDIRECT3DVERTEXBUFFER9			m_pVB;
	LPDIRECT3DTEXTURE9              m_pTexture;

	_vec3 m_vOrigin;      // 파티클이 시작되는 곳
	float m_fSize;        // 모든 파티클의 크기
	float m_fEmitRate;    // 새로운 파티클이 추가되는 비율
	int   m_maxParticles; // 주어진 시간 동안 가질 수 있는 최대 파티클의 수

	DWORD m_vbSize;       // 버텍스 버퍼가 보관할 수 있는 파티클의 수 (실제 개수와는 독립적)
	DWORD m_vbOffset;     // 버텍스 버퍼에서 복사를 시작할 파티클 내 다음 단계로의 오프셋
	DWORD m_vbBatchSize;  // 하나의 단계에 정의된 파티클의 수

	ParticleBoundingBox     m_BoundingBox;      // 파티클 출력 영역
	list<ParticleAttribute> m_ParticleList; // 파티클 속성의 리스트 : 파티클을 만들고 제거하고 갱신하는데 이용.

protected:
	virtual void	Free(void);
};

END

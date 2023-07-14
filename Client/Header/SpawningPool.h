#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CMonster;
class CCubeColBf;

END

class CSpawningPool :
    public Engine::CGameObject
{
    // TODO : Make Observer? IDK...
public:
    explicit CSpawningPool(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CSpawningPool(const CSpawningPool& rhs);
    virtual ~CSpawningPool();

public:
    virtual HRESULT Ready_Object()  override;
    virtual _int    Update_Object(const _float& fTimeDelta) override;

    HRESULT			InitSpawnerBox();

    void            ReserveSpawn();

    _int            Get_PoolCapacity()  { return m_iPoolCapacity; }
    _float          Get_SpawnRadius()   { return m_fSpawnRadius; }
    _float          Get_SpawnTime()     { return m_fSpawnTime; }
    MONSTERTAG      Get_MonsterTag()    { return m_eMonsterTag; }

    vector<_vec3>&  LoadSpawnerVertex() { return m_vecSpawnerVertex; }
    vector<INDEX32>&LoadSpawnerIndex()  { return m_vecSpawnerIndex; }

    void            Set_PoolCapacity(_int _iCapacity) { m_iPoolCapacity = _iCapacity; }
    void            Set_SpawnRadius(_float _fRadius);
    void            Set_SpawnTime(_float _fTime) { m_fSpawnTime = _fTime; }
    void            Set_MonsterTag(MONSTERTAG _eMonsterTag) { m_eMonsterTag = _eMonsterTag; }

private:
    HRESULT		    Add_Component();

protected:
    //_int            m_iReservedCount = 0;
    _int            m_iPoolCapacity = 5;
    _float          m_fSpawnRadius = 1.0f;
    _float          m_fSpawnTime = 10.0f;
    MONSTERTAG      m_eMonsterTag;  //

    deque<_float>   m_dqReserved;
    list<CMonster*> m_MonsterList;

    // Only Used in Debug mode
    CCubeColBf*     m_pBuffer;

    vector<_vec3>	m_vecSpawnerVertex;
    vector<INDEX32>	m_vecSpawnerIndex;

public:
    static CSpawningPool* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
    virtual void	Free(void);

    // CGameObject을(를) 통해 상속됨
    virtual void Render_Object(void) override;
};
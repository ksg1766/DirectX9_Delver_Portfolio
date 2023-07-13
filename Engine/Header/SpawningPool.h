#pragma once
#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CSpawningPool :
    public CBase
{
    // TODO : Make Observer? IDK...
public:
    explicit CSpawningPool();
    virtual ~CSpawningPool();

public:
    void    ReserveSpawn();

    void AddMonsterCount(int value) { m_iMonsterCount += value; }
    void SetKeepMonsterCount(int count) { m_iKeepMonsterCount = count; }

private:
    _int    m_iMonsterCount = 0;
    _int    m_iReserveCount = 0;
    _int    m_iKeepMonsterCount = 5;
    _vec3   m_vSpawnPos;
    _float  m_fSpawnRadius = 20.0f;
    _float  m_fSpawnTime = 10.0f;
};

END
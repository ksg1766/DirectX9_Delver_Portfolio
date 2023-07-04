#pragma once
#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CPoolManager :
    public CBase
{
    DECLARE_SINGLETON(CPoolManager)

	//TODO : Make Template
private:
	explicit CPoolManager();
	virtual ~CPoolManager();

	void	Ready_Pool();
	void	TakeOut_Pool(_vec3 _vSpawnPos);
	void	Return_Pool(CGameObject* _pGameObject);

	virtual void Free() override;


private:
	class ObjectPool:
		public CBase
	{
	public:
		ObjectPool();
		~ObjectPool();

		queue<CGameObject*>& GetPool() { return m_ObjectList; }

		void Ready_Pool();
		
		virtual void Free() override;

	private:
		static const int iMaxCapacity = 255;
		queue<CGameObject*> m_ObjectList;
	};

private:
	ObjectPool* m_pPool;
};

END
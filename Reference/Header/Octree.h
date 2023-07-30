#pragma once
#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)

class CFrustum;
class COctreeNode;
class ENGINE_DLL COctree :
    public CBase
{
    DECLARE_SINGLETON(COctree)

private:
    explicit COctree();
    virtual ~COctree();

public:
    HRESULT			Ready_Octree();
    _int            Update_Octree();
    void            Render_Octree(LPDIRECT3DDEVICE9 pGraphicDev);

    COctreeNode*    GetOctreeRoot() { return m_pOctreeRoot; }
    COctreeNode*    GetParentNodeByPos(_vec3 vPos, COctreeNode* const pNode);
    CFrustum*       GetFrustum() { return m_pFrustum; }

private:
    COctreeNode*    BuildOctree(_vec3 vCenter, _float fHalfWidth, _int iDepth);
    void            FindCurrentPosNode(CTransform* pTransform, COctreeNode* const pNode);

    _int            IsInFrustum(COctreeNode* pNode);
    void	        FrustumCull(COctreeNode* pNode);

private:
    enum eValue     { Depth_Limit = 6, Width = 256 };
    COctreeNode*    m_pOctreeRoot = nullptr;

    enum FrustemTest { FRUSTUM_OUT, FRUSTUM_PRIMARILLY_IN, FRUSTUM_COMPLETELY_IN, FRUSTUM_UNKNOWN = -1};
    CFrustum*       m_pFrustum = nullptr;

public:
    virtual void    Free();
};

END
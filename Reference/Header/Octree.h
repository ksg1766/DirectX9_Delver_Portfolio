#pragma once
#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)

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
    void            Render_Octree(LPDIRECT3DDEVICE9 pGraphicDev);

    COctreeNode*    GetOctreeRoot() { return m_pOctreeRoot; }
    COctreeNode*    GetParentNodeByPos(_vec3 vPos, COctreeNode* const pNode);

private:
    COctreeNode*    BuildOctree(_vec3 vCenter, _float fHalfWidth, _int iDepth);
    void            FindCurrentPosNode(CTransform* pTransform, COctreeNode* const pNode);

private:
    enum eValue     { Depth_Limit = 6, Width = 256 };
    COctreeNode*    m_pOctreeRoot = nullptr;

public:
    virtual void    Free();
};

END
#pragma once
#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)

class BoundingBox
{
public:
    _float  GetRadius() { return m_fRadius; }
    void    SetRadius(_float _fRadius) { m_fRadius = _fRadius; }

private:
    _float  m_fRadius = 0.f;
};

class CGameObject;
class CCollider;

class ENGINE_DLL COctreeNode :
    public CBase
{
public:
    explicit COctreeNode();
    explicit COctreeNode(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit COctreeNode(const CGameObject& rhs);
    virtual ~COctreeNode();

public:
    HRESULT                 InitBoundingBoxVisible();

    void                    AddChildNode(COctreeNode* pChild);
    void                    AddObject(CGameObject* pObject)         { m_vecObjects.push_back(pObject); }

    _bool                   IsInNode(const _vec3 _vPos);
    _vec3                   GetPosition()                           { return m_vPosition; }
    COctreeNode* const      GetParent()                             { return m_pParent; }
    vector<COctreeNode*>&   GetChildren()                           { return m_vecChildren; }
    COctreeNode* const      GetChildNode(_uint _iChildNum)          { return m_vecChildren[_iChildNum]; }

    vector<CGameObject*>&   GetObjectList()                         { return m_vecObjects; }
    BoundingBox*            GetBoundingBox()                        { return m_pBoundBox; }

    void                    SetPosition(_vec3 _vPos)                { m_vPosition = _vPos; }
    void                    SetParent(COctreeNode* const _pParent)  { m_pParent = _pParent; }

    void                    Render_OctreeNode(LPDIRECT3DDEVICE9 pGraphicDev);

    // void                 CullNode(_bool);
    // _bool                IsCulled();

public:
    const static _uint      m_iChild_Node_Count = 8;

private:
    BoundingBox*            m_pBoundBox = nullptr;
    const _float            m_fLooseFactor = 2.f;

    vector<COctreeNode*>    m_vecChildren;
    vector<CGameObject*>    m_vecObjects;
    COctreeNode*            m_pParent   = nullptr;
    _vec3                   m_vPosition;
    //  _bool               m_bCulled;

    // Only Used in Debug mode
    LPDIRECT3DVERTEXBUFFER9 m_pVB;
    LPDIRECT3DINDEXBUFFER9	m_pIB;

public:
    virtual void Free();
};

END
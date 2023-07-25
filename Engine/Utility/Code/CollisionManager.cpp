#include "Export_Utility.h"
#include "OctreeNode.h"

IMPLEMENT_SINGLETON(CCollisionManager)

CCollisionManager::CCollisionManager()
{
	Reset();
	CheckGroup(OBJECTTAG::PLAYER, OBJECTTAG::MONSTER);
	CheckGroup(OBJECTTAG::PLAYER, OBJECTTAG::BOSS);
	CheckGroup(OBJECTTAG::PLAYER, OBJECTTAG::MONSTERBULLET);
	CheckGroup(OBJECTTAG::PLAYER, OBJECTTAG::ITEM);
	CheckGroup(OBJECTTAG::PLAYER, OBJECTTAG::NPC);
	CheckGroup(OBJECTTAG::PLAYER, OBJECTTAG::TRAP);
	CheckGroup(OBJECTTAG::PLAYER, OBJECTTAG::FRAGILE);
	CheckGroup(OBJECTTAG::PLAYER, OBJECTTAG::IMMORTAL);
	CheckGroup(OBJECTTAG::MONSTER, OBJECTTAG::ITEM);
	CheckGroup(OBJECTTAG::MONSTER, OBJECTTAG::PLAYERBULLET);
	CheckGroup(OBJECTTAG::BOSS, OBJECTTAG::LOSTSOUL);
	CheckGroup(OBJECTTAG::BOSS, OBJECTTAG::PLAYERBULLET);
	CheckGroup(OBJECTTAG::ITEM, OBJECTTAG::FRAGILE);
	CheckGroup(OBJECTTAG::ITEM, OBJECTTAG::MONSTERBULLET);
	CheckGroup(OBJECTTAG::FRAGILE, OBJECTTAG::PLAYERBULLET);
}

CCollisionManager::~CCollisionManager()
{
}

void CCollisionManager::LateUpdate_Collision()
{
	if (SceneManager()->Get_GameStop()) { return; }

	// Static Object
	CheckCollisionStatic(OBJECTTAG::PLAYER);
	CheckCollisionStatic(OBJECTTAG::MONSTER);
	CheckCollisionStatic(OBJECTTAG::BOSS);
	CheckCollisionStatic(OBJECTTAG::MONSTERBULLET);
	CheckCollisionStatic(OBJECTTAG::PLAYERBULLET);
	CheckCollisionStatic(OBJECTTAG::RAY);

	// Dtnamic Object
	for (UINT iRow = 0; iRow < (UINT)OBJECTTAG::OBJECT_END; ++iRow)
		for (UINT iCol = iRow; iCol < (UINT)OBJECTTAG::OBJECT_END; ++iCol)
			if (m_arrCheck[iRow] & (1 << iCol))
				CheckCollisionByType((OBJECTTAG)iRow, (OBJECTTAG)iCol);
}

void CCollisionManager::CheckGroup(OBJECTTAG _eLeft, OBJECTTAG _eRight)
{
	_uint iRow = (_uint)_eLeft;
	_uint iCol = (_uint)_eRight;

	if (iCol < iRow)
	{
		iRow = (_uint)_eRight;
		iCol = (_uint)_eLeft;
	}

	if (m_arrCheck[iRow] & (1 << iCol))
	{
		m_arrCheck[iRow] &= ~(1 << iCol);
	}
	else
	{
		m_arrCheck[iRow] |= (1 << iCol);
	}
}

void CCollisionManager::Reset()
{
	ZeroMemory(m_arrCheck, 0, sizeof(UINT) * (UINT)OBJECTTAG::OBJECT_END);
}

bool CCollisionManager::IsCollision(CCollider * _pLeft, CCollider * _pRight)
{
	_vec3	vLeftPos = _pLeft->GetCenterPos();
	float*	vLeftScale = _pLeft->GetAxisLen();

	_vec3	vRightPos = _pRight->GetCenterPos();
	float*	vRightScale = _pRight->GetAxisLen();

	if ((abs(vRightPos.x - vLeftPos.x) <= (vLeftScale[0] + vRightScale[0])
		&& (abs(vRightPos.y - vLeftPos.y) <= (vLeftScale[1] + vRightScale[1])
			&& (abs(vRightPos.z - vLeftPos.z) <= (vLeftScale[2] + vRightScale[2])))))
	{
		return TRUE;
	}
	return FALSE;

	//struct ST_OBB // OBB구조체
	//{
	//	D3DXVECTOR3 vCenterPos; // 상자 중앙의 좌표
	//	D3DXVECTOR3 vAxisDir[3]; //상자에 평행한 세 축의 단위벡터
	//	float  fAxisLen[3]; // 상자의 평행한 세 축의 길이 fAxisLen[n]은 vAxisDir[n]에 각각 대응한다.
	//};

	// ST_OBB 구조체 두개의 포인터를 각각 인자로 받아, 두 OBB의 충돌 여부를 체크하는 함수

	// 충돌 시 TRUE리턴, 충돌하지 않으면 FALSE 리턴
	/*
	float c[3][3];
	float absC[3][3];
	float d[3];

	float r0, r1, r;
	int i;

	const float cutoff = 0.999999f;
	bool existsParallelPair = false;

	D3DXVECTOR3 diff = _pLeft->GetCenterPos() - _pRight->GetCenterPos();

	for (i = 0; i < 3; ++i)
	{
		c[0][i] = D3DXVec3Dot(&_pLeft->GetAxisDir()[0], &_pRight->GetAxisDir()[i]);
		absC[0][i] = abs(c[0][i]);
		if (absC[0][i] > cutoff)
			existsParallelPair = true;
	}
	d[0] = D3DXVec3Dot(&diff, &_pLeft->GetAxisDir()[0]);
	r = abs(d[0]);
	r0 = _pLeft->GetAxisLen()[0];
	r1 = _pRight->GetAxisLen()[0] * absC[0][0] + _pRight->GetAxisLen()[1] * absC[0][1] + _pRight->GetAxisLen()[2] * absC[0][2];

	if (r > r0 + r1)
		return FALSE;

	for (i = 0; i < 3; ++i)
	{
		c[1][i] = D3DXVec3Dot(&_pLeft->GetAxisDir()[1], &_pRight->GetAxisDir()[i]);
		absC[1][i] = abs(c[1][i]);
		if (absC[1][i] > cutoff)
			existsParallelPair = true;
	}
	d[1] = D3DXVec3Dot(&diff, &_pLeft->GetAxisDir()[1]);
	r = abs(d[1]);
	r0 = _pLeft->GetAxisLen()[1];
	r1 = _pRight->GetAxisLen()[0] * absC[1][0] + _pRight->GetAxisLen()[1] * absC[1][1] + _pRight->GetAxisLen()[2] * absC[1][2];

	if (r > r0 + r1)
		return FALSE;

	for (i = 0; i < 3; ++i)
	{
		c[2][i] = D3DXVec3Dot(&_pLeft->GetAxisDir()[2], &_pRight->GetAxisDir()[i]);
		absC[2][i] = abs(c[2][i]);
		if (absC[2][i] > cutoff)
			existsParallelPair = true;
	}
	d[2] = D3DXVec3Dot(&diff, &_pLeft->GetAxisDir()[2]);
	r = abs(d[2]);
	r0 = _pLeft->GetAxisLen()[2];
	r1 = _pRight->GetAxisLen()[0] * absC[2][0] + _pRight->GetAxisLen()[1] * absC[2][1] + _pRight->GetAxisLen()[2] * absC[2][2];

	if (r > r0 + r1)
		return FALSE;

	r = abs(D3DXVec3Dot(&diff, &_pRight->GetAxisDir()[0]));
	r0 = _pLeft->GetAxisLen()[0] * absC[0][0] + _pLeft->GetAxisLen()[1] * absC[1][0] + _pLeft->GetAxisLen()[2] * absC[2][0];
	r1 = _pRight->GetAxisLen()[0];

	if (r > r0 + r1)
		return FALSE;

	r = abs(D3DXVec3Dot(&diff, &_pRight->GetAxisDir()[1]));
	r0 = _pLeft->GetAxisLen()[0] * absC[0][1] + _pLeft->GetAxisLen()[1] * absC[1][1] + _pLeft->GetAxisLen()[2] * absC[2][1];
	r1 = _pRight->GetAxisLen()[1];

	if (r > r0 + r1)
		return FALSE;

	r = abs(D3DXVec3Dot(&diff, &_pRight->GetAxisDir()[2]));
	r0 = _pLeft->GetAxisLen()[0] * absC[0][2] + _pLeft->GetAxisLen()[1] * absC[1][2] + _pLeft->GetAxisLen()[2] * absC[2][2];
	r1 = _pRight->GetAxisLen()[2];

	if (r > r0 + r1)
		return FALSE;

	if (existsParallelPair == true)
		return TRUE;

	r = abs(d[2] * c[1][0] - d[1] * c[2][0]);
	r0 = _pLeft->GetAxisLen()[1] * absC[2][0] + _pLeft->GetAxisLen()[2] * absC[1][0];
	r1 = _pRight->GetAxisLen()[1] * absC[0][2] + _pRight->GetAxisLen()[2] * absC[0][1];
	if (r > r0 + r1)
		return FALSE;

	r = abs(d[2] * c[1][1] - d[1] * c[2][1]);
	r0 = _pLeft->GetAxisLen()[1] * absC[2][1] + _pLeft->GetAxisLen()[2] * absC[1][1];
	r1 = _pRight->GetAxisLen()[0] * absC[0][2] + _pRight->GetAxisLen()[2] * absC[0][0];
	if (r > r0 + r1)
		return FALSE;

	r = abs(d[2] * c[1][2] - d[1] * c[2][2]);
	r0 = _pLeft->GetAxisLen()[1] * absC[2][2] + _pLeft->GetAxisLen()[2] * absC[1][2];
	r1 = _pRight->GetAxisLen()[0] * absC[0][1] + _pRight->GetAxisLen()[1] * absC[0][0];
	if (r > r0 + r1)
		return FALSE;

	r = abs(d[0] * c[2][0] - d[2] * c[0][0]);
	r0 = _pLeft->GetAxisLen()[0] * absC[2][0] + _pLeft->GetAxisLen()[2] * absC[0][0];
	r1 = _pRight->GetAxisLen()[1] * absC[1][2] + _pRight->GetAxisLen()[2] * absC[1][1];
	if (r > r0 + r1)
		return FALSE;

	r = abs(d[0] * c[2][1] - d[2] * c[0][1]);
	r0 = _pLeft->GetAxisLen()[0] * absC[2][1] + _pLeft->GetAxisLen()[2] * absC[0][1];
	r1 = _pRight->GetAxisLen()[0] * absC[1][2] + _pRight->GetAxisLen()[2] * absC[1][0];
	if (r > r0 + r1)
		return FALSE;

	r = abs(d[0] * c[2][2] - d[2] * c[0][2]);
	r0 = _pLeft->GetAxisLen()[0] * absC[2][2] + _pLeft->GetAxisLen()[2] * absC[0][2];
	r1 = _pRight->GetAxisLen()[0] * absC[1][1] + _pRight->GetAxisLen()[1] * absC[1][0];
	if (r > r0 + r1)
		return FALSE;

	r = abs(d[1] * c[0][0] - d[0] * c[1][0]);
	r0 = _pLeft->GetAxisLen()[0] * absC[1][0] + _pLeft->GetAxisLen()[1] * absC[0][0];
	r1 = _pRight->GetAxisLen()[1] * absC[2][2] + _pRight->GetAxisLen()[2] * absC[2][1];
	if (r > r0 + r1)
		return FALSE;

	r = abs(d[1] * c[0][1] - d[0] * c[1][1]);
	r0 = _pLeft->GetAxisLen()[0] * absC[1][1] + _pLeft->GetAxisLen()[1] * absC[0][1];
	r1 = _pRight->GetAxisLen()[0] * absC[2][2] + _pRight->GetAxisLen()[2] * absC[2][0];
	if (r > r0 + r1)
		return FALSE;

	r = abs(d[1] * c[0][2] - d[0] * c[1][2]);
	r0 = _pLeft->GetAxisLen()[0] * absC[1][2] + _pLeft->GetAxisLen()[1] * absC[0][2];
	r1 = _pRight->GetAxisLen()[0] * absC[2][1] + _pRight->GetAxisLen()[1] * absC[2][0];
	if (r > r0 + r1)
		return FALSE;

	return TRUE;
	*/
}

void CCollisionManager::CheckCollisionByType(OBJECTTAG _eObjectLeft, OBJECTTAG _eObjectRight)
{
	CScene* pScene = SceneManager()->Get_Scene();

	const vector<CGameObject*>& vecLeft = pScene->Get_ObjectList(LAYERTAG::GAMELOGIC, _eObjectLeft);
	const vector<CGameObject*>& vecRight = pScene->Get_ObjectList(LAYERTAG::GAMELOGIC, _eObjectRight);
	map<ULONGLONG, bool>::iterator iter;

	//for (auto& iterL = vecLeft.begin(); iterL != vecLeft.end(); ++iterL)
	for(auto& iterL : vecLeft)
	{
		if (nullptr == iterL->Get_Collider())
			continue;

		//for (auto& iterR = vecRight.begin(); iterR != vecRight.end(); ++iterR)
		for (auto& iterR : vecRight)
		{
			if (nullptr == iterR->Get_Collider() || iterL == iterR)
				continue;

			CCollider* pLeftCol = iterL->Get_Collider();
			//dynamic_cast<CCollider*>(iterL->Get_Component(COMPONENTTAG::COLLIDER, ID_DYNAMIC));
			CCollider* pRightCol = iterR->Get_Collider();
			//dynamic_cast<CCollider*>(iterR->Get_Component(COMPONENTTAG::COLLIDER, ID_DYNAMIC));

			COLLIDER_ID ID;
			ID.Left_id = pLeftCol->GetID();
			ID.Right_id = pRightCol->GetID();

			iter = m_mapColInfo.find(ID.ID);

			if (m_mapColInfo.end() == iter)
			{
				m_mapColInfo.insert(make_pair(ID.ID, false));
				iter = m_mapColInfo.find(ID.ID);
			}

			if (IsCollision(pLeftCol, pRightCol))
			{	// 현재 충돌 중
				if (iter->second)
				{	// 이전에도 충돌
					if (iterL->IsDead() || iterR->IsDead())
					{	// 둘 중 하나 삭제 예정이면 충돌 해제
						pLeftCol->OnCollisionExit(pRightCol);
						pRightCol->OnCollisionExit(pLeftCol);
						iter->second = false;
					}
					else
					{
						pLeftCol->OnCollisionStay(pRightCol);
						pRightCol->OnCollisionStay(pLeftCol);
					}
				}
				else
				{	// 이전에는 충돌 x	// 근데 둘 중 하나 삭제 예정이면 충돌하지 않은 것으로 취급
					if (!iterL->IsDead() && !iterR->IsDead())
					{
						pLeftCol->OnCollisionEnter(pRightCol);
						pRightCol->OnCollisionEnter(pLeftCol);
						iter->second = true;
					}
					else
					{
						pLeftCol->OnCollisionExit(pRightCol);
						pRightCol->OnCollisionExit(pLeftCol);
						iter->second = false;
					}
				}
			}
			else
			{		// 현재 충돌 x면
				if (iter->second)
				{	//이전에는 충돌하고 있었다.
					pLeftCol->OnCollisionExit(pRightCol);
					pRightCol->OnCollisionExit(pLeftCol);
					iter->second = false;
				}
			}
		}
	}
}

void CCollisionManager::CheckCollisionStatic(OBJECTTAG _eObjectLeft)
{
	CScene* pScene = SceneManager()->Get_Scene();

	const vector<CGameObject*>& vecLeft = pScene->Get_ObjectList(LAYERTAG::GAMELOGIC, _eObjectLeft);

	map<ULONGLONG, bool>::iterator iter;

	for (auto& iterL : vecLeft)
	{
		if (nullptr == iterL->Get_Collider())
			continue;

		COctreeNode* pParentNode = Octree()->GetParentNodeByPos(iterL->m_pTransform->m_vInfo[INFO_POS], Octree()->GetOctreeRoot());
		if (!pParentNode) return;
		/*if (iterL->Get_ObjectTag() == OBJECTTAG::PLAYER)
			int a = 0;*/
		const vector<CGameObject*>& vecRight = pParentNode->GetObjectList();

		//for (auto& iterR = vecRight.begin(); iterR != vecRight.end(); ++iterR)
		for (auto& iterR : vecRight)
		{
			if (nullptr == iterR->Get_Collider() || iterL == iterR)
				continue;

			CCollider* pLeftCol = iterL->Get_Collider();
			//dynamic_cast<CCollider*>(iterL->Get_Component(COMPONENTTAG::COLLIDER, ID_DYNAMIC));
			CCollider* pRightCol = iterR->Get_Collider();
			//dynamic_cast<CCollider*>(iterR->Get_Component(COMPONENTTAG::COLLIDER, ID_DYNAMIC));

			COLLIDER_ID ID;
			ID.Left_id = pLeftCol->GetID();
			ID.Right_id = pRightCol->GetID();

			iter = m_mapColInfo.find(ID.ID);

			if (m_mapColInfo.end() == iter)
			{
				m_mapColInfo.insert(make_pair(ID.ID, false));
				iter = m_mapColInfo.find(ID.ID);
			}

			if (IsCollision(pLeftCol, pRightCol))
			{	// 현재 충돌 중
				if (iter->second)
				{	// 이전에도 충돌
					if (iterL->IsDead() || iterR->IsDead())
					{	// 둘 중 하나 삭제 예정이면 충돌 해제
						pLeftCol->OnCollisionExit(pRightCol);
						pRightCol->OnCollisionExit(pLeftCol);
						iter->second = false;
					}
					else
					{
						pLeftCol->OnCollisionStay(pRightCol);
						pRightCol->OnCollisionStay(pLeftCol);
					}
				}
				else
				{	// 이전에는 충돌 x	// 근데 둘 중 하나 삭제 예정이면 충돌하지 않은 것으로 취급
					if (!iterL->IsDead() && !iterR->IsDead())
					{
						pLeftCol->OnCollisionEnter(pRightCol);
						pRightCol->OnCollisionEnter(pLeftCol);
						iter->second = true;
					}
					else
					{
						pLeftCol->OnCollisionExit(pRightCol);
						pRightCol->OnCollisionExit(pLeftCol);
						iter->second = false;
					}
				}
			}
			else
			{		// 현재 충돌 x면
				if (iter->second)
				{	//이전에는 충돌하고 있었다.
					pLeftCol->OnCollisionExit(pRightCol);
					pRightCol->OnCollisionExit(pLeftCol);
					iter->second = false;
				}
			}
		}
	}
}

void CCollisionManager::Free()
{
}

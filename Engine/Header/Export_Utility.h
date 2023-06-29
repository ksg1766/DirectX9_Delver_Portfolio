#ifndef Export_Utility_h__
#define Export_Utility_h__

#include "SceneManager.h"
#include "CollisionManager.h"
#include "EventManager.h"
#include "PrototypeManager.h"
#include "Renderer.h"

#include "TriCol.h"
#include "RcCol.h"
#include "RcTex.h"
#include "TerrainTex.h"
#include "CubeTex.h"

#include "Transform.h"
#include "Texture.h"
#include "Collider.h"
#include "RigidBody.h"
#include "Pipeline.h"

#include "GameObject.h"


BEGIN(Engine)

inline CSceneManager*		SceneManager();
inline CPrototypeManager*	PrototypeManager();
inline CCollisionManager*	CollisionManager();
inline CEventManager*		EventManager();
inline CRenderer*			Renderer();

inline HRESULT			Create_SceneManager(LPDIRECT3DDEVICE9 pGraphicDev, CSceneManager** ppInstance);

inline void				Release_Utility();

#include "Export_Utility.inl"

END

#endif // Export_Utility_h__



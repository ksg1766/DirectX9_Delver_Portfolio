#ifndef Export_Utility_h__
#define Export_Utility_h__

#include "SceneManager.h"
#include "CollisionManager.h"
#include "EventManager.h"
#include "PrototypeManager.h"
#include "Renderer.h"
#include "UIManager.h"
#include "LightManager.h"
#include "Octree.h"


#include "TriCol.h"
#include "PlayerBf.h"
#include "RcTex.h"
#include "TerrainTex.h"
#include "CubeBf.h"
#include "CubeColBf.h"
#include "BossBf.h"
#include "DynamicBf.h"

#include "Transform.h"
#include "Texture.h"
#include "Collider.h"
#include "RigidBody.h"
#include "Light.h"
#include "Camera.h"
#include "BasicStat.h"

// AI
#include "BossAI.h"
#include "Monster.h"

#include "Particle.h"
#include "BillBoard.h"

// State
#include "StateMachine.h"
#include "State.h"
#include "PlayerStat.h"

// Animation
#include "Animation.h"
#include "Animator.h"

#include "GameObject.h"

// Inventory
#include "Inventory.h"

//Font
#include "Font.h"

//Shader
//#include "Shader.h"

BEGIN(Engine)

inline CSceneManager*		SceneManager();
inline CPrototypeManager*	PrototypeManager();
inline CCollisionManager*	CollisionManager();
inline CEventManager*		EventManager();
inline CRenderer*			Renderer();
inline CUIManager*          UIManager();
inline COctree*				Octree();

inline HRESULT				Create_SceneManager(LPDIRECT3DDEVICE9 pGraphicDev, CSceneManager** ppInstance);

inline void					Release_Utility();

#include "Export_Utility.inl"

END

#endif // Export_Utility_h__



#include "Export_Utility.h"


IMPLEMENT_SINGLETON(CLightManager)

CLightManager::CLightManager()
{
}

CLightManager::~CLightManager()
{
}

void CLightManager::Swtich_All()
{
	for (auto& iterType : m_mapLights)
		for(auto& iter : iterType.second)
			iter->Light_Switch();
}

void CLightManager::Free()
{
}

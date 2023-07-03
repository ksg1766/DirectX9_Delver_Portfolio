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
	for (auto& iter : m_LightList)
		iter->Light_Switch();
}

void CLightManager::Free()
{
}

#include "Export_Utility.h"

IMPLEMENT_SINGLETON(COctree)

COctree::COctree()
{
}

COctree::~COctree()
{
	Free();
}

void COctree::Free()
{
}

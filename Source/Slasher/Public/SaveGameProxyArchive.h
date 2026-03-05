
#pragma once

#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

struct FSaveGameProxyArchive : public FObjectAndNameAsStringProxyArchive
{
	FSaveGameProxyArchive(FArchive& InInnerArchive)
		: FObjectAndNameAsStringProxyArchive(InInnerArchive, /*bLoadIfFindFails*/ true)
	{
		ArIsSaveGame = true;
	}
};
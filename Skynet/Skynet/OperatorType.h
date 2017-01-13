#pragma once

#include "TypeSafeEnum.h"

struct OperatorTypeDef
{
	enum type
	{
		None,
		Or,
		And
	};
};
typedef SafeEnum<OperatorTypeDef> OperatorType;
#pragma once

#include "CoreMinimal.h"
#include "DelegateHelpers.generated.h"

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSimpleTaskDelegate);
UDELEGATE()
DECLARE_DYNAMIC_DELEGATE(FSimpleSingleTaskDelegate);
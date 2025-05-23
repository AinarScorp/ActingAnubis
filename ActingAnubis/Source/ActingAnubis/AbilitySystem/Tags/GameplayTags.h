﻿#pragma once

#include "NativeGameplayTags.h"

namespace GameplayTags
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Gameplay_Event)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Gameplay_Ability)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Gameplay_Effect)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status)
	//Movement Mode Status
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_MovementMode_None)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_MovementMode_Falling)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_MovementMode_Flying)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_MovementMode_NavmeshWalking)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_MovementMode_Swimming)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_MovementMode_Walking)
}
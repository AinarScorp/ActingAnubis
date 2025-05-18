#include "GameplayTags.h"

namespace GameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Gameplay_Event, "Gameplay.Event", "")
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Gameplay_Ability, "Gameplay.Ability", "These tags are used only to indicate GameplayAbilities")
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Gameplay_Effect, "Gameplay.Effect", "These taga are used only to indicate GameplayEffects")
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status, "Status", "This Tag should be present on things and remain there for a longer time. This tag should be checked against e.g. HasMatchingGameplayTag")

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_MovementMode_None, "Status.MovementMode.None", "MovementMode None from CMC ")
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_MovementMode_Falling, "Status.MovementMode.Falling", "Movement Mode Falling from CMC")
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_MovementMode_Flying, "Status.MovementMode.Flying", "Movement Mode Flying from CMC")
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_MovementMode_NavmeshWalking, "Status.MovementMode.NavmeshWalking", "Movement Mode NavmeshWalking from CMC")
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_MovementMode_Swimming, "Status.MovementMode.Swimming", "Movement Mode Swimming from CMC")
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_MovementMode_Walking, "Status.MovementMode.Walking", "Movement Mode Walking from CMC")
}
// Fill out your copyright notice in the Description page of Project Settings.

#include "AnubisGameplayAbility.h"
#include "AbilitySystemComponent.h"

UAnubisGameplayAbility::UAnubisGameplayAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

FQuat UAnubisGameplayAbility::MakeQuatFromAxisAndAngle(FVector Axis, float Angle)
{
	return FQuat(Axis,Angle);
}

void UAnubisGameplayAbility::RotateStraightUp(FVector Normal, float DeltaTime, float InterpSpeed,float InterpSpeedRoll, bool& PhaseCompleted)
{
	FVector SurfaceNormal = Normal;
	FVector ActorForward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	FRotator CurrentRot = GetAvatarActorFromActorInfo()->GetActorRotation();

	const float MaxYawSpeed = InterpSpeed;  // degrees/second
	const float MaxRollSpeed = InterpSpeedRoll;
	const float YawTolerance = 1.f;
	const float RollTolerance = 1.f;

	// ------------------- PHASE 1: Align forward to surface -------------------
	if (!PhaseCompleted)
	{
		FVector TargetForward = FVector::VectorPlaneProject(ActorForward, SurfaceNormal).GetSafeNormal();
		float TargetYaw = TargetForward.Rotation().Yaw;
		float YawDiff = FMath::FindDeltaAngleDegrees(CurrentRot.Yaw, TargetYaw);

		if (FMath::Abs(YawDiff) <= YawTolerance)
		{
			PhaseCompleted = true; // done with yaw
		}
		else
		{
			float YawStep = FMath::Clamp(YawDiff, -MaxYawSpeed * DeltaTime, MaxYawSpeed * DeltaTime);
			CurrentRot.Yaw += YawStep;
			GetAvatarActorFromActorInfo()->SetActorRotation(CurrentRot);
			return; // don't start phase 2 yet
		}
	}

	// ------------------- PHASE 2: Align up to surface (roll) -------------------
	FVector ActorUp = GetAvatarActorFromActorInfo()->GetActorUpVector();
	FVector ProjectedUp = FVector::VectorPlaneProject(ActorUp, SurfaceNormal).GetSafeNormal();
	float AngleBetween = FMath::Acos(FVector::DotProduct(ProjectedUp, SurfaceNormal));
	float AngleDeg = FMath::RadiansToDegrees(AngleBetween);

	FVector Cross = FVector::CrossProduct(ProjectedUp, SurfaceNormal);
	float Sign = FVector::DotProduct(Cross, ActorForward) < 0 ? -1.0f : 1.0f;
	float RollDiff = AngleDeg * Sign;

	if (FMath::Abs(RollDiff) <= RollTolerance)
	{
		// Finished roll
		return;
	}

	float RollStep = FMath::Clamp(RollDiff, -MaxRollSpeed * DeltaTime, MaxRollSpeed * DeltaTime);
	CurrentRot.Roll += RollStep;
	GetAvatarActorFromActorInfo()->SetActorRotation(CurrentRot);
}

APlayerController* UAnubisGameplayAbility::GetPlayerControllerFromActorInfo() const
{
	if (!ensure(CurrentActorInfo))
	{
		return nullptr;
	}
	return CurrentActorInfo->PlayerController.Get();
}

UMovementComponent* UAnubisGameplayAbility::GetMovementComponentFromActorInfo() const
{
	if (!ensure(CurrentActorInfo))
	{
		return nullptr;
	}
	return CurrentActorInfo->MovementComponent.Get();
}

void UAnubisGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
	UAbilitySystemComponent* Comp = ActorInfo->AbilitySystemComponent.Get();
	Comp->AddLooseGameplayTags(OwnedTagsUponGrantingAbility);
	BP_OnGiveAbility(*ActorInfo);
}

void UAnubisGameplayAbility::OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnRemoveAbility(ActorInfo, Spec);

	UAbilitySystemComponent* Comp = ActorInfo->AbilitySystemComponent.Get();
	Comp->RemoveLooseGameplayTags(OwnedTagsUponGrantingAbility);
	BP_OnRemoveAbility(*ActorInfo);
}

void UAnubisGameplayAbility::InputTriggered(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTag& InputTag)
{
	BP_InputTriggered(*ActorInfo, InputTag);
}

void UAnubisGameplayAbility::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTag& InputTag)
{
	BP_InputReleased(*ActorInfo, InputTag);
}

void UAnubisGameplayAbility::BP_CancelAllAbilities(FGameplayTagContainer AbilitiesToIgnore)
{
	if (const TWeakObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = GetActorInfo().AbilitySystemComponent; AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->CancelAbilities(nullptr,&AbilitiesToIgnore, this);
	}
}
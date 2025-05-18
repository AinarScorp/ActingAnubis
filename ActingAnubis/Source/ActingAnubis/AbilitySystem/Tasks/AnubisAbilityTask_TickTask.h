// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "ActingAnubis/Helpers/DelegateHelpers.h"
#include "AnubisAbilityTask_TickTask.generated.h"

/**
 *
 */
UCLASS()
class ACTINGANUBIS_API UAnubisAbilityTask_TickTask : public UAbilityTask
{
	GENERATED_BODY()
public:
	UAnubisAbilityTask_TickTask(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "Acting Anubis|Ability|Task", DisplayName = "Tick Task", meta = (DefaultToSelf = "InTaskOwner", BlueprintInternalUseOnly = "true"))
	static UAnubisAbilityTask_TickTask* CreateTickTask(UGameplayAbility* InTaskOwner);
public:
	virtual void TickTask(float DeltaTime) override;
	virtual void OnDestroy(bool bInOwnerFinished) override;
public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTickTaskDelegate, float, DeltaTime);

	UPROPERTY(BlueprintAssignable)
	FTickTaskDelegate OnUpdate;
	//Called when this task is destroyed (ended)
	UPROPERTY(BlueprintAssignable)
	FSimpleTaskDelegate OnUpdateFinished;
};
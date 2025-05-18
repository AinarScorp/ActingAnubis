// Fill out your copyright notice in the Description page of Project Settings.

#include "AnubisAbilityTask_TickTask.h"

UAnubisAbilityTask_TickTask::UAnubisAbilityTask_TickTask(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bTickingTask = true;
}

UAnubisAbilityTask_TickTask* UAnubisAbilityTask_TickTask::CreateTickTask(UGameplayAbility* InTaskOwner)
{
	UAnubisAbilityTask_TickTask* Task = NewAbilityTask<UAnubisAbilityTask_TickTask>(InTaskOwner);
	return Task;
}

void UAnubisAbilityTask_TickTask::TickTask(float DeltaTime)
{
	OnUpdate.Broadcast(DeltaTime);
}

void UAnubisAbilityTask_TickTask::OnDestroy(bool bInOwnerFinished)
{
	OnUpdateFinished.Broadcast();
	Super::OnDestroy(bInOwnerFinished);
}
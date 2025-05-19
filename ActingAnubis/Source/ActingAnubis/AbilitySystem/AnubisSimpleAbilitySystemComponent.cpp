// Fill out your copyright notice in the Description page of Project Settings.

#include "AnubisSimpleAbilitySystemComponent.h"

#include "Library/AnubisAbilitySystemLibrary.h"

UAnubisSimpleAbilitySystemComponent::UAnubisSimpleAbilitySystemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	ReplicationMode = EGameplayEffectReplicationMode::Minimal;
}

void UAnubisSimpleAbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();
	UAnubisAbilitySystemLibrary::InitializeAbilitySystemWithAbilitySets(this,AbilitySets, GetOwner(), GetOwner());
}
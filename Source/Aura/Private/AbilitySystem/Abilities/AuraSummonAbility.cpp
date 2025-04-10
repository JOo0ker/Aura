// Fill out your copyright notice in the Description page of Project Settings.
#include "AbilitySystem/Abilities/AuraSummonAbility.h"

TArray<FVector> UAuraSummonAbility::GetSpawnLocations()
{
	const auto Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const auto Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	const float DefaultSpread = SpawnSpread / NumMinions;
	
	const auto LeftOfSpread = Forward.RotateAngleAxis(-SpawnSpread / 2.f, FVector::UpVector);

	TArray<FVector> SpawnLocations;
	
	for (int32 i = 0; i < NumMinions; i++)
	{
		const auto Direction = LeftOfSpread.RotateAngleAxis(DefaultSpread * i, FVector::UpVector);
		auto ChosenSpawnLocation = Location + Direction * FMath::FRandRange(MinSpawnDistance, MaxSpawnDistance);

		FHitResult Hit;
		GetWorld()->LineTraceSingleByChannel(Hit,
			ChosenSpawnLocation + FVector(0.f, 0.f, 400.f),
			ChosenSpawnLocation - FVector(0.f, 0.f, 400.f),
			ECC_Visibility
			);

		if (Hit.bBlockingHit)
		{
			ChosenSpawnLocation = Hit.ImpactPoint;
		}
		
		SpawnLocations.Add(ChosenSpawnLocation);
	}
	return SpawnLocations;
}

TSubclassOf<APawn> UAuraSummonAbility::GetRandomMinionClass()
{
	const auto Selection = FMath::RandRange(0, MinionCLasses.Num() - 1);
	return MinionCLasses[Selection];
}

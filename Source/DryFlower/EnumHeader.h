// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnumHeader.generated.h"

UCLASS()
class DRYFLOWER_API AEnumHeader : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnumHeader();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

UENUM()
enum RoomType
{
	corrider		UMETA(DisplayName = "Corrider"),
	playerSpawn		UMETA(DisplayName = "PlayerSpawn"),
	enemySpawn		UMETA(DisplayName = "EnemySpawn"),
	escape			UMETA(DisplayName = "Escape"),
};
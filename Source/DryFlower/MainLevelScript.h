// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "EnumHeader.h"
#include "MainLevelScript.generated.h"

/**
 * 
 */
UCLASS()
class DRYFLOWER_API AMainLevelScript : public ALevelScriptActor
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RoomActorsRef)
	TSubclassOf<AActor> corriderRoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RoomActorsRef)
	TSubclassOf<AActor> playerSpawnRoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RoomActorsRef)
	TSubclassOf<AActor> enemySpawnRoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RoomActorsRef)
	TSubclassOf<AActor> dangerRoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RoomActorsRef)
	TSubclassOf<AActor> escapeRoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RoomActorsRef)
	TSubclassOf<AActor> officeRoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RoomActorsRef)
	TSubclassOf<AActor> labatoryRoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RoomActorsRef)
	TSubclassOf<AActor> bathRoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RoomActorsRef)
	TArray<FRoomInfo> roomInfo;

	UFUNCTION(BlueprintCallable)
	void InitialRoomSetup();

	int GetRoomNumberFromIndex(FIntPoint index);
};

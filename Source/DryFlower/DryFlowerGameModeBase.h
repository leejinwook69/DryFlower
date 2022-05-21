// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EnumHeader.h"
#include "BasicRoom.h"
#include "Net/UnrealNetwork.h"
#include "DryFlowerGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class DRYFLOWER_API ADryFlowerGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
	//TEnumAsByte<RoomType> roomType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status, Replicated)
	TSubclassOf<AActor> basicRoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status, Replicated)
	TArray<ABasicRoom*> roomList;

	UFUNCTION(BlueprintCallable)
	void Start();

	UFUNCTION(BlueprintCallable)
	void MakePlayerSpawnRoom();

	bool CanMakePlayerSpawnRoom(int x, int y);

	//TEST
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	FRoomInfo roomInfo;
};

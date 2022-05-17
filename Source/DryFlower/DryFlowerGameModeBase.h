// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EnumHeader.h"
#include "DryFlowerGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class DRYFLOWER_API ADryFlowerGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void Start();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
	TEnumAsByte<RoomType> roomType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
	TSubclassOf<AActor> basicRoom;


};

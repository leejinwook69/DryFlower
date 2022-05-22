// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnumHeader.h"
#include "Net/UnrealNetwork.h"
#include "BasicRoom.generated.h"

UCLASS()
class DRYFLOWER_API ABasicRoom : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABasicRoom();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Infomation, Replicated)
	int roomNumber;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Infomation, Replicated)
	RoomType roomType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WallShape, Replicated)
	bool leftWall;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WallShape, Replicated)
	bool rightWall;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WallShape, Replicated)
	bool topWall;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WallShape, Replicated)
	bool bottomWall;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DoorShape, Replicated)
	bool leftDoor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DoorShape, Replicated)
	bool rightDoor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DoorShape, Replicated)
	bool topDoor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DoorShape, Replicated)
	bool bottomDoor;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void InitialStart(int roomNum, RoomType roomTypeData, bool leftWallExist = true, bool rightWallExist = true, bool topWallExist = true, bool bottomWallExist = true, 
						bool leftDoorExist = false, bool rightDoorExist = false, bool topDoorExist = false, bool bottomDoorExist = false);


};

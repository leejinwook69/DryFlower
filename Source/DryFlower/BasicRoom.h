// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WallShape)
	bool leftWall;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WallShape)
	bool rightWall;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WallShape)
	bool topWall;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WallShape)
	bool bottomWall;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DoorShape)
	bool leftDoor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DoorShape)
	bool rightDoor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DoorShape)
	bool topDoor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DoorShape)
	bool bottomDoor;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void InitialStart(bool leftWallExist = true, bool rightWallExist = true, bool topWallExist = true, bool bottomWallExist = true, 
						bool leftDoorExist = false, bool rightDoorExist = false, bool topDoorExist = false, bool bottomDoorExist = false);


};

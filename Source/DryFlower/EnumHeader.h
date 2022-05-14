// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM()
enum RoomType
{
	corriderRoom		UMETA(DisplayName = "corrider"),
	playerSpawnRoom		UMETA(DisplayName = "playerSpawn"),
	enemySpawnRoom		UMETA(DisplayName = "enemySpawn"),
	exitRoom			UMETA(DisplayName = "exit")
};

/**
 * 
 */
class DRYFLOWER_API EnumHeader
{
public:
	EnumHeader();
	~EnumHeader();
};

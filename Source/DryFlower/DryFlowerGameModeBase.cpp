// Copyright Epic Games, Inc. All Rights Reserved.


#include "DryFlowerGameModeBase.h"
#include "BasicRoom.h" //스폰할 블루프린트 참조
#include "Kismet/GameplayStatics.h" //FinishSpawningActor를 사용하기 위해 추가

void ADryFlowerGameModeBase::Start() //블루프린트 BeginPlay()에서 호출
{
    GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, TEXT("Start!!")); //이 구문이 실행되는지 테스트용 출력

    FTransform SpawnLocation;
    ABasicRoom *instance = GetWorld()->SpawnActorDeferred<ABasicRoom>(basicRoom, SpawnLocation); //스폰 시작
    instance->InitialStart(); //초기화, 변수할당 등
    UGameplayStatics::FinishSpawningActor(instance, instance->GetTransform()); //스폰 종료
}
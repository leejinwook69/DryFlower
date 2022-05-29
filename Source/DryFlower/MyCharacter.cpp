// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InputComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Controller.h"

#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// 캡슐 콜리전 크기 설정
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// 컨트롤러가 회전할 때 캐릭터는 회전하지 않도록 설정
	// 캐릭터가 카메라에 영향을 주도록 놔둔다
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// 캐릭터 무브먼트 설정
	GetCharacterMovement()->bOrientRotationToMovement = true;

	// 카메라 붐 생성 (충돌이 발생할 경우 플레이어 쪽으로 다가가도록 하는 애)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 900.0f;
	CameraBoom->SetRelativeRotation(FRotator(-89.f, -90.f, 0.f));
	CameraBoom->bUsePawnControlRotation = false;
	CameraBoom->bInheritPitch = false;
	CameraBoom->bInheritYaw = false;
	CameraBoom->bInheritRoll = false;


	// 캐릭터를 따라다닐 카메라 생성
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	/*
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Cha2_MakeObject
	(TEXT("AnimMontage'/Game/Character/Animation/Mixamo_Adam/Anim2/Cha2_Make.Cha2_Make'"));
	if (Cha2_MakeObject.Succeeded())
	{
		MakeAnim = Cha2_MakeObject.Object;
	}*/
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveRight", this, &AMyCharacter::MoveRight);
	PlayerInputComponent->BindAxis("MoveForward", this, &AMyCharacter::MoveForward);

	PlayerInputComponent->BindAction("Make", IE_Pressed, this, &AMyCharacter::Make);
	PlayerInputComponent->BindAction("Die", IE_Pressed, this, &AMyCharacter::Die); //임시임
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AMyCharacter::Attack);
}

void AMyCharacter::MoveRight(float Value)
{
	if (DieCheck == false)
	{
		const FRotator YawRotation(0, Controller->GetControlRotation().Yaw, 0);
		const FVector Direction = UKismetMathLibrary::GetRightVector(YawRotation);
		AddMovementInput(Direction, Value);
	}
}


void AMyCharacter::MoveForward(float Value)
{
	if (DieCheck == false)
	{
		const FRotator YawRotation(0, Controller->GetControlRotation().Yaw, 0);
		const FVector Direction = UKismetMathLibrary::GetForwardVector(YawRotation);
		AddMovementInput(Direction, Value);
	}
}

void AMyCharacter::Make() //나중에 Search로 변경
{
	//등록된 몽타주 재생
	if (MakeAnim)
	{
		//들어왔나 확인하는 로그 메시지
		//UE_LOG(LogTemp, Log, TEXT("Log Message"));
		PlayAnimMontage(MakeAnim, 1.f, FName("start_1"));
	}

	/*
	//만드는 중에 못 움직이게 하기
	//MakeCheck = true;

	GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
		{
			// MakeTime(시간)후 아래 코드 실행

			MakeCheck = false;

		}), MakeTime, false);
	*/
}

void AMyCharacter::Attack() //살인마 공격 애니메이션 출력
{
	//등록된 몽타주 재생
	if (IsAttacking == false)
	{
		//들어왔나 확인하는 로그 메시지
		//UE_LOG(LogTemp, Log, TEXT("Log Message"));
		PlayAnimMontage(MakeAnim, 1.f, FName("start_1"));
	}

}

void AMyCharacter::Die() //캐릭터 죽을 때 나올 애니메이션
{
	//UE_LOG(LogTemp, Log, TEXT("Log Message"));
	DieCheck = true;
	//캐릭터 피직스 시뮬레이트
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetSimulatePhysics(true);
}
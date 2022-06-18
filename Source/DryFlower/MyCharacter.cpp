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
	// 캡슐 컴포넌트 크기 설정
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// 조작으로 인해 카메라가 돌지 않도록 설정
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// 캐릭터가 입력 방향 따라 움직이게 설정
	GetCharacterMovement()->bOrientRotationToMovement = true;

	// 카메라붐 위치 설정
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 900.0f;
	CameraBoom->SetRelativeRotation(FRotator(-89.f, -90.f, 0.f));
	CameraBoom->bUsePawnControlRotation = false;
	CameraBoom->bInheritPitch = false;
	CameraBoom->bInheritYaw = false;
	CameraBoom->bInheritRoll = false;


	// 카메라 설정 후 카메라붐에 붙임
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

	//PlayerInputComponent->BindAction("Die", IE_Pressed, this, &AMyCharacter::Die);
	//PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AMyCharacter::Attack);

}

void AMyCharacter::MoveRight(float Value)
{
	if (DieCheck == false)
	{
		const FRotator YawRotation(0, Controller->GetControlRotation().Yaw, 0);
		const FVector Direction = UKismetMathLibrary::GetRightVector(YawRotation);
		AddMovementInput(Direction, Value);
		if (Value != 0.f) // 카메라 원래 위치로 돌리기
		{
			CameraBoom->TargetArmLength = 900.0f;
			CameraBoom->SetRelativeRotation(FRotator(-89.f, -90.f, 0.f));
			StopAnimMontage(MakeAnim);
		}
	}
}


void AMyCharacter::MoveForward(float Value)
{
	if (DieCheck == false)
	{
		const FRotator YawRotation(0, Controller->GetControlRotation().Yaw, 0);
		const FVector Direction = UKismetMathLibrary::GetForwardVector(YawRotation);
		AddMovementInput(Direction, Value);
		if (Value != 0.f)
		{
			CameraBoom->TargetArmLength = 900.0f;
			CameraBoom->SetRelativeRotation(FRotator(-89.f, -90.f, 0.f));
			StopAnimMontage(MakeAnim);
		}
	}
}

void AMyCharacter::NotifyActorBeginOverlap(AActor* OtherActor)
{
	//UE_LOG(LogTemp, Log, TEXT("can"));
	NowCanSearch = true;
}

void AMyCharacter::NotifyActorEndOverlap(AActor* OtherActor)
{
	//UE_LOG(LogTemp, Log, TEXT("cannot"));
	NowCanSearch = false;
}

void AMyCharacter::Make() //나중에 Search로 변경
{
	//등록된 몽타주 재생
	if (NowCanSearch == true)
	{
		// 서치 가능한 상황이면 몽타주 재생
		PlayAnimMontage(MakeAnim, 1.f, FName("start_1"));

		CameraBoom->TargetArmLength = 400.0f;
		CameraBoom->SetRelativeRotation(FRotator(-60.f, -90.f, 0.f));
	}
}


void AMyCharacter::Die()
{
	//UE_LOG(LogTemp, Log, TEXT("Log Message"));
	DieCheck = true;
	//캐릭터 래그돌화
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetSimulatePhysics(true);
}
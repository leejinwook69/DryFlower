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
	// ĸ�� �ݸ��� ũ�� ����
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// ��Ʈ�ѷ��� ȸ���� �� ĳ���ʹ� ȸ������ �ʵ��� ����
	// ĳ���Ͱ� ī�޶� ������ �ֵ��� ���д�
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// ĳ���� �����Ʈ ����
	GetCharacterMovement()->bOrientRotationToMovement = true;

	// ī�޶� �� ���� (�浹�� �߻��� ��� �÷��̾� ������ �ٰ������� �ϴ� ��)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 900.0f;
	CameraBoom->SetRelativeRotation(FRotator(-89.f, -90.f, 0.f));
	CameraBoom->bUsePawnControlRotation = false;
	CameraBoom->bInheritPitch = false;
	CameraBoom->bInheritYaw = false;
	CameraBoom->bInheritRoll = false;


	// ĳ���͸� ����ٴ� ī�޶� ����
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
	PlayerInputComponent->BindAction("Die", IE_Pressed, this, &AMyCharacter::Die); //�ӽ���
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AMyCharacter::Attack);
}

void AMyCharacter::MoveRight(float Value)
{
	if (DieCheck == false)
	{
		const FRotator YawRotation(0, Controller->GetControlRotation().Yaw, 0);
		const FVector Direction = UKismetMathLibrary::GetRightVector(YawRotation);
		AddMovementInput(Direction, Value);
		if (Value != 0.f)
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

void AMyCharacter::Make() //���߿� Search�� ����
{
	//��ϵ� ��Ÿ�� ���
	if (MakeAnim)
	{
		//���Գ� Ȯ���ϴ� �α� �޽���
		PlayAnimMontage(MakeAnim, 1.f, FName("start_1"));

		CameraBoom->TargetArmLength = 400.0f;
		CameraBoom->SetRelativeRotation(FRotator(-60.f, -90.f, 0.f));
	}

	/*
	//����� �߿� �� �����̰� �ϱ�
	//MakeCheck = true;

	GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
		{
			// MakeTime(�ð�)�� �Ʒ� �ڵ� ����

			MakeCheck = false;

		}), MakeTime, false);
	*/
}

void AMyCharacter::Attack() //���θ� ���� �ִϸ��̼� ���, ������ �����ڵ� �۵��� ���߿� ������ ��
{
	//��ϵ� ��Ÿ�� ���
	if (IsAttacking == false)
	{
		//���Գ� Ȯ���ϴ� �α� �޽���
		//UE_LOG(LogTemp, Log, TEXT("Log Message"));
		PlayAnimMontage(MakeAnim, 1.f, FName("start_1"));
	}

}

void AMyCharacter::Die() //ĳ���� ���� �� ���� �ִϸ��̼�
{
	//UE_LOG(LogTemp, Log, TEXT("Log Message"));
	DieCheck = true;
	//ĳ���� ������ �ùķ���Ʈ
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetSimulatePhysics(true);
}
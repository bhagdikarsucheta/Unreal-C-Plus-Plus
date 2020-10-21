// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerMan.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"			//For restarting the game


// Sets default values
APowerMan::APowerMan()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.0f, 69.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 600.0f;
	GetCharacterMovement()->AirControl = 0.2f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);		//Attach it to our Capsule root component

	CameraBoom->TargetArmLength = 300.0f;	//How far away is this springarm going to be from user.
	CameraBoom->bUsePawnControlRotation = true;	//Rotate the arm based on controller
	//now create camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));

	//Attach camera to cameraboom
	//This is going to attach the camera at the end of the boom and let the cameraboom adjust the matchcontroller rotation of the camera
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);	

	//Camera does not rotate relative to the Arm
	FollowCamera->bUsePawnControlRotation = false;

	bDead = false; 

	Power = 100.00f;

}


void APowerMan::RestartGame()
{
	//this class is going to open level and get the world and get the name
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);

}

// Called when the game starts or when spawned
void APowerMan::BeginPlay()
{
	Super::BeginPlay();

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &APowerMan::OnBeginOverlap);
	
	if (Player_Power_Widget_Class != nullptr)
	{
		Player_Power_Widget = CreateWidget(GetWorld(),Player_Power_Widget_Class);
		Player_Power_Widget->AddToViewport();
	}
}

// Called every frame
void APowerMan::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Power -= DeltaTime * Power_Treshold;

	if (Power <= 0)
	{
		if (!bDead)
		{
			bDead = true;

			GetMesh()->SetSimulatePhysics(true);

			//Using world time manager restart game function execute after 3.0f seconds and last param.false as i do not want to repeat
			FTimerHandle UnusedHandle;
			GetWorldTimerManager().SetTimer(UnusedHandle, this, &APowerMan::RestartGame,3.0f,false);
		}
	}
}

// Called to bind functionality to input
void APowerMan::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Bind our component to Pitch and Yaw
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);		//LookUp means Y Rotation

	//ACharacter::Jump is the function inside Character class 
	PlayerInputComponent->BindAction("Jump", IE_Pressed,this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);


	//TO MOVE AROUND OUR CHARACTER
	PlayerInputComponent->BindAxis("MoveForward",  this, &APowerMan::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight",  this, &APowerMan::MoveRight);

}


void APowerMan::MoveForward(float Axis)
{
	if (!bDead)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);


		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Axis);
	}
}

void APowerMan::MoveRight(float Axis)
{
	if (!bDead)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);


		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Axis);
	}
}



void APowerMan::OnBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Recharge"))
	{
		Power += 10.0f;

		if (Power > 100.0f)
			Power = 100.0f;

		OtherActor->Destroy();
	}
}
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFrameWork/CharacterMovementComponent.h"
#include "GameFrameWork/Controller.h"
#include "GameFrameWork/SpringArmComponent.h"
#include "Blueprint/UserWidget.h"

#include "PowerMan.generated.h"

UCLASS()
class MYPOWERMAN_API APowerMan : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APowerMan();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		UCameraComponent* FollowCamera;

	void MoveForward(float Axis);
	void MoveRight(float Axis);

	bool bDead;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float Power;

	UPROPERTY(EditAnywhere)
		float Power_Treshold;

	UFUNCTION()
		void OnBeginOverlap(class UPrimitiveComponent* HitComp,
			class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//Spawn player
	UPROPERTY(EditAnywhere, Category = "UI HUD")
		TSubclassOf<UUserWidget> Player_Power_Widget_Class;
	UUserWidget* Player_Power_Widget;

	void RestartGame();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};

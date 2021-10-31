// Nathaniel Richards 2021

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "Tank.generated.h"

UCLASS()
class TOONTANKS_API ATank : public ABasePawn
{
	GENERATED_BODY()

public:
	ATank();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void HandleDestruction();

	APlayerController* GetTankPlayerController() const { return PlayerControllerRef; };

	bool bAlive = true;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float Speed = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float TurnRate = 50.f;

	void Move(float Value);
	void Turn(float Value);
	void Boost();

	bool Boosting = false;
	float TimeSinceLastBoost = 0.f;
	float BoostLength = 3.f;

	void ValidateBoostAbility();

	class UAudioComponent* AudioComp = nullptr;

	APlayerController* PlayerControllerRef;

	UPROPERTY(EditAnywhere, Category = "Other Sounds")
	USoundBase* EngineSound;
};

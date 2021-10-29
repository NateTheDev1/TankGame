// Nathaniel Richards 2021


#include "Tank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/AudioComponent.h"

ATank::ATank()
{
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp);
}

void ATank::BeginPlay()
{
	Super::BeginPlay();

	GEngine->AddOnScreenDebugMessage(-1, 10000, FColor::Yellow, TEXT("Press E To Boost"));

	AudioComp = FindComponentByClass<UAudioComponent>();

	if(!AudioComp)
	{
		UE_LOG(LogTemp, Error, TEXT("No audio component!"));
	}
}

void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ATank::Move);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATank::Turn);

	PlayerInputComponent->BindAction(TEXT("Boost"), IE_Pressed, this, &ATank::Boost);
}

void ATank::Move(float Value)
{
	FVector DeltaLocation(0.f);
	DeltaLocation.X = Value * GetWorld()->GetDeltaSeconds() * Speed;
	AddActorLocalOffset(DeltaLocation, true);
}

void ATank::Turn(float Value)
{
	FRotator DeltaRotation = FRotator::ZeroRotator;
	DeltaRotation.Yaw = Value * TurnRate * GetWorld()->GetDeltaSeconds();
	AddActorLocalRotation(DeltaRotation, true);
}

void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Boosting)
	{
		if (GEngine)
		{
			GEngine->ClearOnScreenDebugMessages();

			const FString BoostMessage = FString::Printf(
				TEXT("%f seconds left boosting"), TimeSinceLastBoost + BoostLength - GetWorld()->GetTimeSeconds());
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, BoostMessage);

			if (TimeSinceLastBoost + BoostLength <= GetWorld()->GetTimeSeconds())
			{
				GEngine->ClearOnScreenDebugMessages();
				Boosting = false;
				Speed -= 400.f;
				GEngine->AddOnScreenDebugMessage(-1, 10000, FColor::Yellow, TEXT("Press E To Boost"));
			}
		}
	}
}


void ATank::Boost()
{
	if (!Boosting)
	{
		if(AudioComp)
		{
			AudioComp->Play();
		}
		
		GEngine->ClearOnScreenDebugMessages();
		Boosting = true;
		TimeSinceLastBoost = GetWorld()->GetTimeSeconds();
		Speed += 400.f;
	}
}

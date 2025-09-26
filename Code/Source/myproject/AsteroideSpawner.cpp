#include "AsteroideSpawner.h"
#include "Asteroide.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"  // pour GetPlayerPawn

AAsteroideSpawner::AAsteroideSpawner()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AAsteroideSpawner::BeginPlay()
{
	Super::BeginPlay();

	IntervalleSpawn = FMath::FRandRange(0.f, 3.f);
}

void AAsteroideSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TempsEcoule += DeltaTime;
	if (TempsEcoule >= IntervalleSpawn && ClasseAsteroide)
	{
		TempsEcoule = 0.0f;

		// Récupère le vaisseau (pawn du joueur)
		APawn* Vaisseau = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		if (!Vaisseau) return;

		FVector VaisseauPos = Vaisseau->GetActorLocation();

		// Offsets par rapport au vaisseau (ta zone de spawn)
		float OffsetXTop = 1130.0f;
		float OffsetXBottom = -1130.0f;
		float OffsetYLeft = -2460.0f;
		float OffsetYRight = 2460.0f;

		FVector Position;
		int Bord = FMath::RandRange(0, 3);

		switch (Bord)
		{
		case 0: // Gauche
			Position = FVector(
				VaisseauPos.X + FMath::RandRange(OffsetXBottom, OffsetXTop),
				VaisseauPos.Y + OffsetYLeft,
				0
			);
			break;

		case 1: // Droite
			Position = FVector(
				VaisseauPos.X + FMath::RandRange(OffsetXBottom, OffsetXTop),
				VaisseauPos.Y + OffsetYRight,
				0
			);
			break;

		case 2: // Bas
			Position = FVector(
				VaisseauPos.X + OffsetXBottom,
				VaisseauPos.Y + FMath::RandRange(OffsetYLeft, OffsetYRight),
				0
			);
			break;

		case 3: // Haut
			Position = FVector(
				VaisseauPos.X + OffsetXTop,
				VaisseauPos.Y + FMath::RandRange(OffsetYLeft, OffsetYRight),
				0
			);
			break;
		}

		GetWorld()->SpawnActor<AAsteroide>(ClasseAsteroide, Position, FRotator::ZeroRotator);

		IntervalleSpawn = FMath::FRandRange(0.f, 3.f);
	}
}

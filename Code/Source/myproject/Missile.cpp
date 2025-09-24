#include "Missile.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"

// Constructeur
AMissile::AMissile()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshMissile = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshMissile"));
	RootComponent = MeshMissile;
}

void AMissile::BeginPlay()
{
	Super::BeginPlay();

	// Sauvegarde la référence du vaisseau
	if (GetWorld() && GetWorld()->GetFirstPlayerController())
	{
		ReferenceVaisseau = GetWorld()->GetFirstPlayerController()->GetPawn();
	}
}

void AMissile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Déplacement du missile
	if (!Direction.IsZero())
	{
		FVector NewLocation = GetActorLocation() + Direction * Vitesse * DeltaTime;
		SetActorLocation(NewLocation);
	}

	// Vérifie si hors zone
	if (ReferenceVaisseau)
	{
		FVector VaisseauPos = ReferenceVaisseau->GetActorLocation();
		FVector Pos = GetActorLocation();

		// Offsets identiques à l’astéroïde
		float OffsetXTop = 1130.0f;
		float OffsetXBottom = -1130.0f;
		float OffsetYLeft = -2460.0f;
		float OffsetYRight = 2460.0f;

		if (Pos.X < VaisseauPos.X + OffsetXBottom ||
			Pos.X > VaisseauPos.X + OffsetXTop ||
			Pos.Y < VaisseauPos.Y + OffsetYLeft ||
			Pos.Y > VaisseauPos.Y + OffsetYRight)
		{
			Destroy(); // Supprime le missile hors zone
		}
	}
}

void AMissile::InitDirection(const FVector& NewDirection)
{
	Direction = NewDirection.GetSafeNormal();
}

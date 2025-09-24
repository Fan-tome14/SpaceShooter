#include "Asteroide.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"
#include "DrawDebugHelpers.h"
#include "Components/StaticMeshComponent.h"

AAsteroide::AAsteroide()
{
	PrimaryActorTick.bCanEverTick = true;

	// Racine
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	// Mesh
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshAsteroide"));
	Mesh->SetupAttachment(RootComponent);

	// Désactiver physique et collision pour le mouvement
	Mesh->SetSimulatePhysics(false);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AAsteroide::BeginPlay()
{
	Super::BeginPlay();

	// Cherche la première instance de la classe assignée
	if (CibleClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("Asteroide CibleClass = %s"), *CibleClass->GetName());

		TArray<AActor*> FoundPawns;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), CibleClass, FoundPawns);

		if (FoundPawns.Num() > 0)
		{
			CiblePawn = Cast<APawn>(FoundPawns[0]);
			UE_LOG(LogTemp, Warning, TEXT("Asteroide cible trouvée = %s"), *CiblePawn->GetName());
		}
	}

	// Si rien trouvé, prend le Player Pawn par défaut
	if (!CiblePawn)
	{
		CiblePawn = GetWorld()->GetFirstPlayerController()->GetPawn();
		if (CiblePawn)
		{
			UE_LOG(LogTemp, Warning, TEXT("Asteroide prend le PlayerPawn = %s"), *CiblePawn->GetName());
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Asteroide n'a trouvé aucune cible !"));
		}
	}
}

void AAsteroide::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!CiblePawn) return;

	// Calculer la direction vers le vaisseau
	FVector DirectionToVaisseau = (CiblePawn->GetActorLocation() - GetActorLocation()).GetSafeNormal();

	// Déplacer l'astéroïde vers la cible
	FVector NewLocation = GetActorLocation() + DirectionToVaisseau * Vitesse * DeltaTime;
	SetActorLocation(NewLocation);

	// Debug line vers le vaisseau
	DrawDebugLine(
		GetWorld(),
		GetActorLocation(),
		CiblePawn->GetActorLocation(),
		FColor::Red,
		false,
		0.1f,
		0,
		2.0f
	);

	// --- Zone dynamique autour du vaisseau ---
	FVector VaisseauPos = CiblePawn->GetActorLocation();

	float OffsetXTop = 1130.0f;
	float OffsetXBottom = -1130.0f;
	float OffsetYLeft = -2460.0f;
	float OffsetYRight = 2460.0f;

	// Position actuelle de l'astéroïde
	FVector Pos = GetActorLocation();

	// Vérification hors zone
	if (Pos.X < VaisseauPos.X + OffsetXBottom ||
		Pos.X > VaisseauPos.X + OffsetXTop ||
		Pos.Y < VaisseauPos.Y + OffsetYLeft ||
		Pos.Y > VaisseauPos.Y + OffsetYRight)
	{
		Destroy();
	}

	// Debug : dessiner la boîte de zone (autour du vaisseau)
	FVector ZoneCenter = VaisseauPos + FVector((OffsetXTop + OffsetXBottom) / 2.0f, (OffsetYLeft + OffsetYRight) / 2.0f, 0);
	FVector ZoneExtent = FVector((OffsetXTop - OffsetXBottom) / 2.0f, (OffsetYRight - OffsetYLeft) / 2.0f, 50.0f);

	DrawDebugBox(
		GetWorld(),
		ZoneCenter,
		ZoneExtent,
		FColor::Green,
		false,
		0.1f,
		0,
		2.0f
	);
}

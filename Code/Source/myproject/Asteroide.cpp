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

	// D�sactiver physique et collision pour le mouvement
	Mesh->SetSimulatePhysics(false);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AAsteroide::BeginPlay()
{
	Super::BeginPlay();

	// Cherche la premi�re instance de la classe assign�e
	if (CibleClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("Asteroide CibleClass = %s"), *CibleClass->GetName());

		TArray<AActor*> FoundPawns;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), CibleClass, FoundPawns);

		if (FoundPawns.Num() > 0)
		{
			CiblePawn = Cast<APawn>(FoundPawns[0]);
			UE_LOG(LogTemp, Warning, TEXT("Asteroide cible trouv�e = %s"), *CiblePawn->GetName());
		}
	}

	// Si rien trouv�, prend le Player Pawn par d�faut
	if (!CiblePawn)
	{
		CiblePawn = GetWorld()->GetFirstPlayerController()->GetPawn();
		if (CiblePawn)
		{
			UE_LOG(LogTemp, Warning, TEXT("Asteroide prend le PlayerPawn = %s"), *CiblePawn->GetName());
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Asteroide n'a trouv� aucune cible !"));
		}
	}
}

void AAsteroide::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!CiblePawn) return;

	// Calculer la direction vers le vaisseau
	FVector DirectionToVaisseau = (CiblePawn->GetActorLocation() - GetActorLocation()).GetSafeNormal();

	// D�placer l'ast�ro�de vers la cible
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

	// Position actuelle de l'ast�ro�de
	FVector Pos = GetActorLocation();

	// V�rification hors zone
	if (Pos.X < VaisseauPos.X + OffsetXBottom ||
		Pos.X > VaisseauPos.X + OffsetXTop ||
		Pos.Y < VaisseauPos.Y + OffsetYLeft ||
		Pos.Y > VaisseauPos.Y + OffsetYRight)
	{
		Destroy();
	}

	// Debug : dessiner la bo�te de zone (autour du vaisseau)
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

#include "Asteroide.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"
#include "Components/StaticMeshComponent.h"

AAsteroide::AAsteroide()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshAsteroide"));
	Mesh->SetupAttachment(RootComponent);

	// --- Collision setup ---
	Mesh->SetCollisionObjectType(ECC_GameTraceChannel1); // Canal "Asteroide"
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Mesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	Mesh->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block); // bloque décor si besoin
	Mesh->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap); // détecte les missiles
}

void AAsteroide::BeginPlay()
{
	Super::BeginPlay();

	Vies = FMath::RandRange(1, 3);

	if (CibleClass)
	{
		TArray<AActor*> FoundPawns;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), CibleClass, FoundPawns);

		if (FoundPawns.Num() > 0)
		{
			CiblePawn = Cast<APawn>(FoundPawns[0]);
		}
	}

	if (!CiblePawn)
	{
		CiblePawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	}
}

void AAsteroide::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!CiblePawn) return;

	FVector DirectionToVaisseau = (CiblePawn->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	FVector NewLocation = GetActorLocation() + DirectionToVaisseau * Vitesse * DeltaTime;
	SetActorLocation(NewLocation);

	FVector VaisseauPos = CiblePawn->GetActorLocation();

	float OffsetXTop = 1130.0f;
	float OffsetXBottom = -1130.0f;
	float OffsetYLeft = -2460.0f;
	float OffsetYRight = 2460.0f;

	FVector Pos = GetActorLocation();

	if (Pos.X < VaisseauPos.X + OffsetXBottom ||
		Pos.X > VaisseauPos.X + OffsetXTop ||
		Pos.Y < VaisseauPos.Y + OffsetYLeft ||
		Pos.Y > VaisseauPos.Y + OffsetYRight)
	{
		Destroy();
	}
}

void AAsteroide::PrendreDegat()
{
	Vies--;

	if (Vies <= 0)
	{
		Destroy();
	}
}

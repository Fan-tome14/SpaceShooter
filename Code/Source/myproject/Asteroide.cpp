#include "Asteroide.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "SpaceShooterGameMode.h"

AAsteroide::AAsteroide()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshAsteroide"));
	Mesh->SetupAttachment(RootComponent);

	Mesh->SetCollisionObjectType(ECC_GameTraceChannel1);
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Mesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	Mesh->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	Mesh->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
}

void AAsteroide::BeginPlay()
{
	Super::BeginPlay();

	Vies = FMath::RandRange(1, 3);
	InialLife = Vies;

	if (CibleClass)
	{
		TArray<AActor*> FoundPawns;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), CibleClass, FoundPawns);
		if (FoundPawns.Num() > 0)
			CiblePawn = Cast<APawn>(FoundPawns[0]);
	}

	if (!CiblePawn)
		CiblePawn = GetWorld()->GetFirstPlayerController()->GetPawn();
}

void AAsteroide::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!CiblePawn) return;

	FVector DirectionToVaisseau = (CiblePawn->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	FVector NewLocation = GetActorLocation() + DirectionToVaisseau * Vitesse * DeltaTime;
	SetActorLocation(NewLocation);

	FVector VaisseauPos = CiblePawn->GetActorLocation();
	FVector Pos = GetActorLocation();
	if (Pos.X < VaisseauPos.X - 1130.f || Pos.X > VaisseauPos.X + 1130.f ||
		Pos.Y < VaisseauPos.Y - 2460.f || Pos.Y > VaisseauPos.Y + 2460.f)
	{
		Destroy();
	}
}

void AAsteroide::RecevoirDegat()
{
	Vies--;
	if (ASpaceshooterGameMode* GM = Cast<ASpaceshooterGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		GM->addScore();
	}
	if (Vies <= 0)
	{
		Destroy();
	}
}

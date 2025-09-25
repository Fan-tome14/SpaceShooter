#include "Missile.h"
#include "Asteroide.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

AMissile::AMissile()
{
	PrimaryActorTick.bCanEverTick = true;

	// Capsule pour la collision
	CapsuleCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleCollision"));
	CapsuleCollision->InitCapsuleSize(15.f, 50.f);
	RootComponent = CapsuleCollision;

	CapsuleCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CapsuleCollision->SetCollisionObjectType(ECC_WorldDynamic);
	CapsuleCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	CapsuleCollision->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap); // Asteroide
	CapsuleCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore); // ignore vaisseau

	// Mesh visuel attaché à la capsule
	MeshMissile = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshMissile"));
	MeshMissile->SetupAttachment(CapsuleCollision);
	MeshMissile->SetRelativeLocation(FVector::ZeroVector);

	// Collision du mesh désactivée pour ne pas interférer
	MeshMissile->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Lier la collision
	CapsuleCollision->OnComponentBeginOverlap.AddDynamic(this, &AMissile::OnOverlap);
}

void AMissile::BeginPlay()
{
	Super::BeginPlay();

	if (GetWorld() && GetWorld()->GetFirstPlayerController())
	{
		ReferenceVaisseau = GetWorld()->GetFirstPlayerController()->GetPawn();
	}
}

void AMissile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!Direction.IsZero())
	{
		FVector NewLocation = GetActorLocation() + Direction * Vitesse * DeltaTime;
		SetActorLocation(NewLocation);
	}

	// Supprimer si hors zone
	if (ReferenceVaisseau)
	{
		FVector VaisseauPos = ReferenceVaisseau->GetActorLocation();
		FVector Pos = GetActorLocation();

		float OffsetXTop = 1130.0f;
		float OffsetXBottom = -1130.0f;
		float OffsetYLeft = -2460.0f;
		float OffsetYRight = 2460.0f;

		if (Pos.X < VaisseauPos.X + OffsetXBottom ||
			Pos.X > VaisseauPos.X + OffsetXTop ||
			Pos.Y < VaisseauPos.Y + OffsetYLeft ||
			Pos.Y > VaisseauPos.Y + OffsetYRight)
		{
			Destroy();
		}
	}
}

void AMissile::InitDirection(const FVector& NewDirection)
{
	Direction = NewDirection.GetSafeNormal();
}

void AMissile::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	AAsteroide* Asteroide = Cast<AAsteroide>(OtherActor);
	if (Asteroide)
	{
		Asteroide->PrendreDegat();
		Destroy();
	}
}

#include "Missile.h"
#include "Asteroide.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

AMissile::AMissile()
{
	PrimaryActorTick.bCanEverTick = true;

	CapsuleCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleCollision"));
	CapsuleCollision->InitCapsuleSize(15.f, 50.f);
	RootComponent = CapsuleCollision;

	CapsuleCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CapsuleCollision->SetCollisionObjectType(ECC_WorldDynamic);
	CapsuleCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	CapsuleCollision->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);

	MeshMissile = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshMissile"));
	MeshMissile->SetupAttachment(CapsuleCollision);
	MeshMissile->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	CapsuleCollision->OnComponentBeginOverlap.AddDynamic(this, &AMissile::OnOverlap);
}

void AMissile::BeginPlay()
{
	Super::BeginPlay();

	if (GetWorld() && GetWorld()->GetFirstPlayerController())
		ReferenceVaisseau = GetWorld()->GetFirstPlayerController()->GetPawn();
}

void AMissile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!Direction.IsZero())
	{
		FVector NewLocation = GetActorLocation() + Direction * Vitesse * DeltaTime;
		SetActorLocation(NewLocation);
	}

	if (ReferenceVaisseau)
	{
		FVector VaisseauPos = ReferenceVaisseau->GetActorLocation();
		FVector Pos = GetActorLocation();

		if (Pos.X < VaisseauPos.X - 1130.f || Pos.X > VaisseauPos.X + 1130.f ||
			Pos.Y < VaisseauPos.Y - 2460.f || Pos.Y > VaisseauPos.Y + 2460.f)
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
		Asteroide->RecevoirDegat();
		Destroy();
	}
}
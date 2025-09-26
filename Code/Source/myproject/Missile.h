#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Missile.generated.h"

class UCapsuleComponent;

UCLASS()
class MYPROJECT_API AMissile : public AActor
{
    GENERATED_BODY()

public:
    AMissile();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    void InitDirection(const FVector& NewDirection);

private:
    UPROPERTY(VisibleAnywhere)
    UCapsuleComponent* CapsuleCollision;

    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* MeshMissile;

    UPROPERTY(EditAnywhere, Category = "Missile")
    float Vitesse = 2000.0f;

    FVector Direction;

    APawn* ReferenceVaisseau = nullptr;

    UFUNCTION()
    void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);

    // Particle system à jouer à l'impact
    UPROPERTY(EditAnywhere, Category = "FX")
    UParticleSystem* ImpactFX;
};

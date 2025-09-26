#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Asteroide.generated.h"

UCLASS()
class MYPROJECT_API AAsteroide : public AActor
{
	GENERATED_BODY()

public:
	AAsteroide();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	void RecevoirDegat();

private:
	// --- Mouvement ---
	UPROPERTY(EditAnywhere, Category = "Mouvement")
	float Vitesse = 200.0f;

	// --- Cible ---
	UPROPERTY(EditAnywhere, Category = "Cible")
	TSubclassOf<APawn> CibleClass;

	APawn* CiblePawn = nullptr;

	// --- Mesh ---
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* Mesh;

	// --- Vie ---
	UPROPERTY(EditAnywhere, Category = "Asteroide")
	int Vies = 1;
	int InialLife;

	// --- Son ---
	UPROPERTY(EditAnywhere, Category = "Effets")
	USoundBase* ExplosionSound;

	// --- Effets ---
	UPROPERTY(EditAnywhere, Category = "Effets")
	UParticleSystem* ExplosionEffect;

	// --- Rotation ---
	FVector RotationSpeed; // vitesse de rotation en deg/s

	void DestroyAsteroide();
};

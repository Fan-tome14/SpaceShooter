#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Missile.generated.h"

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

	// Direction du missile
	void InitDirection(const FVector& NewDirection);

private:
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* MeshMissile;

	// Vitesse du missile
	UPROPERTY(EditAnywhere, Category = "Missile")
	float Vitesse = 2000.0f;

	// Direction de déplacement
	FVector Direction;

	// Référence au vaisseau pour calculer la zone
	APawn* ReferenceVaisseau = nullptr;
};

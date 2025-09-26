#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AsteroideSpawner.generated.h"

UCLASS()
class MYPROJECT_API AAsteroideSpawner : public AActor
{
	GENERATED_BODY()

public:
	AAsteroideSpawner();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, Category = "Spawn")
	float IntervalleSpawn = 1.0f;

	float TempsEcoule = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Spawn")
	TSubclassOf<class AAsteroide> ClasseAsteroide;
};

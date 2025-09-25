#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "spaceshooterGameMode.generated.h"

/**
 *
 */
UCLASS()
class MYPROJECT_API ASpaceshooterGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	// Tableau des points attribu�s selon la vie de l'ast�ro�de
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Score")
	int Points=10;

	// Score actuel du joueur
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Score")
	int PlayerScore;

	// Constructeur
	ASpaceshooterGameMode();

	// Fonction pour ajouter du score
	UFUNCTION(BlueprintCallable, Category = "Score")
	void addScore();

	// Getter rapide
	int GetScore() const { return PlayerScore; }
};

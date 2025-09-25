// Vaisseau.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Vaisseau.generated.h"

class UCapsuleComponent;
class UStaticMeshComponent;
class AMissile;

UCLASS()
class MYPROJECT_API AVaisseau : public APawn
{
    GENERATED_BODY()

public:
    AVaisseau();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    void PerdreVie();

protected:
    // Root fixe
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Composants")
    USceneComponent* Root;

    // Capsule de collision (modifiable dans Blueprint)
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Composants")
    UCapsuleComponent* CapsuleCollision;

    // Mesh du vaisseau (modifiable dans Blueprint)
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Composants")
    UStaticMeshComponent* MeshVaisseau;

    // Vitesse de déplacement
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mouvement")
    float Vitesse;

    // Entrées actuelles pour X/Y
    FVector2D InputActuel;

    void DeplacerAvantArriere(float Valeur);
    void DeplacerGaucheDroite(float Valeur);

    FVector ObtenirDirectionVersSouris();

    // Classe de missile
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tir")
    TSubclassOf<AMissile> MissileClass;

    void Tirer(float Valeur);

    float DernierTir;

    // Vies du vaisseau
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vie")
    int Vie;
};

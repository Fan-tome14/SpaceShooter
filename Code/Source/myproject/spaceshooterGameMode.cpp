#include "spaceshooterGameMode.h"


//Score du joueur

ASpaceshooterGameMode::ASpaceshooterGameMode(){
	PlayerScore = 0;

}

void ASpaceshooterGameMode::addScore()
{
	PlayerScore += Points;
}

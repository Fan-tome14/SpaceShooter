#include "spaceshooterGameMode.h"

ASpaceshooterGameMode::ASpaceshooterGameMode(){
	PlayerScore = 0;

}

void ASpaceshooterGameMode::addScore()
{
	PlayerScore += Points;
}

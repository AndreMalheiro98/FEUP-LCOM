#include <lcom/lcf.h>
#include "highscore.h"
#include "keyboard.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HIGHSCORE_PATH "../shared/proj/res/highscores.txt"

int highScore(int score){
    FILE* highscoresFile = NULL;
	highscoresFile = fopen(HIGHSCORE_PATH, "r");
	if (highscoresFile == NULL) {
		highscoresFile = fopen(HIGHSCORE_PATH, "w");
		fclose(highscoresFile);
		highscoresFile = fopen(HIGHSCORE_PATH, "r");
	}
    int i = 0;
	int index = -1;

	/*char buffer[BUFFER_SIZE];
	while (fgets(buffer, sizeof(buffer), highscoreFile) != NULL) {
		char *dump = strtok(buffer, " ");
		char *lineScore = strtok(NULL, " ");
		int lineScoreInt = atoi(lineScore);
		if (score > lineScoreInt) {
			index = i;
			break;
		}
		i++;
	}
    */
	fclose(scoresFile);

	if (i < 10)
		index = i;

	return index;
}

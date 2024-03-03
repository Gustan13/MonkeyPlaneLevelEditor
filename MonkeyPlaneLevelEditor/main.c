#include <stdio.h>
#include <stdlib.h>

#define LINESIZE 1024
#define MAX_XY_VALUES 1000

/*
* FILE FORMAT:
*	FIRST 4 BYTES = AMOUNT OF Xs AND Ys TOGETHER
*	SECOND 4 BYTES = TYPE OF ENTITY TO SPAWN
*	ALL OF THE OTHER BYTES ARE X AND Y OF THE ENTITIES OF THAT TYPE
*	\n TO END THE LINE
*
*	THIS FORMAT REPEATS ITSELF IN LINES FOR EACH ENTITY TYPE
*/

char* types[5] = {
		"Penguin",
		"Dog",
		"Duck",
		"Elephant",
		"Koala"
};

int load_level_file(const char* lvl_name)
{
	int amount_of_entities = 0;

	FILE* lvl;
	fopen_s(&lvl, lvl_name, "r");
	if (!lvl)
		return -1;

	rewind(lvl);

	while (!feof(lvl)) {
		float position_buffer[2];
		int type = 0;
		char c = ' ';

		if (fread_s(&amount_of_entities, sizeof(int), sizeof(int), 1, lvl) < 1)
			break;
		fread_s(&type, sizeof(int), sizeof(int), 1, lvl);

		printf("%s:\n", types[type]);

		for (int i = 0; i < amount_of_entities; i += 2)
		{
			fread_s(&position_buffer, sizeof(float) * 2, sizeof(float), 2, lvl);
			printf("\tX: %d Y: %d\n", (int)position_buffer[0], (int)position_buffer[1]);
		}

		fread_s(&c, sizeof(char), sizeof(char), 1, lvl);
	}

	fclose(lvl);

	return 0;
}

void save(int indexes[5], float* xy_values[5]) {

	FILE* lvl = NULL;

	fopen_s(&lvl, "level_1.isoo", "w");
	if (!lvl) {
		printf("Unable to open the file\n");
		return;
	}

	for (int i = 0; i < 5; i++)
	{
		printf("%s:\n", types[i]);
		fwrite(&indexes[i], 4, 1, lvl);
		fwrite(&i, 4, 1, lvl);

		for (int j = 0; j < indexes[i]; j += 2)
		{
			printf("\tX: %d Y: %d\n", (int)(xy_values[i][j]), (int)(xy_values[i][j + 1]));
			SDL_Delay(100);
		}
		fwrite(xy_values[i], sizeof(float), indexes[i], lvl);
		fputc(10, lvl);
	}

	fclose(lvl);

	system("pause");
}

int write_to_file(const char* lvl_name)
{
	int type_selected = 1;

	int indexes[5];
	float* xy_values[5];

	for (int i = 0; i < 5; i++) {
		xy_values[i] = malloc(sizeof(float) * MAX_XY_VALUES);
		indexes[i] = 0;
	}

	while (type_selected != 0) {
		int i = 0;
		float* ptrX;
		float* ptrY;

		system("cls");

		printf("MONKEY PLANE LEVEL MAKER\n");
		printf("What type of entity will you create?\n1.PENGUIN\n2.DOG\n3.DUCK\n4.ELEPHANT\n5.KOALA\n");
		printf("Type 0 to leave or Type 6 to save.\n");
		scanf_s("%d", &type_selected);
		if (type_selected > 6 || type_selected < 0) {
			printf("That is not a viable option\n");
			continue;
		}

		system("cls");

		switch (type_selected) {
		case 0:
			continue;
		case 6:
			save(indexes, xy_values);
			break;
		default:
			i = indexes[type_selected - 1];
			ptrX = &(xy_values[type_selected - 1][i]);
			i++;
			ptrY = &(xy_values[type_selected - 1][i]);
			i++;
			indexes[type_selected - 1] = i;


			printf("Type the X and Y of the entity:\n");
			scanf_s("%f %f", ptrX, ptrY);

			printf("\nEntity created\n");
			break;
		}
	}

	return 0;
}
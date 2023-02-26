#include <iostream>
#include "pixeldisplay.h"
#include "stb_image.h"
#include <ctime>

const int tiles[4][3] = { {0, 128, 0}, {0, 255, 0}, {255, 255, 128}, {0, 0, 255} }; \
const int states = 4;
const int xImage = 100;
const int yImage = 100;

struct unit {
	//int superposition = 4;
	int state[4] = { 1, 1, 1, 1 };
	int final = -1;
	int x = 0;
	int y = 0;
};

int superposition(unit* current) {
	int ret = 0;
	for (int i = 0; i < states; i++) {
		ret += current->state[i];
	}
	return ret;
}
void collapse(unit* current) {
	int certainty = superposition(current);
	//if (rand() % 100 == 0 or certainty == 0) {
	//	current->final = rand() % 4;
	//	return;
	//}
	int result = rand() % certainty;
	int count = 0;
	for (int i = 0; i < states; i++) {
		if (current->state[i] and count == result) {
			current->final = i;
			return;
		}
		if (current->state[i]) {
			count++;
		}
	}
}

void propagate(unit* map[yImage][xImage], unit* current) {
	int state = current->final;
	int x = current->x;
	int y = current->y;

	int i = -1;
	int j = 0;
	if (!(y + i < 0 or y + i >= yImage or x + j < 0 or x + j >= xImage)) {
		if (state == 0) {
			map[y + i][x + j]->state[2] = 0;
			map[y + i][x + j]->state[3] = 0;
		}
		if (state == 1) {
			map[y + i][x + j]->state[3] = 0;
		}
		if (state == 2) {
			map[y + i][x + j]->state[0] = 0;
		}
		if (state == 3) {
			map[y + i][x + j]->state[0] = 0;
			map[y + i][x + j]->state[1] = 0;
		}
	}
	i = 1;
	if (!(y + i < 0 or y + i >= yImage or x + j < 0 or x + j >= xImage)) {
		if (state == 0) {
			map[y + i][x + j]->state[2] = 0;
			map[y + i][x + j]->state[3] = 0;
		}
		if (state == 1) {
			map[y + i][x + j]->state[3] = 0;
		}
		if (state == 2) {
			map[y + i][x + j]->state[0] = 0;
		}
		if (state == 3) {
			map[y + i][x + j]->state[0] = 0;
			map[y + i][x + j]->state[1] = 0;
		}
	}
	i = 0;
	j = -1;
	if (!(y + i < 0 or y + i >= yImage or x + j < 0 or x + j >= xImage)) {
		if (state == 0) {
			map[y + i][x + j]->state[2] = 0;
			map[y + i][x + j]->state[3] = 0;
		}
		if (state == 1) {
			map[y + i][x + j]->state[3] = 0;
		}
		if (state == 2) {
			map[y + i][x + j]->state[0] = 0;
		}
		if (state == 3) {
			map[y + i][x + j]->state[0] = 0;
			map[y + i][x + j]->state[1] = 0;
		}
	}
	j = 1;
	if (!(y + i < 0 or y + i >= yImage or x + j < 0 or x + j >= xImage)) {
		if (state == 0) {
			map[y + i][x + j]->state[2] = 0;
			map[y + i][x + j]->state[3] = 0;
		}
		if (state == 1) {
			map[y + i][x + j]->state[3] = 0;
		}
		if (state == 2) {
			map[y + i][x + j]->state[0] = 0;
		}
		if (state == 3) {
			map[y + i][x + j]->state[0] = 0;
			map[y + i][x + j]->state[1] = 0;
		}
	}
	/*
	for (int i = -1; i < 2; i++) {
		for (int j = -1; j < 2; j++) {
			if (y + i < 0 or y + i >= yImage or x + j < 0 or x + j >= xImage) {
				continue;
			}
			if (state == 0) {
				map[y + i][x + j]->state[2] = 0;
				map[y + i][x + j]->state[3] = 0;
			}
			if (state == 1) {
				map[y + i][x + j]->state[3] = 0;
			}
			if (state == 2) {
				map[y + i][x + j]->state[0] = 0;
			}
			if (state == 3) {
				map[y + i][x + j]->state[0] = 0;
				map[y + i][x + j]->state[1] = 0;
			}
		}
	}
	*/
}

void createmap(unit* map[yImage][xImage]) {

	for (int i = 0; i < 100; i++) {
		for (int j = 0; j < 100; j++) {
			map[i][j] = new unit;
			map[i][j]->y = i;
			map[i][j]->x = j;
		}
	}

	unit* mostcertain = map[0][0];
	int certainty = 4;
	int tempcertainty;
	int count = 0;
	while (count < xImage*yImage) {

		mostcertain = map[0][0];
		certainty = 4;

		for (int i = 0; i < 100; i++) {
			for (int j = 0; j < 100; j++) {
				tempcertainty = superposition(map[i][j]);
				if (mostcertain->final != -1 or (tempcertainty < certainty and map[i][j]->final == -1)) {
					mostcertain = map[i][j];
					certainty = tempcertainty;
				};
			}
		}
		collapse(mostcertain);
		propagate(map, mostcertain);
		count++;
		std::cout << "count: " << count << std::endl;
	}


}

unsigned char* createpixelbuffer(unit* map[yImage][xImage]) {
	unsigned char* data = new unsigned char[xImage * yImage * 3];

	if (!data) {
		std::cout << "Failed to initialize memory" << std::endl;
	}

	int x = 0;
	int y = 0;
	while (y < yImage) {
		/*data[xImage * y * 3 + x * 3 + 0] = (unsigned char)(((float)x / (float)xImage) * 255);
		data[xImage * y * 3 + x * 3 + 1] = (unsigned char)(((float)y / (float)yImage) * 255);
		data[xImage * y * 3 + x * 3 + 2] = (unsigned char)0;*/
		if (x == 0 or x == xImage - 1 or y == 0 or y == yImage - 1 or map[y][x]->final == -1) {
			data[xImage * y * 3 + x * 3 + 0] = (unsigned char)255;
			data[xImage * y * 3 + x * 3 + 1] = (unsigned char)0;
			data[xImage * y * 3 + x * 3 + 2] = (unsigned char)0;
		} else {
			int temp = rand() % 4;
			data[xImage * y * 3 + x * 3 + 0] = tiles[temp][0];
			data[xImage * y * 3 + x * 3 + 1] = tiles[temp][1];
			data[xImage * y * 3 + x * 3 + 2] = tiles[temp][2];
		};



		x++;
		if (x == xImage) {
			x = 0;
			y++;
		}
	};
	return data;
};
int main() {
	std::cout << "TerrainGen has started" << std::endl;
	srand(time(0));

	unit* map[yImage][xImage];
	createmap(map);

	unsigned char* data = createpixelbuffer(map);
	init(xImage, yImage, data);
	int status = 0;
	while (status == 0) {
		status = display(data);
	}
	return 0;
}
#include <iostream>
#include "pixeldisplay.h"
#include "stb_image.h"
#include <ctime>
#include "stb_image.h"
#include <vector>

//const int tiles[4][3] = { {0, 128, 0}, {0, 255, 0}, {255, 255, 128}, {0, 0, 255} };
const int states = 4;
const int xImage = 100;
const int yImage = 100;
const int samplex = 16;
const int sampley = 16;
int rules[states][states];
int sfreq[states] = { 0, 0, 0, 0 };
int freq[states] = { 0, 0, 0, 0 };
//const int n = 4;

//std::vector<tile*> vtiles;

struct rule {
	int origin;
	int target;
	float percentage;
};

struct unit {
	//int superposition = 4;
	int state[states] = { 0, 0, 0, 0 };
	int final = -1;
	int x = 0;
	int y = 0;
};

struct tile {
	int id;
	int r = 0;
	int g = 0;
	int b = 0;
};

std::vector<tile*> vtiles;

int superposition(unit* current) {
	int ret = 0;
	for (int i = 0; i < states; i++) {
		ret += current->state[i];
	}
	return ret;
}
void collapse(unit* current) {
	int total = superposition(current);
	//if (rand() % 100 == 0 or certainty == 0) {
	//	current->final = rand() % 4;
	//	return;
	//}
	int result;
	if (total == 0) {
		result = rand() % states;
		current->final = result;
		return;
	}
	std::cout << std::endl << total << std::endl;
	result = rand() % total;
	int count = 0;
	for (int i = 0; i < states; i++) {
		count += current->state[i];
		if (count > result) {
			current->final = i;
			return;
		}
	}
}

void propagate(unit* map[yImage][xImage], unit* current) {
	int state = current->final;
	int x = current->x;
	int y = current->y;

	int i = -1;
	int j = 0;
	/*if (!(y + i < 0 or y + i >= yImage or x + j < 0 or x + j >= xImage)) {
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
	}*/
	
	for (int i = -1; i < 2; i++) {
		for (int j = -1; j < 2; j++) {
			if (y + i < 0 or y + i >= yImage or x + j < 0 or x + j >= xImage) {
				continue;
			}
			for (int a = 0; a < states; a++)
				map[y + i][x + j]->state[a] += rules[state][a];
		}
	}
	
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
	int certainty = 0;
	int tempcertainty;
	int count = 0;
	while (count < xImage*yImage) {

		mostcertain = map[0][0];
		certainty = 4;

		for (int i = 0; i < 100; i++) {
			for (int j = 0; j < 100; j++) {
				tempcertainty = superposition(map[i][j]);
				if (mostcertain->final != -1 or (tempcertainty > certainty and map[i][j]->final == -1)) {
					mostcertain = map[i][j];
					certainty = tempcertainty;
				};
			}
		}
		collapse(mostcertain);
		propagate(map, mostcertain);
		freq[mostcertain->final]++;
		count++;
		std::cout << "count: " << count << std::endl;
	}


}

void readimage(int map[sampley][samplex]) {
	int width, height, nrChannels;
	unsigned char* data = stbi_load("sample.png", &width, &height, &nrChannels, 0);
	int y = 0;
	int x = 0;
	
	int r = 0;
	int g = 0;
	int b = 0;

	int id = 0;
	//int map[sampley][samplex];
	bool unique;
	while (y < height) {
		r = data[width * y * 4 + x * 4];
		g = data[width * y * 4 + x * 4 + 1];
		b = data[width * y * 4 + x * 4 + 2];
		//std::cout << y << " " << x << " " << (int)r << " " << (int)g << " " << (int)b << std::endl;
		unique = true;
		for (auto i = vtiles.begin(); i != vtiles.end(); i++) {
			if ((*i)->r == r and (*i)->g == g and (*i)->b == b) {
				map[y][x] = (*i)->id;
				unique = false;
				break;
			}	
		}
		if (unique) {

			vtiles.push_back(new tile);
			(*vtiles.rbegin())->r = r;
			(*vtiles.rbegin())->g = g;
			(*vtiles.rbegin())->b = b;
			(*vtiles.rbegin())->id = id;
			map[y][x] = id;
			id++;
			//std::cout << "new tile added" << std::endl;
		}

		if (x == width-1) {
			x = 0;
			y++;
		}
		else {
			x++;
		}
	}
	//std::cout << std::endl << map[1][0] << std::endl;
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height;j++) {
			std::cout << map[i][j];
			sfreq[map[i][j]]++;
		}
		std::cout << std::endl;
	}
}

void createrules(int map[sampley][samplex]) {
	//int n = vtiles.size();
	//int rules[states][states];
	for (int i = 0; i < states; i++) {
		for (int j = 0; j < states; j++) {
			rules[i][j] = 0;
		}
	}

	int tid;
	int aid;
	for (int y = 0; y < sampley; y++) {
		for (int x = 0; x < samplex; x++) {
			for (int i = -1; i < 2; i++) {
				for (int j = -1; j < 2; j++) {
					tid = map[y][x];
					if (y + i < 0 or y + i == sampley or x + j < 0 or x + j == samplex) {
						continue;
					}
					aid = map[y + i][x + j];
					rules[tid][aid]++;
				}
			}
		}
	}
	for (int i = 0; i < states; i++) {
		for (int j = 0; j < states; j++) {
			std::cout << rules[i][j] << " ";
		}
		std::cout << std::endl;
	}
};

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
			data[xImage * y * 3 + x * 3 + 0] = vtiles[map[y][x]->final]->r;
			data[xImage * y * 3 + x * 3 + 1] = vtiles[map[y][x]->final]->g;
			data[xImage * y * 3 + x * 3 + 2] = vtiles[map[y][x]->final]->b;
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

	int smap[sampley][samplex];
	readimage(smap);
	createrules(smap);
	createmap(map); 
	for (int i = 0; i < states; i++) {
		std::cout << sfreq[i] << " ";
	};
	std::cout << std::endl;
	for (int i = 0; i < states; i++) {
		std::cout << freq[i] << " ";
	};
	std::cout << std::endl;

	unsigned char* data = createpixelbuffer(map);
	init(xImage, yImage, data);
	int status = 0;
	while (status == 0) {
		status = display(data);
	}
	return 0;
	//std::cout << std::endl << "breakpoint" << std::endl;
}
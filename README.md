# ProceduralGeneration
Procedurally generate small pixel maps from a set of rules.

The rules can either be hardcoded, or learned from a small sample image.

Uses [stb_image](https://github.com/nothings/stb/blob/master/stb_image.h) to read sample image and save output

Uses OpenGL to display results

## Algorithm
Inspired by wave function collapse algorithm from these videos: [](https://www.youtube.com/watch?v=20KHNA9jTsE) [](https://www.youtube.com/watch?v=2SuvO4Gi7uY) [](https://www.youtube.com/watch?v=TO0Tx3w5abQ)

Every tile (I used 10x10 pixel squares) is one of 4 colors (representing terrain types, it can be any number), with restrictions on adjacency rules.

Forest can only be next to grass, grass can be next to forest or sand, sand can be next to grass or water, water can only be next to sand.

Because of this, forest cannot be 2 away from sand, only 3. This means that initially, every tile can be any of the 4 tiles, thus being in "superposition". After placing an initial tile randomly, the possiblities are reduced for adjacent tiles. This then reduces possibilities for further tiles, propagating the rules.

Every time a pixel is colored, the rules are propagated, and the tile with the least number of possibilities is assigned a color from the still available options, thus its "superposition" is "collapsed".


The probability of which of the available tiles it chooses can be either equal for each option, or learned from the sample image.

Throughout different versions, I experimented with sligth alterations to these basic rules, for example by randomly allowing usually forbidden adjacencies on a few tiles.

## Examples
Sample image

![](https://github.com/martinblackd23d/ProceduralGeneration/blob/d1561aae69aec9df0906173f247bad9f3474cb66/sample.png?raw=true)

Output

![](https://github.com/martinblackd23d/ProceduralGeneration/blob/d1561aae69aec9df0906173f247bad9f3474cb66/output.PNG?raw=true)

While it's not as good as I want it to be, the forest tiles and water tiles occupy very distinct areas of the map

Just for comparison, here is a completely randomized image

![](https://github.com/martinblackd23d/ProceduralGeneration/blob/d1561aae69aec9df0906173f247bad9f3474cb66/random.PNG?raw=true)

The difference is obvious, showing that the algorithm does work to some extent.

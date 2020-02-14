
#pragma once

#include "fly/fly.h";

class Tile{

  float verticesTile[20] = {
    //x,   y, z, s, t
    -64,   0, 0, 0, 0,
      0,  32, 0, 0, 1,
     64,   0, 0, 1, 1,
      0, -32, 0, 1, 0
  };
  float verticesTileL[20] = {

    -64,   0, 0, 0, 1,
      0,  32, 0, 1, 1,
      0, 672, 0, 1, 0,
    -64, 640, 0, 0, 0
  };
  float verticesTileR[20] = {

     0,  32, 0, 0, 1,
    64,   0, 0, 1, 1,
    64, 640, 0, 0, 0,
     0, 672, 0, 1, 0
  };
  float verticesBorderL[20] = {

     -1, -32, 0, 1, 0,
    -64,   0, 0, 1, 1,
    -64,  -1, 0, 0, 1,
     -1, -33, 0, 0, 0
  };
  float verticesBorderR[20] = {

     1, -32, 0, 1, 0,
    64,   0, 0, 1, 1,
    64,  -1, 0, 0, 1,
     1, -33, 0, 0, 0
  };
  unsigned int indices[6] = {0, 1, 2, 0, 3, 2};

	fly::Thing center, left, right, borderL, borderR;
  fly::Texture dirt2;

	public:

		void initialize(){
			
			center.initialize(verticesTile, sizeof(verticesTile), indices, sizeof(indices));
      left.initialize(verticesTileL, sizeof(verticesTileL), indices, sizeof(indices));
      right.initialize(verticesTileR, sizeof(verticesTileR), indices, sizeof(indices));
      borderL.initialize(verticesBorderL, sizeof(verticesBorderL), indices, sizeof(indices));
      borderR.initialize(verticesBorderR, sizeof(verticesBorderR), indices, sizeof(indices));

      center.setTexture("textures/grass.png");
      left.setTexture("textures/dirt.png");
      dirt2.load("textures/dirt2.jpg");
		}

    void instance(glm::vec3 positions[], glm::vec3 borderPositions[], unsigned int count){

      center.instance(&positions[0], count);
      left.instance(&positions[0], count);
      right.instance(&positions[0], count);
      borderL.instance(&borderPositions[0], count);
      borderR.instance(&borderPositions[0], count);
    }

    void display(){

      center.display();
      left.display();
      dirt2.bind();
      right.display();
      borderL.display();
      borderR.display();
    }
};

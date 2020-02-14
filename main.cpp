
#include "fly/fly.h"
#include "utility.h"
#include "Tile.h"

int mapSize = 50, tileHeight = 64, current, *heights;
bool editing, p = true, p2 = true;
glm::vec2 clickStart, cursorPosition, translation;
glm::vec3* tilePositions, * tileBorderPositions;

void calculateCursorPosition(){

  cursorPosition = toIsometricUnit(fly::mouseX - translation.x, fly::mouseY - translation.y - tileHeight / 2);
  cursorPosition /= tileHeight;
  cursorPosition.x = round(cursorPosition.x);
  cursorPosition.y = round(cursorPosition.y);
  current = cursorPosition.x * mapSize + cursorPosition.y;
  cursorPosition *= tileHeight;
  cursorPosition = toReal(cursorPosition.x, cursorPosition.y);
}

void viewport(){

  if(fly::mouseX < 10) translation.x += 20;
  else if(fly::mouseX > fly::width - 10) translation.x -= 20;
  if(fly::mouseY < 10) translation.y += 20;
  else if(fly::mouseY > fly::height - 10) translation.y -= 20;
  fly::translateCameraTo(translation.x, translation.y);
}

void mousePressed(GLFWwindow* window, int button, int action, int mods){

  if(button == GLFW_MOUSE_BUTTON_LEFT){

    if(action == GLFW_PRESS){

      editing = true;
      std::cout << "Tile: " << tilePositions[current].z << std::endl;
      p = false;
      clickStart = glm::vec2(fly::mouseX, fly::mouseY);
    }
    else{
      p = true;
      editing = false;
    }
  }
  else if(button == GLFW_MOUSE_BUTTON_RIGHT){

    if(action == GLFW_PRESS){

      std::cout << "Bord: " << tileBorderPositions[current].z << std::endl;
      p2 = false;
    }
    else{

      p2 = true;
    }
  }
}

int main(){

  fly::createWindow("scratch");
  fly::initialize(fly::projectionMethod::ORTHO);
  glfwSetMouseButtonCallback(fly::window, mousePressed);

  Tile tile;
  tile.initialize();
 
  float verticesTile[] = {
    //x,   y, z, s, t
    -64,   0, 0, 0, 0,
      0,  32, 0, 0, 1,
     64,   0, 0, 1, 1,
      0, -32, 0, 1, 0
  };
  float verticesBlockL[] = {

                          0,                             0, 0, 1, 0,
                          0,    (tileHeight / 2) * mapSize, 0, 1, 1,
    -(mapSize * tileHeight),                             0, 0, 0, 0,
    -(mapSize * tileHeight), -((tileHeight / 2) * mapSize), 0, 0, 1
  };
  float verticesBlockR[] = {

                        0,                             0, 0, 1, 0,
                        0,    (tileHeight / 2) * mapSize, 0, 1, 1,
   (mapSize * tileHeight),                             0, 0, 0, 0,
   (mapSize * tileHeight), -((tileHeight / 2) * mapSize), 0, 0, 1
  };
  unsigned int indices[] = { 0, 1, 2, 0, 3, 2 };

  fly::Thing cursor, blockL, blockR;

  cursor.initialize(verticesTile, sizeof(verticesTile), indices, sizeof(indices));
  blockL.initialize(verticesBlockL, sizeof(verticesBlockL), indices, sizeof(indices));
  blockR.initialize(verticesBlockR, sizeof(verticesBlockR), indices, sizeof(indices));

  tilePositions = new glm::vec3[mapSize * mapSize];
  tileBorderPositions = new glm::vec3[mapSize * mapSize];
  heights = new int[mapSize * mapSize];

  for(int i = 0; i < mapSize; i++){

    for(int j = 0; j < mapSize; j++){

      tilePositions[i * mapSize + j] = glm::vec3(toReal(i, j) *= tileHeight, //xy
                                                 fly::map(i * mapSize + j, 0, mapSize * mapSize, 0, 1)); //z
      tileBorderPositions[i * mapSize + j] = glm::vec3(toReal(i, j) *= tileHeight, //xy 
                                                       fly::map(i * mapSize + j - 50, 0, mapSize * mapSize, 0, 1)); //z
    }
  }

  tile.instance(tilePositions, tileBorderPositions, mapSize * mapSize);

  for(; fly::running(); fly::update()){

    fly::useTexture(true);

    tile.display();

    if(!editing) calculateCursorPosition();
    else{

      int distance = (fly::mouseY - clickStart.y);
      heights[current] = roundToMultiple(tileHeight / 2, distance);
      tilePositions[current].y += heights[current];
      tileBorderPositions[current].y += heights[current];
      clickStart = glm::vec2(fly::mouseX, clickStart.y + heights[current]);

      tile.instance(tilePositions, tileBorderPositions, mapSize * mapSize);
    }

    fly::useTexture(false);
    fly::fill(0, 0, 0, 255);

    fly::pushMatrix();
    fly::translate(cursorPosition.x, cursorPosition.y, 1);
    cursor.display();
    fly::popMatrix();

    fly::pushMatrix();
    fly::translate(0, mapSize * tileHeight + tileHeight * 2, 1);
    blockL.display();
    blockR.display();
    fly::popMatrix();

    viewport();
  }

  fly::terminate();
  return 0;
}

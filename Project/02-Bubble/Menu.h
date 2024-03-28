#include "Texture.h"
#include "ShaderProgram.h"
#include "TileMap.h"
#include "SoundManager.h"

class Menu {
    public:
    

        void init();
        void render();

        void initShaders();
   


    private:

        ShaderProgram texProgram;
        TileMap* map;
        glm::mat4 projection;
        SoundManager sound;

};

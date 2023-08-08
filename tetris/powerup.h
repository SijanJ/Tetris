#include <SDL2/SDL.h>
#include<SDL2/SDL_image.h>



void Tetris::powerup(int player_no, int powerup_no)
{
if(player_no == 1)
{
//powerup image rendering
setPosRect(destR, BlockW*powerup_x, BlockH*powerup_y, BlockH, BlockW);
SDL_RenderCopy(render, powerup_img, NULL, &destR);


if(currentTime - powerupStartTime > 5000) // powerup shown for 5 seconds
   isPowerupActive=false;

   for(int i=0; i<8; i++){

   if(items[i].x < powerup_x + 1 && // check for collisions of powerup and block
      items[i].x + 1 > powerup_x &&
      items[i].y < powerup_y +1 &&
      items[i].y + 1 > powerup_y)
      {

        nextT =2;
      nextTetrimino(true); //for different shape in nextTetrimino function
     isPowerupActive=false;
        //powerUp to be added soon
 }
  }
}

else // for player 2
{

setPosRect(destR, BlockW*powerup_x2, BlockH*powerup_y2, BlockH, BlockW);
SDL_RenderCopy(render, powerup_img, NULL, &destR);

if(currentTime2 - powerupStartTime2> 5000)
   isPowerupActive2=false;

   for(int i=0; i<8; i++){

   if(items2[i].x < powerup_x2+ 1 &&
      items2[i].x + 1 > powerup_x2 &&
      items2[i].y < powerup_y2 +1 &&
      items2[i].y + 1 > powerup_y2)
      {

      nextT =1;
      nextTetrimino(true);
isPowerupActive2=false;
      }

   }

}


}


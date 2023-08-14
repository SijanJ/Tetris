#include <SDL2/SDL.h>
#include<SDL2/SDL_image.h>



void Tetris::powerup(int player_no, int powerup_no)
{
    if(total_player==1)
        isPowerupActive[1]= false;
if(player_no == 1)
{
//powerup image rendering
setPosRect(destR, BlockW*powerup_x[1], BlockH*powerup_y[1], BlockH, BlockW);
SDL_RenderCopy(render, powerup_img, NULL, &destR);


if(currentTime[1] - powerupStartTime[1] > 5000) // powerup shown for 5 seconds
   isPowerupActive[1]=false;

   for(int i=0; i<8; i++){

   if(items[1][i].x < powerup_x[1] + 1 && // check for collisions of powerup and block
      items[1][i].x + 1 > powerup_x[1] &&
      items[1][i].y < powerup_y[1] +1 &&
      items[1][i].y + 1 > powerup_y[1])
      {


      nextTetrimino(2, true); //for different shape in nextTetrimino function
     isPowerupActive[1]=false;
        //powerUp to be added soon
 }
  }
}

else // for player 2
{

setPosRect(destR, BlockW*powerup_x[2], BlockH*powerup_y[2], BlockH, BlockW);
SDL_RenderCopy(render, powerup_img, NULL, &destR);

if(currentTime[2] - powerupStartTime[2]> 5000)
   isPowerupActive[2]=false;

   for(int i=0; i<8; i++){

   if(items[2][i].x < powerup_x[2]+ 1 &&
      items[2][i].x + 1 > powerup_x[2] &&
      items[2][i].y < powerup_y[2] +1 &&
      items[2][i].y + 1 > powerup_y[2])
      {

      nextTetrimino(1,true);
isPowerupActive[2]=false;
      }

   }

}


}


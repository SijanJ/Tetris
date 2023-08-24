

void Tetris::powerup(int player_no, int powerup_no)
{
    if(total_player==1)
        isPowerupActive[1]= false;


    if(player_no == 1)
    {
        setPosRect(destR, BlockW*powerup_x[1], BlockH*powerup_y[1], BlockH, BlockW); //powerup image rendering
        renderCopy(render, powerup_img, NULL, &destR);



        if(currentTime[1] - powerupStartTime[1] > 5000) // powerup shown for 5 seconds
            isPowerupActive[1]=false;

        for(int i=0; i<8; i++)
        {
            if(items[1][i].x < powerup_x[1] + 1 && // check for collisions of powerup and block
                    items[1][i].x + 1 > powerup_x[1] &&
                    items[1][i].y < powerup_y[1] +1 &&
                    items[1][i].y + 1 > powerup_y[1])
            {
                m1.playSound(4);
                switch(powerup_no)
                {

                case 1:
                    nextTetrimino(2, true); //for different shape in nextTetrimino function
                    isPowerupActive[1]=false;
                    break;
                case 2:
                    powerupStartTime[1] = currentTime[1];
                    pDelay[2]= tempDelay[2]- 100;
                    powerup_x[1] = -1;
                    break;
                case 3:
                    for(int i=Lines-1; i>Lines-5; i--)
                    {
                        for(int j=5; j<15; j++)
                            field[i][j]=1;
                    }
                    linesCleared[1]=-4;
                    playerScore[1]-=200;



                    isPowerupActive[1]=false;
                    break;

                }
            }
        }
    }

    else // for player 2
    {

        setPosRect(destR, BlockW*powerup_x[2], BlockH*powerup_y[2], BlockH, BlockW);
        renderCopy(render, powerup_img, NULL, &destR);


        if(currentTime[2] - powerupStartTime[2]> 5000)
            isPowerupActive[2]=false;

        for(int i=0; i<8; i++)
        {

            if(items[2][i].x < powerup_x[2]+ 1 &&
                    items[2][i].x + 1 > powerup_x[2] &&
                    items[2][i].y < powerup_y[2] +1 &&
                    items[2][i].y + 1 > powerup_y[2])
            {
                m1.playSound(4);
                switch(powerup_no)
                {

                case 1:
                    nextTetrimino(1, true); //for different shape in nextTetrimino function
                    isPowerupActive[2]=false;
                    break;
                case 2:
                    powerupStartTime[2] = currentTime[2];
                    pDelay[1]= tempDelay[1]- 100;
                    powerup_x[2] = -1;
                    break;
                case 3:

                    for(int i=Lines-1; i>Lines-5; i--)
                    {
                        for(int j=33; j<43; j++)
                            field[i][j]=1;
                    }
                    linesCleared[2]=-4;
                    playerScore[2]-=200;


                    isPowerupActive[2]=false;
                    break;
                };
            }

        }

    }
//std::cout << currentTime[1] << " " << poweruptakenTime[1]<< std::endl;

    if(currentTime[1] - powerupStartTime[1] > 5000)
    {
        pDelay[2] = 0;
        isPowerupActive[1] = false;
    }
    if(currentTime[2] - powerupStartTime[2] > 5000)
    {
        pDelay[1] = 0;
        isPowerupActive[2] = false;
    }

}

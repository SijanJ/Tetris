 #include "tetris.h"
 #include <iostream>
 const int Tetris::figures[7][4]=
 {
     0,1,2,3,
     0,4,5,6,
     2,6,5,4,
     1,2,5,6,
     2,1,5,4,
     1,4,5,6,
     0,1,5,6,
 };

 bool Tetris::init(const char* title)
 {
     if(SDL_Init(SDL_INIT_EVERYTHING)==0)
     {
         SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY,"1");
         window = SDL_CreateWindow("tetris", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, ScreenW, ScreenH, SDL_WINDOW_SHOWN);
         if (window != NULL)
         {
             render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
             if(render!=NULL){
                SDL_SetRenderDrawColor(render,255, 255, 255, 255);
                int imgFlags = IMG_INIT_PNG;
                int initted = IMG_Init(imgFlags);
                if((initted & imgFlags) != imgFlags)
                    std::cout<<"Failed to init required png support\n"<<"IMG_Init()Error: "<<IMG_GetError()<<std::endl;

                SDL_Surface* loadSurf = IMG_Load("images/twobox.png");
                background = SDL_CreateTextureFromSurface(render, loadSurf);
                SDL_FreeSurface(loadSurf);
                loadSurf = IMG_Load("images/blocks.png");
                blocks = SDL_CreateTextureFromSurface(render, loadSurf);
                SDL_FreeSurface(loadSurf);
                nextTetrimino();
             }
         }
         else
            return false;
     }
     else
        return true;

     running = true;
     return true;
 }
    void Tetris::nextTetrimino()
    {
        color = 1 + rand() %7;
        int n = rand() % 7;
        for (int i= 0; i<4; i++)
        {

            if(nextT==2){
                 items2[i].x = 10 + figures[n][i]% 4;
            items2[i].y = int(figures[n][i]/4);
            }
            else{
            items[i].x = figures[n][i]% 4;
            items[i].y = int(figures[n][i]/4);
            }

        }
    }

    void Tetris::handleEvents()
    {
    SDL_Event e;
    while(SDL_PollEvent(&e))
    {
        switch (e.type)
        {
        case SDL_QUIT:
            running = false;
            break;
        case SDL_KEYDOWN:
            switch(e.key.keysym.sym)
            {
            case SDLK_UP:
                rotate2 = true;
                break;
            case SDLK_LEFT:
                dx2 = -1;
                break;
            case SDLK_RIGHT:
                dx2 = 1;
                break;
            case SDLK_DOWN:
                delay2 = 50;
                break;
            case SDLK_w:
                rotate = true;
                break;
            case SDLK_a:
                dx = -1;
                break;
            case SDLK_d:
                dx = 1;
                break;
            case SDLK_s:
                delay = 50;
                break;
            }
        }
    }
    const Uint8* state =SDL_GetKeyboardState(NULL);
    if(state[SDL_SCANCODE_DOWN])
        delay2=50;

    if(state[SDL_SCANCODE_S])
        delay=50;
    }
    void Tetris::setPosRect(SDL_Rect& rect, int x, int y, int w, int h)
    {
        rect={x,y,w,h};
    }

    void Tetris::moveRectPos(SDL_Rect& rect, int x, int y){
        rect.x+=x;
        rect.y+=y;
    }

    bool Tetris::isvalid()
    {

        for(int i=0; i<4; i++)

            if(items[i].x < 0 || items[i].x >=10 || items[i].y >= Lines)
                return false;
            else if (field[items[i].y][items[i].x])
                return false;
        return true;
    }

    bool Tetris::isvalid2()
    {
        for(int i=0; i<4; i++)



        if(items2[i].x < 10 || items2[i].x >=20 || items2[i].y >= Lines)
                return false;
            else if (field[items2[i].y][items2[i].x])
                return false;
        return true;

    }



    void Tetris::gameplay()
    {

    SDL_RenderCopy(render, background, NULL, NULL);
    //backup
    for(int i=0; i<4; i++){
      backup[i]=items[i];
     backup2[i]=items2[i];
    }

     //move
     if(dx){
        for(int i=0; i<4; i++)
     {
         items[i].x += dx;
     }
     if(!isvalid())
        for(int i=0; i<4; i++)
         items[i]=backup[i];
     }
     if(dx2){

     for(int i=0; i<4; i++)
     {

         items2[i].x +=dx2;
     }


    if(!isvalid2())
        for(int i=0; i<4; i++)
        items2[i]=backup2[i];
     }

     //rotate
     if(rotate)
     {
         Point p = items[2]; //center of rotation
         for(int i=0; i<4; i++)
         {
             int x= items[i].y - p.y;
             int y = items[i].x - p.x;
             items[i].x = p.x -x;
             items[i].y = p.y + y;
         }
         if(!isvalid())
            for(int i=0; i<4; i++)
            items[i]=backup[i];
     }
     if(rotate2)
     {
         Point p = items2[2]; //center of rotation
         for(int i=0; i<4; i++)
         {
             int x= items2[i].y - p.y;
             int y = items2[i].x - p.x;
             items2[i].x = p.x -x;
             items2[i].y = p.y + y;
         }
         if(!isvalid2())
            for(int i=0; i<4; i++)
            items2[i]=backup2[i];
     }

 nextT =1;
     //ticks
    if(currentTime - startTime > delay)
    {

        for(int i=0;i<4;i++){
            backup[i]=items[i];


        }
        for (int i=0; i<4 ; i++){
            items[i].y++;


        }
        if(!isvalid())
        {
            for(int i=0; i<4; i++)
                field[backup[i].y][backup[i].x]=color;

                nextT=1;
                nextTetrimino();
        }

        startTime = currentTime;
    }

   if(currentTime2 - startTime2 > delay2)
    {

        for(int i=0;i<4;i++){
            backup2[i]=items2[i];
        }
        for (int i=0; i<4 ; i++){

            items2[i].y++;

        }

        if(!isvalid2())
        {
            for(int i=0; i<4; i++)
                field[backup2[i].y][backup2[i].x]=color;
                nextT=2;
                nextTetrimino();
        }
        startTime2 = currentTime2;
    }

    ////check lines

    int k =Lines - 1;
    for(int i =k;i>0;i--){
        int count=0;
        for(int j=0;j<Cols;j++){
            if(field[i][j])
                count++;
            field[k][j]=field[i][j];
        }
        if(count<Cols)
            k--;
    }



    dx=0;
    rotate = false;
    delay = 1000;


    ////////render
    for(int i=0; i<Lines ; i++)
        for(int j=0; j < Cols; j++)
        if(field[i][j])
    {
        setPosRect(srcR, field[i][j] * BlockW);
        setPosRect(destR, j*BlockW, i*BlockH);
        SDL_RenderCopy(render, blocks, &srcR, &destR );
    }

    for(int i=0; i<4;i++)
    {
        setPosRect(srcR, color *  BlockW);
        setPosRect(destR, items[i].x* BlockW, items[i].y*BlockH);
        SDL_RenderCopy(render,blocks, &srcR, &destR);
    }



    dx2=0;
    rotate2= false;
    delay2 = 1000;
    for(int i=0; i<Lines ; i++)
        for(int j=0; j < Cols; j++)
        if(field[i][j])
    {
        setPosRect(srcR, field[i][j] * BlockW);
        setPosRect(destR, j*BlockW, i*BlockH);
        SDL_RenderCopy(render, blocks, &srcR, &destR );
    }

    for(int j=0; j<4;j++)
    {
        setPosRect(srcR, color *  BlockW);
        setPosRect(destR, items2[j].x* BlockH, items2[j].y*BlockH);
        SDL_RenderCopy(render,blocks, &srcR, &destR);
    }

SDL_RenderPresent(render);


    }

    void Tetris::updateRender()
    {
        /*SDL_RenderCopy(render, background, NULL, NULL);
        for(int i=0;i<Lines;i++)
        {
            for(int j=0;j<Cols;j++){
                if(field[i][j])
                {
                    setPosRect(srcR, field[i][j]* BlockW);
                    setPosRect(destR, j*BlockW,i*BlockH);
                    moveRectPos(destR, BlockW,ScreenH-(Lines+1)*BlockH);
                    SDL_RenderCopy(render, blocks, &srcR,&destR);
                }
            }
        }
        for(int i=0;i<4;i++){
            setPosRect(srcR,color*BlockW);
            setPosRect(destR, items[i].x*BlockW, items[i].y*BlockH);
            setPosRect(destR, items2[i].x*BlockW, items2[i].y*BlockH);
            moveRectPos(destR, BlockW,ScreenH-(Lines+1)*BlockH);
            SDL_RenderCopy(render, blocks, &srcR,&destR);
        }
        SDL_RenderPresent(render);*/

    }

    void Tetris::clean()
    {
        SDL_DestroyTexture(blocks);
        SDL_DestroyTexture(background);
        SDL_DestroyRenderer(render);
        IMG_Quit();
        SDL_QUIT;
    }

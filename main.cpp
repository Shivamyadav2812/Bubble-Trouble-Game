#include <simplecpp> // vishalpuri674
#include "shooter.h"
#include "bubble.h"


/* Simulation Vars */
const double STEP_TIME = 0.017;

/* Game Vars */
const int PLAY_Y_HEIGHT = 450; //area of canvas in which game is played
const int LEFT_MARGIN = 70;
const int TOP_MARGIN = 20;
const int BOTTOM_MARGIN = (PLAY_Y_HEIGHT+TOP_MARGIN);


void move_bullets(vector<Bullet> &bullets)  // function definitin for moving bullets
{
    // move all bullet
    for(unsigned int i=0; i<bullets.size(); i++)
    {
        if(!bullets[i].nextStep(STEP_TIME))  // nextStep becomes false when bullet go out of canvas.
        {
            bullets.erase(bullets.begin()+i); // earising element
        }
    }
}

void move_bubbles(vector<Bubble> &bubbles){
    // move all bubbles
    for (unsigned int i=0; i < bubbles.size(); i++)
    {
        bubbles[i].nextStep(STEP_TIME);
    }
}

vector<Bubble> create_bubbles()  // bubble creation function
{
    // create initial bubbles in the game
    vector<Bubble> bubbles;

    bubbles.push_back(Bubble(WINDOW_X/2.0, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS, -BUBBLE_DEFAULT_VX, BUBBLE_DEFAULT_VY, COLOR(255,100,0)));
    bubbles.push_back(Bubble(WINDOW_X/4.0, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS, BUBBLE_DEFAULT_VX, BUBBLE_DEFAULT_VY, COLOR(0,100,0)));
    bubbles.push_back(Bubble(WINDOW_X + 100, BUBBLE_START_Y/2, BUBBLE_DEFAULT_RADIUS, 0, 0, COLOR(255,255,255))); // not in main screen
    bubbles.push_back(Bubble(WINDOW_X/6.0, BUBBLE_START_Y +50, 2*BUBBLE_DEFAULT_RADIUS, -BUBBLE_DEFAULT_VX, BUBBLE_DEFAULT_VY, COLOR(0,0,255)));
    return bubbles;
}

// collision of bullet and bubble
void collision_bullet_bubble(vector<Bubble> &bubbles, vector<Bullet> &bullets, char &score)
    {
         for(unsigned int i=0; i< bullets.size(); i++)  // loop for bullets
           {
               for(unsigned int j=0; j< bubbles.size(); j++) //loop for bubbles

                 {
                      int  distx= bubbles[j].get_radius() + bullets[i].get_height() /2 ;   //constraints to check collision
                      int  disty= bubbles[j].get_radius() + bullets[i].get_width() /2 ;

                       if(abs(bullets[i].get_center_x() - bubbles[j].get_center_x()) <= distx && abs(bullets[i].get_center_y() - bubbles[j].get_center_y()) <= disty )
                                {
                                    score=score+1;

                                   bullets.erase(bullets.begin()+i);  //destroy bullet
                                   bubbles.erase(bubbles.begin()+j);   // destroy bubble

                                    if(bubbles[j].get_radius()==20) // splitting of bigger bubbles into two small bubbles in opposite direction
                                         {

                                                bubbles.push_back(Bubble(bubbles[j].get_center_x(), bubbles[j].get_center_y(), BUBBLE_DEFAULT_RADIUS, -BUBBLE_DEFAULT_VX, BUBBLE_DEFAULT_VY, COLOR(0,0,255)));
                                                bubbles.push_back(Bubble(bubbles[j].get_center_x(), bubbles[j].get_center_y(), BUBBLE_DEFAULT_RADIUS, BUBBLE_DEFAULT_VX, BUBBLE_DEFAULT_VY, COLOR(0,0,255)));

                                         }

                                }

                }
          }


    }




// function definition to check collision of bubble and shooter
bool collision_bubble_shooter(vector<Bubble> &bubbles, Shooter shooter , char &life )
      {
              for(unsigned int j=0; j< bubbles.size(); j++)
                    {
                            int distx = shooter.get_body_width() /2   +  bubbles[j].get_radius();  // safe distance in x
                            int disty = shooter.get_body_height() /2 + 2*shooter.get_head_radius() +  bubbles[j].get_radius() ;  // safe distance in y

                              if( abs(bubbles[j].get_center_x() -shooter.get_body_center_x()) <=distx  &&  abs(bubbles[j].get_center_y() - shooter.get_body_center_y() )<=disty )
                                         {

                                             bubbles.erase(bubbles.begin()+j);// destroy bubble when it hits the shooter
                                             return true;

                                         }

                    }

        return false;
      }







int main() // main program
{
    initCanvas("Bubble Trouble (Vishal Puri)", WINDOW_X, WINDOW_Y);

    Line b1(0, PLAY_Y_HEIGHT, WINDOW_X, PLAY_Y_HEIGHT);
    b1.setColor(COLOR(0, 0, 255));

    string msg_cmd("Cmd: _");
    Text charPressed(LEFT_MARGIN, BOTTOM_MARGIN, msg_cmd);

    // Intialize the shooter
  Shooter shooter(SHOOTER_START_X, SHOOTER_START_Y, SHOOTER_VX);

    // Initialize the bubbles
    vector<Bubble> bubbles = create_bubbles();

    // Initialize the bullets (empty)
    vector<Bullet> bullets;

    XEvent event;

 // variables for timer
   float countdown=48;
   float countdown2=38;
   float countdown3=28;
   float countdown4=18;
    string cnt("TIMER: _/35");
    Text Timer(40, 10, cnt);

// variable for score
   char score=48;
   string msg_score("Score: _");
   Text gameScore(WINDOW_X - 80, BOTTOM_MARGIN, msg_score);

//variable for lives
   char life=51;
   string msg_life("Life: _/3");
   Text lifeRemain(WINDOW_X - 80, TOP_MARGIN, msg_life);




    // Main game loop
    while (true)
    {
        bool pendingEvent = checkEvent(event);
        if (pendingEvent)
        {
            // Get the key pressed
            char c = charFromEvent(event);
            msg_cmd[msg_cmd.length() - 1] = c;
            charPressed.setMessage(msg_cmd);

            // Update the shooter
            if(c == 'a')
                shooter.move(STEP_TIME, true);
            else if(c == 'd')
                shooter.move(STEP_TIME, false);
            else if(c == 'w')
                bullets.push_back(shooter.shoot());
            else if(c == 'q')
                return 0;
        }


         // function call  to check collision between bunnle and bullet and
         // calculate score
        collision_bullet_bubble(bubbles, bullets, score);

        // display score
        msg_score[msg_score.length() - 1]=score;
        gameScore.setMessage(msg_score);

        // Game win if score is 5/5
        if(score==53)
           {
            string msg_win("|YOU WIN|  score:5/5");
            Text gameWin(WINDOW_X/2.0, WINDOW_Y/2.0, msg_win);



            wait(5);
            return 0;
           }





         //TIMER CALCULATION
           char c = (int)countdown;
           countdown=countdown +0.07;

           char f= (int)countdown2;
           countdown2=countdown2+ 0.07;

           char g = (int)countdown3;
           countdown3=countdown3 +0.07 ;

           char k = (int)countdown4;
           countdown4=countdown4 +0.07 ;





            if(c<58)                  {cnt[cnt.length() -4] = c;  Timer.setMessage(cnt);}

            if(c>=58 && c<68)         {cnt[cnt.length() -5] = 49;  Timer.setMessage(cnt); cnt[cnt.length() -4] = f ;  Timer.setMessage(cnt); }

            if(c>=68 && c<78)         {cnt[cnt.length() -5] = 50;  Timer.setMessage(cnt);  cnt[cnt.length() -4] = g ; Timer.setMessage(cnt);  }

            if(c>=78 && c<88 )        {cnt[cnt.length() -5] = 51; Timer.setMessage(cnt); cnt[cnt.length() -4] = k ; Timer.setMessage(cnt); }



           //GAME END when TIME IS FINISHED
            if((int)countdown== 84)
                 {
                 string msg_time("# TIME FINISHED #");
                 Text msg(WINDOW_X/2.0, WINDOW_Y/2.0, msg_time);

                 string msg_time2("# YOU LOSE #");
                 Text msg2(WINDOW_X/2.0, WINDOW_Y/2.0  + 40, msg_time2);
                  wait(5);
                  return 0;
                }



//collision between bubble and shooter anf LIFE calculation
        if(collision_bubble_shooter(bubbles, shooter , life )) // game over event when bubble hits shooter
              { if(life==51)shooter.change_color();
                if(life==50)shooter.change_color2();
                if(life==49)shooter.change_color3();
                 life--;

               }
    //display life
        msg_life[msg_life.length() - 3]= life;
        lifeRemain.setMessage(msg_life);
    // WHEN ALL LIVES ARE GONE GAME IS FINISHED
       if(life==48)
       {
                string msg_GAMEOVER("RUN OUT OF LIVES 0 / 3");
                 Text gameover(WINDOW_X/2.0, WINDOW_Y/2.0, msg_GAMEOVER);

                 string msg_lose("# YOU LOSE #");
                 Text msgLose(WINDOW_X/2.0, WINDOW_Y/2.0  + 40, msg_lose);
                  wait(5);
                 return 0;
       }
      //WHEN ALL BUBBLES FINISH , then Display score
       if(bubbles.size() ==1)
        {
                 string msg_score("SCORE:   _/5");
                 Text gameScore(WINDOW_X/2.0, WINDOW_Y/2.0, msg_score);

                 string msg_GAMEOVER("GAME OVER");
                 Text gameover(WINDOW_X/2.0, WINDOW_Y/2.0 - 50, msg_GAMEOVER);

                 msg_score[msg_score.length()-3] =score;
                 gameScore.setMessage(msg_score);
                  wait(5);
                  return 0;

        }

        // Update the bubbles
        move_bubbles(bubbles);

        // Update the bullets
        move_bullets(bullets);

        wait(STEP_TIME/10000000);
    }
}




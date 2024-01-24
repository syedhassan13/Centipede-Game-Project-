#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace std;

// Initializing Dimensions.
// resolutionX and resolutionY determine the rendering resolution.
// Don't edit unless required. Use functions on lines 43, 44, 45 for resizing the game window.
const int resolutionX = 960;
const int resolutionY = 960;
const int boxPixelsX = 32;
const int boxPixelsY = 32;
const int gameRows = resolutionX / boxPixelsX; // Total rows on grid //30
const int gameColumns = resolutionY / boxPixelsY; // Total columns on grid//30

// Initializing GameGrid.
int gameGrid[gameRows][gameColumns] ;

// The following 3 exist purely for readability.
const int x = 0;
const int y = 1;
const int exists = 2;



//All Functions total 16
void drawPlayer(sf::RenderWindow& window, float player[], sf::Sprite& playerSprite);
void moveBullet(float bullet[], float player[], sf::Clock& bulletClock);
void drawBullet(sf::RenderWindow& window, float bullet[], sf::Sprite& bulletSprite);
void movementplayer(float player[]);
void bulletmovement(float player[],float bullet[],sf::Sound& fireSound );
void drawMushroomsandCollisions(float mushroomPositionsX[], float mushroomPositionsY[], int numMushrooms,bool &centi_mush,float bullet[],sf::Sprite& mushroomSprite,sf::RenderWindow& window,int mushroomHitCount[],sf::Sprite CentipedeHeadSprite,int &score,float centi_mush_x[12],float centi_mush_y[12],int &centi_head_pos,bool draw_mush[12],float head[][3],float centipedeSegments[][3], int centipedeLength );
void handleBulletMushroomCollisions(float bullet[], float mushroomPositionsX[], float mushroomPositionsY[], int numMushrooms, int mushroomHitCount[],float player[],int& score, sf::Text& scoreText,sf::Sprite& mushroom_1Sprite,sf::RenderWindow& window,sf::Sprite& mushroomSprite,float centi_mush_x[12],float centi_mush_y[12]);
bool bulletHitsMushroom(float bulletX, float bulletY, float mushroomX, float mushroomY);
bool overlapMushroom(float newX,float newY,float  mushroomPositionX[], float mushroomPositionY[],int existingPosition);
void showMenu(sf::RenderWindow& window);
void drawCentipedeSegments(float centipedeSegments[][3], int centipedeLength ,sf::RenderWindow& window, sf::Sprite& centipedeSprite,bool centipedeCondition,sf::Texture CentipedeHeadTexture,sf::Sprite CentipedeHeadSprite,int &centi_head_pos,float head[][3]);
void centipedeMovement(float centipedeSegments[][3], int centipedeLength , sf::Sprite& centipedeSprite, sf::Clock& centipedeClock,float mushroomPositionsX[], float mushroomPositionsY[],int numMushrooms,float centi_mush_x[12],float centi_mush_y[12],int &centi_head_pos,float moveRight[12],bool & PlayerArea,bool& easy,bool &medium ,bool &hard );
void cnetipedePlayerCollision(float centipedeSegments[][3], int centipedeLength ,float player[],bool &player_eliminated,float centi_mush_x[12],float centi_mush_y[12],int &centi_head_pos);
void showGameOverScreen(sf::RenderWindow& window,int &score);
void showVictoryScreen(sf::RenderWindow& window,int &score);
void CentipedeDestroyed(float centipedeSegments[][3], int centipedeLength ,bool & victory);




int main()
{
	float moveRight[12];
	
	srand(time(0));


	// Declaring RenderWindow.
	sf::RenderWindow window(sf::VideoMode(resolutionX, resolutionY), "Centipede Game ", sf::Style::Close | sf::Style::Titlebar | sf::Style::Resize);

	// Used to resize your window if it's too big or too small. Use according to your needs.
	//window.setSize(sf::Vector2u(640, 640)); // Recommended for 1366x768 (768p) displays.
	window.setSize(sf::Vector2u(1280, 1280)); // Recommended for 2560x1440 (1440p) displays.
	 //window.setSize(sf::Vector2u(1920, 1920)); // Recommended for 3840x2160 (4k) displays.
	
	// Used to position your window on every launch. Use according to your needs.
	window.setPosition(sf::Vector2i(100, 0));

	// Initializing Background Music.
	sf::Music bgMusic;
	bgMusic.openFromFile("Music/field_of_hopes.ogg");
	bgMusic.play();
	bgMusic.setVolume(50);

	// Initializing Background.
	sf::Texture backgroundTexture;
	sf::Sprite backgroundSprite;
	backgroundTexture.loadFromFile("Textures/background_4.png");
	backgroundSprite.setTexture(backgroundTexture);
	backgroundSprite.setColor(sf::Color(255, 255, 255, 255 * 0.50)); // Reduces Opacity to 25%

	// Initializing Player and Player Sprites.
	float player[2] = {};
	player[x] = ((gameColumns / 2) * boxPixelsX) -30;//450;//480
	player[y] =((gameColumns * 3 / 4) * boxPixelsY) +180;// 900;//720
	int player_y=player[y];
	sf::Texture playerTexture;
	sf::Sprite playerSprite;
	playerTexture.loadFromFile("Textures/player.png");
	playerSprite.setTexture(playerTexture);
	playerSprite.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));

	// Initializing Bullet and Bullet Sprites.
	float bullet[3] = {};
	bullet[x] = resolutionX+100;//player[x];//480
	bullet[y] = resolutionY+100;//player[y] - boxPixelsY;//688
	bullet[exists] = false;
	sf::Clock bulletClock;
	sf::Texture bulletTexture;
	sf::Sprite bulletSprite;

	//Intializing sound of fire
	sf::SoundBuffer fireSoundBuffer;
    fireSoundBuffer.loadFromFile("Sound Effects/fire1.wav");  
    sf::Sound fireSound(fireSoundBuffer);  
	fireSound.setVolume(100);
	bulletTexture.loadFromFile("Textures/bullet.png");
	bulletSprite.setTexture(bulletTexture);
	bulletSprite.setTextureRect(sf::IntRect(0, 0,7* boxPixelsX, boxPixelsY));



	//Intiliazing mushroom sprite
	sf::Texture mushroomTexture;
	sf::Sprite mushroomSprite;
	mushroomTexture.loadFromFile("Textures/mushroom.png");
	mushroomSprite.setTexture(mushroomTexture);
	mushroomSprite.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));

	
	//Initliazing damagedmushroom sprite
	sf::Texture mushroom_1Texture;
	sf::Sprite mushroom_1Sprite;
	mushroom_1Texture.loadFromFile("Textures/mushroom.png");
	mushroom_1Sprite.setTexture(mushroom_1Texture);
	mushroom_1Sprite.setTextureRect(sf::IntRect(4 * boxPixelsX - boxPixelsX, 0, boxPixelsX, boxPixelsY));

	//Initliazing Poisnus sprite
	sf::Texture mushroom_PTexture;
	sf::Sprite mushroom_PSprite;
	mushroom_PTexture.loadFromFile("Textures/mushroom.png");
	mushroom_PSprite.setTexture(mushroom_PTexture);
	mushroom_PSprite.setTextureRect(sf::IntRect(0, boxPixelsX, boxPixelsX, boxPixelsY));






	//Mushroom quantity
	const int minNumMushrooms = 20;
	const int maxNumMushrooms = 42;
	const int numMushrooms = rand() % ((maxNumMushrooms-12) - minNumMushrooms + 1) + minNumMushrooms;  
    float mushroomPositionsX[numMushrooms];  
    float mushroomPositionsY[numMushrooms]; 
	int mushroomHitCount[numMushrooms] = {0};

	//Intialize score board
	sf::Font font;
	font.loadFromFile("Roboto/Roboto-Black.ttf");
	int score=0;
	sf::Text scoreText;
	scoreText.setFont(font);
	scoreText.setCharacterSize(50); 
	scoreText.setFillColor(sf::Color::White);
	scoreText.setString("Score : ");
	scoreText.setPosition(resolutionX - 300, resolutionY - 70); 
	
	//Intializing Centipede sprite
	sf::Texture centipedeTexture;
	sf::Sprite centipedeSprite;
	centipedeTexture.loadFromFile("Textures/c_body_left_walk.png");
	centipedeSprite.setTexture(centipedeTexture);
	centipedeSprite.setTextureRect(sf::IntRect(0,0,boxPixelsX,boxPixelsY));

	//Condition to print centipede related to drawcentipedeSegment function
	bool centipedeCondition=true;

	sf::Clock centipedeClock;
	//Intiliazing Centipede Head sprite
	sf::Texture CentipedeHeadTexture;
	sf::Sprite CentipedeHeadSprite;
	CentipedeHeadTexture.loadFromFile("Textures/c_head_left_walk.png");
	CentipedeHeadSprite.setTexture(CentipedeHeadTexture);
	CentipedeHeadSprite.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));

	
	//use for mushrooms to draw and then not
	bool centi_mush=true;
	//Several flags for various conditions
	bool player_eliminated=false;
	bool victory=false;
	bool easy=false;
	bool medium = false;
	bool hard = false;

	//for head position
	int centi_head_pos=0;
	//Poisnus mushroom
	float centi_mush_x[12]={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
	float centi_mush_y[12]={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
	//Centipede to move in player area
	bool PlayerArea=true;
	bool draw_mush[12]={false,false,false,false,false,false,false,false,false,false,false,false};
	//for all heads
	float head[12][3]=
	{{0,0,false},
	{0,0,false},
	{0,0,false},
	{0,0,false},
	{0,0,false},
	{0,0,false},
	{0,0,false},
	{0,0,false},
	{0,0,false},
	{0,0,false},
	{0,0,false},
	{0,0,true}};

	
int centipedeLength = 12; 
	
float centipedeSegments[12][3]=
{
{0,0,true},
{30,0,true},
{60,0,true},
{90,0,true},
{120,0,true},
{150,0,true},
{180,0,true},
{210,0,true},
{240,0,true},
{270,0,true},
{300,0,true},
{330,0,true}};


for(int i=0;i<12;i++)
{
	moveRight[i]=true;
}
	

	//window for menu
	while(true)
		{
		showMenu(window);

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
		{
			easy=true;
			break;
		}
		if(  sf::Keyboard::isKeyPressed(sf::Keyboard::Num2) )
		{
			medium =true;
			break;
		}

		if( sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
		{
			hard=true;
			break;
		}
		
		sf::Event e;
		while (window.pollEvent(e)) {
			
			if (e.type == sf::Event::Closed) {
				return 0;
			}
		}
		}
		
		
	
	while((window.isOpen())) {
	
		
		
		movementplayer(player);
		
		window.draw(backgroundSprite);
		
		drawPlayer(window, player, playerSprite);
		
		handleBulletMushroomCollisions(bullet, mushroomPositionsX, mushroomPositionsY, numMushrooms, mushroomHitCount,player,score,scoreText, mushroom_1Sprite,window,mushroomSprite, centi_mush_x, centi_mush_y);
	    
        drawMushroomsandCollisions(mushroomPositionsX, mushroomPositionsY, numMushrooms,centi_mush,bullet, mushroomSprite,window, mushroomHitCount, CentipedeHeadSprite,score,centi_mush_x, centi_mush_y, centi_head_pos, draw_mush, head,centipedeSegments,  centipedeLength );

		window.draw(scoreText);
	
		bulletmovement(player,bullet,fireSound);
		
		drawCentipedeSegments( centipedeSegments, centipedeLength ,window, centipedeSprite,centipedeCondition, CentipedeHeadTexture, CentipedeHeadSprite,centi_head_pos, head );
		
		cnetipedePlayerCollision(centipedeSegments,  centipedeLength ,player, player_eliminated, centi_mush_x,centi_mush_y,centi_head_pos);

		CentipedeDestroyed(centipedeSegments,  centipedeLength,  victory);
		
        centipedeMovement(centipedeSegments,  centipedeLength,  centipedeSprite, centipedeClock, mushroomPositionsX, mushroomPositionsY,numMushrooms, centi_mush_x, centi_mush_y,centi_head_pos,moveRight, PlayerArea, easy,medium ,hard );
		
	    scoreText.setString("Score: " + to_string(score));
		 
		if (bullet[exists] == true) {
			moveBullet(bullet,player, bulletClock);
			drawBullet(window, bullet, bulletSprite);
		}

		
		//To draw Mushrooms
		for (int i = 0; i < numMushrooms; ++i)
		 {
			
			if (mushroomPositionsX[i] != -1 && mushroomPositionsY[i] != -1 )
			{
				mushroomSprite.setPosition(mushroomPositionsX[i], mushroomPositionsY[i]);
				
                    window.draw(mushroomSprite);
                
			}
			
			
	     }
		
		
		 //To draw poisun mushrooms
			for (int i = 0; i < 12; ++i)
		 {
			if(draw_mush[i])
			{
			
			if (centi_mush_x[i] != -1 && centi_mush_y[i] != -1)
			{
				mushroom_PSprite.setPosition(centi_mush_x[i], centi_mush_y[i]);
				window.draw(mushroom_PSprite);
				
			}
			
			
			
	     
			}
		 }

		//end game conditions
		 if(player_eliminated)
		 {
			showGameOverScreen(window,score);
		 }

		 if(victory)
		 {
			showVictoryScreen(window,score);
		 }

        

		

		
		sf::Event e;
		while (window.pollEvent(e)) {
			
			if (e.type == sf::Event::Closed) {
				return 0;
			}
			

		}	
		window.display();
		window.clear();
	}
	
}

//Function for player movement to do edit player movement when required
void movementplayer(float player[])
{
	
if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && player[x] > 0) {
            // Move the player left, but check for the left boundary
            player[x] -= 0.5;  
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && player[x] < resolutionX - boxPixelsX) {
            // Move the player right, but check for the right boundary
            player[x] += 0.5;  
        }
		//5 rows accoding to movement of centipede
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && player[y] >  (resolutionY - (5  * boxPixelsY)))   {
            // Move the player up, but check for the top boundary
            player[y] -= 0.5;  
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && player[y] < resolutionY - boxPixelsY) {
            // Move the player down, but check for the bottom boundary
            player[y] += 0.5;  
        }
}

//Function for bullet fired when W pressed to edit anything related to bullet shoots very imp function
void bulletmovement(float player[],float bullet[],sf::Sound& fireSound)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	 {
		 
			
            // Check if the bullet is not already on the screen
            if (!bullet[exists]) {
                // Shoot the bullet
                bullet[x] = player[x];
                bullet[y] = player[y] - boxPixelsY;  // Adjust the starting position as needed
                bullet[exists] = true;

				fireSound.play();
				

				
            }
			

	}
}

//Function to check mushroom overlap or not
bool overlapMushroom(float newX,float newY,float existingPositionX[],float existingPositionY[],int existingPosition)
{
	for(int i=0 ; i< existingPosition;i++)
	{
		if(newX < existingPositionX[i]+boxPixelsX &&
		   newX + boxPixelsX > existingPositionX[i]  &&
		   newY < existingPositionY[i] + boxPixelsY &&
		   newY + boxPixelsY > existingPositionY[i])
		   return true; //overlap detected
	}
	return false; //not overlap

}

//To generate mushrroms at random position this will solve my issue when need to eidt any thing related to mushrooms position
//Handel Collisions as well for centipede and mushroom and bullet
void drawMushroomsandCollisions(float mushroomPositionsX[], float mushroomPositionsY[], int numMushrooms,bool &centi_mush,float bullet[],sf::Sprite& mushroomSprite,sf::RenderWindow& window,int mushroomHitCount[],sf::Sprite CentipedeHeadSprite,int &score,float centi_mush_x[12],float centi_mush_y[12],int &centi_head_pos,bool draw_mush[12],float head[][3],float centipedeSegments[][3], int centipedeLength )
{


	
	
		if(centi_mush)
	{
		for (int i = 0; i < numMushrooms; ++i)
    {

		float newX,newY;
		do
		{
		
			newX = static_cast<float>(rand() % (resolutionX - boxPixelsX + 1)  );  
			newY = static_cast<float>(rand() % ((resolutionY-4*boxPixelsX) - 4* boxPixelsY) + boxPixelsY);
		
		}while(overlapMushroom(newX,newY, mushroomPositionsX, mushroomPositionsY,i));

		mushroomPositionsX[i]=newX;
		mushroomPositionsY[i]=newY;

	}
     centi_mush=false;
	}
	
   
	//Bullet and centipede Collision
	for (int i = 0; i < 12; ++i) {
    if (centipedeSegments[i][exists] &&
        bullet[x] < centipedeSegments[i][x] + boxPixelsX &&
        bullet[x] + boxPixelsX > centipedeSegments[i][x] &&
        bullet[y] < centipedeSegments[i][y] + boxPixelsY &&
        bullet[y] + boxPixelsY > centipedeSegments[i][y] ) {

        centipedeSegments[i][exists] = false;
		centipedeSegments[0][exists]=false;
		bool head_shot=true;

		//If bullet touch with head whole centipede destroyed
		if(head[i][exists])
		{
			score+=20;
			for(int i=0;i<12;i++)
			{
				if(!(centipedeSegments[i][exists]))
				{
					head_shot=false;
				}
				
			}
			for(int i=0;i<12;i++)
			{
				if(head_shot=true)
				{
					centipedeSegments[i][exists]=false;
				}
				
			}
		}
		
		
		

        for (int j = 0; j < 12; ++j) {
			//which segment is destroyed there mushroom will formed
            if (!draw_mush[j]) {
                centi_mush_x[j] = centipedeSegments[i][x];
                centi_mush_y[j] = centipedeSegments[i][y];
                draw_mush[j] = true;
				head[i][exists]=true;
	            centi_head_pos=i;
				score+=10;
                // Reset bullet when hit centipede segment
                bullet[y] = resolutionY + 100; // Move the bullet out of the screen
                bullet[exists] = false;
				

                break;
            }
        }
		
    }
}

}



// Function to check if a bullet hits a mushroom collision function
bool bulletHitsMushroom(float bulletX, float bulletY, float mushroomX, float mushroomY) {
	
    //assuming both bullet and mushrrom are square
   return bulletX < mushroomX + boxPixelsX && bulletX + boxPixelsX > mushroomX && bulletY < mushroomY + boxPixelsY && bulletY + boxPixelsY > mushroomY;
}



// Function to handle bullet-mushroom collisions will use to manage mushroom bullet collision
void handleBulletMushroomCollisions(float bullet[], float mushroomPositionsX[], float mushroomPositionsY[], int numMushrooms, int mushroomHitCount[],float player[],int& score, sf::Text& scoreText,sf::Sprite& mushroom_1Sprite,sf::RenderWindow& window,sf::Sprite& mushroomSprite,float centi_mush_x[12],float centi_mush_y[12]) {
    
	if (bullet[exists]) {
		
        for (int i = 0; i < numMushrooms; ++i) {
            if (bulletHitsMushroom(bullet[x], bullet[y], mushroomPositionsX[i], mushroomPositionsY[i])) {
                // Bullet hit the mushroom
                mushroomHitCount[i]++;
				
                if (mushroomHitCount[i] >= 2) {
                    // Second hit and beyond: Destroy the mushroom
					
                    mushroomPositionsX[i] = resolutionX + 100;  
                    mushroomPositionsY[i] = resolutionY + 100;
                    score++;
					
                   
					
                }


                // Reset bullet when hit mushroom
                bullet[y] = player[y];
                bullet[exists] = false;
            }
			
                
			
				
				}
		
        }
		
		
			
		
    }



//Very Imp function to manage all type of centipede Movements with wall, mushrooms, poisnus mushrooms etc..
void centipedeMovement(float centipedeSegments[][3], int centipedeLength , sf::Sprite& centipedeSprite, sf::Clock& centipedeClock,float mushroomPositionX[], float mushroomPositionY[],int numMushrooms,float centi_mush_x[12],float centi_mush_y[12],int &centi_head_pos,float moveRight[12],bool & PlayerArea,bool& easy,bool &medium ,bool &hard ) {
  
  //for different levels
   if(easy)
  {
    if (centipedeClock.getElapsedTime().asMilliseconds() <5)
        return;

    centipedeClock.restart();
  }
  if(medium)
  {
	 if (centipedeClock.getElapsedTime().asMilliseconds() <3)
        return;

    centipedeClock.restart();
  }
  if(hard)
  {
	 if (centipedeClock.getElapsedTime().asMilliseconds() <1)
        return;

    centipedeClock.restart();
  }
  
	//check that centipede is above player area
	if(PlayerArea==true)
  {
    for (int i = 0; i < centipedeLength; ++i) {
	
        if (centipedeSegments[i][exists]) {
		
             if (moveRight[i]) {
               centipedeSegments[i][x] += 5;
			 
			  
            } else if (!(moveRight[i])){
                centipedeSegments[i][x] +=-5;
				

            }
			 
			   
	}
}

            // Check if the centipede segment reaches the screen boundaries
            for (int i = 0; i < centipedeLength; ++i) {
			
            if (centipedeSegments[i][x] < 0 )
                 {
				 if(centipedeSegments[i][exists])
			{
                // Reflect back when reaching the left boundary
                moveRight[i] = true;
				centipedeSegments[i][y] += boxPixelsY;
			}
				
            } else if (centipedeSegments[i][x] >= (resolutionX - boxPixelsX) )
			 {
			 	if(centipedeSegments[i][exists])
				{
                // Reflect back when reaching the right boundary
                moveRight[i] = false;
		        centipedeSegments[i][y] += boxPixelsY;
				}
            }
          }
	
		  
	for (int j = 1; j <= 30; j++) {
    for (int i = 0; i < numMushrooms; i++) {
        for (int k = 0; k < 12; k++) {
			
			//collison of centipede with mushrooms both normal and poisnus
            if ((centipedeSegments[k][x] < mushroomPositionX[i] + boxPixelsX &&
                centipedeSegments[k][x] + boxPixelsX > mushroomPositionX[i] &&
                centipedeSegments[k][y] < mushroomPositionY[i] + boxPixelsY &&
                centipedeSegments[k][y] + boxPixelsY > mushroomPositionY[i])
				|| (centipedeSegments[k][x] < centi_mush_x[i] + boxPixelsX &&
                centipedeSegments[k][x] + boxPixelsX > centi_mush_x[i] &&
                centipedeSegments[k][y] < centi_mush_y[i] + boxPixelsY &&
                centipedeSegments[k][y] + boxPixelsY > centi_mush_y[i]) && k!=0 ) {

			
					
				
					
				//j are the rows
                if (j % 2 != 0 ) {
                    centipedeSegments[k][y] += boxPixelsY;
					
                    moveRight[k] = !moveRight[k]; // Reverse the direction
					
                } else {
					
                    centipedeSegments[k][y] += boxPixelsY;
                }
            }
			
        
		
		}
	}
	}
	


	//reaches to bottom limited to player area
	if (centipedeSegments[0][y] > resolutionY - boxPixelsY) 
	{
		PlayerArea=false;
	}
  }

else
	{
		
	
		for (int i = 0; i < centipedeLength; ++i) {
            if (centipedeSegments[i][exists]) {
                centipedeSegments[i][y] =  resolutionY -  boxPixelsY;
	
				if(centipedeSegments[i][x] < 0 && centipedeSegments[i][y] > resolutionY - boxPixelsY)
				{
				
					moveRight[i]=true;
				}
            }
        
    }

	for (int i = 0; i < centipedeLength; ++i) {
            if (centipedeSegments[i][exists]) {
                centipedeSegments[i][y] =  resolutionY -  boxPixelsY;
				
				if (centipedeSegments[i][y] > resolutionY - boxPixelsY && centipedeSegments[i][x] >= (resolutionX - boxPixelsX)) 
				{
		             moveRight[i]=false;

				}
            }
        
    }

	for (int i = 0; i < centipedeLength; ++i) {
        if (centipedeSegments[i][exists]) {
            centipedeSegments[i][y] -= 2*boxPixelsY; // Player area 5 rows
        }
    }

	 // Check if the centipede segment reaches the screen boundaries
            for (int i = 0; i < centipedeLength; ++i) {
            if (centipedeSegments[i][x] < 0 )
                 {
                // Reflect back when reaching the left boundary
                moveRight[i] = true;
				
				
				
            } else if (centipedeSegments[i][x] >= (resolutionX - boxPixelsX) )
			 {
                // Reflect back when reaching the right boundary
                moveRight[i] = false;
				
				
				
            
            }
          }

		for(int i=0;i<12;i++)
		{
			if (centipedeSegments[i][exists]) {
		if(centipedeSegments[i][y] -= 2* boxPixelsY)
		PlayerArea=true;
		}
		}
	

    for (int i = 0; i < centipedeLength; ++i) {
		
        if (centipedeSegments[i][exists]) {
		
             if (moveRight[i]) {
               centipedeSegments[i][x] += 5;
			 
			  
            } else if (!(moveRight[i])){
                centipedeSegments[i][x] +=-5;
				

            }
			 
			   
	}
}
	}

  
}


//Function to draw player
void drawPlayer(sf::RenderWindow& window, float player[], sf::Sprite& playerSprite) {
	playerSprite.setPosition(player[x], player[y]);
	window.draw(playerSprite);
}
//Function for bullet movemnet
void moveBullet(float bullet[], float player[], sf::Clock& bulletClock) {
	
	
	
		if (bulletClock.getElapsedTime().asMilliseconds() < 7)
			return;

			
		

			bulletClock.restart();
			bullet[y] -= 15;	
			if (bullet[y] < -32)
			{
				bullet[y] = resolutionX+100;//player[y];
				bullet[x] = resolutionY+100;//player[x];
				bullet[exists] = false;
			}
			
	 
		
	
}
//Function to draw bullet
void drawBullet(sf::RenderWindow& window, float bullet[], sf::Sprite& bulletSprite) {
	bulletSprite.setPosition(bullet[x], bullet[y]);
	window.draw(bulletSprite);
}

//Function for collision between centipede and player then player will lose 
void cnetipedePlayerCollision(float centipedeSegments[][3], int centipedeLength ,float player[],bool &player_eliminated,float centi_mush_x[12],float centi_mush_y[12],int &centi_head_pos)
{
	for(int i=0 ; i<12 ; i++)
	{
		if(centipedeSegments[i][exists] &&
        player[x] < centipedeSegments[i][x] + boxPixelsX &&
        player[x] + boxPixelsX > centipedeSegments[i][x] &&
        player[y] < centipedeSegments[i][y] + boxPixelsY &&
        player[y] + boxPixelsY > centipedeSegments[i][y] )
		{
			player[x]=resolutionX+100;
			player[y]=resolutionY+100;
			player_eliminated=true;
			break;
		}
	}
	//Player collsion with Poisnus mushroom
	for(int i=0 ; i<12 ; i++)
	{
		if(player[x] < centi_mush_x[i] + boxPixelsX &&
           player[x] + boxPixelsX > centi_mush_x[i] &&
           player[y] < centi_mush_y[i] + boxPixelsY &&
           player[y] + boxPixelsY > centi_mush_y[i])
				{
					player[x]=resolutionX+100;
					player[y]=resolutionY+100;
					player_eliminated=true;
					break;
				}
	}
}
//Function if player destroy sentipede
void CentipedeDestroyed(float centipedeSegments[][3], int centipedeLength ,bool & victory)
{
	victory=true;
	
	for(int i=0 ;i<12 ;i++)
	{
		if((centipedeSegments[i][exists]))
		{
			victory=false;
		}
		
	}
	
}
//This Function will show victory screen if player wins
void showVictoryScreen(sf::RenderWindow& window,int &score) {
    // Display victory message or screen
    sf::Font font;
    font.loadFromFile("Roboto/Roboto-BoldItalic.ttf"); // Replace with the path to your font file

    sf::Text victoryText;
    victoryText.setFont(font);
    victoryText.setString("Congratulations! You Win!");
    victoryText.setCharacterSize(70);
    victoryText.setFillColor(sf::Color::Green);
    victoryText.setPosition(100, 400);
	victoryText.setString("Congratulations! You Win!\n\nScore: " + to_string(score));


    window.clear();
    window.draw(victoryText);
    window.display();

    sf::Event event;
    while (window.waitEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
            break;
        }
    }
}

//This function will show game over screen when player looses
void showGameOverScreen(sf::RenderWindow& window,int &score) {
    // Display game over message or screen
    sf::Font font;
    font.loadFromFile("Roboto/Roboto-BoldItalic.ttf"); // Replace with the path to your font file

    sf::Text gameOverText;
    gameOverText.setFont(font);
    gameOverText.setString("Game Over!");
    gameOverText.setCharacterSize(100);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setPosition(250, 400);
	gameOverText.setString("Game Over!\n\nScore: " + to_string(score));

    window.clear();
    window.draw(gameOverText);
    window.display();

    sf::Event event;
    while (window.waitEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
            break;
        }
    }
}


//Function to show menu levels before game
void showMenu(sf::RenderWindow& window) {
	sf::Texture backgroundTexture;
	sf::Sprite backgroundSprite;
	backgroundTexture.loadFromFile("Textures/Centipede_game_1.png");
	backgroundSprite.setTexture(backgroundTexture);
	backgroundSprite.setColor(sf::Color(255, 255, 255, 255 * 0.20));
	

    sf::Font font;
    font.loadFromFile("Roboto/Roboto-BoldItalic.ttf");

    sf::Text title;
    title.setFont(font);
    title.setCharacterSize(70);
    title.setFillColor(sf::Color::White);
    title.setString("Centipede Game");
    title.setPosition(resolutionX / 2 - 130, resolutionY / 4);

    sf::Text easyText;
    easyText.setFont(font);
    easyText.setCharacterSize(40);
    easyText.setFillColor(sf::Color::White);
    easyText.setString("1. Easy");
    easyText.setPosition(resolutionX / 2 - 30, resolutionY / 2);

    sf::Text mediumText;
    mediumText.setFont(font);
    mediumText.setCharacterSize(40);
    mediumText.setFillColor(sf::Color::White);
    mediumText.setString("2. Medium");
    mediumText.setPosition(resolutionX / 2 - 30, resolutionY / 2 + 50);

    sf::Text hardText;
    hardText.setFont(font);
    hardText.setCharacterSize(40);
    hardText.setFillColor(sf::Color::White);
    hardText.setString("3. Hard");
    hardText.setPosition(resolutionX / 2 - 30, resolutionY / 2 + 100);

    window.clear();
	window.draw(backgroundSprite);

    window.draw(title);
    window.draw(easyText);
    window.draw(mediumText);
    window.draw(hardText);

    window.display();

}



//FUnction to draw centipede and this will manage how centipede will be drawn
void drawCentipedeSegments(float centipedeSegments[][3], int centipedeLength ,sf::RenderWindow& window, sf::Sprite& centipedeSprite,bool centipedeCondition,sf::Texture CentipedeHeadTexture,sf::Sprite CentipedeHeadSprite,int &centi_head_pos,float head[][3]) {
  if(centipedeCondition)
  {
    for (int i = 0; i < centipedeLength; ++i) {
       if (centipedeSegments[i][exists]) {
            centipedeSprite.setPosition(centipedeSegments[i][x], centipedeSegments[i][y]);
            
            // Check if the segment is a head and set the texture accordingly
            if ( head[i][exists]) {
				head[i][x]=centipedeSegments[i][x];
				head[i][y]=centipedeSegments[i][y];

                CentipedeHeadSprite.setPosition(head[i][x], head[i][y]);
                window.draw(CentipedeHeadSprite);
            } else {
				
                window.draw(centipedeSprite);
            }
        }
		
    

    }
  }
	centipedeCondition=false;

	for(int i=1;i<11;i++)
	{
		for(int j=0;j<12;j++)
		{
		if(head[i][exists])
		{
			if(centi_head_pos==j)
			{
			CentipedeHeadSprite.setPosition(centipedeSegments[j-1][x], centipedeSegments[j-1][y]);
            window.draw(CentipedeHeadSprite);
			
			}
		}
	}
	}
}

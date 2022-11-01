#include <math.h>
#include <array>
#include <vector>

#include "picosystem.hpp"

using namespace picosystem;

enum state_t {PLAYING, GAME_OVER, HEARTLOSS};
state_t state = PLAYING;

struct vec_t {
  int32_t x, y;
};

int32_t lives;


constexpr vec_t bounds{.x = 18, .y = 16};
constexpr int scale = 6;
// Player
struct {
  vec_t dir;
  vec_t pos;
  int32_t score;

  vec_t next() {
    return {.x = pos.x + dir.x, .y = pos.y + dir.y};
  }

  void move() {
    double L = sqrt(dir.x*dir.x + dir.y*dir.y);
    dir.x /= L;
    dir.y /= L;
    pos = next();
  }
} player;

vec_t initial_animal_movement(vec_t pos){
  int d = std::rand() % 2;
  vec_t dir = {.x = 0, .y = 0};
  if (d == 2) {
    if (pos.x < bounds.x/2){
      dir = {.x = 1, .y = 0};
    } else {
      dir = {.x = -1, .y = 0};
    } 
  } else {
    if (pos.y < bounds.y/2) {
      dir = {.x = 0, .y = 1};
    } else {
      dir = {.x = 0, .y = -1};
    } 
  }
  return dir;
}

vec_t change_animal_movement(bool x, bool y, vec_t dir){
   int d = std::rand() % 3;
   if(x){
      if(d == 2){
        return {.x = dir.x*-1, .y = 0};
      } else if (d == 1) {
        return {.x = 0, .y = 1};
      } else {
        return {.x = 0, .y = -1};
      }
   }

   if(y){
    if(d == 2){
        return {.x = 0, .y = dir.y*-1};
      } else if (d == 1) {
        return {.x = 1, .y = 0};
      } else {
        return {.x = -1, .y = 0};
      }
   }
}

// Animal
struct {
  vec_t dir;
  vec_t pos;

  vec_t next() {
    return {.x = pos.x + dir.x, .y = pos.y + dir.y};
  }

  void move() {
    pos = next();
    
  }

} animal;

class Fruit{
  public:
  vec_t pos;
  vec_t dir;
  Fruit(int x, int y){
    pos = {.x = x, .y = y};
    dir = {.x = 0, .y = 1};
  }
  vec_t next() {
    return {.x = pos.x + dir.x, .y = pos.y + dir.y};
  }

  void move() {
    pos = next(); 
  }
};

std::string debug = "";
std::vector<Fruit> fruits;

Fruit place_fruit(){
  return Fruit(std::rand() % bounds.x, 2);
}

void place_animal() {
  bool hit = false;
  int32_t y_pos = std::rand() % bounds.y;
  animal.pos.x = std::rand() % bounds.x;
  animal.pos.y = y_pos > 2 ? y_pos : y_pos + 2;
  animal.dir = initial_animal_movement(animal.pos);
}

vec_t transform(vec_t v) {
  return {.x = (v.x * scale) + 6, .y = (v.y * scale) + 18};
}

void set_game(int32_t _lives) {

  state = PLAYING;

  lives = _lives;

  player.pos = {.x = 9, .y = 15};
  player.dir = {.x = 1, .y = 0};

  place_animal();

  fruits.push_back(place_fruit());
}

voice_t blip;

void init() {
  lives = 3;
  player.score = 0;
  set_game(lives);
  blip = voice(50, 50, 50, 50, 10, 2);
}

bool wall_hit_check_x(vec_t next){
  return next.x < 1 || next.x >= bounds.x - 3;
}

bool wall_hit_check_y(vec_t next){
  return next.y < 3 || next.y >= bounds.y - 3;
}

bool wall_hit_check(vec_t next){
  return next.x < 0 || next.x >= bounds.x || next.y < 2 || next.y >= bounds.y;
}



void update(uint32_t tick) {
  if(state == PLAYING) {

    if (tick % 60 == 0) {
      fruits.push_back(place_fruit());
      printf(".\n");
    }

    if(tick % 30 == 0) {
     if(state != GAME_OVER) {
      for(auto & fruit: fruits){
        fruit.move();
        debug = str(fruit.pos.x) + " " + str(fruit.pos.y);
      } 
      // fruits.front().move();
      // debug = str(fruits.front().pos.x) + " " + str(fruits.front().pos.y);
    }}
    
    if(tick % 10 == 0) {
    
      // if(button(UP))    {player.dir.x =  0; player.dir.y = -1; player.move();}
      // if(button(DOWN))  {player.dir.x =  0; player.dir.y =  1; player.move();}
      if(button(LEFT))  {player.dir.x = -1; player.dir.y =  0; player.move();}
      if(button(RIGHT)) {player.dir.x =  1; player.dir.y =  0; player.move();}

      vec_t next = player.next();

       if(intersects(next.x, next.y, 1, 1, animal.pos.x, animal.pos.y, 1, 1)) {
        player.score++;
        play(blip, 1800, 30, 100);
        place_animal();
      }

      // check if the snake's head has collided with the walls
      if(wall_hit_check(next)) {
          if (lives == 0) {
            state = GAME_OVER;
          } else {
            lives-=1; 
            state = HEARTLOSS;
          }       
      }

      if(wall_hit_check_x(animal.next())){
        animal.dir = change_animal_movement(true, false, animal.dir);
      } else if (wall_hit_check_y(animal.next())){
        animal.dir = change_animal_movement(false, true, animal.dir);
      }

      
    }
  } else if (state == GAME_OVER) {

    if(pressed(A)) {
      init();
    }
  } else {

    set_game(lives);
    
  }
}

void label(std::string s) {
  blend(ALPHA);
  pen(1, 2, 3, 8);
  frect(0, 11, SCREEN->w, 15);
  pen(0, 0, 0);
  text("Score: " + s, 3, 16);
  pen(15, 15, 15);
  text("Score: " + s, 2, 15);

  for (int i = lives; i > 0; i--){
    text("\\spr064", SCREEN->w - 10 * i, 15);
  }
  
}


void draw(uint32_t tick) {

  pen(10, 25, 255);
  clear();
  pen(0, 0, 0, 4);

  pen(15, 15, 15);
  frect(0, 0, SCREEN->w, 11);
  pen(0, 0, 0);
  text("Catch'em All!", 2, 2);
  // label(str(player.score));
  label(debug);

  //Limites
  rect(2, 27, 116, 90);
  rect(2, 29, 116, 90);

  //Animal
  sprite(BIRD, transform(animal.pos).x + 3, transform(animal.pos).y + 3 );

  for (auto fruit: fruits){
    sprite(TOMATO, transform(fruit.pos).x + 3, transform(fruit.pos).y + 3 );
  }

  bool flash = ((time() / 250) % 2) == 0;
  if(state == PLAYING || (state == GAME_OVER && flash)) {
      vec_t p = transform(player.pos);
      sprite(HAT_DEMON_KING, p.x, p.y);
  }


  // pen(1, 2, 3, 8);
  // frect(0, SCREEN->h - 74, 80, 74);

  // pen(15, 15, 15);
  // cursor(2, SCREEN->h - 70);
  // text("dir.x: " + str(player.dir.x));
  // text("dir.y: " + str(player.dir.y));
  // text("pos.x: " + str(player.pos.x));
  // text("pos.y: " + str(player.pos.y));

}
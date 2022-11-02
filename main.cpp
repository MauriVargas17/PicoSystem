#include <bits/stdc++.h>
#include <math.h>
#include <array>
#include "../utility/music_render.cpp"

#include "picosystem.hpp"

using namespace picosystem;

//Estados del Juego
enum state_t {PLAYING, GAME_OVER, MENU, SETTINGS};
state_t state = MENU;

struct vec_t {
  int32_t x, y;
};

//Fronteras y escala del mapa
constexpr vec_t bounds{.x = 18, .y = 16};
constexpr int scale = 6;

//Selector de Juego
bool play_selected = true;
//Verificar si el juego está listo
bool is_game_set = false;

//Selectores de Dificultad en Menu
bool easy_selected = true;
bool medium_selected = false;
bool hard_selected = false;
//Velocidad de aparición de enemigos
int32_t difficulty = 300;

//Arreglo de Sprites 
std::array<int, 6> fruit_sprites = {{
    APPLE,
    CHERRY,
    BANANA,
    ORANGE,
    EGGPLANT,
    STRAWBERRY,
  }};

//Estructura de Jugador
struct {
  vec_t dir;
  vec_t pos;
  int32_t score;
  int32_t lives;

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

class Bullet{
  public:
  vec_t pos;
  vec_t dir;
  Bullet(int x, int y){
    pos = {.x = x, .y = y};
    dir = {.x = 0, .y = -1};
  }
  vec_t next() {
    return {.x = pos.x + dir.x, .y = pos.y + dir.y};
  }

  void move() {
    pos = next(); 
  }
};

/*
Funciones de la mecánica de Juego
*/

//Sonidos de selector y aceptar
voice_t blip, enter;
//Fruta aleatoria del arreglo fruit_sprites
int fruit_selected;

//Deques de los elementos del juego
std::deque<Fruit> fruits;
std::deque<Fruit> rotten_fruits;
std::deque<Bullet> bullets;


Fruit place_fruit(){
  return Fruit(std::rand() % bounds.x, 1);
}

Bullet place_bullet(){
  return Bullet(player.pos.x, player.pos.y);
}

void change_state(state_t _state){
  state = _state;
}

//Transformar posiciones del mapa de acuerdo a la escala
vec_t transform(vec_t v) {
  return {.x = (v.x * scale) + 6, .y = (v.y * scale) + 18};
}

/*
Revisión de colisión en paredes
*/

bool in_bounds_right(vec_t next){
  return next.x <= bounds.x - 1;
}

bool in_bounds_left(vec_t next){
  return next.x >= 0;
}

bool wall_hit_check_x(vec_t next){
  return next.x < 1 || next.x >= bounds.x - 3;
}

bool wall_hit_check_y(vec_t next){
  return next.y < 3 || next.y >= bounds.y - 3;
}

bool wall_hit_check(vec_t next){
  return next.x < 0 || next.x >= bounds.x || next.y < 1 || next.y >= bounds.y;
}

//Armar las condiciones de juego
void set_game(int32_t _lives) {

  // state = PLAYING;

  player.lives = _lives;

  player.pos = {.x = 9, .y = 15};
  player.dir = {.x = 1, .y = 0};

  fruits.push_back(place_fruit());

  fruit_selected = fruit_sprites[std::rand() % 5];

  is_game_set = true;
}

void init() { 

  play_selected = true;
  easy_selected = true;
  medium_selected = false;
  hard_selected = false;
  blip = voice(50, 50, 50, 50, 10, 2);
  enter = voice(100, 50, 100, 50, 20, 2);

  if (state == PLAYING) {
    bullets.clear();
    fruits.clear();
    rotten_fruits.clear();
    player.score = 0;
    set_game(3);
  } 
}


void update(uint32_t tick) {
  
  if (state == MENU){

    if(button(A))  {if(play_selected){change_state(PLAYING); play(enter, 2500, 50, 100);} else {change_state(SETTINGS); play(enter, 2500, 50, 100);} sleep(200);}
    if(button(UP) && !play_selected) {play_selected = true; play(blip, 1800, 30, 100);}
    if(button(DOWN) && play_selected) {play_selected = false; play(blip, 1800, 30, 100);}

  } else if (state == SETTINGS){

    if(button(A))  {
      if(easy_selected){
        difficulty=300; 
      } else if (medium_selected){
        difficulty=220;
      } else if (hard_selected){
        difficulty=150;
      }

      sleep(200);
      change_state(MENU); 
      play(enter, 2500, 50, 100);      
    }

    if(pressed(UP) && !easy_selected) {
      if(medium_selected){
        easy_selected = true; 
        medium_selected = false;
        
      } else if (hard_selected){
        medium_selected = true;
        hard_selected = false;
      }

      play(blip, 1800, 30, 100);
      
    }

    if(pressed(DOWN) && !hard_selected) {
      if(medium_selected){
        hard_selected = true; 
        medium_selected = false;
        
      } else if (easy_selected){
        medium_selected = true;
        easy_selected = false;
      }

      play(blip, 1800, 30, 100);
    }

  } else if(state == PLAYING) {

    if (is_game_set) {

      play_song(tick);

      //Colocar frutas y enemigos

      if (tick % difficulty == 0) {
        rotten_fruits.push_back(place_fruit());
      }

      if (tick % difficulty - 100 == 0) {
        fruits.push_back(place_fruit());
      }

      if(tick % 30 == 0) {
        if(state != GAME_OVER) {
          for(auto & fruit: fruits){
            fruit.move();
          }
          for(auto & fruit: rotten_fruits){
            fruit.move();
          }
        }
      }
      
      if(tick % 10 == 0) {

        vec_t next = player.next();
        int sprite_size = 1;

        for(auto & bullet: bullets){
          bullet.move();
        }
      
        if(button(LEFT) && in_bounds_left(next))  {player.dir.x = -1; player.dir.y =  0; player.move();}
        if(button(RIGHT) && in_bounds_right(next)) {player.dir.x =  1; player.dir.y =  0; player.move();}
        if(button(A)) {bullets.push_back(place_bullet());}
        if(button(B)) {is_game_set = false; change_state(MENU);}

        //Revisar colisiones de frutas con el jugador
        for (auto & fruit: fruits) {
          if(intersects(next.x, next.y, sprite_size, sprite_size, fruit.next().x, fruit.next().y, sprite_size, sprite_size)) {
            player.score++;
            play(blip, 1000, 30, 100);
            fruit = Fruit(std::rand() % bounds.x, 100);
          }
          if(wall_hit_check(fruit.pos) || fruit.pos.y >= 18) {

            fruits.pop_front();
                
          }
        }
        //Revisar colisiones de una bala con un enemigo
        for (auto & bullet: bullets) {
          for (auto & rotten_fruit: rotten_fruits){
            if(intersects(bullet.pos.x, bullet.pos.y, sprite_size, sprite_size, rotten_fruit.pos.x, rotten_fruit.pos.y, sprite_size, sprite_size)) {
              player.lives++;
              play(blip, 500, 30, 100);
              rotten_fruit = Fruit(std::rand() % bounds.x, 100);
              bullet = Bullet(std::rand() % bounds.x, -10);
            }
          }
          
          if(wall_hit_check(bullet.pos) || bullet.pos.y <= 5) {
            bullets.pop_front();      
          }
        }
        //Revisar si un enemigo pasó la barrera inferior
        for (auto & rotten_fruit: rotten_fruits){
          if(wall_hit_check(rotten_fruit.pos) || rotten_fruit.pos.y >= 100) {
            rotten_fruits.pop_front();
            if (player.lives == 0) {
              state = GAME_OVER;
            } else {
              player.lives-=1; 
            }       
          }
        }
        
      }
    } else {
      init();
    }
  } else if (state == GAME_OVER) {
    if(pressed(B)) {
      is_game_set = false;
      state = MENU;
    }
  } 
}

/*
Funciones de dibujado
*/

//Etiqueta de Puntaje y Vidas

void label(std::string s) {

  blend(ALPHA);
  pen(1, 2, 3, 8);
  frect(0, 11, SCREEN->w, 15);
  pen(0, 0, 0);
  text("Score: " + s, 3, 16);
  pen(15, 15, 15);
  text("Score: " + s, 2, 15);

  for (int i = player.lives; i > 0; i--){
    text("\\spr064", SCREEN->w - 10 * i, 15);
  }
  
}

//Estilos de texto

auto split_text = [](std::string m, color_t c1, color_t c2, int32_t x, int32_t y) {
        blend(MASK);
        pen(c1);
        clip(0, y, 120, 4);
        text(m, x, y);
        pen(c2);
        clip(0, y + 4, 120, 4);
        text(m, x, y);
        clip();
      };

auto glow_text = [](std::string m, color_t c, int32_t x, int32_t y) {
        blend(PEN);
        pen(c);
        text(m, x - 1, y - 1); text(m, x    , y - 1); text(m, x + 1, y - 1);
        text(m, x - 1, y    ); text(m, x    , y    ); text(m, x + 1, y    );
        text(m, x - 1, y + 1); text(m, x    , y + 1); text(m, x + 1, y + 1);
        blend(PEN);
        pen(0, 0, 0);
        text(m, x, y);
      };

auto glitch_text = [](std::string m, color_t c, int32_t x, int32_t y) {
        blend(MASK);
        pen(c);
        for(uint8_t i = 0; i < 8; i++) {
          clip(0, y + i, 120, 1);
          int8_t o = 0;
          if(std::rand() % 8 == 0) {
            o = (std::rand() % 9) - 4;
          }
          text(m, x + o, y);
        }
        clip();
      };
      
//Constantes de dibujado
float hue = 0.0f, hue_step = 1.0f / 7.0f;
//Colores
color_t c1, c2, c3, c4;

void draw(uint32_t tick) {

  int menu_options_pos_x = 45;
  int menu_options_pos_y = 68;
  int menu_options_pos_main_y = 60;

  hue += 0.01f;

  //Cálculo de colores
  float hue2 = hue + 0.5;
  if(hue2 > 1.0f) hue2 -= 1.0f;
  c1 = hsv(hue + (hue_step * 4), 0.8f, 1.0f); c2 = hsv(hue + (hue_step * 4) + 0.1f, 0.4f, 0.6f);
  int32_t g = 1 + ((sin(time() / 250.0f) + 1.0f) * 2.0f);
  c3 = hsv(hue + (hue_step * 5), 0.8f, 1.0f, 0.2f);
  c4 = hsv(hue + (hue_step * 6), 0.8f, 1.0f);


  if(state == MENU){
    pen(0, 0, 0);
    clear();
      
    split_text("Fruit Invaders \\spr160", c1, c2, 17, 30);
    glitch_text("Play", (255, 255, 255), menu_options_pos_x, menu_options_pos_main_y);
    glitch_text("Settings", (255, 255, 255), menu_options_pos_x - 7, menu_options_pos_main_y + 20);

    if (play_selected){
      pen(0, 0, 0);
      frect(menu_options_pos_x - 17, menu_options_pos_main_y + 21, 5, 5);
      pen((255, 255, 255));
      frect(menu_options_pos_x - 10, menu_options_pos_main_y + 1, 5, 5);
    } else {
      pen(0, 0, 0);
      frect(menu_options_pos_x - 10, menu_options_pos_main_y + 1, 5, 5);
      pen((255, 255, 255)); 
      frect(menu_options_pos_x - 17, menu_options_pos_main_y + 21, 5, 5);
    }
    
  } else if(state == SETTINGS){
    pen(0, 0, 0);
    clear();

    split_text("Settings", c1, c2, menu_options_pos_x - 10, 10);
    glow_text("Play Mode", c1, menu_options_pos_x - 15, 25);
    glitch_text("Easy", c4, menu_options_pos_x, menu_options_pos_y - 20);
    glitch_text("Medium", c4, menu_options_pos_x - 7, menu_options_pos_y);
    glitch_text("Hard", c4, menu_options_pos_x, menu_options_pos_y + 20);
    glow_text("2022 Mauri y Ale", c1, 20, 110);

    if (easy_selected){
      //erasing medium
      pen(0, 0, 0);
      frect(menu_options_pos_x - 17, menu_options_pos_y + 1, 5, 5);
      //erasing hard
      pen(0, 0, 0);
      frect(menu_options_pos_x - 10, menu_options_pos_y + 21, 5, 5);
       //drawing easy
      pen(c4); 
      frect(menu_options_pos_x - 10, menu_options_pos_y - 19, 5, 5);
    } else if (medium_selected){
      //erasing easy
      pen(0, 0, 0);
      frect(menu_options_pos_x - 10, menu_options_pos_y - 19, 5, 5);
      //erasing hard
      pen(0, 0, 0);
      frect(menu_options_pos_x - 10, menu_options_pos_y + 21, 5, 5);
      //drawing medium
      pen(c4); 
      frect(menu_options_pos_x - 17, menu_options_pos_y + 1, 5, 5);
    } else {
      //erasing easy
      pen(0, 0, 0);
      frect(menu_options_pos_x - 10, menu_options_pos_y - 19, 5, 5);
      //erasing medium
      pen(0, 0, 0);
      frect(menu_options_pos_x - 17, menu_options_pos_y + 21, 5, 5);
      //drawing hard
      pen(c4); 
      frect(menu_options_pos_x - 10, menu_options_pos_y + 21, 5, 5);
    }

  } else {

    //Dibujar el cielo azul  
    pen(10, 25, 255);
    clear();

    pen(255,255,255);

    //Dibujo de nubes
    fcircle(20, 43, 2);
    fellipse(20, 45, 8, 3);

    fcircle(90, 53, 2);
    fellipse(90, 55, 8, 3);

    fcircle(45, 68, 2);
    fellipse(45, 70, 8, 3);

    fcircle(60, 39, 2);
    fellipse(60, 40, 6, 2);

    fcircle(80, 88, 2);
    fellipse(80, 90, 8, 3);

    fcircle(20, 89, 2);
    fellipse(20, 90, 6, 2);

    fcircle(107, 74, 2);
    fellipse(107, 75, 6, 2);

    pen(0, 0, 0, 4);

    //Dibujo de etiqueta superior
    pen(15, 15, 15);
    frect(0, 0, SCREEN->w, 11);
    pen(0, 0, 0);
    text("Catch'em All!", 2, 2);

    std::string lvl;
    if (difficulty == 300){
      lvl = "Easy \\spr140";
    } else if (difficulty == 220){
      lvl = "Mid \\spr141";
    } else {
      lvl = "Hard \\spr142";
    }

    text(lvl, 80, 2);
    label(str(player.score));

    //Dibujo de límites
    rect(2, 27, 116, 90);
    rect(2, 29, 116, 90);

    //Dibujo de frutas
    for (auto fruit: fruits){
      sprite(fruit_selected, transform(fruit.pos).x + 3, transform(fruit.pos).y + 3 );
    }
    //Dibujo de enemigos
    for (auto fruit: rotten_fruits){
      sprite(EYEBALL, transform(fruit.pos).x + 3, transform(fruit.pos).y + 3 );
    }
    //Dibujo de balas
    for (auto bullet: bullets){
      pen(255, 255, 255);
      frect(transform(bullet.pos).x + 3, transform(bullet.pos).y + 3, 1, 1);
    }

    //Dibujo de jugador (Parpadeo si hay GAME_OVER)
    bool flash = ((time() / 250) % 2) == 0;
    if(state == PLAYING || (state == GAME_OVER && flash)) {
        vec_t p = transform(player.pos);
        sprite(PILOT_GOGGLES, p.x, p.y);
    }

    //Dibujo de titulo Game Over
    if (state == GAME_OVER){
      glow_text("GAME OVER", c3, 33 , menu_options_pos_y - 10);
      glow_text("Press B to Return", c3, 18, menu_options_pos_y + 10);
    }
  }
}
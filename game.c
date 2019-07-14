#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

#define WIDTH 433
#define HEIGHT 650
#define IMGS 30
#define ELEM 100
#define SCR 10
#define BLC_L 7
#define BLC_C 5
#define SLC_BLC BLC_L*BLC_C

struct IMAGE {
    char title[100];
    ALLEGRO_BITMAP *bitmap;
};

struct EVENT {
    int event_type;
    int value_int;
    char value_char[100];
};

struct BLOCK {
    char title[100];
    float x, y, width, height;
    int value, i, j;
    bool highlight;
    bool usingHighlightCountdown;
    ALLEGRO_BITMAP *image;
    ALLEGRO_COLOR color;
    struct BLOCK *blockConnected;
};

struct UI_ELEMENT {
    char title[100];
    ALLEGRO_BITMAP *image;
    float x, y, width, height;
    struct EVENT event;
    bool draw;
};

struct UI {
    ALLEGRO_TIMER *timer;
    ALLEGRO_EVENT_QUEUE *queue;
    ALLEGRO_COLOR background;
    ALLEGRO_BITMAP *background_image;
    int background_type;
    struct UI_ELEMENT elements[ELEM];
};

struct ASSETS {
    struct IMAGE imageList[IMGS];
};

struct GAME {
    int state;
    int score;
    int min_block;
    int max_block;
    bool sound;
    bool scoreOnRanking;
    ALLEGRO_TIMER *game_timer;
    ALLEGRO_TIMER *highlight_countdown;
    ALLEGRO_DISPLAY *disp;
    ALLEGRO_FONT *font_ranking;
    ALLEGRO_FONT *font_playing;
    ALLEGRO_SAMPLE *music;
    ALLEGRO_SAMPLE_ID music_id;
    FILE *scores;
    struct BLOCK blocks[BLC_L][BLC_C];
    struct BLOCK *selectedBlocks[SLC_BLC];
    struct UI ui;
    struct ASSETS assets;
};

enum STATE { MENU, RANKING, PLAYING, PAUSE, RESTART, GAMEOVER };

enum BG_TYPE { COLOR, IMAGE };

enum EVENTS { CHANGE_STATE, DISABLE_SOUND, MOVEMENT_HELP };

void initTest(bool func, const char *description) {
    if (func)
        return;

    printf("ERRO NA INICIALIZAÇÃO: %s\n", description);
    exit(1);
}

void loadImages(struct IMAGE *imageList, int state) {
    for (int i = 0; i < IMGS; i++) {
        strcpy(imageList[i].title, "");
        imageList[i].bitmap = NULL;
    }

   switch(state) {
       case MENU:
        strcpy(imageList[0].title, "logo");
        imageList[0].bitmap = al_load_bitmap("assets/img/logo.png");

        strcpy(imageList[1].title, "play_btn");
        imageList[1].bitmap = al_load_bitmap("assets/img/play_btn.png");

        strcpy(imageList[2].title, "ranking_btn");
        imageList[2].bitmap = al_load_bitmap("assets/img/ranking_btn.png");

        strcpy(imageList[3].title, "sound-off_btn");
        imageList[3].bitmap = al_load_bitmap("assets/img/sound-off_btn.png");

        strcpy(imageList[4].title, "sound-on_btn");
        imageList[4].bitmap = al_load_bitmap("assets/img/sound-on_btn.png");
       break;

       case RANKING:
        strcpy(imageList[0].title, "star");
        imageList[0].bitmap = al_load_bitmap("assets/img/star.png");

        strcpy(imageList[1].title, "return_btn");
        imageList[1].bitmap = al_load_bitmap("assets/img/return_btn.png");
       break;

       case PLAYING:
        strcpy(imageList[0].title, "1k_block");
        imageList[0].bitmap = al_load_bitmap("assets/img/1k_block.png");

        strcpy(imageList[1].title, "2_block");
        imageList[1].bitmap = al_load_bitmap("assets/img/2_block.png");

        strcpy(imageList[2].title, "4_block");
        imageList[2].bitmap = al_load_bitmap("assets/img/4_block.png");

        strcpy(imageList[3].title, "4k_block");
        imageList[3].bitmap = al_load_bitmap("assets/img/4k_block.png");

        strcpy(imageList[4].title, "8_block");
        imageList[4].bitmap = al_load_bitmap("assets/img/8_block.png");

        strcpy(imageList[5].title, "8k_block");
        imageList[5].bitmap = al_load_bitmap("assets/img/8k_block.png");

        strcpy(imageList[6].title, "16_block");
        imageList[6].bitmap = al_load_bitmap("assets/img/16_block.png");

        strcpy(imageList[7].title, "16k_block");
        imageList[7].bitmap = al_load_bitmap("assets/img/16k_block.png");

        strcpy(imageList[8].title, "32_block");
        imageList[8].bitmap = al_load_bitmap("assets/img/32_block.png");

        strcpy(imageList[9].title, "32k_block");
        imageList[9].bitmap = al_load_bitmap("assets/img/32k_block.png");

        strcpy(imageList[10].title, "64_block");
        imageList[10].bitmap = al_load_bitmap("assets/img/64_block.png");

        strcpy(imageList[11].title, "64k_block");
        imageList[11].bitmap = al_load_bitmap("assets/img/64k_block.png");

        strcpy(imageList[12].title, "128_block");
        imageList[12].bitmap = al_load_bitmap("assets/img/128_block.png");

        strcpy(imageList[13].title, "128k_block");
        imageList[13].bitmap = al_load_bitmap("assets/img/128k_block.png");

        strcpy(imageList[14].title, "256_block");
        imageList[14].bitmap = al_load_bitmap("assets/img/256_block.png");

        strcpy(imageList[15].title, "256k_block");
        imageList[15].bitmap = al_load_bitmap("assets/img/256k_block.png");

        strcpy(imageList[16].title, "512_block");
        imageList[16].bitmap = al_load_bitmap("assets/img/512_block.png");

        strcpy(imageList[17].title, "512k_block");
        imageList[17].bitmap = al_load_bitmap("assets/img/512k_block.png");

        strcpy(imageList[18].title, "background");
        imageList[18].bitmap = al_load_bitmap("assets/img/background.png");

        strcpy(imageList[19].title, "help_btn");
        imageList[19].bitmap = al_load_bitmap("assets/img/help_btn.png");

        strcpy(imageList[20].title, "score_box");
        imageList[20].bitmap = al_load_bitmap("assets/img/score_box.png");

        strcpy(imageList[21].title, "time_box");
        imageList[21].bitmap = al_load_bitmap("assets/img/time_box.png");
        
        strcpy(imageList[22].title, "pause_btn");
        imageList[22].bitmap = al_load_bitmap("assets/img/pause_btn.png");
        
        strcpy(imageList[23].title, "2k_block");
        imageList[23].bitmap = al_load_bitmap("assets/img/2k_block.png");
       break;

       case PAUSE:
        strcpy(imageList[0].title, "resume_btn");
        imageList[0].bitmap = al_load_bitmap("assets/img/resume_btn.png");

        strcpy(imageList[1].title, "pause-sound-off_btn");
        imageList[1].bitmap = al_load_bitmap("assets/img/pause-sound-off_btn.png");

        strcpy(imageList[2].title, "pause-sound-on_btn");
        imageList[2].bitmap = al_load_bitmap("assets/img/pause-sound-on_btn.png");

        strcpy(imageList[3].title, "restart_btn");
        imageList[3].bitmap = al_load_bitmap("assets/img/restart_btn.png");
       break;

       case GAMEOVER:
       strcpy(imageList[0].title, "gameover");
       imageList[0].bitmap = al_load_bitmap("assets/img/gameover.png");
       break;
   }
}

void unloadImages(struct IMAGE *imageList) {
    for (int i = 0; i < IMGS; i++) {
        al_destroy_bitmap(imageList[i].bitmap);
    }
}

ALLEGRO_BITMAP * getImage(struct IMAGE *imageList, char title[]) {
    for (int i = 0; i < IMGS; i++) {
        if (!strcmp(imageList[i].title, title))
            return imageList[i].bitmap;
    }
}

int coordinatesInsideEl(float el_x, float el_y, float el_width, float el_height, float x, float y) {
    if (x >= el_x && x <= el_x + el_width && y >= el_y && y <= el_y + el_height)
        return true;
    else
        return false;        
}

void checkMouseOverBlocks(int x, int y, int *ptr, struct GAME *game) {
    for (int i = 0; i < BLC_L; i++) {
        for (int j = 0; j < BLC_C; j++) {
            struct BLOCK *block = &game->blocks[i][j];

            bool clicked_block = coordinatesInsideEl(block->x - block->width / 2, block->y - block->height / 2, block->width, block->height, x, y);
            
            if (clicked_block) {
                ptr[0] = i;
                ptr[1] = j;
                block->highlight = true;
            } else {
                block->highlight = false;
            }
        }
    }
}

bool availableMovementExists(struct GAME *game, int i, int j) {
    if (game->blocks[i][j].value == game->blocks[i][j + 1].value && j + 1 < BLC_C || 
    game->blocks[i][j].value == game->blocks[i + 1][j].value && i + 1 < BLC_L || 
    game->blocks[i][j].value == game->blocks[i + 1][j + 1].value && j + 1 < BLC_C && i + 1 < BLC_L || 
    game->blocks[i][j].value == game->blocks[i][j - 1].value && j - 1 > -1 || 
    game->blocks[i][j].value == game->blocks[i - 1][j].value && j + 1 < -1 || 
    game->blocks[i][j].value == game->blocks[i - 1][j - 1].value && j - 1 < -1 && i - 1 > -1 || 
    game->blocks[i][j].value == game->blocks[i - 1][j + 1].value && j + 1 < BLC_C && i - 1 < -1 || 
    game->blocks[i][j].value == game->blocks[i + 1][j - 1].value && j - 1 > -1 && i + 1 < BLC_L) {
        return true;
    } else {
        return false;
    }
}

void removeHighlight(struct GAME *game) {
    for (int i = 0; i < BLC_L; i++) {
        for (int j = 0; j < BLC_C; j++) {
            if (game->blocks[i][j].usingHighlightCountdown && al_get_timer_count(game->highlight_countdown) > (int64_t)2) {
                al_stop_timer(game->highlight_countdown);
                game->blocks[i][j].highlight = false;
                game->blocks[i][j].usingHighlightCountdown = false;
            } else if (!game->blocks[i][j].usingHighlightCountdown) {
                game->blocks[i][j].highlight = false;
            }
        }
    }
}

void highlightBlock(struct GAME *game) {
    bool availableMovement = false;
    for (int i = 0; i < BLC_L; i++) {
        for (int j = 0; j < BLC_C; j++) {
            availableMovement = availableMovementExists(game, i, j);
            if (availableMovement) {
                game->blocks[i][j].highlight = true;
                game->blocks[i][j].usingHighlightCountdown = true;
                al_set_timer_count(game->highlight_countdown, (int64_t)0);
                al_start_timer(game->highlight_countdown);
                return;
            }
        }
    }
}

int randomBlockValue(int min, int max) {
    return pow(2, rand() % (max - min + 1) + min);
}

void setBlockTitle(struct BLOCK *block) {
    if (block->value < 1000) {
        sprintf(block->title, "%d", block->value);
        strcat(block->title, "_block");
    } else {
        sprintf(block->title, "%d", block->value / 1000);
        strcat(block->title, "k_block");
    }
}

int calculateScore(struct GAME *game) {
    int score = 0;
    for (int i = 0; game->selectedBlocks[i] != NULL; i++) {
        score += game->selectedBlocks[i]->value;
    }

    return score;
}

void cleanSelectedBlocks(struct GAME *game) {
    for (int i = 0; i < SLC_BLC; i++) {
        game->selectedBlocks[i] = NULL;
    }
}

void removeSelectedBlocks(struct GAME *game) {
    int block_value = calculateScore(game);
    int i;
    for (i = 0; game->selectedBlocks[i + 1] != NULL; i++) {
        game->selectedBlocks[i]->value = randomBlockValue(game->min_block, game->max_block);
        setBlockTitle(&*game->selectedBlocks[i]);
        game->selectedBlocks[i]->image = getImage(game->assets.imageList, game->selectedBlocks[i]->title);
        game->selectedBlocks[i]->width = al_get_bitmap_width(game->selectedBlocks[i]->image);
        game->selectedBlocks[i]->height = al_get_bitmap_height(game->selectedBlocks[i]->image);
        game->selectedBlocks[i]->color = al_get_pixel(game->selectedBlocks[i]->image, 10, 5);
        game->selectedBlocks[i]->blockConnected = NULL;
    }

    int c = 0, d = block_value, v;

    while (d != 1) {
        d = d / 2;
        c++;
    }
    v = pow(2, c);

    if (c > game->max_block + 2) {
        game->max_block++;
        game->min_block++;
    }

    int ex;
    switch (v) {
        case 65536:
        v = 64000;
        ex = 65536 - 64000;
        break;

        case 131072:
        v = 128000;
        ex = 131072 - 128000;
        break;

        case 262144:
        v = 256000;
        ex = 262144 - 256000;
        break;

        default:
        ex = 0;
        break;
    }

    game->selectedBlocks[i]->value = v;
    setBlockTitle(&*game->selectedBlocks[i]);
    game->selectedBlocks[i]->value = v + ex;
    game->selectedBlocks[i]->image = getImage(game->assets.imageList, game->selectedBlocks[i]->title);
    game->selectedBlocks[i]->width = al_get_bitmap_width(game->selectedBlocks[i]->image);
    game->selectedBlocks[i]->height = al_get_bitmap_height(game->selectedBlocks[i]->image);
    game->selectedBlocks[i]->color = al_get_pixel(game->selectedBlocks[i]->image, 10, 5);
    game->selectedBlocks[i]->blockConnected = NULL;

}

void initBlock(int i, int j, struct BLOCK *block, struct GAME *game) {
    block->i = i;
    block->j = j;
    block->x = j * 75 + 65;
    block->y = i * 75 + 100;
    block->value = randomBlockValue(game->min_block, game->max_block);
    setBlockTitle(&*block);
    block->image = getImage(game->assets.imageList, block->title);
    block->width = al_get_bitmap_width(block->image);
    block->height = al_get_bitmap_height(block->image);
    block->color = al_get_pixel(block->image, 10, 5);
    block->highlight = false;
    block->blockConnected = NULL;
} 

void initBlockArray(struct GAME *game) {
    srand(time(NULL));
    for (int i = 0; i < BLC_L; i++) {
        for (int j = 0; j < BLC_C; j++) {
            initBlock(i, j, &game->blocks[i][j], game);
        }
    }
}

void drawBlocks(struct GAME *game) {
    for (int i = 0; i < BLC_L; i++) {
        for (int j = 0; j < BLC_C; j++) {
            struct BLOCK *block = &game->blocks[i][j];
            struct BLOCK *otherBlock = block->blockConnected;
            if (otherBlock && block != otherBlock) {
                al_draw_line(block->x, block->y, otherBlock->x, otherBlock->y , block->color, 5);
            }
        }
    }

    for (int i = 0; i < BLC_L; i++) {
        for (int j = 0; j < BLC_C; j++) {
            struct BLOCK *block = &game->blocks[i][j];;

            if (block->highlight) {
                al_draw_scaled_bitmap(block->image, 0, 0, block->width, block->height, block->x - (block->width * 1.2) / 2, block->y - (block->height * 1.2) / 2, block->width * 1.2, block->height * 1.2, 0);
            } else {
                al_draw_bitmap(block->image, block->x - block->width / 2, block->y - block->height / 2, 0);
            }

        }
    }
    
}

void drawTime(struct GAME *game) {
    char output[5];
    char min_str[3];
    char sec_str[3];
    int time = al_get_timer_count(game->game_timer);
    int min_int = time / 60;
    int sec_int = time % 60;

    sprintf(sec_str, "%02d", sec_int);
    sprintf(min_str, "%02d", min_int);
    
    output[0] = min_str[0];
    output[1] = min_str[1];
    output[2] = ':';
    output[3] = sec_str[0];
    output[4] = sec_str[1];
    
    al_draw_text(game->font_playing, al_map_rgb(255, 255, 255), game->ui.elements[0].width / 2 + game->ui.elements[0].x - 30, game->ui.elements[0].height / 2 - (al_get_font_line_height(game->font_playing) / 2) - 2, 0, output);
}

void drawScore(struct GAME *game) {
    char str[12];
    sprintf(str, "%d", game->score);
    al_draw_text(game->font_playing, al_map_rgb(255, 255, 255), game->ui.elements[1].width / 2 + game->ui.elements[1].x - al_get_text_width(game->font_playing, str) / 2, game->ui.elements[1].height / 2 - (al_get_font_line_height(game->font_playing) / 2) - 2, 0, str);
}

void orderRanking(int arr[]) {
    int aux;
    for (int i = 0; i < SCR ; i++) {
        for (int j = 0; j < SCR - 1; j++) {
            if (arr[j] < arr[j + 1]) {
                aux = arr[j + 1];
                arr[j + 1] = arr[j];
                arr[j] = aux;
            }
        }
    }
}

void drawRanking(int playerScore, struct GAME *game) {
    char text[256];
    int scores[10];
    char *token;
    char output[12];
    char str[11];
    game->scores = fopen("assets/data/scores.txt", "r+");

    fscanf(game->scores, "%s", text);

    token = strtok(text, ",");

    int i = 0;
    while (token != NULL) {
        scores[i] = atoi(token);
        token = strtok(NULL, ",");
        i++;
    }

    bool aux = true;
    if (playerScore < 1) {
        orderRanking(scores);
            
        for (i = 0; i < SCR; i++) {
            int spacing = 0;
            sprintf(output, "%d", i + 1);
            spacing = al_get_text_width(game->font_ranking, output);
            strcat(output, "       ");
            sprintf(str, "%03d", scores[i]);
            strcat(output, str);
            al_draw_text(game->font_ranking, al_map_rgb(255, 255, 255), WIDTH / 2 - 50 - spacing, (HEIGHT / 20) * i + 200, 0, output);
        }
        
    } else {
        orderRanking(scores);
        if (playerScore > scores[SCR - 1] && !game->scoreOnRanking) {
            scores[SCR - 1] = playerScore;
            game->scoreOnRanking = true;
        }
        orderRanking(scores);

        for (i = 0; i < SCR; i++) {
            int spacing = 0;
            sprintf(output, "%d", i + 1);
            spacing = al_get_text_width(game->font_ranking, output);
            strcat(output, "       ");
            sprintf(str, "%03d", scores[i]);
            strcat(output, str);
            if (scores[i] == playerScore && aux) {
                al_draw_text(game->font_ranking, al_map_rgb(239, 206, 74), WIDTH / 2 - 50 - spacing, (HEIGHT / 20) * i + 200, 0, output);
            } else {
                al_draw_text(game->font_ranking, al_map_rgb(255, 255, 255), WIDTH / 2 - 50 - spacing, (HEIGHT / 20) * i + 200, 0, output);
            }
        }
    }

    freopen("assets/data/scores.txt", "w", game->scores);

    strcpy(text, "");
    for (i = 0; i < SCR - 1; i++) {
        sprintf(str, "%d", scores[i]);
        strcat(text, str);
        strcat(text, ",");
    }
    sprintf(str, "%d", scores[SCR - 1]);
    strcat(text, str);

    fprintf(game->scores, "%s", text);

    fclose(game->scores);
}

void checkSelectedBlocks(struct GAME *game) {

    if (game->selectedBlocks[1] != NULL) {
        game->score += calculateScore(game);
        removeSelectedBlocks(game);
        cleanSelectedBlocks(game);
    }
}

void initElements(struct UI_ELEMENT *elements, struct IMAGE *imageList, int state, struct GAME *game) {
    for (int i = 0; i < ELEM; i++) {
        elements[i].image = NULL;
        elements[i].width = 0;
        elements[i].height = 0;
        elements[i].x = 0;
        elements[i].y = 0;
        elements[i].draw = false;
    }

    switch (state) {
        case MENU:
            strcpy(elements[0].title, "logo");
            elements[0].image = getImage(imageList, elements[0].title);
            elements[0].width = al_get_bitmap_width(elements[0].image);
            elements[0].height = al_get_bitmap_height(elements[0].image);
            elements[0].x = (WIDTH / 2) - (elements[0].width / 2);
            elements[0].y = 100;
            elements[0].event.event_type = -1;
            elements[0].event.value_int = -1;
            strcpy(elements[0].event.value_char, "");
            elements[0].draw = 1;

            strcpy(elements[1].title, "play_btn");
            elements[1].image = getImage(imageList, elements[1].title);
            elements[1].width = al_get_bitmap_width(elements[1].image);
            elements[1].height = al_get_bitmap_height(elements[1].image);
            elements[1].x = (WIDTH / 2) - (elements[1].width / 2);
            elements[1].y = 250;
            elements[1].event.event_type = CHANGE_STATE;
            elements[1].event.value_int = PLAYING;
            strcpy(elements[1].event.value_char, "");
            elements[1].draw = 1;

            strcpy(elements[2].title, "ranking_btn");
            elements[2].image = getImage(imageList, elements[2].title);
            elements[2].width = al_get_bitmap_width(elements[2].image);
            elements[2].height = al_get_bitmap_height(elements[2].image);
            elements[2].x = (WIDTH / 2) - (elements[2].width / 2);
            elements[2].y = 350;
            elements[2].event.event_type = CHANGE_STATE;
            elements[2].event.value_int = RANKING;
            strcpy(elements[2].event.value_char, "");
            elements[2].draw = 1;

            if (game->sound) {
                strcpy(elements[3].title, "sound-on_btn");
                strcpy(elements[3].event.value_char, "sound-off_btn");
            } else {
                strcpy(elements[3].title, "sound-off_btn");
                strcpy(elements[3].event.value_char, "sound-on_btn");
            }
            elements[3].image = getImage(imageList, elements[3].title);
            elements[3].width = al_get_bitmap_width(elements[3].image);
            elements[3].height = al_get_bitmap_height(elements[3].image);
            elements[3].x = (WIDTH / 2) - (elements[3].width / 2);
            elements[3].y = 450;
            elements[3].event.event_type = DISABLE_SOUND;
            elements[3].event.value_int = -1;
            elements[3].draw = 1;
        break;

        case RANKING:
            strcpy(elements[0].title, "star");
            elements[0].image = getImage(imageList, elements[0].title);
            elements[0].width = al_get_bitmap_width(elements[0].image);
            elements[0].height = al_get_bitmap_height(elements[0].image);
            elements[0].x = (WIDTH / 2) - (elements[0].width / 2);
            elements[0].y = 50;
            elements[0].event.event_type = -1;
            elements[0].event.value_int = -1;
            strcpy(elements[0].event.value_char, "");
            elements[0].draw = 1;

            strcpy(elements[1].title, "return_btn");
            elements[1].image = getImage(imageList, elements[1].title);
            elements[1].width = al_get_bitmap_width(elements[1].image);
            elements[1].height = al_get_bitmap_height(elements[1].image);
            elements[1].x = (WIDTH / 2) - (elements[1].width / 2);
            elements[1].y = HEIGHT - 100;
            elements[1].event.event_type = CHANGE_STATE;
            elements[1].event.value_int = MENU;
            strcpy(elements[1].event.value_char, "");
            elements[1].draw = 1;
        break;

        case PLAYING:
            strcpy(elements[0].title, "time_box");
            elements[0].image = getImage(imageList, elements[0].title);
            elements[0].width = al_get_bitmap_width(elements[0].image);
            elements[0].height = al_get_bitmap_height(elements[0].image);
            elements[0].x = 25;
            elements[0].y = 0;
            elements[0].event.event_type = -1;
            elements[0].event.value_int = -1;
            strcpy(elements[0].event.value_char, "");
            elements[0].draw = 1;

            strcpy(elements[1].title, "score_box");
            elements[1].image = getImage(imageList, elements[1].title);
            elements[1].width = al_get_bitmap_width(elements[1].image);
            elements[1].height = al_get_bitmap_height(elements[1].image);
            elements[1].x = (WIDTH / 2) - (elements[1].width / 2);
            elements[1].y = 0;
            elements[1].event.event_type = -1;
            elements[1].event.value_int = -1;
            strcpy(elements[1].event.value_char, "");
            elements[1].draw = 1;

            strcpy(elements[2].title, "pause_btn");
            elements[2].image = getImage(imageList, elements[2].title);
            elements[2].width = al_get_bitmap_width(elements[2].image);
            elements[2].height = al_get_bitmap_height(elements[2].image);
            elements[2].x = WIDTH - 25 - elements[2].width;
            elements[2].y = 0;
            elements[2].event.event_type = CHANGE_STATE;
            elements[2].event.value_int = PAUSE;
            strcpy(elements[2].event.value_char, "");
            elements[2].draw = 1;

            strcpy(elements[3].title, "help_btn");
            elements[3].image = getImage(imageList, elements[3].title);
            elements[3].width = al_get_bitmap_width(elements[3].image);
            elements[3].height = al_get_bitmap_height(elements[3].image);
            elements[3].x = (WIDTH / 2) - (elements[1].width / 2);
            elements[3].y = HEIGHT - 50;
            elements[3].event.event_type = MOVEMENT_HELP;
            elements[3].event.value_int = -1;
            strcpy(elements[3].event.value_char, "");
            elements[3].draw = 1;
        break;

        case PAUSE:
            strcpy(elements[0].title, "restart_btn");
            elements[0].image = getImage(imageList, elements[0].title);
            elements[0].width = al_get_bitmap_width(elements[0].image);
            elements[0].height = al_get_bitmap_height(elements[0].image);
            elements[0].x = 50;
            elements[0].y = (HEIGHT / 2) - (elements[0].height / 2);
            elements[0].event.event_type = CHANGE_STATE;
            elements[0].event.value_int = RESTART;
            strcpy(elements[0].event.value_char, "");
            elements[0].draw = 1;

            if (game->sound) {
                strcpy(elements[1].title, "pause-sound-on_btn");
                strcpy(elements[1].event.value_char, "pause-sound-off_btn");
            } else {
                strcpy(elements[1].title, "pause-sound-off_btn");
                strcpy(elements[1].event.value_char, "pause-sound-on_btn");
            }
            elements[1].image = getImage(imageList, elements[1].title);
            elements[1].width = al_get_bitmap_width(elements[1].image);
            elements[1].height = al_get_bitmap_height(elements[1].image);
            elements[1].x = (WIDTH / 2) - (elements[1].width / 2);
            elements[1].y = (HEIGHT / 2) - (elements[1].height / 2);
            elements[1].event.event_type = DISABLE_SOUND;
            elements[1].event.value_int = -1;
            elements[1].draw = 1;

            strcpy(elements[2].title, "resume_btn");
            elements[2].image = getImage(imageList, elements[2].title);
            elements[2].width = al_get_bitmap_width(elements[2].image);
            elements[2].height = al_get_bitmap_height(elements[2].image);
            elements[2].x = WIDTH - 50 - elements[2].width;
            elements[2].y = (HEIGHT / 2) - (elements[1].height / 2);
            elements[2].event.event_type = CHANGE_STATE;
            elements[2].event.value_int = PLAYING;
            strcpy(elements[2].event.value_char, "");
            elements[2].draw = 1;
        break;

        case GAMEOVER:
            strcpy(elements[0].title, "gameover");
            elements[0].image = getImage(imageList, elements[0].title);
            elements[0].width = al_get_bitmap_width(elements[0].image);
            elements[0].height = al_get_bitmap_height(elements[0].image);
            elements[0].x = WIDTH / 2 - elements[0].width / 2;
            elements[0].y = HEIGHT / 2 - elements[0].height / 2;
            elements[0].event.event_type = CHANGE_STATE;
            elements[0].event.value_int = RANKING;
            strcpy(elements[0].event.value_char, "");
            elements[0].draw = 1;
        break;
    }
}

void changeState(struct GAME *game, int newState) {
    switch (game->state) {
        case MENU:
        game->score = 0;
        break;

        case RANKING:
        al_stop_timer(game->game_timer);
        al_set_timer_count(game->game_timer, 0);
        break;

        case PLAYING:
        al_destroy_timer(game->highlight_countdown);
        break;

        case PAUSE:
        al_start_timer(game->game_timer);
        break;
    }

    int oldState = game->state;
    game->state = newState;
    unloadImages(game->assets.imageList);
    loadImages(game->assets.imageList, game->state);
    initElements(game->ui.elements, game->assets.imageList, game->state, game);

    switch (game->state) {
        case MENU:
        game->ui.background_type = COLOR;
        game->ui.background = al_map_rgb(251, 243, 188);
        break;

        case RANKING:
        game->ui.background = al_map_rgb(91, 192, 235);

        break;

        case PLAYING:
        game->scoreOnRanking = false;
        if (!(oldState == PAUSE)) {
            initBlockArray(game);
        } else {
            for (int i = 0; i < BLC_L; i++) {
                for (int j = 0; j < BLC_C; j++) {
                    struct BLOCK* block;
                    block = &game->blocks[i][j];

                    block->image = getImage(game->assets.imageList, block->title);
                }
            }
        }
        game->highlight_countdown = al_create_timer(1.0);
        al_set_timer_count(game->highlight_countdown, (int64_t)0);
        al_start_timer(game->game_timer);
        game->ui.background_type = IMAGE;
        game->ui.background_image = getImage(game->assets.imageList, "background");
        break;

        case PAUSE:
        al_stop_timer(game->game_timer);
        game->ui.background_type = COLOR;
        break;

        case RESTART:
        al_stop_timer(game->game_timer);
        al_set_timer_count(game->game_timer, (int64_t)0);
        game->score = 0;
        game->max_block = 3;
        game->min_block = 1;
        changeState(game, PLAYING);
        break;

        case GAMEOVER:
        game->ui.background_type = COLOR;
        al_stop_timer(game->game_timer);
        al_set_timer_count(game->game_timer, (int64_t)0);
        game->max_block = 3;
        game->min_block = 1;
        break;
    }
}

void checkGameover(struct GAME *game) {
    bool gameOver;
    for (int i = 0; i < BLC_L; i++) {
        for (int j = 0; j < BLC_C; j++) {
            gameOver = !availableMovementExists(game, i, j);
            
            if (!gameOver) 
                return;
            
        }
    }

    if (gameOver) {
        changeState(game, GAMEOVER);
    }
}

void handleEvent(struct EVENT *event, struct UI_ELEMENT *el, struct GAME *game) {
    char aux[100];
    switch (event->event_type) {
        case CHANGE_STATE:
            changeState(game, event->value_int);
            break;
        case DISABLE_SOUND:
            el->image = getImage(game->assets.imageList, event->value_char);

            if (game->sound) 
                al_stop_sample(&game->music_id);
            else
                al_play_sample(game->music, 0.4, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &game->music_id);

            game->sound = !game->sound;

            strcpy(aux, el->title);
            strcpy(el->title, event->value_char);
            strcpy(event->value_char, aux);
            break;
        case MOVEMENT_HELP:
            highlightBlock(game);
            break;
    }
}

void init(struct GAME *game) {
    initTest(al_init(), "allegro");
    initTest(al_install_keyboard(), "teclado");
    initTest(al_install_mouse(), "mouse");
    initTest(al_install_audio(), "audio");

    game->game_timer = al_create_timer(1.0);
    initTest(game->game_timer, "timer (tempo de jogo)");

    game->ui.timer = al_create_timer(1.0 / 30.0);
    initTest(game->ui.timer, "timer (frame)");

    game->highlight_countdown = al_create_timer(1.0);
    initTest(game->highlight_countdown, "timer de highlight");

    game->ui.queue = al_create_event_queue();
    initTest(game->ui.queue, "fila de eventos");

    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
    al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);

    game->disp = al_create_display(WIDTH, HEIGHT);
    initTest(game->disp, "tela");
    al_set_window_title(game->disp, "2248");

    initTest(al_init_font_addon(), "font addon");
    initTest(al_init_ttf_addon(), "ttf addon");

    game->font_ranking = al_load_font("assets/font/arcadeclassic.ttf", 28, 0);
    initTest(game->font_ranking, "fonte do ranking");

    game->font_playing = al_load_font("assets/font/open-sans.ttf", 24, 0);
    initTest(game->font_playing, "fonte principal");

    initTest(al_init_image_addon(), "image addon");

    initTest(al_init_primitives_addon(), "primitives");

    initTest(al_init_acodec_addon(), "codecs de audio");

    game->music = al_load_sample("assets/sound/song17.wav");
    initTest(game->music, "musica");
    game->sound = true;

    al_reserve_samples(1);
    al_play_sample(game->music, 0.4, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &game->music_id);

    al_register_event_source(game->ui.queue, al_get_keyboard_event_source());
    al_register_event_source(game->ui.queue, al_get_mouse_event_source());
    al_register_event_source(game->ui.queue, al_get_display_event_source(game->disp));
    al_register_event_source(game->ui.queue, al_get_timer_event_source(game->ui.timer));

    game->score = 0;
    game->scoreOnRanking = false;
    game->ui.background = al_map_rgb(251, 243, 188);
    game->ui.background_type = COLOR;
    game->state = MENU;
    game->min_block = 1;
    game->max_block = 3;

    cleanSelectedBlocks(game);
    loadImages(game->assets.imageList, game->state);
    initElements(game->ui.elements, game->assets.imageList, game->state, game);
}

void draw(struct GAME *game) {
    if (game->ui.background_type == COLOR) {
        al_clear_to_color(game->ui.background);
    } else if (game->ui.background_type == IMAGE) {
        al_draw_bitmap(game->ui.background_image, 0, 0, 0);
    }

    for (int i = 0; i < ELEM; i++) {
        if (game->ui.elements[i].draw) {
            al_draw_bitmap(game->ui.elements[i].image, game->ui.elements[i].x, game->ui.elements[i].y, 0);
        }
    }


    switch (game->state) {
        case RANKING:
            drawRanking(game->score, game);
            break;

        case PLAYING:
            drawTime(game);
            drawScore(game);
            drawBlocks(game);
            break;
    }


    al_flip_display();
}

void gameLoop(struct GAME *game) {
    float x, y;
    bool done = false;
    bool redraw = true;
    bool isSelected;
    int i, I, j, J, c = 0;
    ALLEGRO_MOUSE_STATE mouseState;
    ALLEGRO_EVENT event;

    int prev[2] = { -1, -1 }, cur[2] = { -1, -1 };

    al_start_timer(game->ui.timer);
    while (1) {
        al_wait_for_event(game->ui.queue, &event);

        switch (event.type) {
            case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
                x = event.mouse.x;
                y = event.mouse.y;

                for (int i = 0; i < ELEM; i++) {
                    struct UI_ELEMENT *el = &game->ui.elements[i];
                    bool clicked_el = coordinatesInsideEl(el->x, el->y, el->width, el->height, x, y);

                    if (clicked_el) {
                        handleEvent(&el->event, el, game);
                        break;
                    }
                }

                checkSelectedBlocks(game);
                c = 0;
                prev[0] = -1;
                prev[1] = -1;
                cur[0] = -1;
                cur[1] = -1;

                redraw = true;
                break;
            case ALLEGRO_EVENT_TIMER:
                checkGameover(game);

                if (game->state == PLAYING) {
                    al_get_mouse_state(&mouseState);
                    if (mouseState.buttons && 1) {
                        
                        checkMouseOverBlocks(mouseState.x, mouseState.y, cur, game);
                        if (cur[0] > -1 
                        && game->blocks[cur[0]][cur[1]].blockConnected == NULL) {
                            if (c == 0) {
                                game->selectedBlocks[c] = &game->blocks[cur[0]][cur[1]];
                                c++;
                            } else {
                                i = prev[0] > cur[0] ? cur[0] : prev[0];
                                I = prev[0] < cur[0] ? cur[0] : prev[0];
                                j = prev[1] > cur[1] ? cur[1] : prev[1];
                                J = prev[1] < cur[1] ? cur[1] : prev[1];

                                if ((prev[0] != cur[0] || prev[1] != cur[1]) && I - i < 2 && J - j < 2) {
                                    if (c == 1) {
                                        if (game->blocks[cur[0]][cur[1]].value == game->blocks[prev[0]][prev[1]].value
                                        && game->blocks[cur[0]][cur[1]].blockConnected == NULL) {
                                            game->selectedBlocks[c] = &game->blocks[cur[0]][cur[1]];
                                            game->blocks[prev[0]][prev[1]].blockConnected = &game->blocks[cur[0]][cur[1]];
                                            c++;
                                        }
                                    } else {
                                        if ((game->blocks[cur[0]][cur[1]].value == game->blocks[prev[0]][prev[1]].value 
                                        || game->blocks[cur[0]][cur[1]].value == game->blocks[prev[0]][prev[1]].value * 2)
                                        && game->blocks[cur[0]][cur[1]].blockConnected == NULL) {
                                            if (game->selectedBlocks[c - 1] == &game->blocks[prev[0]][prev[1]]) {
                                                game->selectedBlocks[c] = &game->blocks[cur[0]][cur[1]];
                                                game->blocks[prev[0]][prev[1]].blockConnected = &game->blocks[cur[0]][cur[1]];
                                                c++;
                                            }
                                        }
                                    }
                                }
                            } 

                            prev[0] = cur[0];
                            prev[1] = cur[1];
                        }
                    } else {
                        removeHighlight(game);
                    }
                }
                redraw = true;
                break;

            case ALLEGRO_EVENT_KEY_DOWN:
                if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                    done = true;
                break;

            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                done = true;
                break;
        }

        if (done)
            break;
        
        if (redraw && al_is_event_queue_empty(game->ui.queue)) {
            draw(game);

            redraw = false;
        }
    }
}

int main(void) {
    struct GAME game;

    init(&game);

    gameLoop(&game);

    al_destroy_font(game.font_ranking);
    al_destroy_display(game.disp);
    al_destroy_timer(game.ui.timer);
    al_destroy_sample(game.music);
    al_destroy_event_queue(game.ui.queue);

    return 0;
}
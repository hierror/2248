# 2248

A game developed in C with the graphical library [Allegro5](https://liballeg.org/). 

![](assets/img/2248.gif)

*Read this in other languages: [Portuguese](README.pt-br.md)*

## Usage

You will need to install all the Allegro5 packages and its addons.

    sudo apt update
    sudo apt install liballegro5-dev liballegro-audio5-dev liballegro-acodec5-dev liballegro-ttf5-dev liballegro-image5-dev

After the installation, just compile the game using the following command:

    gcc -Wall game.c `pkg-config --cflags --libs allegro-5 allegro_audio-5 allegro_acodec-5 allegro_primitives-5 allegro_ttf-5 allegro_image-5` -lm
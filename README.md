# 2248

A game developed in C using the [Allegro5](https://liballeg.org/) graphics library. This project was created during my first semester at Universidade Federal de Santa Maria as part of the Sistemas de Informação Bachelor's degree curriculum.

![](assets/img/2248.gif)

_Read this in other languages: [Portuguese](README.pt-br.md)_

## Usage

You will need to install all the Allegro packages. [Follow the instructions in the website](https://liballeg.org/download.html). After the installation, compile the game with `gcc` or your preferred compiler:

```bash
gcc -Wall game.c -o game $(pkg-config --cflags --libs \
    allegro-5 \
    allegro_main-5 \
    allegro_primitives-5 \
    allegro_image-5 \
    allegro_font-5 \
    allegro_ttf-5 \
    allegro_audio-5 \
    allegro_acodec-5)
```

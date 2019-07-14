# 2248

Um jogo desenvolvido com C usando a biblioteca gráfica [Allegro5](https://liballeg.org/). Esse projeto foi desenvolvido no meu primeiro semestre na Universidade Federal de Santa Maria onde estudo TI.

![](assets/img/2248.gif)

*Leia em outras línguas: [Inglês](README.md)*

## Utilização

Você precisará instalar todos os pacotes do Allegro5 e seus addons.

    sudo apt update
    sudo apt install liballegro5-dev liballegro-audio5-dev liballegro-acodec5-dev liballegro-ttf5-dev liballegro-image5-dev

Após a instalação, basta compilar com o seguinte comando:

    gcc -Wall game.c `pkg-config --cflags --libs allegro-5 allegro_audio-5 allegro_acodec-5 allegro_primitives-5 allegro_ttf-5 allegro_image-5` -lm
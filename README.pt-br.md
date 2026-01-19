# 2248

Um jogo desenvolvido com C usando a biblioteca gráfica [Allegro5](https://liballeg.org/). Esse projeto foi desenvolvido no meu primeiro semestre na Universidade Federal de Santa Maria no curso de Sistemas de Informação.

![](assets/img/2248.gif)

_Leia em outras línguas: [Inglês](README.md)_

## Utilização

Você precisará instalar todos os pacotes do Allegro5. [Siga as instruções no site](https://liballeg.org/download.html). Após a instalação, compile o jogo com `gcc` ou seu compilador preferido:

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

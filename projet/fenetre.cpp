
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <iostream>

#include "fenetre.h"

void fenetre::dessinerpixel(unsigned int posx, unsigned int posy, sf::Color couleur){

    unsigned int y;
    unsigned int maxx = posx + TAILLE_PIXEL;
    unsigned int maxy = posy + TAILLE_PIXEL;

    for ( ; posx < maxx ; ++posx)
        for (y = posy ; y < maxy ; ++y)
            buffer.setPixel(y, posx, couleur);
        
    
}

void fenetre::boucle(){

    while ( app.isOpen() ){

        app.clear();

        sf::Event event;
        while ( app.pollEvent(event) ){

            if (event.type == sf::Event::Closed) app.close();
        }

        app.clear();

        dessinerpixel(10,10,nouvelle);
        dessinerpixel(20,40,mourante);
        dessinerpixel(100,90,vivante);

        texture.update(buffer);
        app.draw(sprite);

        app.display();
    }
}
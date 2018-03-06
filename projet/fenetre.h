
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <iostream>

#ifndef __fenetre_h__
#define __fenetre_h__ 1

#define FENETRE_X 200
#define FENETRE_Y 200
#define TAILLE_PIXEL 10

class fenetre {

    public:
        fenetre(): app(sf::VideoMode(FENETRE_X, FENETRE_Y, 32),"amazing shit !") 
        {
            buffer.create(FENETRE_X, FENETRE_Y, sf::Color(0, 0, 0));
            texture.loadFromImage(buffer);
            sprite.setTexture(texture, true);
        };
        ~fenetre() {};
        void boucle();


    private:

        const sf::Color nouvelle = sf::Color::Green;
        const sf::Color vivante = sf::Color::Blue;
        const sf::Color mourante = sf::Color::Red;

        sf::RenderWindow app;
        sf::Image buffer;
        sf::Texture texture;
        sf::Sprite sprite;

        void dessinerpixel(unsigned int posx, unsigned int posy, sf::Color couleur);
        

};

#endif
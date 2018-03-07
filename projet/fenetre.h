
#include <mutex>
#include <condition_variable>
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <iostream>


#ifndef __fenetre_h__
#define __fenetre_h__ 1

#define TAILLE_PIXEL 20

class fenetre {

    public:
        fenetre(int mx, int my): app(sf::VideoMode(mx*TAILLE_PIXEL,my*TAILLE_PIXEL, 32),"amazing shit !") 
        {
            buffer.create(mx*TAILLE_PIXEL,my*TAILLE_PIXEL, couleurs[0]);
            texture.loadFromImage(buffer);
            sprite.setTexture(texture, true);
        };

        ~fenetre() {};
        
        void boucle(){

            std::unique_lock<std::mutex> lock{m};
            
            while ( app.isOpen() ){
            
                app.clear();
            
                sf::Event event;
                while ( app.pollEvent(event) ){
            
                    if (event.type == sf::Event::Closed) app.close();
                }

                texture.update(buffer);
                app.draw(sprite);
            
                app.display();

                mettreAJour = false;
                cv.wait(lock,[this] {return mettreAJour;});
            }
        };

        void changerPixel(int x, int y, int indice){

            dessinerpixel( (unsigned int) x, (unsigned int) y, couleurs[indice]);
        };

        void maj(){

            mettreAJour = true;
            cv.notify_all();
            sleep(2);
        }

    private:

        const sf::Color couleurs[4] =  {sf::Color::Black,
                                       sf::Color::Green,
                                       sf::Color::Blue,
                                       sf::Color::Red};

        sf::RenderWindow app;
        sf::Image buffer;
        sf::Texture texture;
        sf::Sprite sprite;

        std::mutex m;
        bool mettreAJour;
        std::condition_variable cv;

        void dessinerpixel(unsigned int posx, unsigned int posy, sf::Color couleur){
            
            unsigned int y;
            unsigned int maxx = posx + TAILLE_PIXEL;
            unsigned int maxy = posy + TAILLE_PIXEL;
            
            for ( ; posx < maxx ; ++posx)
                for (y = posy ; y < maxy ; ++y)
                    buffer.setPixel(y, posx, couleur); 
        };
};

#endif
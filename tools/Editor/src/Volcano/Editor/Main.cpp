//
//
#include <SFML/Graphics.hpp>

#include <Volcano/Editor/Common.h>

int main(int argc, char* argv[]) {
    sf::RenderWindow main_window(sf::VideoMode(800, 600), "VolcanoLauncher");
    main_window.setFramerateLimit(60);

    while (main_window.isOpen()) {
        sf::Event event;
        while (main_window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                main_window.close();
            }
        }

        main_window.clear();

        main_window.display();
    }

    return EXIT_SUCCESS;
}

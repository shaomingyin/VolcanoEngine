//
//
#include <argh.h>
#include <SFML/Graphics.hpp>

#include <Volcano/Editor/Common.hpp>

int main(int argc, char* argv[]) {
    sf::RenderWindow main_window(sf::VideoMode({ 800, 600 }), "VolcanoLauncher");
    main_window.setFramerateLimit(60);

    while (main_window.isOpen()) {
        std::optional<sf::Event> evt;
        while (evt = main_window.pollEvent()) {
            if (!evt) {
                break;
            }
            if (evt->is<sf::Event::Closed>()) {
                main_window.close();
            }
        }

        main_window.clear();

        main_window.display();
    }

    return EXIT_SUCCESS;
}

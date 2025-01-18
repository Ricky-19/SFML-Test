#include <SFML/Graphics.hpp>
#include <vector>
using namespace sf;

int main() {
    RenderWindow window(VideoMode({800, 600}), "Scrolling con Collisione");
    window.setFramerateLimit(60);

    // Caricamento della texture
    Texture texture;
    if (!texture.loadFromFile("character.png")) {
        return -1;
    }

    // Sprite del personaggio
    Sprite character(texture);
    character.setOrigin(texture.getLocalBounds().width / 2.0f, texture.getLocalBounds().height / 2.0f); // Centro del personaggio
    character.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f); // Centro dello schermo

    // Velocità del personaggio
    float speed = 200.0f;

    // Posizione logica del personaggio nella mappa globale
    Vector2f playerPosition(400.0f, 300.0f); // Posizione iniziale nella mappa globale

    // Ostacoli (hitbox) nella mappa globale
    std::vector<FloatRect> obstacles = {
        FloatRect(350.0f, 250.0f, 100.0f, 100.0f), // Rettangolo in coordinate globali
        FloatRect(500.0f, 400.0f, 150.0f, 50.0f),
    };

    // Clock per il delta time
    Clock clock;

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
        }

        // Calcola il delta time
        float deltaTime = clock.restart().asSeconds();

        // Movimento logico del personaggio
        Vector2f movement(0.0f, 0.0f);
        if (Keyboard::isKeyPressed(Keyboard::W)) {
            movement.y -= speed * deltaTime;
        }
        if (Keyboard::isKeyPressed(Keyboard::S)) {
            movement.y += speed * deltaTime;
        }
        if (Keyboard::isKeyPressed(Keyboard::A)) {
            movement.x -= speed * deltaTime;
        }
        if (Keyboard::isKeyPressed(Keyboard::D)) {
            movement.x += speed * deltaTime;
        }

        // Verifica collisioni con gli ostacoli
        FloatRect playerBounds(playerPosition + movement, {character.getLocalBounds().width, character.getLocalBounds().height});
        bool canMove = true;
        for (const auto& obstacle : obstacles) {
            if (playerBounds.intersects(obstacle)) {
                canMove = false;
                break;
            }
        }

        // Aggiorna posizione logica se non ci sono collisioni
        if (canMove) {
            playerPosition += movement;
        }

        // Offset della mappa
        Vector2f mapOffset = {window.getSize().x / 2.0f - playerPosition.x, window.getSize().y / 2.0f - playerPosition.y};

        // Disegna la scena
        window.clear();

        // Disegna gli ostacoli (adattati all'offset)
        for (const auto& obstacle : obstacles) {
            RectangleShape rect(Vector2f(obstacle.width, obstacle.height));
            rect.setPosition(Vector2f(obstacle.left, obstacle.top) + mapOffset);
            rect.setFillColor(Color::Red);
            window.draw(rect);
        }

        // Disegna il personaggio (rimane al centro)
        window.draw(character);

        window.display();
    }

    return 0;
}

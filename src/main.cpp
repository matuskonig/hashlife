#include <vector>
#include <SFML/OpenGL.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics.hpp>
#include "Node.hpp"
#include "constants.hpp"
#include "GameWrapper.hpp"
#include "constants.hpp"
#include "Context.hpp"

using namespace std;

const auto light_gray = sf::Color(240, 240, 240);
const auto dark_gray = sf::Color(80, 80, 80);

int main()
{
    GameWrapper gw(GAME_PLAN_TREE_WIDTH, GAME_PLAN_TREE_HEIGHT, TREE_DEPTH);


    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Game of life", sf::Style::Default, sf::ContextSettings(8));

    //Enable openGL
    glEnable(GL_TEXTURE_2D);
    window.setVerticalSyncEnabled(true);

    //Size of one level 0 node
    auto square_vector = sf::Vector2f(ONE_POINT_PIXEL_SIZE, ONE_POINT_PIXEL_SIZE);

    auto &context = Context::instance();
    bool running = true;

    //Main event loop, rendering the window content
    while (running)
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                running = false;

            else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {

                auto &mouseButtonEvent = event.mouseButton;
                gw.toggle_at(mouseButtonEvent.x / ONE_POINT_PIXEL_SIZE, mouseButtonEvent.y / ONE_POINT_PIXEL_SIZE);
            }

            else if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::S)
                {

                    context.toggle_should_update();
                }
                else if (event.key.code == sf::Keyboard::H)
                {
                    context.toggle_hash_life();
                }
            }
        }
        //Clearing the previous window
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (context.should_update)
            gw.update_all();

        for (size_t i = 0; i < BIT_WIDTH; i++)
        {
            for (size_t j = 0; j < BIT_HEIGHT; j++)
            {
                auto shape = sf::RectangleShape(square_vector);
                shape.setFillColor(gw.alive_at(i, j) ? sf::Color::Black : light_gray);
                shape.setOutlineColor(dark_gray);
                shape.setOutlineThickness(1);
                shape.setPosition(sf::Vector2f(i * ONE_POINT_PIXEL_SIZE, j * ONE_POINT_PIXEL_SIZE));
                window.draw(shape);
            }
        }
        //Displaying the current frame
        window.display();
    }
    return 0;
}

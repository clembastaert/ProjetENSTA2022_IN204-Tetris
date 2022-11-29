#ifndef GAME_HPP
# define GAME_HPP

#include <iostream>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "../inc/grid.hpp"
#include "../inc/block.hpp"

const int WIN_WIDTH = 800;
const int WIN_HEIGHT = 600;

class Game
{
     public:
        void Run();
        void set_running(bool new_running);
        void set_fps_grid(float new_fps_grid);
      
        Grid get_grid()const;
        Block get_current_Block()const;

    private:

        bool _running = true;
        sf::RenderWindow window;
        sf::View grid_view;
        sf::View menu_view;

        sf::RectangleShape bgGrid_;
        sf::RectangleShape bgMenu_;

        Grid grid;

        //A suppr
        //std::vector<Block> list_Blocks;

        Block *current_block;

        sf::Clock clock;
        float fps_grid;


        void Initialize();
        void Shutdown();
        void Frame();

};

void InputHandler(sf::Event, Game &, Block &);
        
#endif // ! GAME_HPP
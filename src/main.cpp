
//#include "SFML/Graphics.hpp"
//#include "GLAD/glad.h"
//#include <Array>
#include "Window.h"
#include "Math/mat.h"
#include "DeltaTime.hpp"
#include "Input.hpp"

//we can define our own window class
class myWin : public sf::RenderWindow
{
    //copying RenderWindow constructors
    using sf::RenderWindow::RenderWindow;
};

int main()
{
    //Window Initialization OpenGL version 4.6 (Might not work on mac)
    GameWindow window(sf::VideoMode(900, 900), "OpenGL SFML");
    
    sf::Shader shader1;
    shader1.loadFromFile("shaders/circle_default.vert", "shaders/circle_default.frag");

    sf::Shader shader2;
    shader2.loadFromFile("shaders/Ring.vert", "shaders/Ring.frag");

    sf::Font font;
    if (!font.loadFromFile("assets/Exo.ttf"))
    {
        std::cout << "couldn't load fonts\n";
        return 1;
    }
    sf::Text text;
    text.setFont(font);
    text.setString("RHYTHM");
    text.setCharacterSize(50);
    text.setFillColor(sf::Color(200,0,200));
    text.setStyle(sf::Text::Bold | sf::Text::Underlined);
    text.setOutlineThickness(2);
    text.setOutlineColor(sf::Color::Black);
    


    DeltaTime Time;
    Time.set_target_fps(266);


    //This is just for testing!
    float intensity = 0;
    float anim = 0;
    bool animation_flag = 0;
    sf::Vector2f pos;
    while (window.isOpen())
    {
        if (Input::mouse_pressed(sf::Mouse::Left))
        {
            animation_flag = 1;
            anim = 0;
            pos = sf::Vector2f(Input::mouse_x()/450-1, -1*(Input::mouse_y()/450-1));
        }
        if (animation_flag == 1 && anim < 1)
            anim += Time.frame_time()/2;
        else {
            anim = 0;
            animation_flag = 0;
        }
        intensity = 4.0f * sin(Time.get_time()/4);
        Time.handle_time();
        Input::handle_events(window);
        //Time.show_fps();

        //glClearColor(0.07, 0.13, 0.17, 1);
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        //setting the variables in the shader
        shader1.setUniform("u_iTime", Time.get_time());
        shader1.setUniform("u_intensity", pow(abs(intensity/2),0.6f));
        shader1.bind(&shader1);

        window.draw_square();
        //Setting the variables in the shader
        shader2.setUniform("u_iTime", anim);
        shader2.setUniform("u_scale", 1.0f);
        shader2.setUniform("u_position", pos);
        shader2.setUniform("u_color", sf::Vector3f(0, 1, 0));
        shader2.bind(&shader2);
        //glBindVertexArray(vao);
        if(animation_flag==1)
            window.draw_square();
        //unbinding to draw text
        glBindVertexArray(0);
        shader1.bind(0);

        window.draw(text);

        window.display();
    }


    window.close();
}
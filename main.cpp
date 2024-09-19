#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include <imgui-SFML.h>
#include <imgui.h>
#include "D:\Programming\Assigmnet 1\out\build\x64-Debug\_deps\imgui-src\misc\cpp\imgui_stdlib.h" //do somethins with this

#include <iostream>
#include <vector>
#include <fstream>
#include <string>

class Entity;

void initConfig(std::vector<std::shared_ptr<Entity>>& allEntities, sf::VideoMode& mode, sf::Font& globalFont) {

    std::vector<std::string> tempStrings{  };

    std::ifstream fs;
    fs.open("config.txt");
    if (!fs.is_open()) {
        std::cerr << "Cant open file\n";
    }

    std::string temp;

    while (std::getline(fs, temp, ' ')) {
        
        tempStrings.push_back(temp);
    }
    

    std::size_t start = 0;
    for (auto iter{ tempStrings.begin() }; iter < tempStrings.end(); iter++) {

        if (*iter == "\n") {
            tempStrings.erase(iter);
            iter = tempStrings.begin();
        }

        std::size_t pos = iter->find('\n');
        
        if (pos != std::string::npos) {

            if (start == pos) {
                pos = iter->length();
                start++;
            }

            std::string sub = iter->substr(start, pos);

            iter->erase(start, pos);

            tempStrings.insert(iter, sub);
            
            iter = tempStrings.begin();
            
        }
        else {
            continue;
        }

        start = 0;
    }

    for (auto iter{ tempStrings.begin() }; iter < tempStrings.end(); iter++) {

        if (*iter == "Window") {
            mode.width = std::stoi(*(++iter));
            mode.height = std::stoi(*(++iter));
            continue;
        }
        else if (*iter == "Font") {
            
            if (!globalFont.loadFromFile((*(++iter)))) {
                std::cerr << "Error load font\n";
            }
            continue;
        }
        else if (*iter == "Circle" || *iter == "Rectangle") {
            
            std::string doingShape = *iter;
            // TODO: Optimize this.
            std::string name_ = (*(iter += 1));
            float pos_x = std::stof(*(iter += 1));
            float pos_y = std::stof(*(iter += 1));

            float dir_x = std::stof(*(iter += 1));
            float dir_y = std::stof(*(iter += 1));

            sf::Uint8 r_ = std::stoi(*(iter += 1));
            sf::Uint8 g_ = std::stoi(*(iter += 1));
            sf::Uint8 b_ = std::stoi(*(iter += 1));
            sf::Uint8 a_ = std::stoi(*(iter += 1));

            sf::Color t_color{ r_, g_, b_, a_ };

            // TODO : do rectangle stuff with rectangle shapes
            if (doingShape == "Rectangle") {
                auto temp = std::make_shared<Entity>(4, name_, pos_x, pos_y, dir_x, dir_y, t_color, globalFont);
                allEntities.push_back(temp);
            }
            else {
                auto temp = std::make_shared<Entity>((rand() % 15) + 3, name_, pos_x, pos_y, dir_x, dir_y, t_color, globalFont);
                allEntities.push_back(temp);
            }
            
            

        }
        

    }

}

class Entity {

private:

    sf::CircleShape shape;
    sf::Text nameText;

    int segments;
    float xspeed, yspeed;
    float radius;

    sf::Vector2f direction;

    std::string name;
    bool circleExist;

public:


    Entity(int segments_, std::string name_, float pos_x, float pos_y, float dir_x, float dir_y, sf::Color color_, sf::Font& mainFont)
        : circleExist(true)
    {
        
        name = name_;

        xspeed = dir_x;
        yspeed = dir_y;

        direction = { xspeed, yspeed };
        segments = segments_;
        radius = (float)(rand() % 30 + 25);

      
        shape = sf::CircleShape(radius, segments_);

        shape.setOrigin(shape.getGlobalBounds().width / 2, shape.getGlobalBounds().height / 2);

        //if square
        if (segments == 4) {
            shape.setRotation(225.f);
        }

        shape.setFillColor(color_);
        shape.setPosition(sf::Vector2f{ pos_x, pos_y });

        nameText.setFont(mainFont);
        nameText.setString(name);
        nameText.setCharacterSize(24);
    }

    void Update()
    {
        shape.move(direction);

        if (shape.getGlobalBounds().getPosition().x < 0) {
            xspeed = -xspeed;
        }
        else if (shape.getGlobalBounds().getPosition().x + shape.getGlobalBounds().width > 1280) {
            xspeed = -xspeed;
        }

        if (shape.getGlobalBounds().getPosition().y < 0) {
            yspeed = -yspeed;
        }
        else if (shape.getGlobalBounds().getPosition().y + shape.getGlobalBounds().height > 720) {
            yspeed = -yspeed;
        }

        direction.x = xspeed;
        direction.y = yspeed;

        shape.setRadius(radius);
        shape.setPointCount(segments);

        
        nameText.setPosition(shape.getGlobalBounds().left + shape.getGlobalBounds().width / 2.f - nameText.getGlobalBounds().width / 2.f,
            shape.getGlobalBounds().top + shape.getGlobalBounds().height);
    }

    void Render(sf::RenderTarget& target) { target.draw(shape); target.draw(nameText); }

    //getters
    bool& getExist(){ return circleExist; }
    float& getRadius() { return radius; }
    int& getSegments() { return segments; }
    float& getXSpeed() { return xspeed; }
    float& getYSpeed() { return yspeed; }

    const sf::Color& getColor() { return shape.getFillColor(); }

    std::string& getName() { return name; }
    void setName(std::string name_) { name = name_; nameText.setString(name); }

    //setter
    void setColor(sf::Color color_) { shape.setFillColor(color_); };
};


int main() {

    srand(time(0));


    sf::Font mainFont;
    std::vector<std::shared_ptr<Entity>> allEntities{};
    std::vector<std::string> names{};

    sf::VideoMode vidMode{ 0,0 };

    initConfig(allEntities, vidMode, mainFont);

    sf::RenderWindow window(vidMode, "ImGui + SFML = <3");
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);

    ImGui::GetStyle().ScaleAllSizes(1.f);

    ImVec4 color = ImVec4(allEntities[0]->getColor().r / 255.0f, allEntities[0]->getColor().g / 255.0f,
                          allEntities[0]->getColor().b / 255.0f, allEntities[0]->getColor().a / 255.0f);

    for (auto& i : allEntities) {
        names.push_back(i->getName());
    }
    
    int item_current_idx = 0;
    int savedItem = item_current_idx;

    std::string name_ = "None";

    sf::Clock deltaClock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        ImGui::Begin("Shape properties");

        //ImGui::Combo("Shape", &item_current_idx, items, IM_ARRAYSIZE(items));

        if (ImGui::BeginCombo("Shape", allEntities[item_current_idx]->getName().c_str()))
        {
            for (int n = 0; n < names.size(); n++)
            {
                const bool is_selected = (item_current_idx == n);
                if (ImGui::Selectable(allEntities[n]->getName().c_str(), is_selected))
                    item_current_idx = n;

                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }


        ImGui::Checkbox("Draw shape", &allEntities[item_current_idx]->getExist());
        ImGui::SliderFloat("Scale", &allEntities[item_current_idx]->getRadius(), 25.f, 500.f);
        ImGui::SliderInt("Segments", &allEntities[item_current_idx]->getSegments(), 3, 32);
        ImGui::SliderFloat("VelocityX", &allEntities[item_current_idx]->getXSpeed(), -5.f, 5.f);
        ImGui::SliderFloat("VelocityY", &allEntities[item_current_idx]->getYSpeed(), -5.f, 5.f);

        if (savedItem != item_current_idx) {
            color = ImVec4(allEntities[item_current_idx]->getColor().r / 255.f, allEntities[item_current_idx]->getColor().g / 255.f,
                allEntities[item_current_idx]->getColor().b / 255.f,
                allEntities[item_current_idx]->getColor().a / 255.f);
            savedItem = item_current_idx;
            name_ = names[item_current_idx];
        }

        ImGui::ColorEdit3("Color", (float*)&color);
        allEntities[item_current_idx]->setColor(sf::Color(color.x * 255, color.y * 255, color.z * 255, color.w * 255));

        
        ImGui::InputText("Name", &name_);

        if (ImGui::Button("Set name")) 
        {
            allEntities[item_current_idx]->setName(name_);

        };


        ImGui::End();



        for (auto& i : allEntities) {
            i->Update();
        }

        window.clear();
        
        for (auto& i : allEntities) {
            if (i->getExist()) {
                i->Render(window);
            }
        }
    
        ImGui::ShowDemoWindow();

        ImGui::SFML::Render(window);
        window.display();
    }

    

    ImGui::SFML::Shutdown();

    return 0;
}



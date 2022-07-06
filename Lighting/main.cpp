#include <iostream>
#include <vector>
#include <random>
#include <time.h>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <cmath>
#define M_PI 3.14159265358979323846

std::vector<std::vector<char> > map2D = {
    {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0},
    {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0},
    {0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
    {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0},
    {0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0},
    {0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
    {0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0},
    {0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

sf::Texture lighteffect;

sf::Vector2f closestPoint(const float& angle, const sf::Vector2i& currentTitle, const sf::Vector2f& point, const sf::Vector2i& titleSize, const sf::Vector2f& dir)
{
    sf::Vector2f dv(0, 0);
    sf::Vector2f dh(0, 0);

    if (point.x == currentTitle.x * titleSize.x)
    {
        if (dir.x < 0)
        {
            dv.x = 1;
        }
        else
        {
            dv.x = titleSize.x;
        }
    }
    else
    {
        if (dir.x < 0)
        {
            dv.x = point.x - (currentTitle.x * titleSize.x);
        }
        else
        {
            dv.x = (currentTitle.x + 1) * titleSize.x - point.x;
        }
    }
    dv.y = dv.x * tan(angle);
    
    if (dir.x < 0)
    {
        dv.x = -dv.x;
        dv.y = -dv.y;
    }

    if (point.y == currentTitle.y * titleSize.y)
    {
        if (dir.y < 0)
        {
            dh.y = 1;
        }
        else
        {
            dh.y = titleSize.y;
        }
    }
    else
    {
        if (dir.y < 0)
        {
            dh.y = point.y - (currentTitle.y * titleSize.y);
        }
        else
        {
            dh.y = (currentTitle.y + 1) * titleSize.y - point.y;
        }
    }

    dh.x = dh.y / tan(angle);

    if (dir.y < 0)
    {
        dh.x = -dh.y;
        dh.y = -dh.y;
    }

    float hLen = dh.x * dh.x + dh.y * dh.y;
    float vLen = dv.x * dv.x + dv.y * dv.y;

    sf::Vector2f d(hLen < vLen ? dh.x : dv.x,hLen < vLen ? dh.y : dv.y);

    return d;

}

void drawLight(const sf::Vector2f& origin, const float& radius, sf::Color color, const sf::Vector3f& angleInfo, const float& intensity, const sf::Vector2i& tileSize, const sf::Vector2i& mapBound, sf::RenderTexture& rx)
{
    std::vector<sf::Vector2f> points;

    if (intensity < 1)
    {
        color.a = 229 * intensity;
    }
    else
    {
        color.a = 100;
    }

    for (float dangle = angleInfo.x; dangle < angleInfo.y; dangle += angleInfo.z)
    {
        float angle = dangle * M_PI / 180.f;
        sf::Vector2i currentTile(origin.x / tileSize.x, origin.y / tileSize.y);
        sf::Vector2f dir(cos(angle), sin(angle));
        sf::Vector2f dist = closestPoint(angle, currentTile, origin, tileSize, dir); 
        sf::Vector2f rayPos = { origin.x + dist.x, origin.y + dist.y }; 
        currentTile = { int(rayPos.x / tileSize.x), int(rayPos.y / tileSize.y) }; 

        while (true)
        {
            if (((rayPos.x - origin.x) * (rayPos.x - origin.x) + (rayPos.y - origin.y) * (rayPos.y - origin.y)) >= radius * radius)
            {
                rayPos.x = origin.x + radius * dir.x;
                rayPos.y = origin.y + radius * dir.y;
                points.push_back(rayPos);
                break;
            }
            
            if (currentTile.x < 0 || currentTile.x >= mapBound.x || currentTile.y < 0 || currentTile.y >= mapBound.y)
            {
                points.push_back(rayPos);
                break;
            }

            sf::Vector2i next = currentTile;
            
            if ((rayPos.x == next.x * tileSize.x) && dir.x < 0)
            {
                next.x--;
            }

            if ((rayPos.y == next.y * tileSize.y) && dir.y < 0)
            {
                next.y--;
            }

            if (next.y < 0 || next.y >= mapBound.y || next.x < 0 || next.x >= mapBound.x || map2D[next.y][next.x] == 1)
            {
                points.push_back(rayPos);
                break;
            }
            else
            {
                dist = closestPoint(angle, currentTile, rayPos, tileSize, dir); 
                rayPos = { rayPos.x + dist.x, rayPos.y + dist.y }; 
                currentTile = { int(rayPos.x / tileSize.x), int(rayPos.y / tileSize.y) }; 
            }
        }
    }

    if (points.empty())
    {
        return;
    }

    sf::VertexArray zone(sf::TriangleFan, points.size() + 1);
    zone[0].position = origin; 
    zone[0].color = color;
    zone[0].texCoords = sf::Vector2f(64.f, 64.f);
    float ratio = 28.f / radius;
    for (size_t i = 0; i < points.size(); ++i)
    {
        zone[i + 1].position = points[i];
        zone[i + 1].color = color;
        zone[i + 1].texCoords.x = 64.f + (points[i].x - origin.x) * ratio; 
        zone[i + 1].texCoords.y = 64.f + (points[i].y - origin.y) * ratio;
    }
    rx.draw(zone, &lighteffect); 
}

int main()
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 2;
    sf::RenderWindow window(sf::VideoMode(400, 400), "Light", sf::Style::Titlebar | sf::Style::Close, settings);
    window.setFramerateLimit(30);
    sf::Event event;
    sf::RenderTexture rx[2];
    sf::Sprite render[2];
    rx[0].create(400, 400);
    rx[1].create(400, 400);
    render[0].setTexture(rx[0].getTexture());
    render[1].setTexture(rx[1].getTexture());
    sf::RectangleShape square;
    square.setSize(sf::Vector2f(20, 20));
    square.setFillColor(sf::Color(20, 20, 20));
    sf::View view;
    view.setCenter(200, 200);
    view.setSize(400, 400);

    sf::CircleShape point;
    point.setRadius(2);
    point.setFillColor(sf::Color::Red);
    point.setPosition(0, 0);
    point.setOrigin(2, 2);

    lighteffect.loadFromFile("mask.png");

    std::vector<sf::Vector2f> torchs = { {80, 122}, {350, 97}, {90, 314}, {318, 318}, {182, 250} };
    float vti = -1.f;
    float vtid = 0.1f;

    bool keys[4] = { false, false, false, false };

    std::mt19937 mt(time(NULL));
    std::uniform_int_distribution<int> dist(0, 360);

    std::vector<int> randAngle;
    for (size_t i = 0; i < torchs.size(); ++i)
        randAngle.push_back(dist(mt));

    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyPressed:
                switch (event.key.code)
                {
                case sf::Keyboard::Right: keys[1] = true; break;
                case sf::Keyboard::Left: keys[3] = true; break;
                case sf::Keyboard::Up: keys[0] = true; break;
                case sf::Keyboard::Down: keys[2] = true; break;
                default: break;
                }
                break;
            case sf::Event::KeyReleased:
                switch (event.key.code)
                {
                case sf::Keyboard::Right: keys[1] = false; break;
                case sf::Keyboard::Left: keys[3] = false; break;
                case sf::Keyboard::Up: keys[0] = false; break;
                case sf::Keyboard::Down: keys[2] = false; break;
                default: break;
                }
                break;
            case sf::Event::MouseButtonPressed:
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    sf::Vector2i mp(event.mouseButton.x, event.mouseButton.y);
                    mp.x /= 20;
                    mp.y /= 20;
                    if (mp.x >= 0 && mp.y >= 0 && mp.x < 20 && mp.y < 20 && map2D[mp.y][mp.x] == 0)
                    {
                        std::cout << "mouse - " << event.mouseButton.x << " " << event.mouseButton.y << " (" << mp.x << "," << mp.y << ")" << std::endl;
                        point.setPosition(event.mouseButton.x, event.mouseButton.y);
                    }

                }
                break;
            default:
                break;
            }
        }

        if (*((long*)keys) != 0) 
        {
            sf::Vector2f fp = point.getPosition();
            sf::Vector2i p(fp.x, fp.y);
            if (keys[0]) p.y--;
            else if (keys[2]) p.y++;
            if (keys[1]) p.x++;
            else if (keys[3]) p.x--;

            if (p.y < 400 && p.x < 400 && p.x >= 0 && p.y >= 0 && map2D[p.y / 20][(p.x / 20) % 20] == 0)
            {
                point.setPosition(p.x, p.y);
                std::cout << "move - " << point.getPosition().x << " " << point.getPosition().y << "(" << (((int)point.getPosition().x / 20) % 20) << "," << ((int)point.getPosition().y / 20) << "): " << (int)map2D[(int)point.getPosition().y / 20][((int)point.getPosition().x / 20) % 20] << std::endl;
            }
        }

        rx[0].clear(sf::Color(229, 229, 229));
        for (auto i = 0; i < 400; ++i)
        {

            switch (map2D[i / 20][i % 20])
            {
            case 0: break;
            case 1:
                square.setPosition((i % 20) * 20, (i / 20) * 20);
                rx[0].draw(square);
                break;
            }
        }
        rx[0].draw(point);
        rx[0].display();

        rx[1].clear(sf::Color(10, 10, 10));
        drawLight(point.getPosition(), 120, sf::Color::White, { 0.f, 361.f, 5.f }, 0.5, sf::Vector2i(20, 20), sf::Vector2i(20, 20), rx[1]);
        for (size_t i = 0; i < torchs.size(); ++i)
            drawLight(torchs[i], 60 + vti, sf::Color(227, 140, 45), { 0.f + randAngle[i], 100.f + randAngle[i], 5.f }, 0.85, sf::Vector2i(20, 20), sf::Vector2i(20, 20), rx[1]);
        rx[1].display();
        vti += vtid;
        if (vti >= 1.1f) vtid = -0.1f;
        else if (vti <= -1.1f) vtid = 0.1f;

        window.clear();
        window.setView(view);
        window.draw(render[0]);
        window.draw(render[1], sf::BlendMultiply);
        window.display();
    }

    return 0;
}
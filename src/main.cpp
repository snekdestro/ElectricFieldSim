#include <SFML/Graphics.hpp>
#include <math.h>
#include <list>
#include <iostream>




sf::Vector2f screenSize(sf::VideoMode::getFullscreenModes()[0].width,sf::VideoMode::getFullscreenModes()[0].height);
sf::Vector2f pref(1920,1080);

class Particle{
    const float kqq = 8.987551785e4;
    //fiddle around with these
    const float MASS = 5; //in KG
    //SASABIUDGASHJDAHJSXDY
    public:
        bool STATIC;
        float x;
        float y;
        float vx;
        float vy;
        int sign; //either 1 or -1 (shrug)
        sf::CircleShape* shape;
        sf::RectangleShape* pprect;
    
    void set(float xi, float yi){
        x = xi;
        y = yi;
        shape->setPosition(xi, screenSize.y - yi);
    }
    void op(float aX, float aY, float delta){
        vx += aX * delta;
        vy += aY * delta;
        x += vx*delta;
        y += vy*delta;
        shape->setPosition(x,screenSize.y - y);
    }
    void calc(float delta, Particle* p){
        if(x < 0 || x > screenSize.x){
            vx *= -1;
            if(x < 0){
                x = 0;
            }else{
                x=screenSize.x;
            }
        }

        if(y <  300 || y > screenSize.y + 300){
            vy *= -1;
            if( y < 300){
                y = 300;
            }else{
                y = screenSize.y + 300;
            }
        }
        float dist = std::sqrt(std::pow(x-p->x,2) + std::pow(y-p->y,2));
        //if(dist < 25){
        //    return;
        //}
        if(dist == 0){
            return;
        }
        if(dist < 20){
            //x += vx  * delta;
            //y += vy * delta;
            sf::Vector2f vecCol(p->x - x, p->y - y);
            double theta = std::atan(std::abs(vecCol.y / vecCol.x))  + 3.1415926535 / 2.0;
            float vMag = sqrt(vx * vx + vy * vy);
            vx = vMag * std::cos(theta);
            vy = vMag * std::sin(theta);
            x += vx  * delta;
            y += vy * delta;
            
            return;
        }
        float force = kqq/(dist * dist);
        

        force /= MASS;
        //now is A
    
        float theta = std::atan(std::abs((p->y - y) / (p->x - x)));
        float aX = force * std::cos(theta);
        float aY = force * std::sin(theta);
        //perform reflections
        if(x <  p->x){
            aX *= -1;
        }
        if(y < p->y){
            aY *= -1;
        }

        aX  *= p->sign * sign;
        aY *= (p->sign) * sign;
        if(!STATIC){
        vx += aX * delta;
        vy += aY * delta;
        x += vx  * delta;
        y += vy * delta;
        shape->setPosition(sf::Vector2f(x,1920 - y));
        }


        if(!p->STATIC){
        p->op(-aX,-aY,delta);
        }
        //gotta parse back....
        
        //or not???
    }
    Particle(bool s, float xi, float yi, int si ){
        STATIC = s;
        sign =si;
        x = xi;
        y = 1920-yi;
        vx = 0.0;
        vy = 0.0;
        shape = new sf::CircleShape();
        shape->setPosition(sf::Vector2f(xi,yi));
        if(si == 1){
            shape->setFillColor(sf::Color::Blue);
        }else{
            shape->setFillColor(sf::Color::Red);
        }
        shape->setRadius(25);
        pprect = new sf::RectangleShape;
        pprect->setFillColor(sf::Color::White);
        pprect->setSize(screenSize);
    }
};



int main()
{
    
    sf::RectangleShape screenRect(sf::Vector2f(sf::VideoMode::getFullscreenModes()[0].width,sf::VideoMode::getFullscreenModes()[0].height));
    sf::Shader fieldShader;
    fieldShader.loadFromFile("shaders/fieldShader.frag",sf::Shader::Fragment);
    sf::Shader fieldVecShader;
    fieldVecShader.loadFromFile("shaders/fieldvecshader.frag", sf::Shader::Fragment);
    sf::Shader spaceWarping;
    spaceWarping.loadFromFile("shaders/spaceWarping.frag",sf::Shader::Fragment);
    int mode = 0;
    auto window = sf::RenderWindow();
    window.create(sf::VideoMode::getFullscreenModes()[0], "Electrostatic Force simulator");
    window.setFramerateLimit(144);
    std::vector<Particle*> particles;
    sf::Clock clock;
    int tracker = 0;
    float* points[140];
    float pv[140];
    float signs[70];
    bool shift = false;
    float cc =0.0f;
    float pp = 3.14159265358979 ;


    
    while (window.isOpen())
    {
        screenSize = sf::Vector2f(sf::VideoMode::getFullscreenModes()[0].width,sf::VideoMode::getFullscreenModes()[0].height);
        for (auto event = sf::Event(); window.pollEvent(event);)
        {
            
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            if(event.type == sf::Event::KeyPressed){
                shift=event.key.shift;
                if(event.key.alt){
                    mode++;
                    mode %= 3;
                }
            }
            if(event.type == sf::Event::KeyReleased){
                shift = event.key.shift;
            }
            
            
            if(event.type == sf::Event::MouseButtonPressed){
                if(event.mouseButton.button == sf::Mouse::Left){
                    particles.push_back(new Particle(shift,event.mouseButton.x -25,event.mouseButton.y -25,-1));
                    if(tracker < 140){
                        
                    
                    signs[tracker /2] = -1;
                    points[tracker++] = &(particles[particles.size() -1]->x);
                    points[tracker++] = &(particles[particles.size() -1]->y);
                    }
                }
                else if(event.mouseButton.button == sf::Mouse::Right){
                    particles.push_back(new Particle(shift,event.mouseButton.x -25,  event.mouseButton.y -25,1));
                    if(tracker < 140){
                    signs[tracker / 2] = 1;
                    points[tracker++] = &particles[particles.size() -1]->x;
                    points[tracker++] = &particles[particles.size() -1]->y;
                    }
                }
            }
        }
        float delta = clock.restart().asSeconds();
        cc += 0.1 * delta;
        pp -= 0.1 * delta;
        if(cc >= 3.14159265358979 * 2){
            cc = 0;
        }
        if(pp <= 0){
            pp = 3.14159265358979 ;
        }
        
        for(int i = 0; i < particles.size(); i++){
            for(int j = i+1; j < particles.size(); j++){
                particles[i]->calc(delta,particles[j]);
            }
        }
        
        window.clear();
        if(mode != 0){
        for(int i = 0; i < particles.size(); i++){
            
            if(mode == 1){
            
            
            if(particles[i]->sign < 0){
                fieldShader.setParameter("waveTime", pp);
                
            }else{
                fieldShader.setParameter("waveTime", cc);
            }
            fieldShader.setParameter("u_resolution", screenSize);
            
            fieldShader.setParameter("xy", sf::Vector2f(particles[i]->x, particles[i]->y - 300) );
            window.draw(*particles[i]->pprect,  &fieldShader);
            }else if (mode == 2){
                
                spaceWarping.setParameter("u_resolution",screenSize);
            
                spaceWarping.setParameter("xy", sf::Vector2f(particles[i]->x, particles[i]->y - 300));
                spaceWarping.setParameter("neg", (particles[i]->sign < 0));
                window.draw(*(particles[i]->pprect), &spaceWarping);
            }
            
        }
        }else{
            
            fieldVecShader.setUniform("size", tracker);
            
            for(int i = 0; i < tracker; i++){
                if(i % 2 == 0){
                fieldVecShader.setUniform("data[" + std::to_string(i) + "]",*points[i]);
                }else{
                    fieldVecShader.setUniform("data[" + std::to_string(i) + "]", *points[i] - 300);
                }
                
            }
            fieldVecShader.setUniformArray("signs", signs, 70 );
            window.draw(screenRect, &fieldVecShader);
        }

        window.display();
        
    }
}

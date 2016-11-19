#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

const float kScale = 30.f;

void createGround(b2World& world, float x, float y);
void createBox(b2World& world, int mouseX, int mouseY);

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600, 32), "Test");
    window.setFramerateLimit(60);

    b2Vec2 gravity(0.f, 9.8f);
    b2World world(gravity);

    sf::Texture groundTexture;
    sf::Texture boxTexture;

    groundTexture.loadFromFile("ground.png");
    boxTexture.loadFromFile("box.png");

    createGround(world, 400.f, 500.f);

    while (window.isOpen())
    {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            int mouseX = sf::Mouse::getPosition(window).x;
            int mouseY = sf::Mouse::getPosition(window).y;
            createBox(world, mouseX, mouseY);
        }

        world.Step(1/60.f, 8, 3);

        window.clear(sf::Color::White);
        for (b2Body* it = world.GetBodyList(); it != 0; it = it->GetNext())
        {
            sf::Sprite sprite;

            if (it->GetType() == b2_dynamicBody)
            {
                sprite.setTexture(boxTexture);
                sprite.setOrigin(16.f, 16.f);
                sprite.setPosition(kScale * it->GetPosition().x, kScale * it->GetPosition().y);
                sprite.setRotation(it->GetAngle() * 180 / b2_pi);
                window.draw(sprite);

                continue;
            }
            sprite.setTexture(groundTexture);
            sprite.setOrigin(400.f, 8.f);
            sprite.setPosition(kScale * it->GetPosition().x, kScale * it->GetPosition().y);
            sprite.setRotation(it->GetAngle() * 180 / b2_pi);
            window.draw(sprite);
        }
        window.display();
    }

    return 0;
}

void createGround(b2World& world, float x, float y)
{
    b2BodyDef bodyDef;
    bodyDef.position.Set(x/30.f, y/30.f);
    bodyDef.type = b2_staticBody;

    b2Body* body = world.CreateBody(&bodyDef);

    b2PolygonShape shape;
    shape.SetAsBox((800.f / 2) / kScale, (16.f / 2) / kScale);

    b2FixtureDef fixtureDef;
    fixtureDef.density = 0.f;
    fixtureDef.shape = &shape;

    body->CreateFixture(&fixtureDef);
}

void createBox(b2World& world, int mouseX, int mouseY)
{
    b2BodyDef bodyDef;
    bodyDef.position.Set(mouseX / kScale, mouseY / kScale);
    bodyDef.type = b2_dynamicBody;

    b2Body* body = world.CreateBody(&bodyDef);

    b2PolygonShape shape;
    shape.SetAsBox((32.f / 2) / kScale, (32.f / 2) / kScale);

    b2FixtureDef fixtureDef;
    fixtureDef.density = 1.f;
    fixtureDef.friction = .7f;
    fixtureDef.shape = &shape;

    body->CreateFixture(&fixtureDef);
}

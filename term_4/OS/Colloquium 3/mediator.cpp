#include <iostream>
#include <string>
#include <memory>

class Mediator;

class Component
{
protected:
    Mediator *mediator;

public:
    void setMediator(Mediator *m)
    {
        mediator = m;
    }
    virtual void notify(const std::string &msg) = 0;
};

class Mediator
{
public:
    virtual void send(const std::string &msg, Component *sender) = 0;
};

class ConcreteComponentA;
class ConcreteComponentB;

class ConcreteMediator : public Mediator
{
    ConcreteComponentA *a;
    ConcreteComponentB *b;

public:
    void setA(ConcreteComponentA *compA) { a = compA; }
    void setB(ConcreteComponentB *compB) { b = compB; }

    void send(const std::string &msg, Component *sender) override;
};

class ConcreteComponentA : public Component
{
public:
    void notify(const std::string &msg) override
    {
        std::cout << "A received: " << msg << '\n';
    }

    void send(const std::string &msg)
    {
        if (mediator)
            mediator->send(msg, this);
    }
};

class ConcreteComponentB : public Component
{
public:
    void notify(const std::string &msg) override
    {
        std::cout << "B received: " << msg << '\n';
    }

    void send(const std::string &msg)
    {
        if (mediator)
            mediator->send(msg, this);
    }
};

void ConcreteMediator::send(const std::string &msg, Component *sender)
{
    if (sender == a)
        b->notify(msg);
    else
        a->notify(msg);
}

int main()
{
    ConcreteComponentA compA;
    ConcreteComponentB compB;
    ConcreteMediator mediator;

    mediator.setA(&compA);
    mediator.setB(&compB);

    compA.setMediator(&mediator);
    compB.setMediator(&mediator);

    compA.send("Ping from A");
    compB.send("Pong from B");
}

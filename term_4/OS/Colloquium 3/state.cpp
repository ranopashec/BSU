#include <iostream>
#include <memory>

class State
{
public:
    virtual void handle() = 0;
    virtual ~State() {}
};

class Context
{
    std::shared_ptr<State> state;

public:
    void setState(std::shared_ptr<State> s)
    {
        state = s;
    }

    void request()
    {
        if (state)
            state->handle();
    }
};

class StateA : public State
{
public:
    void handle() override
    {
        std::cout << "State A\n";
    }
};

class StateB : public State
{
public:
    void handle() override
    {
        std::cout << "State B\n";
    }
};

int main()
{
    Context ctx;
    ctx.setState(std::make_shared<StateA>());
    ctx.request();

    ctx.setState(std::make_shared<StateB>());
    ctx.request();
}

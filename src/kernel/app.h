#pragma once

class Kernel;

class App {
public:
    virtual ~App() = default;
    virtual void render(Kernel& k) = 0;
    virtual const char* title() const = 0;
};

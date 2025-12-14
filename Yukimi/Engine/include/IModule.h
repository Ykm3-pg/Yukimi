#pragma once

namespace Yukimi
{

class IModule
{
public:
    IModule() = default;
    virtual ~IModule() = default;
    virtual void Initialize() = 0;
    virtual void Update() = 0;
    virtual void Finalize() = 0;
};

}
#pragma once
#include <string>

class IObserver {
public:
    virtual void update(IObserver* sender, const std::string& message) = 0;
    virtual ~IObserver() {}
};
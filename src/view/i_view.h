#pragma once

class IView {
   public:
    virtual ~IView() = default;
    virtual void show() = 0;
    virtual void refresh() = 0;
};
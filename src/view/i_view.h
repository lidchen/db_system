#pragma once

class IView {
   public:
    virtual ~IView() = default;
    virtual void Show() = 0;
    virtual void Refresh() = 0;
};
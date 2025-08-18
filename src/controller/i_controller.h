#pragma once
#include <memory>

#include "src/model/i_model.h"
#include "src/view/i_view.h"

template <typename ModelT, typename ViewT>
class IController {
   private:
    std::unique_ptr<ModelT> model_;
    std::unique_ptr<ViewT> view_;

   public:
    ~IController() = default;
    void set_model(std::unique_ptr<ModelT> model) { model_ = std::move(model); }
    void set_view(std::unique_ptr<ViewT> view) { view_ = std::move(view); }
};
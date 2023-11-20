//
// Created by oasg on 2023/11/20.
//

#ifndef NS_FDTD_MENU_HPP
#define NS_FDTD_MENU_HPP
#include "GUIElement.hpp"
namespace GUI{
    class MenuElement : public GUIElement{
    public:
        virtual void draw() override;
    };
}


#endif //NS_FDTD_MENU_HPP

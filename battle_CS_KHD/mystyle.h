#ifndef MYSTYLE_H
#define MYSTYLE_H

#include <QString>
#include <QGraphicsDropShadowEffect>


#pragma execution_character_set("utf-8")
class myStyle
{
public:
    //本项目的通用style；
    //style、instruction等都是可以迁移的、可复用的类，设计和逐渐填充丰富这个库是一个长远的目标

    myStyle();
    QString bkGrey(int);
    QString bkGreyH(int);
    QString style_buttonLabel();
    QString style_buttonLabelChoose();
    QString style_returnButtonLabelLeft();
    QString style_longLabel();
    QString style_hover();
    QString style_roLabel();

private:



};

#endif // MYSTYLE_H

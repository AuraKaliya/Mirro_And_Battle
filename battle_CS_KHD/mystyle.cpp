#include "mystyle.h"

myStyle::myStyle()
{

}

QString myStyle::bkGrey(int p)
{
    return QString("background:rgba(130,130,130,"+QString::number(p)+");");

}

QString myStyle::bkGreyH(int p)
{
    return QString("background:rgba(70,70,70,"+QString::number(p)+");");
}

QString myStyle::style_buttonLabel()
{
    return QString("border-radius:10%;color:rgb(255,255,255); background-color:rgba(180,180,180,150);font: 30 15pt 'Microsoft YaHei' ;");
}

QString myStyle::style_buttonLabelChoose()
{
    return QString("border-radius:10%;color:rgb(255,255,255); background-color:rgba(80,80,80,200);font: 30 15pt 'Microsoft YaHei'; ");
}

QString myStyle::style_returnButtonLabelLeft()
{
    return QString("border-bottom-right-radius: 80px 40px;border-top-left-radius:0px 0px;color:rgb(255,255,255); background-color:rgba(150,150,150,120);font: 30 25pt 'Microsoft YaHei' ;");
}

QString myStyle::style_longLabel()
{
    return QString("border-radius:10%;color:rgb(255,255,255); background-color:rgba(180,180,180,150);font: 30 15pt 'Microsoft YaHei' ;");
}

QString myStyle::style_hover()
{

    return QString("hover{background-color:rgba(100,100,100,180)}");
}

QString myStyle::style_roLabel()
{
    return QString("border-radius:100%;background:rgba(70,70,70,20);color:rgb(255,255,255);font: 30 25pt 'Microsoft YaHei' ;");
}





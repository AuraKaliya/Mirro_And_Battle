#include "instruction.h"

instruction::instruction()
{

}


QString instruction::ATK()
{
    return  QString(style+"ATK"+style);
}

QString instruction::DEF()
{
    return  QString(style+"DEF"+style);
}

QString instruction::REC()
{
    return  QString(style+"REC"+style);
}

QString instruction::SendTalk()
{
    return  QString(style+"SendTalk"+style);
}

QString instruction::OPHP()
{
    return  QString(style+"OPHP"+style);
}

QString instruction::MEHP()
{
    return  QString(style+"MEHP"+style);
}

QString instruction::Winner()
{
    return  QString(style+"Winner"+style);
}

QString instruction::NextTurn()
{
    return QString(style+"NextTurn"+style);
}

QString instruction::initBattle()
{
    return QString(style+"initBattle"+style);
}

QString instruction::match()
{
    return QString(style+"match"+style);
}

QString instruction::stopMatch()
{
    return QString(style+"stopMatch"+style);
}

QString instruction::matchSuccess()
{
    return QString(style+"matchSuccess"+style);
}

QString instruction::setIdx()
{
    return QString(style+"setIdx"+style);
}

QString instruction::inst(QString st)
{
    return  QString(st+stEnd);
}





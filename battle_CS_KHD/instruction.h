#ifndef INSTRUCTION_H
#define INSTRUCTION_H



#include <QString>

#pragma execution_character_set("utf-8")
class instruction
{

    //这是指令系统类，用于规范双端交互的指令
    //关于指令的处理采用split分割字符串的方式进行

public:
    instruction();


    QString ATK();
    QString DEF();
    QString REC();
    QString SendTalk();

    QString OPHP();
    QString MEHP();
    QString Winner();
    QString NextTurn();
    QString initBattle();
    QString match();
    QString stopMatch();
    QString matchSuccess();

    QString setIdx();



    QString inst(QString);
private:
    QString style="##";
    QString stEnd="|";


};

#endif // INSTRUCTION_H

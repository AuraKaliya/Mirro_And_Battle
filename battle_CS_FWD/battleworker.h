#ifndef BATTLEWORKER_H
#define BATTLEWORKER_H

#include <QObject>

class battleWorker : public QObject
{
    Q_OBJECT
public:
    explicit battleWorker(QObject *parent = nullptr);

signals:

};

#endif // BATTLEWORKER_H

#ifndef BATTLETHREAD_H
#define BATTLETHREAD_H

#include <QThread>


class battleThread : public QThread
{
    Q_OBJECT
public:
    explicit  battleThread(QObject *parent=nullptr);


private:


signals:


protected:
    void run() override;
};

#endif // BATTLETHREAD_H

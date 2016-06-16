#ifndef STREAMERVIEW_H
#define STREAMERVIEW_H

#include <QWidget>
#include "entities.h"

namespace Ui {
class StreamerView;
}

class StreamerView : public QWidget
{
    Q_OBJECT

public:
    explicit StreamerView(QWidget *parent = 0);
    void load(StreamerDesc * s);
    static QString hourToString(int h);
    ~StreamerView();

private:
    Ui::StreamerView *ui;
};

#endif // STREAMERVIEW_H

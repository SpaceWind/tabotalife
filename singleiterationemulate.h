#ifndef SINGLEITERATIONEMULATE_H
#define SINGLEITERATIONEMULATE_H

#include <QWidget>

namespace Ui {
class SingleIterationEmulate;
}

class SingleIterationEmulate : public QWidget
{
    Q_OBJECT

public:
    explicit SingleIterationEmulate(QWidget *parent = 0);
    ~SingleIterationEmulate();

private slots:
    void on_pushButton_clicked();

private:
    Ui::SingleIterationEmulate *ui;
};

#endif // SINGLEITERATIONEMULATE_H

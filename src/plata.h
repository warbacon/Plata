#ifndef PLATA_H
#define PLATA_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class Plata; }
QT_END_NAMESPACE

class Plata : public QMainWindow
{
    Q_OBJECT

public:
    Plata(QWidget *parent = nullptr);
    ~Plata();

private:
    Ui::Plata *ui;

private slots:
    void open_document();
};
#endif // PLATA_H

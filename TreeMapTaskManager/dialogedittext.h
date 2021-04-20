#ifndef DIALOGEDITTEXT_H
#define DIALOGEDITTEXT_H

#include <QDialog>

namespace Ui {
class cDialogEditText;
}

class cDialogEditText : public QDialog
{
    Q_OBJECT

public:
    explicit cDialogEditText(QWidget *parent = nullptr);
    ~cDialogEditText();

    int ShowIt();

    void SetContent(const QString& str);
    QString GetContent();

private:
    Ui::cDialogEditText *ui;
};

#endif // DIALOGEDITTEXT_H

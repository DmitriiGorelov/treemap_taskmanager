#pragma once

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QComboBox>
#include <QScreen>
#include <QTreeView>
#include <QStandardItemModel>
#include <QAbstractTableModel>
#include <QAbstractItemModel>
#include <QResizeEvent>

class cFocusedTaskPopUp : public QWidget
{
    Q_OBJECT
public:
    cFocusedTaskPopUp(QWidget *parent);
    ~cFocusedTaskPopUp()
    {

    }

    void UpdateGeometry(QPoint point, int newHeight, int parentWidth, int parentHeight);
    void SetValueRange(int min, int max, int value);
    void SetUsers(const QString& user, const QStringList& allUsers);

protected:
    void resizeEvent(QResizeEvent* event) final;
    void paintEvent(QPaintEvent* event) final {}

private slots:
    void bViewTaskClicked();
    void bEditTaskClicked();
    void bDeleteTaskClicked();
    void bTaskColorClicked();
    void bAddTaskClicked();
    void sTaskVolumeChanged(int position);
    void sUserOfTaskChanged(const QString &);

private:
signals:
    void ViewTask();
    void EditTask();
    void DeleteTask();
    void AddTask();
    void ColorTask(QColor col);
    void TaskVolumeChanged(int position);
    void UserOfTaskChanged(const QString);

private:
    std::vector<QObject*> m_b;
    int comboboxes;
    //QPushButton* m_bDeleteTask;
};

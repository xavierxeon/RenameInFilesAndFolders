#ifndef MainWidgetH
#define MainWidgetH

#include <QWidget>
#include "ui_MainWidget.h"

class MainWidget : public QWidget, private Ui::MainWidget
{
   Q_OBJECT
public:
   MainWidget(QWidget* parent);
private slots:
   void delayLoad();
private:
   void showEvent(QShowEvent* se) override;
   void closeEvent(QCloseEvent* ce) override;
};

#endif // MainWidgetH

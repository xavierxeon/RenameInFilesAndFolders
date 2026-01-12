#ifndef MainWidgetH
#define MainWidgetH

#include <QWidget>

#include <QSplitter>

#include "InputWidget.h"
#include "PreviewWidget.h"

class MainWidget : public QWidget
{
   Q_OBJECT

public:
   MainWidget(QWidget* parent);

private slots:
   void delayLoad();

private:
   void showEvent(QShowEvent* se) override;
   void closeEvent(QCloseEvent* ce) override;

private:
   QSplitter* splitter;
   InputWidget* inputWidget;
   PreviewWidget* previewWidget;
};

#endif // MainWidgetH

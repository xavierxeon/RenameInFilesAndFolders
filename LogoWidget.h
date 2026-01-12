#ifndef LogoWidgetH
#define LogoWidgetH

#include <QWidget>
#include "ui_LogoWidget.h"

class LogoWidget : public QWidget, private Ui::LogoWidget
{
   Q_OBJECT
public:
   LogoWidget(QWidget* parent);
};

#endif // LogoWidgetH

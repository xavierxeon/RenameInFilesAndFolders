#include "LogoWidget.h"

#include <QIcon>

LogoWidget::LogoWidget(QWidget* parent)
   : QWidget(parent)
{
   setupUi(this);

   QIcon icon = QIcon(":/BulkRenamer.svg");
   logo->setPixmap(icon.pixmap(256, 256));
}

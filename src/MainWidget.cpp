#include "MainWidget.h"

#include <QApplication>

#include <QSettings>
#include <QTimer>

MainWidget::MainWidget(QWidget* parent)
   : QWidget(parent)
{
   setupUi(this);

   QIcon icon(":/BulkRenamer.svg");
   setWindowIcon(icon);

   connect(inputWidget, &InputWidget::inputChanged, previewWidget, &PreviewWidget::updatePreview);
   connect(inputWidget, &InputWidget::executeRename, previewWidget, &PreviewWidget::executeRename);

   QSettings qtsettings;
   restoreGeometry(qtsettings.value("geometry").toByteArray());
   splitter->restoreState(qtsettings.value("splitter").toByteArray());
}

void MainWidget::delayLoad()
{
   inputWidget->load();
}

void MainWidget::showEvent(QShowEvent* se)
{
   QTimer::singleShot(200, this, &MainWidget::delayLoad);
   QWidget::showEvent(se);
}

void MainWidget::closeEvent(QCloseEvent* ce)
{
   QSettings qtsettings;
   qtsettings.setValue("geometry", saveGeometry());
   qtsettings.setValue("splitter", splitter->saveState());

   inputWidget->save();

   QWidget::closeEvent(ce);
}

// main function

int main(int argc, char** argv)
{
   QApplication app(argc, argv);

   app.setOrganizationName("xavierxeon");
   app.setOrganizationDomain("schweinesystem.eu");
   app.setApplicationName("RenameInFilesAndFolders");

   MainWidget mw(nullptr);
   mw.show();

   return app.exec();
}

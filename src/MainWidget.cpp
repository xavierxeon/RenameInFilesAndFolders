#include "MainWidget.h"

#include <QApplication>
#include <QTimer>

#include "Settings.h"

MainWidget::MainWidget(QWidget* parent)
   : QWidget(parent)
{
   setupUi(this);

   QIcon icon(":/BulkRenamer.svg");
   setWindowIcon(icon);

   connect(inputWidget, &InputWidget::inputChanged, previewWidget, &PreviewWidget::updatePreview);
   connect(inputWidget, &InputWidget::executeRename, previewWidget, &PreviewWidget::executeRename);

   Settings settings;
   restoreGeometry(settings.bytes("geometry"));
   splitter->restoreState(settings.bytes("splitter"));
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
   Settings settings;
   settings.write("geometry", saveGeometry());
   settings.write("splitter", splitter->saveState());

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

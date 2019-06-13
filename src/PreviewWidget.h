#ifndef PreviewWidgetH
#define PreviewWidgetH

#include <QWidget>
#include "ui_PreviewWidget.h"

#include <QDateTime>

#include "FileSystemModel.h"
#include "RenameModel.h"

class PreviewWidget : public QWidget, private Ui::PreviewWidget
{
   Q_OBJECT
public:
   PreviewWidget(QWidget* parent);
public slots:
   void updatePreview(const QString& search
                      , const QString& replace
                      , const QStringList& directoryList
                      , bool replaceInFiles);
   void executeRename();
private slots:
   void updateProgress(int value, int max);
private:
   FileSystemModel* fileSystemModel;
   RenameModel* renameModel;
   int busyIndex;
   QDateTime lastProgress;
};

#endif // PreviewWidgetH

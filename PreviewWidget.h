#ifndef PreviewWidgetH
#define PreviewWidgetH

#include "ui_PreviewWidget.h"
#include <QWidget>

#include "RenameModel.h"

class PreviewWidget : public QWidget, private Ui::PreviewWidget
{
   Q_OBJECT

public:
   PreviewWidget(QWidget* parent);

public slots:
   void updatePreview(const QString& search, const QString& replace, const QStringList& directoryList, bool replaceInFiles);
   void executeRename();

private slots:
   void updateProgress(int value, int max);

private:
   RenameModel* renameModel;
   int busyIndex;
};

#endif // PreviewWidgetH

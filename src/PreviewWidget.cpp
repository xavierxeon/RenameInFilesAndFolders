#include "PreviewWidget.h"

#include <QApplication>

PreviewWidget::PreviewWidget(QWidget* parent)
    : QWidget(parent)
    , fileSystemModel(nullptr)
    , renameModel(nullptr)
    , busyIndex(0)
    , lastProgress(QDateTime::currentDateTime())
{
   setupUi(this);

   fileSystemModel = new FileSystemModel(this);
   renameModel = new RenameModel(fileSystemModel);

   previewTree->setModel(renameModel);

   connect(fileSystemModel, &FileSystemModel::progressUpdate, this, &PreviewWidget::updateProgress);
   connect(renameModel, &RenameModel::progressUpdate, this, &PreviewWidget::updateProgress);
}

void PreviewWidget::updatePreview(const QString& search
                   , const QString& replace
                   , const QStringList& directoryList
                   , bool replaceInFiles)
{
   previewTree->setEnabled(false);
   QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);

   fileSystemModel->update(directoryList);

   bool updateNeeded = false;

   if(renameModel->search != search)
   {
      renameModel->search = search;
      updateNeeded = true;
   }
   if(renameModel->replace != replace)
   {
      renameModel->replace = replace;
      updateNeeded = true;
   }
   if(renameModel->replaceInFiles != replaceInFiles)
   {
      renameModel->replaceInFiles = replaceInFiles;
      updateNeeded = true;
   }

   if(updateNeeded)
      renameModel->update();

   previewTree->setEnabled(true);
   previewTree->resizeColumnToContents(0);
   previewTree->resizeColumnToContents(1);
   previewTree->resizeColumnToContents(2);
}

void PreviewWidget::executeRename()
{
   renameModel->execute();
}

void PreviewWidget::updateProgress(int value, int max)
{
   if(value < 0)
   {
      progressBar->setRange(0, 1);
      progressBar->setValue(0);
   }
   else
   {
      QDateTime now = QDateTime::currentDateTime();
      qint64 timePassed = lastProgress.secsTo(now);
      if(timePassed < 2)
         return;

      lastProgress = now;

      if(0 == max && 0 == value)
      {
         value = ++busyIndex;
         if(busyIndex >= 100)
            busyIndex = 0;
      }
      progressBar->setRange(0, max);
      progressBar->setValue(value);
   }

   QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
}

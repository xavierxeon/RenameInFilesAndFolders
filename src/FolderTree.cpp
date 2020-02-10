#include "FolderTree.h"

#include <QMimeData>
#include <QFileInfo>
#include <QDragEnterEvent>

FolderTree::FolderTree(QWidget* parent)
    : QTreeWidget(parent)
{
   setAcceptDrops(true);
}

void FolderTree::dragEnterEvent(QDragEnterEvent* de)
{
   if(de->mimeData()->hasUrls())
      de->acceptProposedAction();
}

void FolderTree::dragMoveEvent(QDragMoveEvent* de)
{
   if(de->mimeData()->hasUrls())
      de->acceptProposedAction();
}

void FolderTree::dropEvent(QDropEvent* de)
{
   bool hasUpdate = false;
   for(const QUrl& url : de->mimeData()->urls())
   {
      QString localFile = url.toLocalFile();
      if(localFile.isEmpty())
         continue;

      QFileInfo info(localFile);
      if(!info.isDir())
         continue;

      emit addDirectory(info.filePath(), true);
      hasUpdate = true;
   }

   if(hasUpdate)
      emit addDirectory(QString(), false);
}

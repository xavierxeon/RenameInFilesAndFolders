#include "FileSystemModel.h"

#include <QDir>
#include <QFileInfo>

FileSystemModel::FileSystemModel(QObject* parent)
   : QStandardItemModel(parent)
   , directoryList()
   , fileIcon(":/File.svg")
   , folderIcon(":/Folder.svg")

{
   setHorizontalHeaderLabels( {"old name", "new name", "# in file"} );
}


void FileSystemModel::update(const QStringList& newDirectoryList)
{
   if(newDirectoryList == directoryList)
      return;

   directoryList = newDirectoryList;
   clear();
   setHorizontalHeaderLabels( {"old name", "new name", "# in file"} );

   emit progressUpdate(0, 0);

   for(const QString& directory : directoryList)
   {
      QFileInfo info(directory);
      if(info.isDir())
         createAndTraverseChildren(info, invisibleRootItem());
   }

   emit progressUpdate(-1, 0);
}

void FileSystemModel::createAndTraverseChildren(const QFileInfo& info, QStandardItem* parent)
{
   emit progressUpdate(0, 0);

   const QString name = info.fileName();
   QStandardItem* oldNameItem = new QStandardItem(name);
   oldNameItem->setData(info.filePath());

   QStandardItem* newNameItem = new QStandardItem("");
   QStandardItem* countItem = new QStandardItem("");

   QList<QStandardItem*> itemList = { oldNameItem, newNameItem, countItem };
   parent->appendRow(itemList);

   if(info.isDir())
   {
      oldNameItem->setIcon(folderIcon);

      QDir dir(info.filePath());
      dir.setFilter(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
      dir.setSorting(QDir::Name);

      for(QFileInfo childInfo : dir.entryInfoList())
      {
         createAndTraverseChildren(childInfo, oldNameItem);
      }
   }
   else
   {
      oldNameItem->setIcon(fileIcon);
   }
}

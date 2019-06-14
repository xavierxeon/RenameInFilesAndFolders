#include "RenameModel.h"

#include <QFile>
#include <QFileInfo>
#include <QDir>

RenameModel::RenameModel(QObject* parent)
   : QStandardItemModel(parent)
   , search()
   , replace()
   , directoryList()
   , replaceInFiles(false)
   , fileIcon(":/File.svg")
   , folderIcon(":/Folder.svg")
{
   setHorizontalHeaderLabels( {"old name", "new name", "# in file"} );
}

void RenameModel::update()
{
   clear();
   setHorizontalHeaderLabels( {"old name", "new name", "# in file"} );

   if(directoryList.isEmpty())
       return;
   if(search.isEmpty() || replace.isEmpty())
       return;

   emit progressUpdate(0, 0);

   for(const QString& directory : directoryList)
       parseRecursive(directory, invisibleRootItem());
   removeUnchangedItems();

   emit progressUpdate(-1, 0);
}

void RenameModel::execute()
{
   int topLevelItemCount = invisibleRootItem()->rowCount();
   emit progressUpdate(0, topLevelItemCount);

   for(int row = 0; row < topLevelItemCount; row++)
   {
      QStandardItem* topLevelItem = invisibleRootItem()->child(row);
      traverseChildrenAndRenameItem(topLevelItem, invisibleRootItem(), row);
      emit progressUpdate(row, topLevelItemCount);
   }
   update();
}

void RenameModel::parseRecursive(const QString& dirPath, QStandardItem* parent)
{
   QDir dir(dirPath);
   dir.setFilter(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
   dir.setSorting(QDir::Name);

   for(QFileInfo fileInfo : dir.entryInfoList())
   {
      emit progressUpdate(0, 0);

      const QString oldName = fileInfo.fileName();
      QStandardItem* oldNameItem = new QStandardItem(oldName);
      oldNameItem->setData(fileInfo.filePath());

      QStandardItem* newNameItem = new QStandardItem("");
      QStandardItem* countItem = new QStandardItem("");
      bool willChange = false;

      QString newName = oldName;
      newName.replace(search, replace);
      if(oldName != newName)
      {
         newNameItem->setText(newName);
         willChange = true;
      }

      if(fileInfo.isFile() && replaceInFiles)
      {
         const int count = countChangesInFile(fileInfo.filePath());
         if(count > 0)
         {
            countItem->setText(QString::number(count));
            willChange = true;
         }
      }

      QList<QStandardItem*> itemList = { oldNameItem, newNameItem, countItem };
      parent->appendRow(itemList);

      //qDebug() << dirPath << willChange << oldNameItem->text() << newNameItem->text() << countItem->text();

      if(fileInfo.isDir())
      {
         oldNameItem->setIcon(folderIcon);
         parseRecursive(fileInfo.filePath(), oldNameItem);
      }
      else
      {
         oldNameItem->setIcon(fileIcon);
      }

      for(QStandardItem* item : itemList)
      {
         item->setEnabled(willChange);
         item->setEditable(false);
      }

   }
}

int RenameModel::countChangesInFile(const QString& fileName) const
{
   QFile file(fileName);
   if(!file.open(QIODevice::ReadOnly))
      return 0;

   QString content = QString::fromUtf8(file.readAll());
   file.close();

   int count = 0;
   int index = content.indexOf(search);
   while(index >= 0)
   {
      count++;
      index = content.indexOf(search, index + 1);
   }

   return count;
}

void RenameModel::removeUnchangedItems()
{
   QList<int> removeList;
   int topLevelItemCount = invisibleRootItem()->rowCount();

   for(int row = topLevelItemCount - 1; row >= 0; row--)
   {
      QStandardItem* topLevelItem = invisibleRootItem()->child(row);
      if(!itemIsEnbaledOrHasEnabledChildren(topLevelItem))
          removeList.append(row);
   }

   for(int row : removeList)
       invisibleRootItem()->removeRow(row);
}

bool RenameModel::itemIsEnbaledOrHasEnabledChildren(QStandardItem* item) const
{
   if(item->isEnabled())
       return true;

   for(int row = 0; row < item->rowCount(); row++)
   {
      QStandardItem* childItem = item->child(row);
      if(itemIsEnbaledOrHasEnabledChildren(childItem))
          return true;
   }

   return false;
}

void RenameModel::traverseChildrenAndRenameItem(QStandardItem* item, QStandardItem* parent, int row)
{
   for(int childRow = 0; childRow < item->rowCount(); childRow++)
   {
       QStandardItem* childItem = item->child(childRow);
       traverseChildrenAndRenameItem(childItem, item, childRow);
   }
   if(!item->isEnabled())
       return;

   emit progressUpdate(0, 0);

   const QString path = item->data().toString();
   const QString oldName = item->text();

   QStandardItem* newNameItem = parent->child(row, 1);
   const QString newName = newNameItem->text();

   QStandardItem* countItem = parent->child(row, 2);
   int count = countItem->text().isEmpty() ? 0 : countItem->text().toInt();

   if(replaceInFiles && count > 0)
       replaceTextInFile(path);

   if(!newName.isEmpty())
   {
      QString newPath = path;
      newPath.replace(oldName, newName);
      bool ok = QFile::rename(path, newPath);
      if(!ok)
         qDebug() << "unable to rename" << path << newPath;
   }
}

void RenameModel::replaceTextInFile(const QString& fileName) const
{
   QFile file(fileName);
   if(!file.open(QIODevice::ReadOnly))
      return;

   QString content = QString::fromUtf8(file.readAll());
   file.close();

   content.replace(search, replace);

   if(!file.open(QIODevice::WriteOnly))
      return;

   file.write(content.toUtf8());
   file.close();
}

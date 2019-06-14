#ifndef RenameModelH
#define RenameModelH

#include <QSortFilterProxyModel>

#include "FileSystemModel.h"

class RenameModel : public QSortFilterProxyModel
{
   Q_OBJECT
public:
   RenameModel(FileSystemModel* model);
signals:
   void progressUpdate(int value, int max);
private:
   friend class PreviewWidget;
private:
   void update();
   void execute();
   void parseRecursive(const QString& dirPath, QStandardItem* parent);
   int countChangesInFile(const QString& fileName) const;
   void removeUnchangedItems();
   bool itemIsEnbaledOrHasEnabledChildren(QStandardItem* item) const;
   void traverseChildrenAndRenameItem(QStandardItem* item, QStandardItem* parent, int row);
   void replaceTextInFile(const QString& fileName) const;
private:
   FileSystemModel* model;
   QString search;
   QString replace;   
   bool replaceInFiles;
};

#endif // RenameModelH

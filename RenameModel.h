#ifndef RenameModelH
#define RenameModelH

#include <QStandardItemModel>

class RenameModel : public QStandardItemModel
{
   Q_OBJECT

public:
   RenameModel(QObject* parent);

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
   QString search;
   QString replace;
   QStringList directoryList;
   bool replaceInFiles;
   QIcon fileIcon;
   QIcon folderIcon;
};

#endif // RenameModelH

#ifndef FileSystemModelH
#define FileSystemModelH

#include <QStandardItemModel>

#include <QFileInfo>

class FileSystemModel : public QStandardItemModel
{
   Q_OBJECT
public:
   FileSystemModel(QObject* parent);
signals:
   void progressUpdate(int value, int max);
public:
   void update(const QStringList& newDirectoryList);
private:
   void createAndTraverseChildren(const QFileInfo& info, QStandardItem* parent);
private:
   QStringList directoryList;
   QIcon fileIcon;
   QIcon folderIcon;
};

#endif // FileSystemModelH

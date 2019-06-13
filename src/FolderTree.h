#ifndef FolderTreeH
#define FolderTreeH

#include <QTreeWidget>

class FolderTree : public QTreeWidget
{
   Q_OBJECT
public:
   FolderTree(QWidget* parent);
signals:
   void addDirectory(const QString& directory, bool supressUpdate);
private:
   void dragEnterEvent(QDragEnterEvent* de) override;
   void dragMoveEvent(QDragMoveEvent* de) override;
   void dropEvent(QDropEvent* de) override;
};

#endif // FolderTreeH

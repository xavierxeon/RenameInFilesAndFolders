#ifndef InputWidgetH
#define InputWidgetH

#include "ui_InputWidget.h"
#include <QWidget>

class InputWidget : public QWidget, private Ui::InputWidget
{
   Q_OBJECT
public:
   InputWidget(QWidget* parent);

signals:
   void inputChanged(const QString& search, const QString& replace, const QStringList& directoryList, bool replaceInFiles);
   void executeRename();

public:
   void load();
   void save();
   SearchDrop::Data getSearchData() const;

private slots:
   void searchChanged();
   void replaceChanged();
   void update(const SearchDrop::Data& data);
   void replaceInFilesChanged();
   void removeDirectory();
   void updatePreview();
   void addDirectoryViaDialog();
   void addDirectory(const QString& directory, bool supressUpdate = false);

private:
   QString search;
   QString replace;
   QStringList directoryList;
   bool replaceInFiles;
};

#endif // InputWidgetH

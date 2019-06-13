#include "InputWidget.h"

#include <QSettings>
#include <QFileDialog>
#include <QTreeWidgetItem>

#include "Settings.h"

InputWidget::InputWidget(QWidget* parent)
   : QWidget(parent)
   , search()
   , replace()
   , directoryList()
   , replaceInFiles(false)
{
   setupUi(this);

   dropZone->setInputWidget(this);

   connect(searchEdit, &QLineEdit::editingFinished, this, &InputWidget::searchChanged);
   connect(replaceEdit, &QLineEdit::editingFinished, this, &InputWidget::replaceChanged);
   connect(dropZone, &SearchDrop::update, this, &InputWidget::update);

   directoryTree->setAcceptDrops(true);
   connect(directoryTree, &FolderTree::addDirectory, this, &InputWidget::addDirectory);

   QString toolButtonStyleSheet = "QToolButton {border: none; }";
   toolButtonStyleSheet += "QToolButton:pressed {background-color: #aaaaaa;}";

   QIcon addIcon(":/Add.svg");
   addDirectoryButton->setIcon(addIcon);
   addDirectoryButton->setStyleSheet(toolButtonStyleSheet);
   connect(addDirectoryButton, &QToolButton::clicked, this, &InputWidget::addDirectoryViaDialog);

   QIcon removeIcon(":/Remove.svg");
   removeDirectoryButton->setIcon(removeIcon);
   removeDirectoryButton->setStyleSheet(toolButtonStyleSheet);
   connect(removeDirectoryButton, &QToolButton::clicked, this, &InputWidget::removeDirectory);

   connect(replaceInFilesCheck, &QCheckBox::clicked, this, &InputWidget::replaceInFilesChanged);
   connect(executeButton, &QPushButton::clicked, this, &InputWidget::executeRename);
}

void InputWidget::load()
{
   Settings settings;

   search = settings.string("search");
   searchEdit->setText(search);

   replace = settings.string("replace");
   replaceEdit->setText(replace);

   const QStringList& dirList = settings.stringList("directories");
   for(const QString& directory : dirList)
      addDirectory(directory, true);

   replaceInFiles = settings.boolean("replaceInFiles");
   replaceInFilesCheck->setChecked(replaceInFiles ? Qt::Checked : Qt::Unchecked);

   updatePreview();
}

void InputWidget::save()
{
   Settings settings;

   settings.write("search", search);
   settings.write("replace", replace);
   settings.write("directories", directoryList);
   settings.write("replaceInFiles", replaceInFiles);
}

SearchDrop::Data InputWidget::getSearchData() const
{
   return { search, replace };
}

void InputWidget::searchChanged()
{
   if(searchEdit->text() == search)
       return;

   search = searchEdit->text();
   updatePreview();
}

void InputWidget::replaceChanged()
{
   if(replaceEdit->text() == replace)
       return;

   replace = replaceEdit->text();
   updatePreview();
}

void InputWidget::update(const SearchDrop::Data& data)
{
   bool hasChanged = false;
   if(data.search != search)
   {
      search = data.search;
      searchEdit->setText(search);
      hasChanged = true;
   }
   if(data.replace != replace)
   {
      replace = data.replace;
      replaceEdit->setText(replace);
      hasChanged = true;
   }

   if(hasChanged)
      updatePreview();
}

void InputWidget::replaceInFilesChanged()
{
  const bool replaceInFilesTest = (replaceInFilesCheck->checkState() == Qt::Checked);
   if(replaceInFilesTest == replaceInFiles)
       return;

   replaceInFiles = replaceInFilesTest;
   updatePreview();
}

void InputWidget::addDirectoryViaDialog()
{
   const QString directory = QFileDialog::getExistingDirectory(this, "Choose folder");
   if(directory.isEmpty()) // user has pressed cancel on dialog
       return;

   addDirectory(directory);
}

void InputWidget::addDirectory(const QString& directory, bool supressUpdate)
{
   if(directoryList.contains(directory ))
       return;

   if(!directory.isEmpty())
   {
      directoryList.append(directory);
      directoryList.sort();
   }

   directoryTree->clear();

   for(const QString& directory : directoryList)
   {
       QTreeWidgetItem* item = new QTreeWidgetItem(directoryTree);
       item->setText(0, directory);
   }

   if(!supressUpdate)
       updatePreview();
}
void InputWidget::removeDirectory()
{
   QTreeWidgetItem* item = directoryTree->currentItem();
   if(!item)
      return;

   const QString& path = item->text(0);
   if(!directoryList.contains(path))
      return;

   const int index = directoryTree->indexOfTopLevelItem(item);
   directoryTree->takeTopLevelItem(index);

   directoryList.removeAll(path);
   updatePreview();
}

void InputWidget::updatePreview()
{
   setEnabled(false);
   emit inputChanged(search, replace, directoryList, replaceInFiles);
   setEnabled(true);
}

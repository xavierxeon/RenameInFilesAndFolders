#include "SearchDrop.h"

#include <QDrag>
#include <QMimeData>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QTemporaryDir>
#include <QMouseEvent>

#include "InputWidget.h"

SearchDrop::SearchDrop(QWidget* parent)
    : QLabel(parent)
    , dragPixmap()
    , inputWidget(nullptr)
{
   QIcon dragIcon(":/BulkRenamer.svg");
   dragPixmap = dragIcon.pixmap(32, 32);

   hide();

   setAcceptDrops(true);
}

void SearchDrop::setInputWidget(InputWidget* inputWidget)
{
   this->inputWidget = inputWidget;

   if(!inputWidget)
      return hide();

   show();
   setToolTip("Drag from here to save search patterns to file.\nOr drop search pattern files here.");

   QIcon dropIcon(":/Drop.svg");
   setPixmap(dropIcon.pixmap(32, 32));
}

void SearchDrop::mousePressEvent(QMouseEvent* me)
{
   if (me->button() != Qt::LeftButton)
      return;

   if(!inputWidget)
      return;

   const Data data = inputWidget->getSearchData();

   QTemporaryDir tmpDir;
   QString filePath = tmpDir.filePath(data.search + "_" + data.replace + ".bulkrename");
   QFile tmpFile(filePath);
   if(!tmpFile.open(QIODevice::WriteOnly))
      return;

   QJsonObject dataObject;
   dataObject["search"] = data.search;
   dataObject["replace"] = data.replace;

   QJsonDocument doc(dataObject);
   tmpFile.write(doc.toJson());
   tmpFile.close();

   QMimeData* mimeData = new QMimeData;
   mimeData->setUrls( { QUrl::fromLocalFile(filePath) });
   mimeData->setText("Hello");

   QDrag* drag = new QDrag(this);
   drag->setMimeData(mimeData);
   drag->setPixmap(dragPixmap);

   Qt::DropAction dropAction = drag->exec();
   if(Qt::MoveAction != dropAction)
      QFile::remove(filePath);
}

void SearchDrop::dragEnterEvent(QDragEnterEvent* de)
{
   QString payload = localFile(de);
   if(!payload.isEmpty())
      de->acceptProposedAction();
}

void SearchDrop::dropEvent(QDropEvent* de)
{
   QString payload = localFile(de);
   if(payload.isEmpty())
      return;

   QFile file(payload);
   if(!file.open(QIODevice::ReadOnly))
      return;

   QJsonParseError error;
   QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &error);
   file.close();

   QJsonObject dataObject = doc.object();
   Data data = { dataObject["search"].toString(), dataObject["replace"].toString() };
   emit update(data);
}

QString SearchDrop::localFile(QDropEvent* de)
{
   for(const QUrl& url : de->mimeData()->urls())
   {
      QString localFile = url.toLocalFile();
      if(localFile.isEmpty())
         continue;

      QFileInfo info(localFile);
      if(info.suffix() != "bulkrename")
         continue;

      return localFile;
   }

   return QString();
}



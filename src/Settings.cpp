#include "Settings.h"

#include <QStandardPaths>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDir>

QJsonObject Settings::object = QJsonObject();
quint32 Settings::useCount = 0;

Settings::Settings()
{
   if(0 == useCount) // read new object
   {
      QFile file(fileName());
      if(file.open(QIODevice::ReadOnly)) // file exists
      {
         QJsonParseError error;
         QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &error);
         file.close();

         object = doc.object();
      }
   }
   useCount++;
}

Settings::~Settings()
{
   if(0 == useCount)
      Q_ASSERT(false); // this should not happen!

   useCount--;
   if(0 < useCount) // object still in use
      return;

   // save object
   QFile file(fileName());
   if(!file.open(QIODevice::WriteOnly))
      return;

   QJsonDocument doc(object);
   file.write(doc.toJson());
   file.close();
}

void Settings::write(const QString& key, const QString& value)
{
   object[key] = value;
}

void Settings::write(const QString& key, const QStringList& value)
{
   QJsonArray array;
   for(const QString& content : value)
      array.append(content);

   object[key] = array;
}

void Settings::write(const QString& key, const QByteArray& value)
{
   QByteArray asciiValue = value.toBase64();
   object[key] = QString::fromUtf8(asciiValue);
}

void Settings::write(const QString& key, const int& value)
{
   object[key] = value;
}

void Settings::write(const QString& key, const double& value)
{
   object[key] = value;
}

void Settings::write(const QString& key, bool value)
{
   object[key] = value;
}

QString Settings::string(const QString& key) const
{
   QJsonValue value = object[key];
   if(value.isNull())
      return QString();

   return value.toString();
}

QStringList Settings::stringList(const QString& key) const
{
   QJsonValue value = object[key];
   if(value.isNull())
      return QStringList();

   QStringList list;
   for(QJsonValue value : object[key].toArray())
      list.append(value.toString());

   return list;
}

QByteArray Settings::bytes(const QString& key) const
{
   QJsonValue value = object[key];
   if(value.isNull())
      return QByteArray();

   QByteArray asciiValue = value.toString().toUtf8();
   return QByteArray::fromBase64(asciiValue);
}

int Settings::integer(const QString& key) const
{
   QJsonValue value = object[key];
   if(value.isNull())
      return 0;

   return value.toInt();
}

double Settings::real(const QString& key) const
{
   QJsonValue value = object[key];
   if(value.isNull())
      return 0.0;

   return value.toDouble();
}

bool Settings::boolean(const QString& key) const
{
   QJsonValue value = object[key];
   if(value.isNull())
      return false;

   return object[key].toBool();
}

const QString& Settings::fileName() const
{
   // do not make variable a class static, because QApplication needs to be constructed first!
   static const QString settingsFileName = [&]()
   {
      const QString path = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
      QDir().mkpath(path);
      return path + "/settings.json";
   }();

   return settingsFileName;
}

#ifndef SettingsH
#define SettingsH

#include <QJsonObject>

class Settings
{
public:
   Settings();
   ~Settings();

public:
   void write(const QString& key, const QString& value);
   void write(const QString& key, const QStringList& value);
   void write(const QString& key, const QByteArray& value);
   void write(const QString& key, const int& value);
   void write(const QString& key, const double& value);
   void write(const QString& key, bool value);

   QString string(const QString& key) const;
   QStringList stringList(const QString& key) const;
   QByteArray bytes(const QString& key) const;
   int integer(const QString& key) const;
   double real(const QString& key) const;
   bool boolean(const QString& key) const;

private:
   const QString& fileName() const;

private:
   static QJsonObject object;
   static quint32 useCount;
};

#endif // SettingsH

#ifndef SearchDropH
#define SearchDropH

#include <QLabel>

#include <QIcon>

class InputWidget;

class SearchDrop : public QLabel
{
   Q_OBJECT
public:
   struct Data
   {
      const QString search;
      const QString replace;
   };
public:
   SearchDrop(QWidget* parent);
signals:
   void update(const Data& data);
public:
   void setInputWidget(InputWidget* inputWidget);
private:
   void mousePressEvent(QMouseEvent* me) override;
   void dragEnterEvent(QDragEnterEvent* de) override;
   void dropEvent(QDropEvent* de) override;
   QString localFile(QDropEvent* de);
private:
   QPixmap dragPixmap;
   InputWidget* inputWidget;
};

#endif // SearchDropH

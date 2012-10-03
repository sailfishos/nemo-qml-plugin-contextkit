#ifndef _CK_QML_CK_PROPERTY_HPP_
#define _CK_QML_CK_PROPERTY_HPP_

#include <QObject>
#include <QVariant>
#include <QString>
#include <QScopedPointer>

class ContextProperty;

namespace ContextKitQml
{

class Property : public QObject
{
        Q_OBJECT;
        Q_PROPERTY(QString key READ key WRITE setKey);
        Q_PROPERTY(QVariant value READ value WRITE setDefaultValue
                   NOTIFY valueChanged);
        Q_CLASSINFO("DefaultProperty", "value");
public:
        Property(QObject *parent = 0);
        ~Property();

        QString key() const;
        void setKey(QString const &);

        QVariant value() const;
        void setDefaultValue(QVariant const &);
        
        Q_INVOKABLE void subscribe() const;
        Q_INVOKABLE void unsubscribe() const;

signals:
        void valueChanged();

private:
        QScopedPointer<ContextProperty> prop;
        QVariant default_value;
};

}

#endif // _CK_QML_CK_PROPERTY_HPP_

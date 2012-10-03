#include "ck_property.hpp"
#include <contextproperty.h>

namespace ContextKitQml
{

Property::Property(QObject *parent) : QObject(parent)
{
}

Property::~Property()
{
}

QString Property::key() const
{
        return prop ? prop->key() : QString("");
}

void Property::setKey(QString const &key)
{
        prop.reset(new ContextProperty(key));
        connect(prop.data(), SIGNAL(valueChanged()),
                SIGNAL(valueChanged()));
}

void Property::setDefaultValue(QVariant const &value)
{
        default_value = value;
}

void Property::subscribe() const
{
        if (prop)
                prop->subscribe();
}

void Property::unsubscribe() const
{
        if (prop)
                prop->unsubscribe();
}

QVariant Property::value() const
{
        return prop ? prop->value(default_value) : default_value;
}

}

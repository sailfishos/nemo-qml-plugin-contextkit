#include "ck_property.hpp"

#include <QtDeclarative/qdeclarative.h>
#include <QtDeclarative/QDeclarativeExtensionPlugin>

namespace ContextKitQml
{

class Plugin : public QDeclarativeExtensionPlugin
{
public:
    void registerTypes(char const *uri)
    {
        qmlRegisterType<Property>(uri, 1, 0, "ContextProperty");
    }
};
 
}

Q_EXPORT_PLUGIN2(ContextKit, ContextKitQml::Plugin);

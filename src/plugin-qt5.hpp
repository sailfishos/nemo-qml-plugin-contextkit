#ifndef _CK_QML_PLUGIN_QT5_HPP_
#define _CK_QML_PLUGIN_QT5_HPP_

#include <QtQml/QQmlExtensionPlugin>

namespace ContextKitQml
{

class Plugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.freedesktop.contextkit")
public:
    void registerTypes(char const* uri);
};

}

#endif // _CK_QML_PLUGIN_QT5_HPP_

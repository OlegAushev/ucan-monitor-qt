#include <QAbstractListModel>
#include <QStringList>
#include <iostream>

#include <ucanopen_devices/srmdrive/server/srmdrive_server.h>
#include <ucanopen_devices/launchpad/server/launchpad_server.h>


namespace ucanopen {

class ServerSelector : public QObject {
    Q_OBJECT
    Q_PROPERTY(QStringList server_list READ server_list CONSTANT)
private:
    QStringList _server_list;
public:
    ServerSelector(std::pair<std::string_view, std::shared_ptr<srmdrive::Server>> server1,
                   std::pair<std::string_view, std::shared_ptr<launchpad::Server>> server2);

    QStringList server_list() const { return _server_list; }
    Q_INVOKABLE void create(QString name) { std::cout << name.toStdString() << std::endl; }
};

} // namespace ucanopen

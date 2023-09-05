#include "serverselector.h"


namespace ucanopen {

ServerSelector::ServerSelector(std::pair<std::string_view, std::shared_ptr<srmdrive::Server>> server1,
                               std::pair<std::string_view, std::shared_ptr<launchpad::Server>> server2) {
    _server_list << server1.first.data() << server2.first.data();
}

}


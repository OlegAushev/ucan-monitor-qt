#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QIcon>
#include <QQuickStyle>

#include <ucanopen/client/client.h>
#include <ucanopen_devices/serverselector/serverselector.h>
#include <ucanopen_devices/srmdrive/server/srmdrive_server.h>
#include <ucanopen_devices/launchpad/server/launchpad_server.h>


int main(int argc, char *argv[]) {
    QCoreApplication::setOrganizationName("M614");
	QCoreApplication::setApplicationName("ucan-monitor");

    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QGuiApplication app(argc, argv);
    QQuickStyle::setStyle("Fusion");
    app.setWindowIcon(QIcon(":/icons/ucan-monitor.png"));

    auto can_socket = std::make_shared<can::Socket>();
    auto ucanopen_client = std::make_shared<ucanopen::Client>(ucanopen::NodeId(0x14), can_socket);

    std::shared_ptr<srmdrive::Server> srmdrive_server;
    std::shared_ptr<launchpad::Server> launchpad_server;

    ucanopen::ServerSelector server_selector({"SRM-Drive-80", srmdrive_server}, {"LaunchPad", launchpad_server});

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("server_selector", &server_selector);

    const QUrl url("../serverselector_window.qml");
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
                     &app, []() { QCoreApplication::exit(-1); },
                     Qt::QueuedConnection);
    engine.load(url);

    return app.exec();

    // engine.rootContext()->setContextProperty("server_selector", &server_selector);
    // engine.load(QUrl(QStringLiteral("qrc:///serverselector_window.qml")));
    // if (engine.rootObjects().isEmpty()) {
    //     return -1;
    // }

	// return app.exec();
}
































// #include <ucanopen/client/client.h>
// #include <ucanopen_devices/srmdrive/server/srmdrive_server.h>
// #include <ucanopen_devices/crd600/server/crd600_server.h>
// #include <ucanopen_devices/launchpad/server/launchpad_server.h>
// #include <ucanopen_devices/bmsmain/server/bmsmain_server.h>
// #include <gnuplotter/gnuplotter.h>


// namespace api {
// extern void register_can_socket(std::shared_ptr<can::Socket> can_socket_);
// extern void register_ucanopen_client(std::shared_ptr<ucanopen::Client> ucanopen_client_);
// extern void register_srmdrive_server(std::shared_ptr<srmdrive::Server> srmdrive_server_);
// extern void register_crd600_server(std::shared_ptr<crd600::Server> crd600_server_);
// extern void register_launchpad_server(std::shared_ptr<launchpad::Server> launchpad_server_);
// extern void register_bmsmain_server(std::shared_ptr<bmsmain::Server> bmsmain_server_);
// }


// // used in VALA code
// bool backend_is_ready = false;
// const char* backend_ucanopen_server;
// const char* backend_ucanopen_server_list[4] = {"SRM-Drive-80", "CRD600", "LaunchPad", "BMS-Main"};
// const char* backend_ucanopen_server_config_category;


// namespace {
// std::thread thread_main;
// std::promise<void> signal_exit_main;
// }


// extern "C" 
// int backend_main_loop(std::future<void> signal_exit) {
//     Log() << "Started backend main loop thread. Thread id: " << std::this_thread::get_id() << ".\n" << LogPrefix::ok;

//     auto can_socket = std::make_shared<can::Socket>();
//     api::register_can_socket(can_socket);

//     auto ucanopen_client = std::make_shared<ucanopen::Client>(ucanopen::NodeId(0x14), can_socket);
//     api::register_ucanopen_client(ucanopen_client);

//     std::shared_ptr<srmdrive::Server> srmdrive_server;
//     std::shared_ptr<crd600::Server> crd600_server;
//     std::shared_ptr<launchpad::Server> launchpad_server;
//     std::shared_ptr<bmsmain::Server> bmsmain_server;

//     std::string server_name(backend_ucanopen_server);
//     if (server_name == "SRM-Drive-80") {
//         srmdrive_server = std::make_shared<srmdrive::Server>(can_socket, ucanopen::NodeId(0x01), server_name);
//         ucanopen_client->register_server(srmdrive_server);
//         api::register_srmdrive_server(srmdrive_server);
//     } else if (server_name == "CRD600") {
//         crd600_server = std::make_shared<crd600::Server>(can_socket, ucanopen::NodeId(0x01), server_name);
//         ucanopen_client->register_server(crd600_server);
//         api::register_crd600_server(crd600_server);
//     } else if (server_name == "LaunchPad") {
//         launchpad_server = std::make_shared<launchpad::Server>(can_socket, ucanopen::NodeId(0x01), server_name);
//         ucanopen_client->register_server(launchpad_server);

//         auto callback_create_tpdo1 = [launchpad_server](){ return launchpad_server->create_client_tpdo1(); };
//         auto callback_create_tpdo2 = [launchpad_server](){ return launchpad_server->create_client_tpdo2(); };
//         auto callback_create_tpdo3 = [launchpad_server](){ return launchpad_server->create_client_tpdo3(); };
//         auto callback_create_tpdo4 = [launchpad_server](){ return launchpad_server->create_client_tpdo4(); };

//         ucanopen_client->register_tpdo(ucanopen::TpdoType::tpdo1, std::chrono::milliseconds(50), callback_create_tpdo1);
//         ucanopen_client->register_tpdo(ucanopen::TpdoType::tpdo2, std::chrono::milliseconds(100), callback_create_tpdo2);
//         ucanopen_client->register_tpdo(ucanopen::TpdoType::tpdo3, std::chrono::milliseconds(250), callback_create_tpdo3);
//         ucanopen_client->register_tpdo(ucanopen::TpdoType::tpdo4, std::chrono::milliseconds(1000), callback_create_tpdo4);

//         api::register_launchpad_server(launchpad_server);
//     } else if (server_name == "BMS-Main") {
//         bmsmain_server = std::make_shared<bmsmain::Server>(can_socket, ucanopen::NodeId(0x20), server_name);
//         ucanopen_client->register_server(bmsmain_server);
//         api::register_bmsmain_server(bmsmain_server);
//     }

//     backend_ucanopen_server_config_category = ucanopen_client->server(server_name)->dictionary().config.config_category.data();

//     Log() << "Backend ready.\n" << LogPrefix::ok;
//     backend_is_ready = true;

//     while (signal_exit.wait_for(std::chrono::milliseconds(100)) == std::future_status::timeout) {
//         // wait for promise to exit
//     }

//     Log() << "Stopped backend main loop thread.\n" << LogPrefix::ok;
//     return 0;
// }


// extern "C"
// int backend_main_enter() {
//     Log() << "GUI thread id: " << std::this_thread::get_id() << ".\n" << LogPrefix::align;
//     Log() << "Starting new thread for backend main loop...\n" << LogPrefix::align;

//     std::future<void> signal_exit = signal_exit_main.get_future();
//     thread_main = std::thread(backend_main_loop, std::move(signal_exit));
//     return 0;
// }


// extern "C"
// void backend_main_exit() {
//     Log() << "Sending signal to backend main loop thread to stop...\n" << LogPrefix::align;

//     signal_exit_main.set_value();
//     thread_main.join();
// }


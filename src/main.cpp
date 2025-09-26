// main.cpp : Defines the entry point for the application.
//

#include <csignal>
#include <cxxopts.hpp>
#include <httplib.h>
#include <iostream>
#include <spdlog/spdlog.h>
#include <thread>

#include "debug_macros.h"
#include "inc1.h"
#include "logging.h"


static bool running = true;

void signal_handler(int signum) {
	spdlog::warn("Received signal {}, shutting down...", signum);
	running = false;
}

int main(int argc, char *argv[]) {
	// Parse CLI
	cxxopts::Options options("DebuggableTemplate", "Demo debuggable C++ program");
	options.add_options()
		("c,config", "Config file", cxxopts::value<std::string>()->default_value("config/config.json"))
		("log", "Log level (debug,info,warn,error)", cxxopts::value<std::string>()->default_value("info"))
		("enable-api", "Enable HTTP API", cxxopts::value<bool>()->default_value("false"))
		("h,help", "Print usage");

	auto result = options.parse(argc, argv);
	if (result.count("help")) {
		std::cout << options.help() << std::endl;
		return EXIT_SUCCESS;
	}

	// Init logger
    auto logLevel = AppLogging::from_string(result["log"].as<std::string>());
    AppLogging::init(logLevel);
	spdlog::info("Starting DebuggableTemplate...");
    DEBUG_LOG("Debug mode is active");
	spdlog::debug("Config path: {}", result["config"].as<std::string>());

	// Setup signals
	std::signal(SIGINT, signal_handler);
	std::signal(SIGTERM, signal_handler);

	// Optional HTTP API
	std::unique_ptr<std::thread> api_thread;
	if (result["enable-api"].as<bool>()) {
		api_thread = std::make_unique<std::thread>([]() {
			httplib::Server svr;
			svr.Get("/hello", [](const httplib::Request &, httplib::Response &res) {
				res.set_content("Hello World", "text/plain");
			});
			spdlog::info("HTTP API running at http://localhost:8080/hello");
			svr.listen("0.0.0.0", 8080);
		});
	}

	// Main loop
	int counter = 0;
	while (running) {
		std::cout << "Hello CMake." << std::endl;
		DEBUG_LOG("Loop counter: {}", counter++);
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
		ASSERT_MSG(counter < 20, "Loop exceeded safe threshold");
	}

	spdlog::info("Shutting down cleanly...");
	if (api_thread && api_thread->joinable()) {
		spdlog::info("Stopping HTTP API...");
		api_thread->detach();	// TODO: not graceful, will improve later
	}
	return EXIT_SUCCESS;
}

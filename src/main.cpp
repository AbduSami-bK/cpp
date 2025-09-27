// main.cpp : Defines the entry point for the application.
//

#include <csignal>
#include <CLI/CLI.hpp>
#include <httplib.h>
#include <iostream>
#include <spdlog/spdlog.h>
#include <thread>

#include "config.h"
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
	AppConfig config;

	// Parse args
	config.parse_args(argc, argv);

	// 3. Debug print effective config
	std::cout << "Effective config:\n"
				<< "  logLevel = " << config.logLevel << "\n"
				<< "  dryRun   = " << std::boolalpha << config.dryRun << "\n";

	// Init logger
	auto logLevel = AppLogging::from_string(config.logLevel);
	AppLogging::init(logLevel);
	spdlog::info("Starting DebuggableTemplate...");
	DEBUG_LOG("Debug mode is active");
	spdlog::debug("Config path: {}", config.configFile);

	// Setup signals
	std::signal(SIGINT, signal_handler);
	std::signal(SIGTERM, signal_handler);

	// Optional HTTP API
	std::unique_ptr<std::thread> apiThread;
	if (config.httpApi) {
		apiThread = std::make_unique<std::thread>([]() {
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
	if (apiThread && apiThread->joinable()) {
		spdlog::info("Stopping HTTP API...");
		apiThread->detach();	// TODO: not graceful, will improve later
	}
	return EXIT_SUCCESS;
}

#include <CLI/CLI.hpp>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

#include "config.h"

using json = nlohmann::json;

void AppConfig::load(const std::string &path) {
	std::ifstream f(path);
	if (!f.is_open()) {
		std::cerr << "Warning: Could not open config file: " << path << std::endl;
		return;	// return defaults
	}

	json j;
	f >> j;

	logLevel = j.value("log_level", "info");
	if (j.contains("features")) {
		httpApi = j["features"].value("http_api", false);
		dry_run = j["features"].value("dry_run", false);
	}

	// f.close();
}

void AppConfig::parse_args(int argc, char *argv[]) {
	CLI::App app{"C++ Template"};

	// Step 1: config file path only
	std::string cfgFile = "config/config.json";
	app.add_option("-c,--config", cfgFile, "Path to config file");

	// Do a *partial parse* (only looks for -c/--config, ignores the rest)
	app.allow_extras();
	CLI11_PARSE(app, argc, argv);

	// Step 2: load config file
	load(cfgFile);

	// Step 3: now set up the *rest* of the options
	app.add_flag("--dry-run", dry_run, "Run without performing actions");
	app.add_flag("--http-api", httpApi, "Enable HTTP API");
	app.add_option("--log", log_level, "Log level");

	// Reparse fully to override from CLI
	app.reset();	// wipe previous parse results
	CLI11_PARSE(app, argc, argv);
}

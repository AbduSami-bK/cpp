#include <CLI/CLI.hpp>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

#include "config.h"
#include "version.h"

using json = nlohmann::json;

void AppConfig::load(const std::string &path) {
	std::ifstream f(path);
	if (!f.is_open()) {
		std::cerr << "Warning: Could not open config file: " << path << std::endl;
		return;	// return defaults
	}

	json j;
	f >> j;

	logLevel = j.value("logLevel", "info");
	if (j.contains("features")) {
		httpApi = j["features"].value("httpApi", false);
		dryRun = j["features"].value("dryRun", false);
	}

	// f.close();
}

void AppConfig::parse_args(int argc, char *argv[]) {
	CLI::App app{"C++ Template"};

	// Step 1: config file path only
	// All options at once
	app.add_option("-c,--config", configFile, "Path to config file")->default_val("config/config.json");
	app.add_flag("--dry-run", dryRun, "Run without performing actions")->default_val(false);
	app.add_flag("--http-api", httpApi, "Enable HTTP API")->default_val(false);
	app.add_option("-l,--log", logLevel, "Log level")->default_val("info");
	app.set_version_flag("-v,--version", GIT_VERSION, "Show version and exit");

	try {
		app.parse(argc, argv);
	} catch (const CLI::CallForHelp &e) {
		app.exit(e);	// prints help + exits
		std::exit(EXIT_SUCCESS);
	} catch (const CLI::CallForVersion &e) {
		app.exit(e);	// prints version + exits
		std::exit(EXIT_SUCCESS);
	} catch (const CLI::ParseError &e) {
		configFile = "config/config.json";
		std::cerr << "Invalid config flag. Using default conf file: '" << configFile << "'" << std::endl;
	}

	// Step 2: load config file
	load(configFile);

	// Reparse to override from CLI
	try {
		app.parse(argc, argv);
	} catch (const CLI::ParseError &e) {
		std::cerr << "Invalid CLI args. Falling back to config values from '" << configFile << "'" << std::endl;
	}
}

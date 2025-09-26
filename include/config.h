#pragma once

#include <string>

/// Represents the application configuration loaded from JSON and CLI
class AppConfig {
	public:
		std::string log_level = "info";	///< Logging level: debug/info/warn/error
		std::string config_file;		///< Path to the loaded config file
		bool dry_run = false;			///< If true, don’t perform real actions
		bool httpApi = false;			///<

		/// Loads configuration from a JSON file into an AppConfig
		void load(const std::string &path);

		void parse_args(int argc, char *argv[]);

		// Later we’ll add more fields (ports, feature flags, etc.)
};

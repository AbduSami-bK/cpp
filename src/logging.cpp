#include <algorithm>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
#include <vector>

#include "logging.h"

namespace AppLogging {

	void init(Level level, const std::string &logfile, size_t maxFileSize, size_t maxFiles) {
		try {
			auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
			auto fileSink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(logfile, maxFileSize, maxFiles);

			std::vector<spdlog::sink_ptr> sinks{consoleSink, fileSink};
			auto logger = std::make_shared<spdlog::logger>("appLogger", sinks.begin(), sinks.end());

			spdlog::set_default_logger(logger);

			// Log format: [time] [level] file:line message
			spdlog::set_pattern("[%Y-%m-%d %H:%M:%S] [%^%l%$] %v");

			switch (level) {
				case Level::Debug:
					spdlog::set_level(spdlog::level::debug);
					break;
				case Level::Info:
					spdlog::set_level(spdlog::level::info);
					break;
				case Level::Warn:
					spdlog::set_level(spdlog::level::warn);
					break;
				case Level::Error:
					spdlog::set_level(spdlog::level::err);
					break;
			}

			spdlog::info("Logger initialized at level {}",
						spdlog::level::to_string_view(spdlog::get_level()));
		} catch (const spdlog::spdlog_ex &ex) {
			fprintf(stderr, "Logging init failed: %s\n", ex.what());
		}
	}

	Level from_string(const std::string &level) {
		std::string l = level;
		std::transform(l.begin(), l.end(), l.begin(), ::tolower);
		if (l == "debug")	return Level::Debug;
		if (l == "info")	return Level::Info;
		if (l == "warn")	return Level::Warn;
		if (l == "error")	return Level::Error;
		/* default */		return Level::Info;
	}

} // namespace AppLogging

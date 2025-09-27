#pragma once

#include <memory>
#include <string>

namespace AppLogging {

	enum class Level { Debug, Info, Warn, Error };

	/// Initialize logging: console + optional rotating file sink
	void init(Level level = Level::Info, const std::string &logfile = "logs/app.log",
				size_t maxFileSize = 1048576 * 5,		// 5 MB
				size_t maxFiles = 3);

	/// Convert string ("debug","info",...) to Level
	Level from_string(const std::string &level);

} // namespace AppLogging

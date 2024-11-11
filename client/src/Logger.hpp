#pragma once

#include <functional>
#include <unordered_map>
#include <string>

enum class LogLevel : int {
    NOLOG       = -1,
    Fatal       = 0,
    Error       = 1,
    Warn        = 2,
    Info        = 3,
    Debug       = 4,
    Trace       = 5,
    MAXLOGLEVEL = 6,
};

/**
 * @brief Logger
 *
 * LogLevel:
 * LogLevel::NOLOG - no messages
 * LogLevel::Fatal - only `fatal` are displayed
 * LogLevel::Error - `error` and `fatal` are displayed
 * LogLevel::Warn - `warn`, `error` and `fatal` are displayed
 * LogLevel::Info - `info`, `warn`, `error` and `fatal` are displayed
 * LogLevel::Debug - `debug`, `info`, `warn`, `error` and `fatal` are
 * displayed LogLevel::Trace - `trace`, `debug`, `info`, `warn`, `error` and
 * `fatal` are displayed
 *
 * Attention:
 * `debug` and `trace` will only do something in debug mode compilation.
 * (`set(CMAKE_BUILD_TYPE Debug)` in `CMakeLists.txt`)
 */
class Logger {
    public:
        /**
         * @brief Logger fata
         *
         * Means that something critical is broken, and the application
         * cannot continue to do any more useful work without the
         * intervention of an engineer.
         *
         * @param message the message
         */
        static void fatal(const std::string &message)
        {
            if (getLogLevel() < LogLevel::Fatal) {
                return;
            }
            Logger::print(LogLevel::Fatal, "FATAL", message);
        }
        /**
         * @brief Logger error
         *
         * The ERROR log level is used to represent error conditions in
         * an application that prevent a specific operation from running,
         * but the application itself can continue working even if it is
         * at a reduced level of functionality or performance.
         *
         * @param message the message
         */
        static void error(const std::string &message)
        {
            if (getLogLevel() < LogLevel::Error) {
                return;
            }
            Logger::print(LogLevel::Error, "ERROR", message);
        }
        /**
         * @brief Logger warn
         *
         * Messages logged at the WARN level typically indicate that
         * something unexpected happened, but the application can recover
         * and continue to function normally.
         *
         * @param message the message
         */
        static void warn(const std::string &message)
        {
            if (getLogLevel() < LogLevel::Warn) {
                return;
            }
            Logger::print(LogLevel::Warn, "WARN", message);
        }
        /**
         * @brief Logger info
         *
         * INFO-level messages indicate events in the system that are
         * significant to the business purpose of the application. Such
         * events are logged to show that the system is operating normally.
         *
         * @param message the message
         */
        static void info(const std::string &message)
        {
            if (getLogLevel() < LogLevel::Info) {
                return;
            }
            Logger::print(LogLevel::Info, "INFO", message);
        }
#ifndef NDEBUG
        /**
         * @brief Logger debug
         *
         * The DEBUG level is used for logging messages that help
         * developers find out what went wrong during a debugging session.
         *
         * @param message the message
         */
        static void debug(const std::string &message /* unused */)
        {
            if (getLogLevel() < LogLevel::Debug) {
                return;
            }
            Logger::print(LogLevel::Debug, "DEBUG", message);
        }
#else
        static void debug(const std::string & /* unused */)
        {}
#endif
#ifndef NDEBUG
        /**
         * @brief Logger trace
         *
         * The TRACE level is used for tracing the path of code execution
         * in a program.
         *
         * @param message the message
         */
        static void trace(const std::string &message /* unused */)
        {
            if (getLogLevel() < LogLevel::Trace) {
                return;
            }
            Logger::print(LogLevel::Trace, "TRACE", message);
        }
#else
        static void trace(const std::string & /* unused */)
        {}
#endif
        /**
         * @brief Logger subscribe
         *
         * @param type the type to subscribe for
         * @param name the name
         * @param callback the callback
         */
        static void subscribeCallback(
            LogLevel type,
            const std::string &name,
            std::function<void(const std::string &)> callback)
        {
            if (getCallbacks().find(type) == getCallbacks().end()) {
                getCallbacks().emplace(type, std::unordered_map<std::string, std::function<void(const std::string &)>>());
            }
            getCallbacks()[type].emplace(name, callback);
        }
        /**
         * @brief Logger remove
         *
         * @param type the type
         * @param name the name
         */
        static void unsubscribeCallback(LogLevel type, const std::string &name)
        {
            if (getCallbacks().find(type) == getCallbacks().end()) {
                return;
            }
            if (getCallbacks()[type].find(name) == getCallbacks()[type].end()) {
                return;
            }
            getCallbacks()[type].erase(name);
        }
        /**
         * @brief set log level
         *
         * @param logLevel the log level
         */
        static void setLogLevel(LogLevel logLevel)
        {
            getLogLevelRef() = logLevel;
        }
        /**
         * @brief get log level
         *
         * @return the log level
         */
        [[nodiscard]] static LogLevel getLogLevel()
        {
            return getLogLevelRef();
        }

    private:
        static LogLevel &getLogLevelRef()
        {
            static LogLevel logLevel;
            return logLevel;
        }

        static std::unordered_map<LogLevel, std::unordered_map<std::string, std::function<void(const std::string &)>>> &getCallbacks()
        {
            static std::unordered_map<
                LogLevel,
                std::unordered_map<std::string, std::function<void(const std::string &)>>>
                callbacks;
            return callbacks;
        }

        static void print(LogLevel levelT, const std::string &level, const std::string &message);
};

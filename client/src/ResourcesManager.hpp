#include <filesystem>
#include <mutex>
#include <string>
#include "Logger.hpp"
#include "PathResolver.hpp"

class ResourcesManager {
    public:
        /**
         * @brief Init the resource manager
         * @param execPath The path of the executable
         */
        static void init(std::string execPath)
        {
            if (!std::filesystem::exists(execPath)) {
                Logger::fatal("RESOURCE_MANAGER: Invalid execPath: " + execPath);
                return;
            }
            std::filesystem::path path_tmp = execPath;
            path_tmp                       = path_tmp.remove_filename();
            path_tmp                       = std::filesystem::absolute(path_tmp);
#ifdef PACKAGED
            getRessourcePath() = ResourcesManager::getRessourcePathPackaged(path_tmp.string());
#else
            getRessourcePath() = ResourcesManager::getRessourcePathNormal(path_tmp.string());
#endif
            Logger::info("RESOURCE_MANAGER: Path Assets: " + getRessourcePath());
            PathResolver::setResolver(ResourcesManager::convertPath);
        }

        /**
         * @brief Get the resource path
         * @param path_const The path to convert
         * @return The converted path
         * @note You need to have 'assets' in your path at the beginning
         */
        static std::string convertPath(const std::string &path_const)
        {
            std::string found_path;

            ResourcesManager::getMutex().lock();
            std::string resource_path = ResourcesManager::getRessourcePath();
            if (resource_path.empty()) {
                ResourcesManager::getMutex().unlock();
                Logger::fatal("RESOURCE_MANAGER: need to call init first (" + path_const + ")");
                return "";
            }
            if (ResourcesManager::getMap().find(path_const) != ResourcesManager::getMap().end()) {
                ResourcesManager::getMutex().unlock();
                return ResourcesManager::getMap().at(path_const);
            }
            ResourcesManager::getMutex().unlock();
            if (found_path.empty() && path_const.starts_with("assets")) {
                if (ResourcesManager::isExists(resource_path, "share/r-type")) {
                    std::filesystem::path path_tmp = resource_path;
                    path_tmp                       = path_tmp.append("share");
                    path_tmp                       = path_tmp.append("r-type");
                    std::string path(path_const, std::string("assets").length(), std::string::npos);
                    path_tmp = path_tmp.append(path);
                    path_tmp = path_tmp.make_preferred();
                    Logger::debug("RESOURCE_MANAGER: Path found: " + path_tmp.string());
                    found_path = path_tmp.string();
                }
                if (found_path.empty() && ResourcesManager::isExists(resource_path, "assets")) {
                    std::filesystem::path path_tmp = resource_path;
                    path_tmp                       = path_tmp.append(path_const);
                    path_tmp                       = path_tmp.make_preferred();
                    Logger::debug("RESOURCE_MANAGER: Path found: " + path_tmp.string());
                    found_path = path_tmp.string();
                }
            }
            if (found_path.empty()) {
                if (ResourcesManager::isExists(resource_path, path_const)) {
                    std::filesystem::path path_tmp = resource_path;
                    path_tmp                       = path_tmp.append(path_const);
                    path_tmp                       = path_tmp.make_preferred();
                    Logger::debug("RESOURCE_MANAGER: Path found: " + path_tmp.string());
                    found_path = path_tmp.string();
                }
                if (found_path.empty() && ResourcesManager::isExists(resource_path, "bin")) {
                    std::filesystem::path path_tmp = ResourcesManager::getRessourcePath();
                    path_tmp                       = path_tmp.append("bin");
                    path_tmp                       = path_tmp.append(path_const);
                    path_tmp                       = path_tmp.make_preferred();
                    if (std::filesystem::exists(path_tmp)) {
                        Logger::debug("RESOURCE_MANAGER: Path found: " + path_tmp.string());
                        found_path = path_tmp.string();
                    }
                }
            }
            ResourcesManager::getMutex().lock();
            ResourcesManager::getMap()[path_const] = found_path;
            ResourcesManager::getMutex().unlock();
            return found_path;
        }

    private:
        static std::string &getRessourcePath()
        {
            static std::string resourcePath;
            return resourcePath;
        }

        static std::unordered_map<std::string, std::string> &getMap()
        {
            static std::unordered_map<std::string, std::string> map;
            return map;
        }

        static std::mutex &getMutex()
        {
            static std::mutex mutex;
            return mutex;
        }

        static bool isExists(const std::string &path, const std::string &rest)
        {
            std::filesystem::path path_tmp = path;
            path_tmp                       = path_tmp.append(rest);
            path_tmp                       = path_tmp.make_preferred();
            return std::filesystem::exists(path_tmp);
        }

        static std::string getRessourcePathPackaged(const std::string &pathFolderInit)
        {
            std::filesystem::path path_tmp = pathFolderInit;
            path_tmp                       = path_tmp.parent_path();

            if (!ResourcesManager::isExists(path_tmp.string(), "share")
                || !ResourcesManager::isExists(path_tmp.string(), "bin")) {
                Logger::warn("RESOURCE_MANAGER: Path don't contain OK: " + path_tmp.string());
                path_tmp = path_tmp.parent_path();
                if (!ResourcesManager::isExists(path_tmp.string(), "share")
                    || !ResourcesManager::isExists(path_tmp.string(), "bin")) {
                    Logger::warn("RESOURCE_MANAGER: Path don't contain OK: " + path_tmp.string());
                    return ResourcesManager::getRessourcePathNormal(pathFolderInit);
                }
            }
            return path_tmp.string();
        }

        static std::string getRessourcePathNormal(const std::string &pathFolderInit)
        {
            std::filesystem::path path_tmp = pathFolderInit;

            if (!ResourcesManager::isExists(path_tmp.string(), "assets")) {
                Logger::warn("RESOURCE_MANAGER: Path don't contain OK: " + path_tmp.string());
                path_tmp = path_tmp.parent_path();
                if (!ResourcesManager::isExists(path_tmp.string(), "assets")) {
                    Logger::warn("RESOURCE_MANAGER: Path don't contain OK: " + path_tmp.string());
                    return "";
                }
                return "";
            }
            return path_tmp.string();
        }
};

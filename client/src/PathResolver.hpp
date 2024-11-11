#pragma once

#include <string>
#include <functional>

class PathResolver {
    public:
        using Resolver = std::function<std::string(const std::string &)>;

        static std::string resolve(const std::string &path)
        {
            return getCustomResolver()(path);
        }

        static void setResolver(Resolver resolver)
        {
            getCustomResolver() = resolver;
        }

        static std::string defaultResolver(const std::string &path)
        {
            return path;
        }

    private:

        static Resolver &getCustomResolver()
        {
            static Resolver resolver = defaultResolver;
            return resolver;
        }
};

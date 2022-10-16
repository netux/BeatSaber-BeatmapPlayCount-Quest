#pragma once

#include "common.hpp"
#include <unordered_map>
#include <dirent.h>

namespace BeatmapPlayCount {
    class Storage {
        public:
        Storage(const ModInfo& info) {
            this->modDataDir = getDataDir(info);

            if (!direxists(modDataDir)) {
                mkpath(modDataDir);
            }

            getLogger().info("Storing play count data at %s", this->modDataDir.c_str());
        }

        // TODO(netux): This goes unused as the cache is populated lazily
        bool load() {
            auto dir = opendir(this->modDataDir.c_str());
            if (dir == NULL) {
                return false;
            }

            struct dirent* entry;
            while ((entry = readdir(dir)) != NULL) {
                auto name = std::string(entry->d_name);
                auto extPos = name.find_first_of(".count");
                auto beatmapId = name.substr(0, extPos);

                auto count = this->readPlayCount(beatmapId);
                this->countsCache[beatmapId] = count;
            }
            closedir(dir);

            return true;
        }

        void incrementPlayCount(std::string beatmapId) {
            auto new_count = this->getPlayCount(beatmapId) + 1;
            this->countsCache[beatmapId] = new_count;
            this->writePlayCount(beatmapId, new_count);

            getLogger().debug("Incremented play count for %s to %d", beatmapId.c_str(), new_count);
        }

        int getPlayCount(std::string beatmapId) {
            if (!this->countsCache.contains(beatmapId)) {
                auto count = this->readPlayCount(beatmapId);
                this->countsCache[beatmapId] = count;
            }
            return this->countsCache[beatmapId];
        }

        std::string getModDataDir() {
            return this->modDataDir;
        }

        private:
        std::string modDataDir;

        std::unordered_map<std::string, int> countsCache;

        std::string getPlayCountPath(std::string beatmapId) {
            return this->modDataDir + "/" + beatmapId + ".count";
        }

        int readPlayCount(std::string beatmapId) {
            auto path = this->getPlayCountPath(beatmapId);

            if (!fileexists(path)) {
                return 0;
            } else {
                return std::stoi(readfile(path));
            }
        }

        bool writePlayCount(std::string beatmapId, int count) {
            return writefile(this->getPlayCountPath(beatmapId), std::to_string(count));
        }
    };
}

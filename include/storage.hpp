#pragma once

#include "common.hpp"
#include <unordered_map>
#include <dirent.h>

namespace BeatmapPlayCount {
    class Storage {
        public:
        Storage(const ModInfo& info) {
            this->mod_data_dir = getDataDir(info);

            if (!direxists(mod_data_dir)) {
                mkpath(mod_data_dir);
            }

            getLogger().info("Storing play count data at %s", this->mod_data_dir.c_str());
        }

        // TODO(netux): This goes unused as the cache is populated lazily
        bool load() {
            auto dir = opendir(this->mod_data_dir.c_str());
            if (dir == NULL) {
                return false;
            }

            struct dirent* entry;
            while ((entry = readdir(dir)) != NULL) {
                auto name = std::string(entry->d_name);
                auto extPos = name.find_first_of(".count");
                auto beatmapId = name.substr(0, extPos);

                auto count = this->readPlayCount(beatmapId);
                this->counts_cache[beatmapId] = count;
            }
            closedir(dir);

            return true;
        }

        void incrementPlayCount(std::string beatmapId) {
            auto new_count = this->getPlayCount(beatmapId) + 1;
            this->counts_cache[beatmapId] = new_count;
            this->writePlayCount(beatmapId, new_count);
        }

        int getPlayCount(std::string beatmapId) {
            if (!this->counts_cache.contains(beatmapId)) {
                auto count = this->readPlayCount(beatmapId);
                this->counts_cache[beatmapId] = count;
            }
            return this->counts_cache[beatmapId];
        }

        std::string getModDataDir() {
            return this->mod_data_dir;
        }

        private:
        std::string mod_data_dir;

        std::unordered_map<std::string, int> counts_cache;

        std::string getPlayCountPath(std::string beatmapId) {
            return this->mod_data_dir + "/" + beatmapId + ".count";
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

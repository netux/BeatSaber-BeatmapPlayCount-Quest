namespace BeatmapPlayCount::API {
    /**
     * Get the amount of times a beatmap was played in total.
     */
    int GetPlayCount(std::string beatmapId);

    /**
     * Register a check function that returns false during play time if a
     * song's play count shouldn't be incremented when the progress threshold
     * is passed.
     *
     * This might be useful when your mod gameplay scenes for things that
     * aren't gameplay (like replays or visualizations), and you don't
     * want this mod to increment user play count during it.
     *
     * The name argument is used to identify a check function and be able to
     * unregister it using UnregisterCanIncrementPlayCountCheck().
     *
     * @note The check function is triggered on every frame or Update() call.
     * If you are doing any expensive operations here, make sure to cache them.
     */
    void RegisterCanIncrementPlayCountCheck(std::string name, bool (*checkFn)());

    /**
     * Unregister a check function registered by RegisterCanIncrementPlayCountCheck().
     */
    void UnregisterCanIncrementPlayCountCheck(std::string name);
}

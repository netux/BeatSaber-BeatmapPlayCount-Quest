namespace BeatmapPlayCount::API {
    /**
     * Get the amount of times a beatmap was played in total.
     */
    int GetPlayCount(std::string beatmapId);

    /**
     * Sets whenever the current play would increment play count when the progress threshold is passed.
     *
     * It may be useful unset this when a mod uses gameplay scenes for things that aren't gameplay
     * (like replays or visualizations).
     *
     * @note If the progress threshold has already been passed, then the play count has already
     * been incremented and calling this function is useless.
     *
     * @returns False if there is no current song playing (no TrackPlaytime gameobject instance).
     * True if the operation succeeded.
     */
    bool SetAllowCurrentPlayToIncrementCount(bool flag);
}

#pragma once

#include <string>
#include <unordered_map>

#include "video.h"
/** A class used to represent a Playlist */

class VideoPlaylist
{
private:
    std::vector<std::string> list;
    std::string mPlaylistID;

public:
    VideoPlaylist(const std::string name);

    VideoPlaylist(const VideoPlaylist &v);

    // This class is movable.
    VideoPlaylist(VideoPlaylist &&);
    VideoPlaylist &operator=(VideoPlaylist &&v)
    {
        mPlaylistID = v.mPlaylistID;
        list = v.list;
        v.mPlaylistID = "";
        v.list = {};
        return *this;
    }

    std::vector<std::string> getVideoIds() const;
    // Returns the playlist id of the playlist.
    const std::string &getPlaylistId() const;
    void addVideo(const std::string videoId);
    void removeVideo(const Video *video);
    void clearPlaylist();
    bool contains(const std::string videoId);
};

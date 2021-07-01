#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "video.h"
#include "videoplaylist.h"

/**
 * A class used to represent a Video Library.
 */
class VideoLibrary {
 private:
  std::unordered_map<std::string, Video> mVideos;
  std::vector<VideoPlaylist> playlistsVec;
  std::unordered_map<std::string, VideoPlaylist> mPlaylists;
  std::unordered_map<std::string, std::string> mFlags;

  public:
  VideoLibrary();

  // This class is not copyable to avoid expensive copies.
  VideoLibrary(const VideoLibrary&) = delete;
  VideoLibrary& operator=(const VideoLibrary&) = delete;

  // This class is movable.
  VideoLibrary(VideoLibrary&&) = default;
  VideoLibrary& operator=(VideoLibrary&&) = default;

  std::vector<Video> getVideos() const;
  const Video *getVideo(const std::string& videoId) const;

  std::vector<VideoPlaylist> getPlaylists();
  VideoPlaylist *getPlaylist(const std::string &playlistId);
  VideoPlaylist *createPlaylist(const std::string &playlistId);
  void deletePlaylist(VideoPlaylist playlist);

  const std::string *getFlag(const std::string &videoId);
  std::vector<std::string> getFlaggedVideoIds();
  void addFlag(const std::string &videoId,
               const std::string &reason = "Not supplied");
  void deleteFlag(const std::string &videoId);
};

#include "videolibrary.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <utility>
#include <vector>

#include "helper.h"
#include "video.h"

VideoLibrary::VideoLibrary() {
  std::ifstream file("./src/videos.txt");
  if (file.is_open()) {
    std::string line;
    while (std::getline(file, line)) {
      std::stringstream linestream(line);
      std::string title;
      std::string id;
      std::string tag;
      std::vector<std::string> tags;
      std::getline(linestream, title, '|');
      std::getline(linestream, id, '|');
      while (std::getline(linestream, tag, ',')) {
        tags.emplace_back(trim(std::move(tag)));
      }
      Video video = Video(trim(std::move(title)), trim(id), std::move(tags));
      mVideos.emplace(trim(std::move(id)), std::move(video));
    }
  } else {
    std::cout << "Couldn't find videos.txt" << std::endl;
  }
}

std::vector<Video> VideoLibrary::getVideos() const {
  std::vector<Video> result;
  for (const auto& video : mVideos) {
    result.emplace_back(video.second);
  }
  return result;
}

const Video* VideoLibrary::getVideo(const std::string& videoId) const {
  const auto found = mVideos.find(videoId);
  if (found == mVideos.end()) {
    // std::cout << "Video not found in video library" << std::endl;
    return nullptr;
  } else {
    return &(found->second);
  }
}

std::vector<VideoPlaylist> VideoLibrary::getPlaylists() {
  std::vector<VideoPlaylist> result;
  for (const auto &playlist : mPlaylists) {
    result.emplace_back(playlist.second);
  }
  return result;
}

VideoPlaylist *VideoLibrary::getPlaylist(const std::string &playlistId) {
  auto found = mPlaylists.find(stringToUpper(playlistId));
  if (found == mPlaylists.end()) {
    // std::cout << "Video not found in video library" << std::endl;
    return nullptr;
  } else {
    return &(found->second);
  }
}

VideoPlaylist *VideoLibrary::createPlaylist(const std::string &playlistId) {
  if (getPlaylist(playlistId)) {
    return nullptr;
  } else {
    VideoPlaylist playlist = VideoPlaylist(playlistId);
    mPlaylists.emplace(std::piecewise_construct,
                       std::forward_as_tuple(stringToUpper(playlistId)),
                       std::forward_as_tuple(playlistId));
    return getPlaylist(stringToUpper(playlistId));
  }
}

void VideoLibrary::deletePlaylist(VideoPlaylist playlist) {
  mPlaylists.erase(playlist.getPlaylistId());
}

const std::string *VideoLibrary::getFlag(const std::string &videoId) {
  auto found = mFlags.find(videoId);
  if (found == mFlags.end()) {
    return nullptr;
  } else {
    return &(found->second);
  }
}

void VideoLibrary::addFlag(const std::string &videoId,
                           const std::string &reason) {
  mFlags.emplace(videoId, reason);
}

void VideoLibrary::deleteFlag(const std::string &videoId) {
  mFlags.erase(videoId);
}

std::vector<std::string> VideoLibrary::getFlaggedVideoIds() {
  std::vector<std::string> result;
  for (const auto &flag : mFlags) {
    result.emplace_back(flag.first);
  }
  return result;
}
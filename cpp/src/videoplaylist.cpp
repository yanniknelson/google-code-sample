#include "videoplaylist.h"

VideoPlaylist::VideoPlaylist(std::string name) { mPlaylistID = name; }

const std::string &VideoPlaylist::getPlaylistId() const { return mPlaylistID; }

VideoPlaylist::VideoPlaylist(VideoPlaylist &&v)
    : mPlaylistID{v.mPlaylistID}, list{v.list} {
  v.mPlaylistID = "";
  v.list = {};
}

VideoPlaylist::VideoPlaylist(const VideoPlaylist &v) {
  mPlaylistID = v.mPlaylistID;
  list = v.list;
}

std::vector<std::string> VideoPlaylist::getVideoIds() const { return list; }

bool VideoPlaylist::contains(const std::string videoId) {
  return std::find(list.begin(), list.end(), videoId) != list.end();
}

void VideoPlaylist::addVideo(const std::string videoId) {
  list.push_back(videoId);
}

void VideoPlaylist::removeVideo(const Video *video) {
  list.erase(std::remove(list.begin(), list.end(), video->getVideoId()),
             list.end());
}

void VideoPlaylist::clearPlaylist() { list.clear(); }
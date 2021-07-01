#include "videoplayer.h"

#include <iostream>

#include "helper.h"

// compares two videos by name lexographically
bool videoLexCompare(const Video &a, const Video &b) {
  return a.getTitle() < b.getTitle();
}

// compares two playlists by name lexographically
bool playlistLexCompare(const VideoPlaylist &a, const VideoPlaylist &b) {
  return a.getPlaylistId() < b.getPlaylistId();
}

// takes in a video and outputs a string describing its properties
std::string VideoPlayer::VideoToString(const Video video) {
  std::string output = "";
  output += video.getTitle() + " (" + video.getVideoId() + ") [";
  std::vector<std::string> tags = video.getTags();
  for (auto tag : tags) {
    output += tag;
    if (tag != tags[tags.size() - 1]) {
      output += " ";
    }
  }
  output += "]";

  if (auto flagReason = mVideoLibrary.getFlag(video.getVideoId())) {
    output += " - FLAGGED (reason: " + *flagReason + ")";
  }
  return output;
}

void VideoPlayer::numberOfVideos() {
  std::cout << mVideoLibrary.getVideos().size() << " videos in the library"
            << std::endl;
}

void VideoPlayer::showAllVideos() {
  std::cout << "Here's a list of all available videos:" << std::endl;
  // get all the videos
  auto videos = mVideoLibrary.getVideos();
  // sort all the videos by name (lexographically)
  std::sort(videos.begin(), videos.end(), videoLexCompare);
  // list all the videos
  for (auto video : videos) {
    std::cout << "\t" << VideoToString(video) << std::endl;
  }
}

void VideoPlayer::playVideo(const std::string &videoId) {
  // get a pointer to the videoif the pointer is not null (meaning the video was
  // found)
  if (auto video = mVideoLibrary.getVideo(videoId)) {
    // if the currentlyplaying pointer is not null (already playing a video)
    if (auto flagReason = mVideoLibrary.getFlag(video->getVideoId())) {
      std::cout << "Cannot play video: Video is currently flagged (reason: "
                << *flagReason << ")" << std::endl;
    } else {
      if (CurrentlyPlaying) {
        std::cout << "Stopping video: " << CurrentlyPlaying->getTitle()
                  << std::endl;
      }
      std::cout << "Playing video: " << video->getTitle() << std::endl;
      playing = true;
      CurrentlyPlaying = video;
    }
  } else {
    std::cout << "Cannot play video: Video does not exist" << std::endl;
  }
}

void VideoPlayer::stopVideo() {
  // if the currentlyPlaying pointer is not null (already playing a video)
  if (CurrentlyPlaying) {
    std::cout << "Stopping video: " << CurrentlyPlaying->getTitle()
              << std::endl;
    CurrentlyPlaying = nullptr;
    playing = false;
  } else {
    std::cout << "Cannot stop video: No video is currently playing"
              << std::endl;
  }
}

void VideoPlayer::playRandomVideo() {
  auto videos = mVideoLibrary.getVideos();
  std::vector<Video> validVideos;
  auto flaggedVideos = mVideoLibrary.getFlaggedVideoIds();
  for (auto video : videos) {
    if (std::find(flaggedVideos.begin(), flaggedVideos.end(),
                  video.getVideoId()) != flaggedVideos.end()) {
      continue;
    }
    validVideos.push_back(video);
  }
  // if there are no videos in the library
  if (validVideos.size() == 0) {
    std::cout << "No videos available" << std::endl;
  } else {
    playVideo(validVideos[std::rand() % validVideos.size()].getVideoId());
  }
}

void VideoPlayer::pauseVideo() {
  // if the currentlyPlaying pointer is not null (already playing a video)
  if (CurrentlyPlaying) {
    if (playing) {
      std::cout << "Pausing video: " << CurrentlyPlaying->getTitle()
                << std::endl;
      playing = false;
    } else {
      std::cout << "Video already paused: " << CurrentlyPlaying->getTitle()
                << std::endl;
    }
  } else {
    std::cout << "Cannot pause video: No video is currently playing"
              << std::endl;
  }
}

void VideoPlayer::continueVideo() {
  // if the currentlyPlaying pointer is not null (already playing a video)
  if (CurrentlyPlaying) {
    if (!playing) {
      std::cout << "Continuing video: " << CurrentlyPlaying->getTitle()
                << std::endl;
      playing = true;
    } else {
      std::cout << "Cannot continue video: Video is not paused" << std::endl;
    }
  } else {
    std::cout << "Cannot continue video: No video is currently playing"
              << std::endl;
  }
}

void VideoPlayer::showPlaying() {
  // if the currentlyPlaying pointer is not null (currently playing avideo)
  if (CurrentlyPlaying) {
    std::cout << "Currently playing: " << VideoToString(*CurrentlyPlaying);
    if (!playing) {
      std::cout << " - PAUSED";
    }
    std::cout << std::endl;
  } else {
    std::cout << "No video is currently playing" << std::endl;
  }
}

void VideoPlayer::createPlaylist(const std::string &playlistName) {
  // if the store of playlists already has a playlist with a matching Id
  if (auto playlist = mVideoLibrary.createPlaylist(playlistName)) {
    std::cout << "Successfully created new playlist: "
              << playlist->getPlaylistId() << std::endl;
  } else {
    std::cout << "Cannot create playlist: A playlist with the same name "
                 "already exists"
              << std::endl;
  }
}

void VideoPlayer::addVideoToPlaylist(const std::string &playlistName,
                                     const std::string &videoId) {
  if (auto playlist = mVideoLibrary.getPlaylist(playlistName)) {
    if (auto video = mVideoLibrary.getVideo(videoId)) {
      if (auto flagReason = mVideoLibrary.getFlag(video->getVideoId())) {
        std::cout << "Cannot add video to " << playlistName
                  << ": Video is currently flagged (reason: " << *flagReason
                  << ")" << std::endl;
      } else {
        if (playlist->contains(videoId)) {
          std::cout << "Cannot add video to " << playlistName
                    << ": Video already added" << std::endl;
        } else {
          playlist->addVideo(video->getVideoId());
          std::cout << "Added video to " << playlistName << ": "
                    << video->getTitle() << std::endl;
        }
      }
    } else {
      std::cout << "Cannot add video to " << playlistName
                << ": Video does not exist" << std::endl;
    }
  } else {
    std::cout << "Cannot add video to " << playlistName
              << ": Playlist does not exist" << std::endl;
  }
}

void VideoPlayer::showAllPlaylists() {
  auto playlists = mVideoLibrary.getPlaylists();
  if (playlists.size()) {
    std::cout << "Showing all playlists:" << std::endl;
    // sort all the playlists by name (lexographically)
    std::sort(playlists.begin(), playlists.end(), playlistLexCompare);
    // list all the videos
    for (auto playlist : playlists) {
      std::cout << "\t" << playlist.getPlaylistId() << std::endl;
    }
  } else {
    std::cout << "No playlists exist yet" << std::endl;
  }
}

void VideoPlayer::showPlaylist(const std::string &playlistName) {
  if (auto playlist = mVideoLibrary.getPlaylist(playlistName)) {
    std::cout << "Showing playlist: " << playlistName << std::endl;
    auto videoIds = playlist->getVideoIds();
    if (videoIds.size()) {
      for (auto videoId : videoIds) {
        std::cout << "\t" << VideoToString(*mVideoLibrary.getVideo(videoId))
                  << std::endl;
      }
    } else {
      std::cout << " \t No videos here yet" << std::endl;
    }
  } else {
    std::cout << "Cannot show playlist " << playlistName
              << ": Playlist does not exist" << std::endl;
  }
}

void VideoPlayer::removeFromPlaylist(const std::string &playlistName,
                                     const std::string &videoId) {
  if (auto playlist = mVideoLibrary.getPlaylist(playlistName)) {
    if (auto video = mVideoLibrary.getVideo(videoId)) {
      if (playlist->contains(videoId)) {
        playlist->removeVideo(video);
        std::cout << "Removed video from " << playlistName << ": "
                  << video->getTitle() << std::endl;
      } else {
        std::cout << "Cannot remove video from " << playlistName
                  << ": Video is not in playlist" << std::endl;
      }
    } else {
      std::cout << "Cannot remove video from " << playlistName
                << ": Video does not exist" << std::endl;
    }
  } else {
    std::cout << "Cannot remove video from " << playlistName
              << ": Playlist does not exist" << std::endl;
  }
}

void VideoPlayer::clearPlaylist(const std::string &playlistName) {
  if (auto playlist = mVideoLibrary.getPlaylist(playlistName)) {
    playlist->clearPlaylist();
    std::cout << "Successfully removed all videos from " << playlistName
              << std::endl;
  } else {
    std::cout << "Cannot clear playlist " << playlistName
              << ": Playlist does not exist" << std::endl;
  }
}

void VideoPlayer::deletePlaylist(const std::string &playlistName) {
  if (auto playlist = mVideoLibrary.getPlaylist(playlistName)) {
    mVideoLibrary.deletePlaylist(*playlist);
    std::cout << "Deleted playlist: " << playlistName << std::endl;
  } else {
    std::cout << "Cannot delete playlist " << playlistName
              << ": Playlist does not exist" << std::endl;
  }
}

void VideoPlayer::searchVideos(const std::string &searchTerm) {
  std::regex pat{stringToUpper(searchTerm)};
  auto videos = mVideoLibrary.getVideos();
  std::vector<Video> matches;
  for (auto video : videos) {
    if (std::regex_search(stringToUpper(video.getTitle()), pat)) {
      if (mVideoLibrary.getFlag(video.getVideoId())) {
        continue;
      }
      matches.push_back(video);
    }
  }
  if (matches.size()) {
    std::cout << "Here are the results for " << searchTerm << ":" << std::endl;
    int counter = 1;
    std::sort(matches.begin(), matches.end(), videoLexCompare);
    for (auto video : matches) {
      std::cout << "\t" << counter << (") ") << VideoToString(video)
                << std::endl;
      counter++;
    }
    std::cout << "Would you like to play any of the above? If yes, specify the "
                 "number of the video."
              << std::endl;
    std::cout
        << "If your answer is not a valid number, we will assume it's a no."
        << std::endl;
    std::string userInput;
    std::string command;
    std::vector<std::string> commandList;
    for (;;) {
      if (!std::getline(std::cin, userInput)) {
        break;
      }
      if (userInput.empty()) {
        break;
      } else {
        try {
          int index = std::stoi(userInput);
          if (index > 0 && index <= matches.size()) {
            playVideo(matches[index - 1].getVideoId());
            break;
          } else {
            break;
          }
        } catch (const std::invalid_argument &ia) {
          break;
        }
      }
    }
  } else {
    std::cout << "No search results for " << searchTerm << std::endl;
  }
}

void VideoPlayer::searchVideosWithTag(const std::string &videoTag) {
  auto videos = mVideoLibrary.getVideos();
  std::vector<Video> matches;
  for (auto video : videos) {
    if (mVideoLibrary.getFlag(video.getVideoId())) {
      continue;
    }
    std::vector<std::string> tags;
    for (auto tag : video.getTags()) {
      tags.push_back(stringToUpper(tag));
    }
    if (std::find(tags.begin(), tags.end(), stringToUpper(videoTag)) !=
        tags.end()) {
      matches.push_back(video);
    }
  }
  if (matches.size()) {
    std::cout << "Here are the results for " << videoTag << ":" << std::endl;
    int counter = 1;
    std::sort(matches.begin(), matches.end(), videoLexCompare);
    for (auto video : matches) {
      std::cout << "\t" << counter << (") ") << VideoToString(video)
                << std::endl;
      counter++;
    }
    std::cout << "Would you like to play any of the above? If yes, specify the "
                 "number of the video."
              << std::endl;
    std::cout
        << "If your answer is not a valid number, we will assume it's a no."
        << std::endl;
    std::string userInput;
    std::string command;
    std::vector<std::string> commandList;
    for (;;) {
      if (!std::getline(std::cin, userInput)) {
        break;
      }
      if (userInput.empty()) {
        break;
      } else {
        try {
          int index = std::stoi(userInput);
          if (index > 0 && index <= matches.size()) {
            playVideo(matches[index - 1].getVideoId());
            break;
          } else {
            break;
          }
        } catch (const std::invalid_argument &ia) {
          break;
        }
      }
    }
  } else {
    std::cout << "No search results for " << videoTag << std::endl;
  }
}

void VideoPlayer::flagVideo(const std::string &videoId) {
  flagVideo(videoId, "Not supplied");
}

void VideoPlayer::flagVideo(const std::string &videoId,
                            const std::string &reason) {
  if (auto video = mVideoLibrary.getVideo(videoId)) {
    if (mVideoLibrary.getFlag(videoId)) {
      std::cout << "Cannot flag video: Video is already flagged" << std::endl;
    } else {
      mVideoLibrary.addFlag(videoId, reason);
      if (CurrentlyPlaying) {
        if (CurrentlyPlaying->getVideoId().compare(videoId) == 0) {
          stopVideo();
        }
      }
      std::cout << "Successfully flagged video: " << video->getTitle()
                << " (reason: " << reason << ")" << std::endl;
    }
  } else {
    std::cout << "Cannot flag video: Video does not exist" << std::endl;
  }
}

void VideoPlayer::allowVideo(const std::string &videoId) {
  if (auto video = mVideoLibrary.getVideo(videoId)) {
    if (mVideoLibrary.getFlag(videoId)) {
      mVideoLibrary.deleteFlag(videoId);
      std::cout << "Successfully removed flag from video: " << video->getTitle()
                << std::endl;
    } else {
      std::cout << "Cannot remove flag from video: Video is not flagged"
                << std::endl;
    }
  } else {
    std::cout << "Cannot remove flag from video: Video does not exist"
              << std::endl;
  }
}

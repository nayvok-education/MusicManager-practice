#ifndef SONG_H
#define SONG_H

#include <string>

class Song {
private:
    std::string title;
    std::string artist;

public:
    Song(const std::string& title, const std::string& artist)
        : title(title), artist(artist) {}

    std::string getTitle() const { return title; }
    std::string getArtist() const { return artist; }
};

#endif // SONG_H

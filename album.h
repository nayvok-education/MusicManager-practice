#ifndef ALBUM_H
#define ALBUM_H

#include <string>
#include <list>
#include "song.h"

class Album {
private:
    std::string albumName;
    std::string coverPhoto;
    std::string artist;
    std::list<Song> songs;
    int year;
    std::string country;

public:
    Album(const std::string& albumName, const std::string& coverPhoto, const std::string& artist, const std::list<Song>& songs, int year, const std::string& country)
        : albumName(albumName), coverPhoto(coverPhoto), artist(artist), songs(songs), year(year), country(country) {}

    std::string getAlbumName() const { return albumName; }
    std::string getCoverPhoto() const { return coverPhoto; }
    std::string getArtist() const { return artist; }
    std::list<Song> getSongs() const { return songs; }
    int getYear() const { return year; }
    std::string getCountry() const { return country; }
};

#endif // ALBUM_H

#include <iostream>
#include <string>
#include <list>
#include <sstream>
#include <memory>
#include <iomanip>
#include <algorithm>

class Song
{
private:
    std::string name;
    std::string artist;
    int ranking;

public:
    Song(std::string name = "No name", std::string artist = "No artist", int ranking = 0)
        : name{name}, artist{artist}, ranking{ranking} {}

    // we will use this for overloading >> operator
    void input(std::istream &is)
    {

        std::cout << "Name of the song: ";
        std::cin.clear();
        std::cin >> std::ws;
        getline(is, name);

        std::cout << "Name of the artist: ";
        getline(is, artist);

        std::cout << "Ranking between 1-5: ";

        // Checking for valid integer entry
        std::string entry{};
        bool check = false;

        do
        {
            std::cin >> entry;
            std::istringstream iss{entry};

            if ((iss >> ranking) && (ranking >= 1 && ranking <= 5) && entry.length() == 1)
            {
                check = true;
            }
            else
                std::cout << "Invalid input! Try again: ";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        } while (!check);
    }

    // We will use this for overloading << operator
    void output(std::ostream &os) const
    {
        os << std::setw(30) << std::left << name << std::setw(15) << std::left << artist
           << std::setw(10) << std::right << ranking << std::endl;
    }

    bool operator<(const Song &rhs)
    {
        return (this->ranking < rhs.ranking);
    }

    bool operator==(const Song &rhs)
    {
        return (this->name == rhs.name && this->artist == rhs.artist);
    }
};

std::istream &operator>>(std::istream &is, Song &s)
{
    s.input(is);
    return is;
}

std::ostream &operator<<(std::ostream &os, const Song &s)
{
    s.output(os);
    return os;
}

void display(const std::list<Song> &l)
{
    if (l.size() != 0)
    {
        for (const auto &s : l)
            std::cout << s;
    }
    else
        std::cout << "No songs" << std::endl;
    std::cout << std::endl
              << std::endl;
}

void add_song(std::list<Song>::iterator &it, std::list<Song> &l)
{
    Song temp;
    std::cin >> temp;

    auto loc = std::find(l.begin(), l.end(), temp);
    if (loc == l.end())
    {
        l.insert(it, temp);
        std::advance(it, -1);
        std::cout << *it << std::endl;
    }
    else
        std::cout << "Sorry! The song is already in the playlist!" << std::endl;
}

auto delete_song(std::list<Song>::iterator &it, std::list<Song> &l)
{
    // to prevent iterator invalidation we are returning a valid iterator
    if (l.size() != 0)
    {
        auto it1 = it;
        std::advance(it1, 1);
        l.erase(it);
        return it1;
    }
    else
        std::cout << "No songs" << std::endl;
}

void play_song(std::list<Song>::iterator &it, const std::list<Song> &l)
{
    if (l.size() != 0)
    {
        std::cout << *it << std::endl;
    }
    else
        std::cout << "No songs" << std::endl;
}

void next_song(std::list<Song>::iterator &it, std::list<Song> &l)
{
    if (l.size() != 0)
    {
        if (it == (--l.end()))
            it = l.begin();
        else
            std::advance(it, 1);
        std::cout << *it << std::endl;
    }
    else
        std::cout << "No songs" << std::endl;
}

void previous_song(std::list<Song>::iterator &it, const std::list<Song> &l)
{
    if (l.size() != 0)
    {
        if (it == l.begin())
            std::advance(it, -2);
        else
            std::advance(it, -1);
        std::cout << *it << std::endl;
    }
    else
        std::cout << "No songs" << std::endl;
}

int main()
{
    std::unique_ptr<std::list<Song>> ptr = std::make_unique<std::list<Song>>();

    // compared to push_back, emplace_back() doesn't use copy constructor. Hence more efficient.
    ptr->emplace_back("Old Town Road", "Lil Nas X", 5);
    ptr->emplace_back("Guelpembe", "Baris Manco", 5);
    ptr->emplace_back("Somewhere I Belong", "Linkin Park", 5);
    ptr->emplace_back("Belalim", "Sezen Aksu", 5);
    ptr->emplace_back("Elfida", "Haluk Levent", 5);

    std::list<Song>::iterator it = ptr->begin();

    char c{};
    std::string str;
    bool check = false;

    do
    {
        std::cout << "What do you want to do?" << std::endl
                  << std::endl;
        std::cout << "F: Play current song" << std::endl
                  << "N: Play next song" << std::endl
                  << "P: Play previous song" << std::endl
                  << "A: Add and play a new song at current location" << std::endl
                  << "E: Erase the song at current location" << std::endl
                  << "S: Show the current playlist" << std::endl
                  << "Q: Quit" << std::endl
                  << std::endl;

        //Checking for valid char input. If it is not valid, than goes to default in switch(c) and so on.
        std::cin >> str;
        if (str.length() == 1)
        {
            c = toupper(str.at(0));
        }
        else
            c = 'x';

        std::cin.clear();
        std::cout << std::endl;

        switch (c)
        {
        case 'F':
            play_song(it, *ptr);
            break;
        case 'N':
            next_song(it, *ptr);
            break;
        case 'P':
            previous_song(it, *ptr);
            break;
        case 'A':
            add_song(it, *ptr);
            break;
        case 'E':
            it = delete_song(it, *ptr);
            break;
        case 'S':
            display(*ptr);
            break;
        case 'Q':
            std::cout << "Goodbye!" << std::endl;
            check = true;
            break;
        default:
            std::cout << "Undefined input!" << std::endl;
            break;
        }

    } while (check == false);

    return 0;
}
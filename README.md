# Pitch Perfect – C Music Player

**Pitch Perfect** is a menu-driven music player developed in C that allows users to create and manage playlists, play songs based on their mood, queue upcoming tracks, and control playback. The project uses a **doubly linked list** to manage songs dynamically and the Windows Multimedia API to play audio files.

## Features

- **Default Playlist:** Generates a mixed playlist by randomly selecting songs from the music catalog.
- **Custom Playlist:** Create your own playlist by selecting songs from the available catalog.
- **Mood-Based Playlist:** Choose songs based on moods such as Romantic, Feel-good, and Sad.
- **Queue Management:** Play a selected song immediately and add songs to play next.
- **Pick a Song:** Select and play an individual song, with options to pause, resume, or stop playback.
- **Next and Previous Navigation:** Move forward and backward through the playlist.
- **Insert Songs:** Add a song immediately after the currently selected song.
- **Delete Songs:** Remove the current song from the playlist.
- **Reverse Playlist:** Navigate through the playlist in reverse order.
- **Playlist Sorting:** Sort songs alphabetically by song name or mood by rearranging linked-list nodes.
- **Playlist Display:** View the songs currently stored in a playlist or queue.

## Technologies Used

- **Programming Language:** C
- **Data Structure:** Doubly Linked List
- **Audio Playback:** Windows Multimedia API (`mciSendString`)
- **Libraries:** `stdio.h`, `stdlib.h`, `windows.h`, `time.h`, `string.h`, `conio.h`

## Data Structures and Concepts

### Doubly Linked List

Each node stores a song and pointers to the previous and next nodes. This allows navigation in both directions and supports dynamic playlist operations.

### Playlist Operations

The project implements insertion, deletion, traversal, and sorting of linked-list nodes. During sorting, the nodes themselves are rearranged instead of merely swapping song data.

### Windows Multimedia API

The `mciSendString()` function is used to open, play, pause, resume, and close audio playback through Windows multimedia commands.

### Random Song Selection

The default playlist uses random selection to create a mixed listening experience.

## Requirements

- A Windows operating system
- A C compiler, such as GCC with Windows support or MinGW
- MP3 audio files stored locally on the computer
- A terminal that supports the required Windows libraries

## How to Run

1. Clone or download this repository.
2. Ensure that the required MP3 files are available on your computer.
3. Update the file paths in the song database to match the locations of your audio files.
4. Compile the source file using a compatible Windows C compiler.

Example using GCC:

```bash
gcc pitch_perfect.c -o pitch_perfect.exe
```

5. Run the executable:

```bash
pitch_perfect.exe
```

*Replace `pitch_perfect.c` with the actual name of your C source file.*

## How to Use

1. Launch the program.
2. Select an option from the main menu.
3. Create a playlist, choose a mood, start a queue, or pick an individual song.
4. Use the playback menu to navigate, insert or delete songs, reverse the playlist, or sort it.
5. Return to the main menu when you finish listening.

## Limitations

- The program is designed for Windows because it uses Windows-specific multimedia and keyboard functions.
- Song file paths are configured directly in the source code.
- The song catalog currently contains nine initialized songs.
- Playlist data is maintained in memory and is not saved permanently between program executions.

## Future Improvements

- Add support for searching songs by name or artist.
- Store playlists in files so they can be loaded in future sessions.
- Add artist-based sorting and filtering.
- Implement automatic playback of the next queued song when the current song finishes.
- Improve input validation and playback error handling.
- Add a graphical user interface for a better user experience.

## Project Overview

Pitch Perfect demonstrates the practical application of C programming concepts, particularly structures, pointers, dynamic memory allocation, doubly linked lists, and modular programming, in building an interactive music player.

**Project:** Pitch Perfect – C Music Player  
**Language:** C  
**Core Data Structure:** Doubly Linked List

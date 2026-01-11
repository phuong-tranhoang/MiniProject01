// #include "../include/InputHandler.h"
// #include <iostream>

// void processInput(MusicPlayer& player, AudioEngine& engine) {
//     std::cout << "Command (n=Next, p=Prev, space=Pause/Play, s=Shuffle): ";
//     char cmd;
//     std::cin >> cmd; // This BLOCKS. In GUI, this would be an Event Callback.

//     switch(cmd) {
//         case 'n': 
//             player.playNext(); 
//             // Logic to sync AudioEngine
//             if(player.getCurrentSong()) 
//                 engine.playFile(player.getCurrentSong()->filePath);
//             break;
            
//         case 'p': 
//             player.playPrevious(); 
//              if(player.getCurrentSong()) 
//                 engine.playFile(player.getCurrentSong()->filePath);
//             break;
            
//         case ' ': // Spacebar
//             if (engine.isPlaying()) engine.togglePaused(true);
//             else engine.togglePaused(false);
//             break;
            
//         case 's':
//             player.enableShuffle();
//             break;
//     }
// }
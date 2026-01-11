// LoopSimulation.h
// Audio Player simulation

#ifndef LOOP_SIM_H
#define LOOP_SIM_H

#include "MusicPlayer.h"

class LoopSimulation{
private:
    MusicPlayer player;
    std::string folderPath;
public:
    /**===================================================
     * 
     * Description: Construct LoopSimulation class
     *
     * @param {none}
     * @returns {none}  
     */
    LoopSimulation();
    /**===================================================
     * 
     * Description: Destruct LoopSimulation class
     *
     * @param {none}
     * @returns {none}  
     */
    ~LoopSimulation();
    /**===================================================
     * 
     * Description: Auto play when song finishes
     *
     * @param {none}
     * @returns {none}  
     */
    void autoPlay();
    /**===================================================
     * 
     * Description: Simulation superloop
     *
     * @param {none}
     * @returns {none}  
     */
    void simLoop();
};
#endif
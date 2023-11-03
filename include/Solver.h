#ifndef SOLVER_H
#define SOLVER_H

#include "Objects.h"
#include "Grid.h"

class Solver
{
private:
    float time;                     // seconds
        
    std::vector<Circle> objects;
    Grid grid;
    Vec2D GRAVITY;
    RectBounds BOUNDS;
    int FRAMERATE;                  // fps
    int SUBSTEPS;
    float DT;                       // seconds
    float SUBDT;                    // seconds
    int MAX_OBJECTS;
    float SPAWN_INTERVAL;           // seconds

    void applyGravity();
    void applyCollisions();
    void applyRestitution();
    void updateObjects();

    void collisionDetectionThread(int, int);
        
public:
    Solver();

    void setGravity(const Vec2D&);
    void setBounds(const RectBounds&);
    void setFramerate(int);
    void setSubsteps(int);
    void setMaxObjects(int);
    void setSpawnInterval(float);

    RectBounds* getBounds();
    Grid* getGrid();
    int getFramerate() const;
    int getSubsteps() const;
    int getMaxObjects() const;
    float getSpawnInterval() const;
    int getObjectCount() const;
    const std::vector<Circle>& getObjects() const;
        
    void addObject(const Circle &);
    void updateSolver();
};

#endif
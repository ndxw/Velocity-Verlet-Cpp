#ifndef SOLVER_H
#define SOLVER_H

#include "Objects.h"
#include "Grid.h"

class Solver
{
    private:
        float time;                     // seconds
        
        
        Vec2D GRAVITY;
        RectBounds BOUNDS;
        int FRAMERATE;                  // fps
        int SUBSTEPS;
        float DT;                       // seconds
        float SUBDT;                    // seconds
        int MAX_OBJECTS;
        float SPAWN_INTERVAL;           // seconds

        void applyGravity();
        void applyBounds();
        void applyCollisions();
        void applyRestitution();
        void updateObjects();

        bool isTopRow(int);
        bool isBottomRow(int);
        bool isLeftCol(int);
        bool isRightCol(int);
        
    public:
        std::vector<Circle> objects;
        Grid grid;  // private it once done testing

        Solver();
        ~Solver();

        void setGravity(const Vec2D&);
        void setBounds(const RectBounds&);
        void setFramerate(int);
        void setSubsteps(int);
        void setMaxObjects(int);
        void setSpawnInterval(float);

        const RectBounds& getBounds() const;
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
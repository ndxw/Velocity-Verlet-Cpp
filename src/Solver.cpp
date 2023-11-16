#include "../include/Solver.h"
#include <iostream>
#include <thread>
#include <cmath>

#include <QtWidgets/qmessagebox.h>

/// <summary>
/// Constructs a solver environment with the following parameters:
///     <c>GRAVITY</c>: (0, 3000)px/s/s
///     <c>BOUNDS</c>: (0, 700, 0, 700) px
///     <c>FRAMERATE</c>: 60 fps
///     <c>SUBSTEPS</c>: 1
///     <c>MAX_OBJECTS</c>: 100
///     <c>SPAWN_INTERVAL</c>: 1 second
/// </summary>
Solver::Solver()
{
    objects.clear();
    GRAVITY = Vec2D(0.f, 3000.f);
    BOUNDS = RectBounds();
    grid = Grid(Circle::getMaxRadius(), BOUNDS.right, BOUNDS.down);
    FRAMERATE = 60;
    SUBSTEPS = 1;
    MAX_OBJECTS = 100;
    SPAWN_INTERVAL = 1.f;
    paused = false;
    autoSpawning = true;
}


/// <summary>
/// Setter for <c>GRAVITY</c>.
/// </summary>
/// <param name="gravity"></param>
void Solver::setGravity(const Vec2D& gravity)
{
    GRAVITY = gravity;
}

/// <summary>
/// Setter for <c>BOUNDS</c>.
/// </summary>
/// <param name="bounds"></param>
void Solver::setBounds(const RectBounds& bounds)
{
    BOUNDS = bounds;
}

/// <summary>
/// Setter for <c>FRAMERATE</c>.
/// </summary>
/// <param name="framerate">Must be a value between 30 and 240 inclusive.</param>
void Solver::setFramerate(int framerate)
{
    framerate = std::max(framerate, 30);
    framerate = std::min(framerate, 240);
    FRAMERATE = framerate;
}

/// <summary>
/// Setter for <c>SUBSTEPS</c>. This is the number of times bounds checking, collision detection, and object updates will be performed for each frame. Higher values will reduce object intersection at the cost of framerate.
/// </summary>
/// <param name="substeps">Must be a value between 1 and 16 inclusive.</param>
void Solver::setSubsteps(int substeps)
{
    substeps = std::max(substeps, 1);
    substeps = std::min(substeps, 16);
    SUBSTEPS = substeps;
}

/// <summary>
/// Setter for <c>MAX_OBJECTS</c>. Solver will stop spawning objects once this limit has been reached.
/// </summary>
/// <param name="maxObjects">Must be a value greater than or equal to 0.</param>
void Solver::setMaxObjects(int maxObjects)
{
    maxObjects = std::max(maxObjects, 0);
    MAX_OBJECTS = maxObjects;
    objects.reserve(MAX_OBJECTS);
}

/// <summary>
/// Setter for <c>SPAWN_INTERVAL</c>. This is the rate at which the solver will spawn objects, in seconds.
/// </summary>
/// <param name="interval">Must be a value greater than or equal to 0.001 seconds.</param>
void Solver::setSpawnInterval(float interval)
{
    interval = std::max(interval, 0.001f);
    SPAWN_INTERVAL = interval;
}

RectBounds* Solver::getBounds()             { return &BOUNDS; }
Grid* Solver::getGrid()                     { return &grid; }
int Solver::getFramerate() const            { return FRAMERATE; }
int Solver::getSubsteps() const             { return SUBSTEPS; }
int Solver::getMaxObjects() const           { return MAX_OBJECTS; }
float Solver::getSpawnInterval() const      { return SPAWN_INTERVAL; }
int Solver::getObjectCount() const          { return int(objects.size()); }
const std::vector<Circle>& Solver::getObjects() const { return objects; }

/// <summary>
/// Sets object acceleration value to that of <c>GRAVITY</c>.
/// </summary>
void Solver::applyGravity()
{
    for (auto &object : objects)
    {
        object.acl.setX(GRAVITY.x());
        object.acl.setY(GRAVITY.y());
    }
}

/// <summary>
/// Partitions objects into grid cells and creates threads to perform collision detection on the objects.
/// </summary>
void Solver::applyCollisions()
{
    grid.partitionObjects(objects);

    // should be solver attributes
    int threadCount = 4;
    int cellsPerThread = int(grid.WIDTH / threadCount) * grid.HEIGHT;
    int finalThreadCellCount = grid.WIDTH - (threadCount * cellsPerThread);

    // create threads 
    std::vector<std::thread*> threadPool;
    for (int thread = 0; thread < threadCount; thread++) {
        int startCellIdx = thread * cellsPerThread;
        int endCellIdx = (thread == threadCount - 1) ? int(grid.cells.size()) : startCellIdx + cellsPerThread;
        std::thread* th_collision = new std::thread(&Solver::collisionDetectionThread, this, startCellIdx, endCellIdx);
        threadPool.push_back(th_collision);
    }

    // wait for all threads to finish
    for (auto th_collision : threadPool) { th_collision->join(); delete th_collision; }

    grid.resetCells();
}

/// <summary>
/// The thread function that performs collision detection.
/// </summary>
/// <param name="startCellIdx">Always in the bottom row of cells by design.</param>
/// <param name="endCellIdx">Always in the top row of cells by design.</param>
void Solver::collisionDetectionThread(int startCellIdx, int endCellIdx) {

    for (int cellIdx = startCellIdx; cellIdx < endCellIdx; cellIdx++) {
        if (grid.isLeftCol(cellIdx) || grid.isRightCol(cellIdx) ||
            grid.isTopRow(cellIdx) || grid.isBottomRow(cellIdx)) continue;

        // create list of obj in current and adjacent cells
        int kernelCells[9] = { cellIdx - grid.HEIGHT - 1, cellIdx - grid.HEIGHT, cellIdx - grid.HEIGHT + 1,
                               cellIdx - 1,               cellIdx,               cellIdx + 1,
                               cellIdx + grid.HEIGHT - 1, cellIdx + grid.HEIGHT, cellIdx + grid.HEIGHT + 1 };
        std::vector<Circle*> kernelObjs;
        for (int kCellIdx : kernelCells) {
            for (Circle* obj : grid.cells.at(kCellIdx)) { kernelObjs.push_back(obj); }
        }

        // begin collision checks within kernel
        for (int i = 0; i < kernelObjs.size(); i++) {
            for (int j = i + 1; j < kernelObjs.size(); j++) {
                Circle* obj1 = kernelObjs.at(i);
                Circle* obj2 = kernelObjs.at(j);
                Vec2D posDiff12;
                Vec2D::subtract(posDiff12, obj1->pos, obj2->pos);

                // no overlap, therefore no collision, so skip
                if (posDiff12.length() >= obj1->radius + obj2->radius) continue;

                /*
                Calculate new velocities using equations for two-dimensional
                collision with two moving objects

                Equations in vector representation can be
                found @ https://en.wikipedia.org/wiki/Elastic_collision
                */
                Vec2D velDiff12, posDiffScaled12;
                Vec2D::subtract(velDiff12, obj1->vel, obj2->vel);
                float scalar1 = (2.0f * obj2->mass / (obj1->mass + obj2->mass))
                    * Vec2D::dot(velDiff12, posDiff12)
                    / (posDiff12.length() * posDiff12.length());
                Vec2D::scale(posDiffScaled12, posDiff12, scalar1);

                Vec2D posDiff21, velDiff21, posDiffScaled21;
                Vec2D::subtract(posDiff21, obj2->pos, obj1->pos);
                Vec2D::subtract(velDiff21, obj2->vel, obj1->vel);
                float scalar2 = (2.0f * obj1->mass / (obj1->mass + obj2->mass))
                    * Vec2D::dot(velDiff21, posDiff21)
                    / (posDiff21.length() * posDiff21.length());
                Vec2D::scale(posDiffScaled21, posDiff21, scalar2);

                /*
                Update velocities after computing both to avoid using new v1
                in calculation for new v2
                */
                Vec2D::subtract(obj1->vel, obj1->vel, posDiffScaled12);
                Vec2D::subtract(obj2->vel, obj2->vel, posDiffScaled21);

                /*
                Update positions by shifting each object by half the overlap
                in opposite directions along the collision axis
                */
                float overlap = obj1->radius + obj2->radius - posDiff12.length();

                Vec2D updatePos1, updatePos2;
                Vec2D::scale(updatePos1, posDiff12, 0.5f * overlap / posDiff12.length());
                obj1->pos.add(updatePos1);

                Vec2D::scale(updatePos2, posDiff21, 0.5f * overlap / posDiff21.length());
                obj2->pos.add(updatePos2);

                //Set collision status for velocity scaling later
                obj1->collided = true; obj2->collided = true;
            }
        }
    }
}

/// <summary>
/// Calls the <c>update</c> function on all objects.
/// </summary>
void Solver::updateObjects(float subdt)
{
    for (auto &object : objects) { object.update(subdt); }
}

/// <summary>
/// Applies the coefficient of restitution to the velocities of objects that underwent collision this frame.
/// </summary>
void Solver::applyRestitution()
{
    /*
    Applying coefficient of restitution within applyCollisions() loop
    without a collided flag resulted in it being applied multiple times 
    for a single collision
    */
    for (auto& obj : objects) {
        if (obj.collided) {
            obj.vel.scale(obj.restitutionCoeff);
            obj.collided = false;
        }
    }
}

/// <summary>
/// Adds a <c>Circle</c> object to the solver environment.
/// </summary>
/// <param name="obj"></param>
void Solver::addObject(const Circle &obj){ objects.push_back(obj); }

void Solver::addSpawner(const Spawner& spawner) { spawners.push_back(spawner); }

/// <summary>
/// Calls all the necessary functions <c>SUBSTEPS</c> times to calculate the objects' parameters in the succeeding frame. 
/// </summary>
void Solver::updateSolver(float dt)
{
    if (paused) return;
    float subdt = dt / float(SUBSTEPS);

    for (int substep = 0; substep < SUBSTEPS; substep++)
    {
        applyGravity();
        BOUNDS.applyBounds(objects);
        applyCollisions();
        applyRestitution();
        updateObjects(subdt);
    }

    if (!autoSpawning) return;
    for (Spawner& spawner : spawners) {
        if (objects.size() >= MAX_OBJECTS) break;

        if (spawner.timer.getElapsedTime().asSeconds() >= spawner.interval) {
            Circle circle = Circle();
            Circle::generateRandomObject(circle);
            circle.pos = spawner.pos;
            circle.vel = spawner.vel;
            addObject(circle);
            spawner.timer.restart();
        }
    }
}

void Solver::restart() { objects.clear(); }
void Solver::togglePause() { paused = !paused; }
void Solver::setAutoSpawning(bool value) { autoSpawning = value; }
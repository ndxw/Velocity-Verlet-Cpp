#include "../include/Solver.h"
#include <iostream>
#include <thread>
#include <cmath>

Solver::Solver()
{
    time = 0.f;
    objects.clear();
    GRAVITY = Vec2D(0.f, 3000.f);
    BOUNDS = RectBounds();
    grid = Grid(Circle::getMaxRadius(), BOUNDS.right, BOUNDS.down);
    FRAMERATE = 60;
    SUBSTEPS = 1;
    DT = 1 / float(FRAMERATE);
    SUBDT = DT / float(SUBSTEPS);
    MAX_OBJECTS = 100;
    SPAWN_INTERVAL = 1.f;
}

Solver::~Solver() {}

void Solver::setGravity(const Vec2D& gravity)
{
    GRAVITY = gravity;
}

void Solver::setBounds(const RectBounds& bounds)
{
    BOUNDS = bounds;
}

void Solver::setFramerate(int framerate)
{
    framerate = std::max(framerate, 0);
    framerate = std::min(framerate, 240);
    FRAMERATE = framerate;
    DT = 1 / float(FRAMERATE);
    SUBDT = DT / float(SUBSTEPS);
}

void Solver::setSubsteps(int substeps)
{
    substeps = std::max(substeps, 0);
    substeps = std::min(substeps, 16);
    SUBSTEPS = substeps;
    SUBDT = DT / float(SUBSTEPS);
}

void Solver::setMaxObjects(int maxObjects)
{
    maxObjects = std::max(maxObjects, 0);
    MAX_OBJECTS = maxObjects;
    objects.reserve(MAX_OBJECTS);
}

void Solver::setSpawnInterval(float interval)
{
    interval = std::max(interval, 0.001f);
    SPAWN_INTERVAL = interval;
}

const RectBounds& Solver::getBounds() const { return BOUNDS; }
int Solver::getFramerate() const            { return FRAMERATE; }
int Solver::getSubsteps() const             { return SUBSTEPS; }
int Solver::getMaxObjects() const           { return MAX_OBJECTS; }
float Solver::getSpawnInterval() const      { return SPAWN_INTERVAL; }
int Solver::getObjectCount() const          { return int(objects.size()); }
const std::vector<Circle>& Solver::getObjects() const { return objects; }

void Solver::applyGravity()
{
    for (auto &object : objects)
    {
        object.acl.setX(GRAVITY.x());
        object.acl.setY(GRAVITY.y());
    }
}

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
        int endCellIdx = (thread == threadCount - 1) ? (int)grid.cells.size() : startCellIdx + cellsPerThread;
        std::thread* th_collision = new std::thread(&Solver::collisionDetectionThread, this, startCellIdx, endCellIdx);
        threadPool.push_back(th_collision);
    }

    // wait for all threads to finish
    for (auto th_collision : threadPool) { th_collision->join(); delete th_collision; }

    grid.resetCells();
}

void Solver::collisionDetectionThread(int startCellIdx, int endCellIdx) {

    for (int cellIdx = startCellIdx; cellIdx < endCellIdx; cellIdx++) {
        if (isLeftCol(cellIdx) || isRightCol(cellIdx) ||
            isTopRow(cellIdx) || isBottomRow(cellIdx)) continue;

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

void Solver::applyBounds()
{
    for (auto &object : objects)
    {
        // collision with right wall
        if (object.pos.x() + object.radius > BOUNDS.right)
        {
            object.pos.setX(float(BOUNDS.right - object.radius));
            object.vel.mirrorAboutY();
            object.vel.scale(object.restitutionCoeff);
        }
        // collision with left wall
        else if (object.pos.x() - object.radius < BOUNDS.left)
        {
            object.pos.setX(float(BOUNDS.left + object.radius));
            object.vel.mirrorAboutY();
            object.vel.scale(object.restitutionCoeff);
        }
        // collision with ceiling
        if (object.pos.y() - object.radius < BOUNDS.up)
        {
            object.pos.setY(float(BOUNDS.up + object.radius));
            object.vel.mirrorAboutX();
            object.vel.scale(object.restitutionCoeff);
        }
        // collision with floor
        else if (object.pos.y() + object.radius > BOUNDS.down)
        {
            object.pos.setY(float(BOUNDS.down - object.radius));
            object.vel.mirrorAboutX();
            object.vel.scale(object.restitutionCoeff);
        }
    }
}

void Solver::updateObjects()
{
    for (auto &object : objects) { object.update(SUBDT); }
}

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

void Solver::addObject(const Circle &obj)
{
    objects.push_back(obj);
}

void Solver::updateSolver()
{
    for (int substep = 0; substep < SUBSTEPS; substep++)
    {
        applyGravity();
        applyBounds();
        applyCollisions();
        applyRestitution();
        updateObjects();
    }
    time += DT;
}

bool Solver::isTopRow(int cellIdx) { return (cellIdx % grid.HEIGHT) == (grid.HEIGHT - 1); }
bool Solver::isBottomRow(int cellIdx) { return (cellIdx % grid.HEIGHT) == 0; }
bool Solver::isLeftCol(int cellIdx) { return cellIdx < grid.HEIGHT; }
bool Solver::isRightCol(int cellIdx) { return cellIdx >= (grid.cells.size() - grid.HEIGHT); }
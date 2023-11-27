#ifndef SOLVER_H
#define SOLVER_H

#include "Objects.h"
#include "Grid.h"
#include "DTO.h"

#include <QtCore/qobject.h>
#include <QtWidgets/qabstractbutton.h>

class Solver : public QObject
{
    Q_OBJECT

private:
    std::vector<Circle> objects;
    std::vector<Spawner> spawners;
    Grid grid;
    Vec2D GRAVITY;
    RectBounds BOUNDS;
    int FRAMERATE;                  // fps
    int SUBSTEPS;
    int MAX_OBJECTS;
    float SPAWN_INTERVAL;           // seconds

    void applyGravity();
    void applyCollisions();
    void applyRestitution();
    void updateObjects(float);

    void collisionDetectionThread(int, int);
        
public:
    bool paused;
    bool autoSpawning;

    Solver();

    void setGravity(const Vec2D&);
    void setBounds(const RectBounds&);
    void setSpawnInterval(float);

    Vec2D getGravity() const;
    RectBounds* getBounds();
    Grid* getGrid();
    int getFramerate() const;
    int getSubsteps() const;
    int getMaxObjects() const;
    float getSpawnInterval() const;
    int getObjectCount() const;
    const std::vector<Circle>& getObjects() const;
        
    void addObject(const Circle&);
    void addSpawner(const Spawner&);
    void updateSolver(float);

signals:
    void returnSpawner(Spawner*);
    void returnSpawnerIDs(std::vector<std::string>);

public slots:
    void restart();
    void togglePause();

    void setAutoSpawning(bool);
    void setFramerate(int);
    void setSubsteps(int);
    void setMaxObjects(int);
    void setGravity(float, float);

    void addSpawner(SpawnerDTO);
    void retrieveSpawner(std::string);
    void updateSpawner(std::string, SpawnerDTO);
    void retrieveSpawnerIDs();
};

#endif
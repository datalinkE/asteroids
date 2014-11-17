#pragma once

#include <list>
#include <memory>
#include "GameObject.h"
#include "Geometry.hpp"
#include "Logger.hpp"

class QuadTree;
typedef std::unique_ptr<QuadTree> QuadTreePtr;

typedef std::list<GameObject*> GameObjectList;

class QuadTree
{
public:
    QuadTree(int level, const Geometry::Rect &bounds)
        : mLevel(level)
        , mBounds(bounds)
    {
        if (mLevel < MAX_LEVELS)
        {
            glm::vec2 center = 0.5f * (bounds.leftBot + bounds.rightTop);
            glm::vec2 rightC(bounds.rightTop[0], center[1]);
            glm::vec2 topC(center[0], bounds.rightTop[1]);
            glm::vec2 leftC(bounds.leftBot[0], center[1]);
            glm::vec2 botC(center[0], bounds.leftBot[1]);

            mNodes[0].reset(new QuadTree(mLevel+1, Geometry::Rect {center, bounds.rightTop} ));
            mNodes[1].reset(new QuadTree(mLevel+1, Geometry::Rect {leftC, topC} ));
            mNodes[2].reset(new QuadTree(mLevel+1, Geometry::Rect {bounds.leftBot, center} ));
            mNodes[3].reset(new QuadTree(mLevel+1, Geometry::Rect {botC, rightC} ));
        }
    }

    virtual ~QuadTree()
    {
    }

    void split()
    {

    }

    int getIndex(GameObject* obj)
    {
        return -1;
    }

    void insert(GameObject* obj)
    {

    }

    GameObjectList retrieve(GameObject* obj)
    {
        return GameObjectList();
    }

    void clear()
    {
        mObjects.clear();
        if (mLevel >= MAX_LEVELS)
        {
            return;
        }

        for(int i = 0; i < NODES_COUNT; i++)
        {
            if (mNodes[i])
            {
                mNodes[i]->clear();
            }
        }
    }


private:
    static const int MAX_OBJECTS = 10;
    static const int MAX_LEVELS = 5;
    static const int NODES_COUNT = 4;

    int mLevel;
    GameObjectList mObjects;
    Geometry::Rect mBounds;

    GameObjectList mOutOfBoundsObjects; // level 0 only!

    QuadTreePtr mNodes[NODES_COUNT];
};

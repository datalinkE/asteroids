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

    enum { TOP_RIGHT = 0, TOP_LEFT = 1, BOT_LEFT = 2, BOT_RIGHT = 4, NOT_FIT = -1, OUT_OF_BOUNDS = -2};

    virtual ~QuadTree()
    {
    }

    int getIndex(GameObject* obj)
    {
        const glm::vec3& pos = obj->position();
        float r = obj->boundingRadius();

        glm::vec2 center = 0.5f * (mBounds.leftBot + mBounds.rightTop);

        bool itersectsX = std::abs(pos[0] - center[0]) < r;
        bool itersectsY = std::abs(pos[1] - center[1]) < r;

        if (itersectsX || itersectsY)
        {
            return NOT_FIT;
        }

        float distLeft = pos[0] - mBounds.leftBot[0];
        float distBot = pos[1] - mBounds.leftBot[1];
        float distRight = mBounds.rightTop[0] - pos[0];
        float distTop = mBounds.rightTop[1] - pos[1];

        if (distLeft < r ||
            distBot < r ||
            distRight < r ||
            distTop < r )
        {
            return NOT_FIT;
        }

        if (distLeft < 0 ||
            distBot < 0 ||
            distRight < 0 ||
            distTop < 0 )
        {
            return OUT_OF_BOUNDS;
        }

        int inLeftQuadrant = pos[0] < center[0];
        int inBotQuadrant = pos[1] < center[1];
        int posIndex = inLeftQuadrant + (1 << inBotQuadrant);

        return posIndex;
    }

    void insert(GameObject* obj)
    {
        if (mNodes[0])
        {
            int index = getIndex(obj);
            if (index != -1)
            {
                mNodes[index]->insert(obj);
                return;
            }
        }
        mObjects.push_back(obj);
    }

    GameObjectList retrieve(GameObject* obj)
    {
        GameObjectList returnObjects;

        int index = getIndex(obj);
        if (index != -1)
        {
            returnObjects.splice(returnObjects.end(), mNodes[index]->retrieve(obj));
        }
        returnObjects.insert(returnObjects.end(), mObjects.begin(), mObjects.end());
        return returnObjects;
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
    static const int MAX_LEVELS = 5;
    static const int NODES_COUNT = 4;

    int mLevel;
    GameObjectList mObjects;
    Geometry::Rect mBounds;

    QuadTreePtr mNodes[NODES_COUNT];
};

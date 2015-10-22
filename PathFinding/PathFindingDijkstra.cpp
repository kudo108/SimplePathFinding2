/****************************************************************************
 Copyright (c) 2015 QuanNguyen
 
 http://quannguyen.info
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/


#include "PathFindingDijkstra.h"

USING_NS_CC;

#define DEBUG_PRINT 1

namespace pathfinding {
    
    namespace dijkstra {
        
        PathFinding::PathFinding()
        {
            
        }
        
        PathFinding::~PathFinding()
        {
            for (auto ite = _graph.begin(); ite != _graph.end(); ite ++) {
                delete *ite;
            }
            
            _graph.clear();
        }
        
        bool PathFinding::init()
        {
            return true;
        }
        
        void PathFinding::setupMap(CollisionData *map)
        {
            CCASSERT(map, "Map must be not null");
            _map = map;
            
            //re generate the graph
            for (auto ite = _graph.begin(); ite != _graph.end(); ite ++) {
                delete *ite;
            }
            _graph.clear();
            
            generateGraph();
        }
        
        void PathFinding::generateGraph()
        {
            for (int x = 0; x < _map->getWidth(); x ++) {
                for (int y = 0; y < _map->getHeight(); y ++) {
                    if(!_map->haveCollisionAtCoord(x, y)){
                        Vertex *v = new Vertex(Vec2(x, y));
                        _graph.push_back(v);
                    }
                }
            }
        }
        
        std::vector<Vec2> PathFinding::getNearbyTileCoord(const cocos2d::Vec2 &tileCoord)
        {
            std::vector<Vec2> result;
            //top
            Vec2 top = Vec2(tileCoord.x, tileCoord.y - 1);
            if(isValidCoord(top) && canMoveAtCoord(top)){
                result.push_back(top);
            }
            
            //bottom
            Vec2 bottom = Vec2(tileCoord.x, tileCoord.y + 1);
            if(isValidCoord(bottom) && canMoveAtCoord(bottom)){
                result.push_back(bottom);
            }
            
            //left
            Vec2 left = Vec2(tileCoord.x - 1, tileCoord.y);
            if(isValidCoord(left) && canMoveAtCoord(left)){
                result.push_back(left);
            }
            
            //right
            Vec2 right = Vec2(tileCoord.x + 1, tileCoord.y);
            if(isValidCoord(right) && canMoveAtCoord(right)){
                result.push_back(right);
            }
            
            //top right
            Vec2 topR = Vec2(tileCoord.x + 1, tileCoord.y - 1);
            if(isValidCoord(topR) && canMoveAtCoord(topR)
               && canMoveAtCoord(top) && canMoveAtCoord(right)){
                result.push_back(topR);
            }
            
            //bottom right
            Vec2 bottomR = Vec2(tileCoord.x + 1, tileCoord.y + 1);
            if(isValidCoord(bottomR) && canMoveAtCoord(bottomR)
               && canMoveAtCoord(bottom) && canMoveAtCoord(right)){
                result.push_back(bottomR);
            }
            
            //top left
            Vec2 topL = Vec2(tileCoord.x - 1, tileCoord.y - 1);
            if(isValidCoord(topL) && canMoveAtCoord(topL)
               && canMoveAtCoord(top) && canMoveAtCoord(left)){
                result.push_back(topL);
            }
            
            //bottom left
            Vec2 bottomL = Vec2(tileCoord.x - 1, tileCoord.y + 1);
            if(isValidCoord(bottomL) && canMoveAtCoord(bottomL)
               && canMoveAtCoord(bottom) && canMoveAtCoord(left)){
                result.push_back(bottomL);
            }
            
            return result;
        }
        
        void PathFinding::insertIntoOpenList(pathfinding::dijkstra::Vertex *vertex, float weight)
        {
            int i = 0;
            if(_openList.size() == 0){
                _openList.push_back(vertex);
            }else{
                do {
                    if(weight < _openList.at(i)->getWeight()){
                        _openList.insert(_openList.begin() + i, vertex);
                        break;
                    }
                    i ++;
                }while(i < _openList.size());
                if(i == _openList.size()){
                    _openList.push_back(vertex);
                }
            }
        }
        
        std::vector<Vec2> PathFinding::getShortestPath(const cocos2d::Vec2 &fromCoord, const cocos2d::Vec2 &toCoord)
        {
#if DEBUG_PRINT
            CCLOG("*** PATH SEARCH BEGIN : dijkstra");
#endif
            std::vector<Vec2> result;
            // Check that there is a path to compute ;-)
            if(fromCoord.equals(toCoord)){
                return result;
            }
            
            if(!isValidCoord(fromCoord) || !canMoveAtCoord(fromCoord)){
                return result;
            }
            
            // Must check that the desired location is walkable
            // In our case it's really easy, because only wall are unwalkable
            if(!isValidCoord(toCoord) || !canMoveAtCoord(toCoord)){
                return result;
            }
            
#if DEBUG_PRINT
            CCLOG("num of vertex = %ld", _graph.size());
#endif
            for (auto ite = _graph.begin(); ite != _graph.end(); ite ++) {
                auto obj = *ite;
                obj->setMarked(false);
                obj->setWeight(FLT_MAX);
                obj->setParent(nullptr);
            }
            _openList.clear();
            
            //start at first position
            auto startInGraph = *getIte(_graph, fromCoord);
            startInGraph->setParent(nullptr);
            startInGraph->setWeight(0.0f);
            
            do{
                startInGraph->setMarked(true);
                
                //push all nearby vertex to open list (if it not in close list)
                auto nearbyVertex = getNearbyTileCoord(startInGraph->getPosition());
                for (auto ite = nearbyVertex.begin(); ite != nearbyVertex.end(); ite ++) {
                    auto findIteInGraph = getIte(_graph, *ite);
                    auto vertexInGraph = *findIteInGraph;
                    if(!vertexInGraph->getMarked() && !contains(_openList, vertexInGraph)){
                        float lengthFromStart = (startInGraph->getPosition() - vertexInGraph->getPosition()).length();
                        auto weight = startInGraph->getWeight() + lengthFromStart;
                        insertIntoOpenList(vertexInGraph, weight);
                        
                        if(vertexInGraph->getWeight() > weight){
                            vertexInGraph->setWeight(weight);
                            vertexInGraph->setParent(startInGraph);
                        }
                    }
                }
                
                startInGraph = _openList.front();
                _openList.erase(_openList.begin());
            }
            
            while(_openList.size() != 0 && !startInGraph->getPosition().equals(toCoord));
            
#if DEBUG_PRINT
            CCLOG("*** PATH SEARCH END :");
#endif
            
            auto to = *getIte(_graph, toCoord);
#if DEBUG_PRINT
            CCLOG("start dump path: dijksra");
#endif
            while (to->getParent() != nullptr) {
#if DEBUG_PRINT
                CCLOG("%.0f %.0f", to->getPosition().x, to->getPosition().y);
#endif
                result.insert(result.begin(), to->getPosition());
                to = to->getParent();
            }
            //begin pos
#if DEBUG_PRINT
            CCLOG("%.0f %.0f", to->getPosition().x, to->getPosition().y);
#endif
            result.push_back(to->getPosition());
#if DEBUG_PRINT
            CCLOG("end dump path");
#endif
            for (auto ite = _graph.begin(); ite != _graph.end(); ite ++) {
                delete *ite;
            }
            _graph.clear();
            
            return result;
        }
    }
}
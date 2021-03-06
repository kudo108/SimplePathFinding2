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

#ifndef __Funny__PathFinding_Dijkstra__
#define __Funny__PathFinding_Dijkstra__

#include "cocos2d.h"
#include "CollisionData.h"

namespace pathfinding {
    namespace dijkstra {
        class Vertex {
            
        public:
            Vertex(const cocos2d::Vec2& pos) : Vertex() {
                _position = pos;
            }
            
            Vertex() :
            _weight(INT_MAX),
            _parent(NULL),
            _marked(false)
            {}
            
            virtual ~Vertex(){
                _parent = nullptr;
            }
            
            CC_SYNTHESIZE_PASS_BY_REF(cocos2d::Vec2, _position, Position);
            CC_SYNTHESIZE(float, _weight, Weight);
            CC_SYNTHESIZE(Vertex *, _parent, Parent);
            CC_SYNTHESIZE(bool, _marked, Marked);
            
        };
        
        class PathFinding : public cocos2d::Ref {
            
        public:
            
            PathFinding();
            virtual ~PathFinding();
            
            CREATE_FUNC(PathFinding);
            
            void setupMap(CollisionData* map);
            
            std::vector<cocos2d::Vec2> getShortestPath(const cocos2d::Vec2& fromCoord,
                                                       const cocos2d::Vec2& toCoord);
        protected:
            virtual bool init();
            virtual void generateGraph();
            
            CC_SYNTHESIZE_READONLY(CollisionData *, _map, Map);
            CC_SYNTHESIZE_READONLY(std::vector<Vertex *>, _graph, Graph);
            CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<Vertex *>, _openList, OpenList);
            
            void insertIntoOpenList(Vertex *vertex, float weight);
            std::vector<cocos2d::Vec2> getNearbyTileCoord(const cocos2d::Vec2& tileCoord);
            
            inline std::vector<Vertex *>::iterator
            getIte(std::vector<Vertex *>& array, const cocos2d::Vec2& pos){
                for (auto ite = array.begin(); ite != array.end(); ite ++) {
                    if((*ite)->getPosition().equals(pos)){
                        return ite;
                    }
                }
                return array.end();
            }
            
            inline std::vector<Vertex *>::iterator
            getIte(std::vector<Vertex *>& array, Vertex* step){
                for (auto ite = array.begin(); ite != array.end(); ite ++) {
                    if((*ite)->getPosition().equals(step->getPosition())){
                        return ite;
                    }
                }
                return array.end();
            }
            
            inline bool contains(std::vector<Vertex *>& array, Vertex* step){
                return getIte(array, step) != array.end();
            }
            
            inline bool isValidCoord(const cocos2d::Vec2& coord){
                return (coord.x >= 0 && coord.x < _map->getWidth() &&
                        coord.y >= 0 && coord.y < _map->getHeight());
            }
            inline bool canMoveAtCoord(const cocos2d::Vec2& coord){
                return !_map->haveCollisionAtCoord(coord.x, coord.y);
            }
        };
    }
}

#endif /* defined(__Funny__PathFinding_Dijkstra__) */

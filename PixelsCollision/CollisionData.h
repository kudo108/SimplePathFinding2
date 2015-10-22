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

#ifndef __Funny__CollisionData__
#define __Funny__CollisionData__

#include "cocos2d.h"

/** 
 *   Define the mask type.
 *
 *   This type will be effect to the performance so you will need choose right type to use
 *
 *   More bit use in MaskType -> less element in array -> faster check
 */

typedef uint32_t MaskType;

/** 
 *  CollisionData contain information of a map
 */
class CollisionData
{
public:
    CollisionData() :
    _width(0),
    _height(0),
    _map(nullptr)
    {
    };
    
    virtual ~CollisionData()
    {
        CC_SAFE_DELETE_ARRAY(_map);
    }
    
    /**
     *  init with size, default will be have no collision
     *  @returns true if init successful
     */
    virtual bool initWithSize(int width, int height);
    
    /** 
     *   init with read pixels from image
     *   @return true if init successful
     */
    virtual bool initWithFile(const std::string& fileName);
    
    /**
     *  update the collision data
     *  @return true if data changed
     */
    virtual bool setCollisionInfo(int x, int y, bool coli);
    
    /**
     *  check the collision at coordinate
     *  return true if have collision
     */
    bool haveCollisionAtCoord(int x, int y);
    
#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
    /** debug dump map
     */
    void printMap();
#endif
    
protected:
    MaskType* _map;
    
    bool haveCollisionAt(ssize_t pos);
    
    CC_SYNTHESIZE_READONLY(unsigned int, _width, Width);
    CC_SYNTHESIZE_READONLY(unsigned int, _height, Height);
};

#endif /* defined(__Funny__CollisionData__) */

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

#include "CollisionManager.h"

#include <sys/time.h>
#include <stdlib.h>

USING_NS_CC;

static double getTimeMicroSeconds()
{
    struct timeval now;
    gettimeofday(&now,  0);
    return ( ((double) now.tv_sec) * 1000*1000 + now.tv_usec);
}

static CollisionManager *_sharedInstance = nullptr;

CollisionManager* CollisionManager::getInstance()
{
    if(!_sharedInstance){
        _sharedInstance = new CollisionManager();
        _sharedInstance->init();
    }
    return _sharedInstance;
}

bool CollisionManager::init()
{
    return true;
}

bool CollisionManager::loadCollisionData(const std::string &file, const std::string &cachename)
{
    auto m = new CollisionData();
    if(m->initWithFile(file)){
        _collisionArray.insert(std::make_pair(cachename, m));
        return true;
    }else{
        CC_SAFE_DELETE(m);
        return false;
    }
}

CollisionData* CollisionManager::getCollisionData(const std::string &name)
{
    auto ite = _collisionArray.find(name);
    if(ite == _collisionArray.end()){
        return nullptr;
    }
    
    return ite->second;
}
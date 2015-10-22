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

#include "CollisionData.h"

USING_NS_CC;

static int kMaskSize = sizeof(MaskType)*8;

bool CollisionData::initWithSize(int w, int h)
{
    _width = w;
    _height = h;
    
    //calculate how many elemment need
    int totalBitNeed = _width*_height;
    totalBitNeed = (floor(totalBitNeed/(float)kMaskSize) + 1)*kMaskSize;
    _map = new MaskType[totalBitNeed];
    
    int count = 0;
    MaskType c = 0;
    unsigned int idx = 0;
    
    for (int y = _height - 1; y >= 0; y--)
    {
        for (int x = 0; x < _width; x++)
        {
            //can move
            MaskType mask = 1 << (kMaskSize - 1 - count);
            c |= mask;
            
            count ++;
            if(count >= kMaskSize)
            {
                //add & reset all
                _map[idx] = c;
                count = 0;
                c = 0;
                idx ++;
            }
        }
    }
    
    {
        //need add final
        _map[idx] = c;
        c = 0;
        count = 0;
    }
    
    return true;
}

bool CollisionData::initWithFile(const std::string& fileName)
{
    CCLOG("mask size = %d", kMaskSize);
    
    Image* img = new Image();
    img->initWithImageFile(fileName);
    
    _width = img->getWidth();
    _height = img->getHeight();
    
    int count = 0;
    MaskType c = 0;
    unsigned int idx = 0;
    unsigned char * data = img->getData();
    
    for (int y = _height - 1; y >= 0; y--)
    {
        for (int x = 0; x < _width; x++)
        {
            unsigned char *pixel = data + (x + y * _width) * 4;
            
            // You can see/change pixels' RGBA value(0-255) here !
            unsigned char a = *(pixel + 3);
            
            if (a < 10)
            {
                //can move
                MaskType mask = 1 << (kMaskSize - 1 - count);
                c |= mask;
            }
            
            count ++;
            if(count >= kMaskSize)
            {
                //add & reset all
                _map[idx] = (c);
                count = 0;
                c = 0;
                idx ++;
            }
        }
    }
    
    {
        //need add final
        _map[idx] = (c);
        c = 0;
        count = 0;
    }
    
    CCLOG("Collision maps size: %ld", sizeof(_map)/sizeof(*_map));
    
    CC_SAFE_DELETE(img);
    
    return true;
}

bool CollisionData::setCollisionInfo(int x, int y, bool coli)
{
    if(x < 0 || y < 0 || x >= _width || y >= _height){
        return false;
    }
 
    ssize_t pos = x + y * _width;
    ssize_t idx = floor((float)pos/kMaskSize);
    int shift = kMaskSize - 1 - pos%kMaskSize;
    
    MaskType v_Old = _map[idx];
    MaskType mask = (1 << shift);
    MaskType t = mask & v_Old;
    if(t == 0 && coli){
        //already coli before
        return false;
    }else if(t != 0 && !coli){
        //already not coli before
        return false;
    }else{
        //must XOR that value
        MaskType v_New = v_Old ^ mask;
        _map[idx] = v_New;
        return true;
    }
}

bool CollisionData::haveCollisionAt(ssize_t pos)
{
    ssize_t idx = floor((float)pos/kMaskSize);
    MaskType v = _map[idx];
    int shift = kMaskSize - 1 - pos%kMaskSize;
    MaskType mask = (1 << shift);
    MaskType t = mask & v;
    return t == 0;
}

bool CollisionData::haveCollisionAtCoord(int x, int y)
{
    if(x < 0 || y < 0 || x >= _width || y >= _height){
        return false;
    }
    
    ssize_t pos = x + y * _width;
    bool coli =  haveCollisionAt(pos);
    return coli;
}

#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
void CollisionData::printMap()
{
    CCLOG("CollisionData BEGIN DUMP");
    std::string ss;
    
    for (int y = 0; y < _height; y ++) {
        std::string line;
        for (int x = 0; x < _width; x ++) {
            line += (haveCollisionAtCoord(x, y)?"1":"0");
        }
        ss += line;
        ss += '\n';
    }
    
    CCLOG(ss.c_str());
    CCLOG("CollisionData END DUMP");
}

#endif
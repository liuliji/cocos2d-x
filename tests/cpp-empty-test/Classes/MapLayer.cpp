#include "MapLayer.h"
#include "Roads.h"

//#define MAX_ROW_COUNT     10
//#define MAX_COLUMN_COUNT  10
//
//#define BLOCK_SIZE        50
//#define POS_INDEX_SIZE    2
//
//#define RANDOM_ROAD_PERCENT     25

using namespace cocos2d;

/////////////////////////////////
//
// BlockSprite
//
/////////////////////////////////
BlockSprite::BlockSprite(BlockType type)
: _bShownForever(false)
{
    Sprite::init();
    this->setType(type);
    this->setAnchorPoint(Vec2(0,0));
}

BlockSprite::~BlockSprite()
{
    
}

void BlockSprite::hideByFadeOut()
{
    auto doCheck = [this]() {
        if (this->isShownForever()) {
            this->setVisible(true);
            this->setOpacity(255);
        }
    };
    runAction(Sequence::create(FadeOut::create(1.0f), CallFunc::create(doCheck), NULL) );
}

void BlockSprite::setType(BlockType type)
{
    switch (type) {
        case BlockTypeNull:
            break;
        case BlockTypeRoad:
            this->setTexture("block_road.png");
            break;
        case BlockTypeWall:
            //this->setTexture("block_wall.jpg");
            break;
        default:
            break;
    }
    
    _type = type;
}

/////////////////////////////////
//
// Map
//
/////////////////////////////////
MapData::MapData()
{
    for (int i = 0; i < MAX_ROW_COUNT; i++) {
        ColumnData* column = new Vector<BlockSprite*>();
        _data.push_back(column);
        for (int j = 0; j < MAX_COLUMN_COUNT; j++) {
            BlockSprite* block = new BlockSprite(BlockTypeWall);
            column->pushBack(block);
        }
    }

    this->generateRoad();
    this->randomRoad();
}

MapData::~MapData()
{
    for (int i = 0; i < MAX_ROW_COUNT; i++) {
        ColumnData* column = _data.at(i);
        for (int j = 0; j < MAX_COLUMN_COUNT; j++) {
            BlockSprite* block = this->getBlockByIdx(PosIndex(i, j));
            block->release();
        }
        CC_SAFE_DELETE(column);
    }
}

void MapData::generateRoad()
{
    int randValue = RandomHelper::random_int(0, 3);
    std::vector<PosIndex> selectRoad;
    switch (randValue) {
        case 0:
            selectRoad = road1;
            break;
        case 1:
            selectRoad = road2;
            break;
        case 2:
            selectRoad = road3;
            break;
        case 3:
            selectRoad = road4;
            break;
        default:
            break;
    }
    
    int roadStepCount = selectRoad.size();
    for (int i = 0; i < roadStepCount; i++)
    {
        int rowIdx = selectRoad.at(i).rowIdx;
        int columnIdx = selectRoad.at(i).columnIdx;
        PosIndex pos(rowIdx, columnIdx);
        BlockSprite* block = this->getBlockByIdx(pos);
        block->setType(BlockTypeRoad);
    }
    
    _beginPos = selectRoad.at(0);
    _endPos = selectRoad.at(roadStepCount - 1);
}

void MapData::randomRoad()
{
    for (int i = 0; i < MAX_ROW_COUNT; i++) {
        for (int j = 0; j < MAX_COLUMN_COUNT; j++) {
            PosIndex curPos(i, j);
            if (this->isRoadPos(curPos))
            {
                continue;
            }
            
            int randValue = RandomHelper::random_int(1, 100);
            if (randValue <= RANDOM_ROAD_PERCENT)
            {
                auto block = this->getBlockByIdx(curPos);
                block->setType(BlockTypeRoad);
            }
        }
    }
}

bool MapData::isRoadPos(PosIndex idx)
{
    auto block = this->getBlockByIdx(idx);
    if (block != nullptr && block->getType() == BlockTypeRoad)
    {
        return true;
    }
    else
    {
        return false;
    }
}

BlockSprite* MapData::getBlockByIdx(const PosIndex & pos)
{
    if (pos.columnIdx >= MAX_COLUMN_COUNT || pos.columnIdx < 0) {
        return nullptr;
    }
    
    if (pos.rowIdx >= MAX_ROW_COUNT || pos.rowIdx < 0) {
        return nullptr;
    }

    ColumnData* col = _data.at(pos.columnIdx);
    BlockSprite* block = col->at(pos.rowIdx);
    
    return block;
}

PosIndex& MapData::getBeginPos()
{
    return _beginPos;
}

PosIndex& MapData::getEndPos()
{
    return _endPos;
}

/////////////////////////////////
//
// MapLayer
//
/////////////////////////////////
MapLayer::MapLayer()
{
    LayerColor::initWithColor(Color4B(255, 0, 0, 0));
    _map = new MapData();
    for (int i = 0; i < MAX_ROW_COUNT; i++)
    {
        for (int j = 0; j < MAX_COLUMN_COUNT; j++)
        {
            BlockSprite* block = _map->getBlockByIdx(PosIndex(i, j));
            auto blockPos = Vec2(j * BLOCK_SIZE, i * BLOCK_SIZE);
            block->setPosition(blockPos);
            this->addChild(block);
        }
    }
}

void MapLayer::hideAllBlocks()
{
    for (int i = 0; i < MAX_ROW_COUNT; i++)
    {
        for (int j = 0; j < MAX_COLUMN_COUNT; j++)
        {
            BlockSprite* block = _map->getBlockByIdx(PosIndex(i, j));
            if (!block->isShownForever()) {
                block->hideByFadeOut();
            }
        }
    }
}

void MapLayer::showBlock(PosIndex pos, bool bShowForever)
{
    auto block = _map->getBlockByIdx(pos);
    if (block == nullptr)
    {
        return;
    }

    block->setVisible(true);
    block->setOpacity(255);
    if (bShowForever)
    {
        block->setShownForever(bShowForever);
    }
}

bool MapLayer::isRoadPos(PosIndex idx)
{
    return _map->isRoadPos(idx);
}

MapData* MapLayer::getMapData()
{
    return _map;
}

MapLayer::~MapLayer()
{
    CC_SAFE_DELETE(_map);
}

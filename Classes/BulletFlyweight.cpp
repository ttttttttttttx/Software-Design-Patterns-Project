#include "BulletFlyweight.h"

// Static member initialization
std::unordered_map<int, BulletFlyweight> BulletFlyweightFactory::flyweights;

BulletFlyweight::BulletFlyweight(const std::string& spriteFrameName, float speed, int buffID, int ack, int dieID)
    : spriteFrameName(spriteFrameName)
    , speed(speed)
    , buffID(buffID)
    , ack(ack)
    , dieID(dieID) {
}

BulletFlyweight* BulletFlyweightFactory::getFlyweight(int bulletID) {
    // Return existing flyweight if found
    auto it = flyweights.find(bulletID);
    if (it != flyweights.end()) {
        return &it->second;
    }

    // Create new flyweight based on bullet type
    switch(bulletID) {
        case 1:
            flyweights[bulletID] = BulletFlyweight("common_bullet.png", 200.0f, 1, 10, 100);
            break;
        case 2:
            flyweights[bulletID] = BulletFlyweight("through_bullet.png", 250.0f, 2, 15, 101);
            break;
        case 3:
            flyweights[bulletID] = BulletFlyweight("radial_bullet.png", 150.0f, 3, 20, 102);
            break;
        case 4:
            flyweights[bulletID] = BulletFlyweight("static_bullet.png", 0.0f, 4, 25, 103);
            break;
        default:
            return nullptr;
    }

    return &flyweights[bulletID];
}

void BulletFlyweightFactory::cleanup() {
    flyweights.clear();
} 
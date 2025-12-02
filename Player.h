#pragma once
#include "Creature.h"
#include "WorldPosition.h"
#include "Configs.h"
#include "VirtualGamepad.h"
#include "Tiles.h"
#include "Maps.h"
#include "Renderer.h"
#include "Globals.h" 
#include "ChestManager.h"
#include "Inventory.h"   
#include "Items.h"
#include "Arduino.h"
#include "pSprites.h"

class Player : public Creature 
{
public:
    Player(const String& n, int l, int h, int m, int a, int d, int s)
      : Creature(n, l, h, m, a, d, s) 
      { 
      }
    void posClean();
    void PerformCombatAction(Creature& target) override;
    void Draw(LGFX_Sprite* target, int screenX, int screenY) const override;
    void UpdateTopDown();
    enum  Facing { UP, DOWN, LEFT, RIGHT };
    Facing facing = UP;
    void GetTileInFront();
    void OnInteract();   // Called when pressing A
    bool CheckCollision();
    //target tile coordinates for OnInteract in top-down mode
    int interactTargetX = 0;
    int interactTargetY = 0;

    int x = 0; // pixel position
    int y = 0;

    // movement target coordinates
    int moveTargetX;
    int moveTargetY;

    // Smooth pixel position
    float px, py;   // rendered position
    float speed = .2f;
    bool moving = false;
    uint8_t dungeonFloor = 0;
    WorldPosition pos;

    //interior id tracking for context when interacting with chests
    void SetCurrentInteriorID(uint8_t id) { currentInteriorID = id; }
    uint8_t GetCurrentInteriorID() const { return currentInteriorID; }

private:
    Renderer renderer;
    LGFX_Sprite pSprite;
    unsigned long now;
    unsigned long movementDelay = 80;
    unsigned long lastMove = 0;
    unsigned long lastInteractTime = 0;
    static constexpr unsigned long interactCooldown = 180; // ms

    // interior map id the player is currently in (0 = overworld / no interior)
    uint8_t currentInteriorID = 0;

    struct Animator
    {
        uint16_t frame = 0;           // 0 = A, 1 = B
        uint32_t frameInterval = 225;
        uint32_t lastFrameTime = 0;    

        // Return pointer to the correct PROGMEM sprite
        const uint16_t* GetCurrentSprite(Facing facing) const
        {
            switch (facing)
            {
                case DOWN:  return (frame == 0 ? FacingDownA  : FacingDownB);
                case UP:    return (frame == 0 ? FacingUpA    : FacingUpB);
                case LEFT:  return (frame == 0 ? FacingLeftA  : FacingLeftB);
                case RIGHT: return (frame == 0 ? FacingRightA : FacingRightB);
            }
            return FacingDownA;
        }

        void Update(bool isMoving, uint32_t now)
        {
            if (now - lastFrameTime >= frameInterval)
            {
                lastFrameTime = now;
                frame++;
                if (frame >1) frame = 0;
            }
        }
    };  Animator anim;
};

extern Player player;

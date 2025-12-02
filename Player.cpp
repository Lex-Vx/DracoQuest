#include "Player.h"


void Player::PerformCombatAction(Creature& target) 
{
    int dmg = CalculateAttack();
    target.TakeDamage(dmg);
}


void Player::Draw(LGFX_Sprite* target, int screenX, int screenY) const
{
    const uint16_t* spriteData = anim.GetCurrentSprite(facing);

    const uint16_t transparentColor = 0x0240; // color to ignore

    int idx = 0;
    for (int row = 0; row < 16; ++row)
    {
        for (int col = 0; col < 16; ++col)
        {
            uint16_t color = pgm_read_word(&spriteData[idx++]);

            // Only draw if not the transparent color
            if (color != transparentColor)
            {
                target->drawPixel(screenX + col, screenY + row, color);
            }
        }
    }
}

void Player::posClean()
{
    px = x;
    py = y;
    moveTargetX = pos.tileX;
    moveTargetY = pos.tileY;
    moving = false;
}

uint16_t GetTileAt(uint8_t interior, int tileX, int tileY)
{
    const MapInfo* map = GetInterior(interior); // <- use this

    if (!map) return 0;
    if (tileX < 0 || tileY < 0 || tileX >= map->width || tileY >= map->height)
        return 0; // out-of-bounds treated as blocked or empty

    uint32_t index = (uint32_t)tileY * map->width + tileX;
    return pgm_read_word(&map->tiles[index]);
}


bool IsSolid(uint16_t gid)
{
    for (uint8_t i = 0; i < SOLID_TILE_COUNT; i++)
    {
        if (gid == pgm_read_byte(&SOLID_TILES[i]))
            return true;
    }
    return false;
}

bool Player::CheckCollision()
{
    // Compute the tile coordinates in front
    GetTileInFront();

    int tx = interactTargetX;
    int ty = interactTargetY;

    // Fetch GID from the interior map
    uint16_t gid = GetTileAt(currentInteriorID, tx, ty);

    // Check against solid tiles
    return IsSolid(gid);
}



void Player::UpdateTopDown()
{
    now = millis();

    // ──────────────────────────────────
    // MESSAGE WINDOW INPUT HANDLING
    // ──────────────────────────────────
    if (messageWindow.IsVisible())
    {
        if (gamepad.isPressed("A") && (now - lastInteractTime >= interactCooldown))
        {
            lastInteractTime = now;
            messageWindow.AdvancePage();
        }
        return;
    }


    // ──────────────────────────────────
    // MOVEMENT & INTERACTION DECISION
    // ──────────────────────────────────
    if (!moving && (now - lastMove) >= movementDelay)
    {
        int dx = 0;
        int dy = 0;

        if (gamepad.isPressed("UP"))
        {
            dy = -1;
            facing = UP;
        }
        else if (gamepad.isPressed("DOWN"))
        {
            dy = 1;
            facing = DOWN;
        }
        else if (gamepad.isPressed("LEFT"))
        {
            dx = -1;
            facing = LEFT;
        }
        else if (gamepad.isPressed("RIGHT"))
        {
            dx = 1;
            facing = RIGHT;
        }
        else if (gamepad.isPressed("A"))
        {
            if (now - lastInteractTime >= interactCooldown)
            {
                lastInteractTime = now;
                OnInteract();
            }
        }

        if ((dx != 0 || dy != 0) && !CheckCollision())
        {
            moveTargetX = pos.tileX + dx;
            moveTargetY = pos.tileY + dy;
            moving = true;
            lastMove = now;
        }
    }
    // Smoothen movement via linear interpolation
    if (moving)
    {
        float targetX = moveTargetX * TILE_SIZE;
        float targetY = moveTargetY * TILE_SIZE;

        // Time-based per-frame motion
        float dt = (now - lastMove) * 0.001f;  // convert to seconds
        lastMove = now;

        float step = 40.0f * dt;              // pixels per second (tweakable)

        // Move X
        if (fabs(px - targetX) <= step)
            px = targetX;
        else if (px < targetX)
            px += step;
        else
            px -= step;

        // Move Y
        if (fabs(py - targetY) <= step)
            py = targetY;
        else if (py < targetY)
            py += step;
        else
            py -= step;

        // Arrival
        if (px == targetX && py == targetY)
        {
            x = px;
            y = py;

            pos.tileX = moveTargetX;
            pos.tileY = moveTargetY;

            moving = false;
        }
    }

    x = px;
    y = py;
    if (moving) anim.Update(true, now);
}

void Player::GetTileInFront()
{
    int dY = 0, dX = 0;
    switch (facing)
    {
        case UP:
            dY = -1;
            dX = 0;
            break;
        case DOWN:
            dY = 1;
            dX = 0;
            break;
        case LEFT: 
            dX = -1;
            dY = 0;
            break;
        case RIGHT:
            dX = 1;
            dY = 0;
            break;
    }
    interactTargetX = pos.tileX + dX;
    interactTargetY = pos.tileY + dY;
}

void Player::OnInteract()
{
    GetTileInFront();
    int tx = interactTargetX;
    int ty = interactTargetY;

    ChestEntry* ce = chestManager.FindChestAt(currentInteriorID, tx, ty);

    uint8_t itemID = ITEM_NONE;
    if (chestManager.OpenChestAt(currentInteriorID, tx, ty, itemID))
    {
        bool added = inventory.AddItem(itemID);
        if (added)
        {
            messageWindow.Show(String("Thou hast found\n") + ItemName(itemID) + "!");
        }
        else
        {
            messageWindow.Show(String("Found ") + ItemName(itemID) + "\nbut inventory is full.");
        }
        return;
    }

    messageWindow.Show("There is nothing of interest.");
}



/*
 * Autor: Sarah Rachel Estrada Bonilla
 * Fecha de creación: 12/9/2025
 */

#include "../headers/Ghost.h"
#include "../headers/Object.h"
#include "../headers/Pacman.h"
#include <ncursesw/curses.h>
#include <cstdlib>
#include <string>
#include <ctime>
#include <cmath>
#include <algorithm>

Ghost::Ghost(int x, int y, GhostType t, Mapa& mapa, int id) :
    Object(x, y, (t == CONTROLABLE) ? 'M' : 'G'),
    mapa(mapa),
    tipo(t),
    estado(IN_BOX),
    ghostID(id),
    releaseTimer(0)
{
    srand(time(nullptr) + x + y + id);
    velocidad = 1;
    frameCounter = 0;
    direction = 0;
    moveDelay = (t == CONTROLABLE) ? 200 : 50;
    frameCounterDirection = 0;
    moveDirectionDelay = 10 * moveDelay;
    lastKey = 0;
    spawnPoint = {x, y};
    
    int w = mapa.getAncho();
    int h = mapa.getAlto();
    
    switch(id) {
        case 0: targetCorner = {w - 3, 2}; break;
        case 1: targetCorner = {2, 2}; break;
        case 2: targetCorner = {w - 3, h - 3}; break;
        case 3: targetCorner = {2, h - 3}; break;
        default: targetCorner = {w/2, h/2}; break;
    }
    
    if (t != CONTROLABLE) releaseTimer = id * 60;
}

void Ghost::setReleaseTimer(int frames) 
{ 
    releaseTimer = frames; 
}

void Ghost::moveInput() {}

bool Ghost::checkCollision(int tempX, int tempY, const std::vector<Ghost*>& ghosts) const 
{
    if (tempX <= 0 || tempX >= mapa.getAncho() - 1 || 
        tempY <= 0 || tempY >= mapa.getAlto() - 1) 
        return true;
        
    if (estado != IN_BOX && estado != EXITING_BOX) 
        if (mapa.isWall(tempX, tempY)) 
            return true;
            
    for (const auto& fantasma : ghosts) {
        if (fantasma && fantasma != this && 
            fantasma->getX() == tempX && fantasma->getY() == tempY) 
            return true;
    }
    
    return false;
}

int Ghost::getDirectionToTarget(int targetX, int targetY) 
{
    std::vector<int> validDirs;
    
    // Encontrar direcciones válidas
    for (int dir = 0; dir < 4; dir++) {
        int testX = x, testY = y;
        
        if (dir == 0) testY--;
        else if (dir == 1) testY++;
        else if (dir == 2) testX--;
        else if (dir == 3) testX++;
        
        if (testX > 0 && testX < mapa.getAncho() - 1 && 
            testY > 0 && testY < mapa.getAlto() - 1)
            if (estado == IN_BOX || estado == EXITING_BOX || !mapa.isWall(testX, testY)) 
                validDirs.push_back(dir);
    }
    
    if (validDirs.empty()) 
        return direction;
        
    // Encontrar la mejor dirección hacia el objetivo
    int bestDir = validDirs[0];
    int bestScore = 999999;
    
    for (int dir : validDirs) {
        int testX = x, testY = y;
        
        if (dir == 0) testY--;
        else if (dir == 1) testY++;
        else if (dir == 2) testX--;
        else if (dir == 3) testX++;
        
        int score = abs(targetX - testX) + abs(targetY - testY);
        int opposite = (direction + 2) % 4;
        
        if (dir == opposite) 
            score += 50;
            
        if (score < bestScore) { 
            bestScore = score; 
            bestDir = dir; 
        }
    }
    
    return bestDir;
}

void Ghost::exitBox() 
{
    int cx = mapa.getAncho() / 2;
    int cy = mapa.getAlto() / 2;
    int exitY = cy - 3;
    
    if (abs(x - cx) > 0) 
        direction = (x < cx) ? 3 : 2;
    else if (y > exitY) 
        direction = 0;
    else { 
        estado = CHASE; 
        frameCounterDirection = 0; 
    }
}

std::pair<int, int> Ghost::getChaseTarget(Pacman* pacman) 
{
    if (!pacman) 
        return {x, y};
        
    int px = pacman->getX();
    int py = pacman->getY();
    int lastKey = pacman->getLastKey();
    
    switch(ghostID) {
        case 0: 
            return {px, py};
            
        case 1: 
        { 
            int tx = px, ty = py; 
            if (lastKey == KEY_UP) { ty -= 4; tx -= 4; } 
            else if (lastKey == KEY_DOWN) ty += 4; 
            else if (lastKey == KEY_LEFT) tx -= 4; 
            else if (lastKey == KEY_RIGHT) tx += 4; 
            return {tx, ty}; 
        }
        
        case 2: 
        { 
            int midX = px, midY = py; 
            if (lastKey == KEY_UP) midY -= 2; 
            else if (lastKey == KEY_DOWN) midY += 2; 
            else if (lastKey == KEY_LEFT) midX -= 2; 
            else if (lastKey == KEY_RIGHT) midX += 2; 
            return {midX + (midX - x), midY + (midY - y)}; 
        }
        
        case 3: 
        { 
            int dist = abs(px - x) + abs(py - y); 
            return (dist > 8) ? std::make_pair(px, py) : targetCorner; 
        }
        
        default: 
            return {px, py};
    }
}

std::pair<int, int> Ghost::getScatterTarget() 
{ 
    return targetCorner; 
}

std::pair<int, int> Ghost::getFrightenedTarget(Pacman* pacman) 
{
    if (!pacman) 
        return {x, y};
        
    int px = pacman->getX();
    int py = pacman->getY();
    int dx = x - px;
    int dy = y - py;
    
    // Mueve el objetivo lejos en la misma dirección de huida
    return {x + dx * 2, y + dy * 2};
}

void Ghost::moveAI(Pacman* pacman) 
{
    std::pair<int, int> target;
    static int modeTimer = 0;
    static bool chaseMode = true;
    
    if (tipo == ALEATORIO && estado == CHASE) { 
        modeTimer++; 
        if (modeTimer > 400) { 
            chaseMode = !chaseMode; 
            modeTimer = 0; 
        } 
    }
    
    switch(estado) {
        case IN_BOX: 
            if (frameCounterDirection >= moveDirectionDelay) { 
                frameCounterDirection = 0; 
                direction = (direction == 0) ? 1 : 0; 
            } 
            break;
            
        case EXITING_BOX: 
            exitBox(); 
            break;
            
        case CHASE: 
            target = (chaseMode) ? getChaseTarget(pacman) : getScatterTarget(); 
            direction = getDirectionToTarget(target.first, target.second); 
            break;
            
        case SCATTER: 
            target = getScatterTarget(); 
            direction = getDirectionToTarget(target.first, target.second); 
            break;
            
        case FRIGHTENED: 
            target = getFrightenedTarget(pacman); 
            direction = getDirectionToTarget(target.first, target.second); 
            break;
    }
}

bool Ghost::shouldMoveThisFrame() 
{
    frameCounter++;
    
    if (tipo == CONTROLABLE) {
        int currentKey = getch();
        
        if (currentKey != ERR) {
            if (currentKey == 87 || currentKey == 119 || currentKey == 83 || 
                currentKey == 115 || currentKey == 65 || currentKey == 97 || 
                currentKey == 68 || currentKey == 100) { 
                lastKey = currentKey; 
                frameCounter = 0; 
                return true; 
            } else {
                ungetch(currentKey);
            }
        }
        
        if (frameCounter >= moveDelay) { 
            frameCounter = 0; 
            return true; 
        }
    } else {
        frameCounterDirection++;
        if (frameCounter >= moveDelay) { 
            frameCounter = 0; 
            return true; 
        }
    }
    
    return false;
}

void Ghost::update(const std::vector<Ghost*>& ghosts, Pacman* pacman) 
{
    if (estado == IN_BOX && tipo != CONTROLABLE) { 
        if (releaseTimer > 0) { 
            releaseTimer--; 
            return; 
        } else { 
            estado = EXITING_BOX; 
        } 
    }
    
    if (!shouldMoveThisFrame()) 
        return;
        
    if (tipo == CONTROLABLE) 
        moveInput(); 
    else 
        moveAI(pacman);
        
    int tempX = x, tempY = y;
    
    if (tipo == CONTROLABLE) {
        if (lastKey == 87 || lastKey == 119) tempY -= velocidad;
        else if (lastKey == 83 || lastKey == 115) tempY += velocidad;
        else if (lastKey == 65 || lastKey == 97) tempX -= velocidad;
        else if (lastKey == 68 || lastKey == 100) tempX += velocidad;
    } else {
        if (direction == 0) tempY -= velocidad;
        else if (direction == 1) tempY += velocidad;
        else if (direction == 2) tempX -= velocidad;
        else if (direction == 3) tempX += velocidad;
    }
    
    if (!checkCollision(tempX, tempY, ghosts)) { 
        x = tempX; 
        y = tempY; 
    } else {
        // Buscar rutas alternativas cuando hay colisión
        std::vector<int> validDirections;
        
        for (int testDir = 0; testDir < 4; testDir++) {
            int tx = x, ty = y;
            
            if (testDir == 0) ty--;
            else if (testDir == 1) ty++;
            else if (testDir == 2) tx--;
            else if (testDir == 3) tx++;
            
            if (!checkCollision(tx, ty, ghosts)) {
                int opposite = (direction + 2) % 4;
                if (testDir != opposite || validDirections.empty()) 
                    validDirections.push_back(testDir);
            }
        }
        
        if (!validDirections.empty()) {
            int opposite = (direction + 2) % 4;
            std::vector<int> nonOpposite;
            
            for (int d : validDirections) 
                if (d != opposite) 
                    nonOpposite.push_back(d);
                    
            if (!nonOpposite.empty()) 
                direction = nonOpposite[rand() % nonOpposite.size()];
            else 
                direction = validDirections[rand() % validDirections.size()];
                
            frameCounterDirection = 0;
            
            if (direction == 0) y -= velocidad;
            else if (direction == 1) y += velocidad;
            else if (direction == 2) x -= velocidad;
            else if (direction == 3) x += velocidad;
        }
    }
}

void Ghost::setEstado(GhostState nuevoEstado) 
{ 
    estado = nuevoEstado; 
}

void Ghost::respawn() 
{ 
    x = spawnPoint.first; 
    y = spawnPoint.second; 
    estado = IN_BOX; 
    releaseTimer = ghostID * 60; 
}

GhostState Ghost::getEstado() const 
{ 
    return estado; 
}

GhostType Ghost::getTipo() const 
{ 
    return tipo; 
}
#include "Renderer.h"
#include "Player.h"
#include "Monster.h"
#include "TaskSystem.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <cmath>
#include <string>

Renderer::Renderer(int width, int height)
    : screenWidth(width), screenHeight(height),
      ambientLight(0.3f), fogEnabled(true) {
}

void Renderer::initialize() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    setupLighting();
    
    // Enable fog for atmosphere
    if (fogEnabled) {
        glEnable(GL_FOG);
        glFogi(GL_FOG_MODE, GL_LINEAR);
        float fogColor[4] = {0.1f, 0.1f, 0.15f, 1.0f};
        glFogfv(GL_FOG_COLOR, fogColor);
        glFogf(GL_FOG_DENSITY, 0.05f);
        glFogf(GL_FOG_START, 10.0f);
        glFogf(GL_FOG_END, 50.0f);
    }
}

void Renderer::setupLighting() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    float ambient[] = {ambientLight, ambientLight, ambientLight, 1.0f};
    float diffuse[] = {0.7f, 0.7f, 0.7f, 1.0f};
    
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
}

void Renderer::setupPerspective() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(70.0, (double)screenWidth / (double)screenHeight, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void Renderer::setup2D() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, screenWidth, screenHeight, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
}

void Renderer::beginFrame() {
    glClearColor(0.05f, 0.05f, 0.08f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    setupPerspective();
}

void Renderer::endFrame() {
    // Nothing to do here, swap is handled by SDL
}

void Renderer::setCamera(const Vector3& position, float yaw, float pitch) {
    glLoadIdentity();
    
    // Apply camera rotation
    glRotatef(-pitch, 1.0f, 0.0f, 0.0f);
    glRotatef(-yaw, 0.0f, 1.0f, 0.0f);
    
    // Apply camera translation
    glTranslatef(-position.x, -position.y, -position.z);
    
    // Update light position
    float lightPos[] = {position.x, position.y + 5.0f, position.z, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
}

void Renderer::drawCube(const Vector3& pos, const Vector3& size, float r, float g, float b, float a) {
    glPushMatrix();
    glTranslatef(pos.x, pos.y, pos.z);
    
    glColor4f(r, g, b, a);
    
    float w = size.x / 2.0f;
    float h = size.y / 2.0f;
    float d = size.z / 2.0f;
    
    glBegin(GL_QUADS);
    
    // Front face
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-w, -h, d);
    glVertex3f(w, -h, d);
    glVertex3f(w, h, d);
    glVertex3f(-w, h, d);
    
    // Back face
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(-w, -h, -d);
    glVertex3f(-w, h, -d);
    glVertex3f(w, h, -d);
    glVertex3f(w, -h, -d);
    
    // Top face
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-w, h, -d);
    glVertex3f(-w, h, d);
    glVertex3f(w, h, d);
    glVertex3f(w, h, -d);
    
    // Bottom face
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(-w, -h, -d);
    glVertex3f(w, -h, -d);
    glVertex3f(w, -h, d);
    glVertex3f(-w, -h, d);
    
    // Right face
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(w, -h, -d);
    glVertex3f(w, h, -d);
    glVertex3f(w, h, d);
    glVertex3f(w, -h, d);
    
    // Left face
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(-w, -h, -d);
    glVertex3f(-w, -h, d);
    glVertex3f(-w, h, d);
    glVertex3f(-w, h, -d);
    
    glEnd();
    glPopMatrix();
}

void Renderer::drawFloor(float size) {
    glColor3f(0.2f, 0.15f, 0.1f);
    
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    
    // Draw floor with grid pattern
    float step = 5.0f;
    for (float x = -size; x < size; x += step) {
        for (float z = -size; z < size; z += step) {
            float shade = ((int)(x/step + z/step) % 2 == 0) ? 0.2f : 0.18f;
            glColor3f(shade, shade * 0.8f, shade * 0.6f);
            
            glVertex3f(x, 0.0f, z);
            glVertex3f(x + step, 0.0f, z);
            glVertex3f(x + step, 0.0f, z + step);
            glVertex3f(x, 0.0f, z + step);
        }
    }
    
    glEnd();
}

void Renderer::drawWalls(const Room& room) {
    Vector3 pos = room.position;
    Vector3 size = room.size;
    float w = size.x / 2.0f;
    float h = size.y;
    float d = size.z / 2.0f;
    
    glColor3f(0.4f, 0.35f, 0.3f);
    
    glBegin(GL_QUADS);
    
    // Front wall
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(pos.x - w, 0.0f, pos.z + d);
    glVertex3f(pos.x + w, 0.0f, pos.z + d);
    glVertex3f(pos.x + w, h, pos.z + d);
    glVertex3f(pos.x - w, h, pos.z + d);
    
    // Back wall
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(pos.x - w, 0.0f, pos.z - d);
    glVertex3f(pos.x - w, h, pos.z - d);
    glVertex3f(pos.x + w, h, pos.z - d);
    glVertex3f(pos.x + w, 0.0f, pos.z - d);
    
    // Left wall
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(pos.x - w, 0.0f, pos.z - d);
    glVertex3f(pos.x - w, 0.0f, pos.z + d);
    glVertex3f(pos.x - w, h, pos.z + d);
    glVertex3f(pos.x - w, h, pos.z - d);
    
    // Right wall
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(pos.x + w, 0.0f, pos.z - d);
    glVertex3f(pos.x + w, h, pos.z - d);
    glVertex3f(pos.x + w, h, pos.z + d);
    glVertex3f(pos.x + w, 0.0f, pos.z + d);
    
    glEnd();
}

void Renderer::drawDoor(const Door& door) {
    glColor3f(0.3f, 0.2f, 0.15f);
    drawCube(door.position, Vector3(2.0f, 3.0f, 0.2f), 0.3f, 0.2f, 0.15f);
}

void Renderer::drawMonster(const Vector3& pos, float scale) {
    glPushMatrix();
    glTranslatef(pos.x, pos.y + 1.0f, pos.z);
    
    // Draw a creepy dark figure
    glColor3f(0.1f, 0.1f, 0.1f);
    
    // Body
    drawCube(Vector3(0, 0.5f, 0), Vector3(1.0f, 2.0f, 0.5f), 0.1f, 0.1f, 0.1f);
    
    // Head
    glPushMatrix();
    glTranslatef(0, 1.5f, 0);
    
    // Simplified sphere for head
    glColor3f(0.15f, 0.1f, 0.1f);
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i <= 20; i++) {
        float angle = i * 2.0f * M_PI / 20.0f;
        glVertex3f(cos(angle) * 0.5f, sin(angle) * 0.5f, 0);
    }
    glEnd();
    
    // Glowing eyes
    glColor3f(1.0f, 0.1f, 0.1f);
    glPointSize(8.0f);
    glBegin(GL_POINTS);
    glVertex3f(-0.2f, 0.1f, 0.4f);
    glVertex3f(0.2f, 0.1f, 0.4f);
    glEnd();
    
    glPopMatrix();
    glPopMatrix();
}

void Renderer::drawTaskMarker(const Vector3& pos, bool completed) {
    glPushMatrix();
    glTranslatef(pos.x, pos.y + 1.0f, pos.z);
    
    if (completed) {
        glColor4f(0.2f, 0.8f, 0.2f, 0.6f);
    } else {
        glColor4f(0.8f, 0.8f, 0.2f, 0.8f);
        
        // Pulsing effect
        static float pulse = 0.0f;
        pulse += 0.05f;
        float scale = 1.0f + 0.2f * sin(pulse);
        glScalef(scale, scale, scale);
    }
    
    // Draw marker sphere
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i <= 20; i++) {
        float angle = i * 2.0f * M_PI / 20.0f;
        glVertex3f(cos(angle) * 0.3f, sin(angle) * 0.3f, 0);
    }
    glEnd();
    
    glPopMatrix();
}

void Renderer::renderMansion(const std::vector<Room>& rooms, const std::vector<Door>& doors) {
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    
    // Draw floor
    drawFloor(100.0f);
    
    // Draw rooms
    for (const auto& room : rooms) {
        drawWalls(room);
    }
    
    // Draw doors
    for (const auto& door : doors) {
        drawDoor(door);
    }
}

void Renderer::renderPlayer(const Player& player) {
    // Player is the camera, so we don't render it in first person
}

void Renderer::renderMonster(const Monster& monster, const Vector3& playerPos) {
    float distance = (monster.getPosition() - playerPos).length();
    
    // Only render if within reasonable distance
    if (distance < 60.0f) {
        drawMonster(monster.getPosition());
    }
}

void Renderer::renderTasks(const std::vector<Task>& tasks) {
    for (const auto& task : tasks) {
        drawTaskMarker(task.location, task.completed);
    }
}

void Renderer::renderHidingSpots(const std::vector<HidingSpot>& spots) {
    for (const auto& spot : spots) {
        glColor4f(0.4f, 0.4f, 0.6f, 0.5f);
        drawCube(spot.position, Vector3(1.5f, 2.0f, 1.5f), 0.4f, 0.4f, 0.6f, 0.5f);
    }
}

void Renderer::renderHUD(const Player& player, const TaskSystem& taskSystem, const Monster& monster) {
    setup2D();
    
    // Health bar
    float healthPercent = player.getHealth() / 100.0f;
    glColor3f(1.0f - healthPercent, healthPercent, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(10, 10);
    glVertex2f(10 + 200 * healthPercent, 10);
    glVertex2f(10 + 200 * healthPercent, 30);
    glVertex2f(10, 30);
    glEnd();
    
    // Stamina bar
    float staminaPercent = player.getStamina() / 100.0f;
    glColor3f(0.2f, 0.6f, 1.0f);
    glBegin(GL_QUADS);
    glVertex2f(10, 40);
    glVertex2f(10 + 200 * staminaPercent, 40);
    glVertex2f(10 + 200 * staminaPercent, 55);
    glVertex2f(10, 55);
    glEnd();
    
    // Task counter
    renderText("Tasks: " + std::to_string(taskSystem.getCompletedTaskCount()) + "/" + 
               std::to_string(taskSystem.getTotalTaskCount()), 10, 70);
    
    // Current objective
    renderText(taskSystem.getTaskDescription(), 10, screenHeight - 50);
    
    // Hiding indicator
    if (player.isHiding()) {
        renderText("HIDING", screenWidth / 2 - 40, 50, 0.2f, 0.8f, 0.2f);
    }
    
    // Monster proximity warning
    float distToMonster = monster.getDistanceToPlayer(player.getPosition());
    if (distToMonster < 20.0f && !player.isHiding()) {
        float intensity = 1.0f - (distToMonster / 20.0f);
        renderText("DANGER", screenWidth / 2 - 40, 100, 1.0f, 0.0f, 0.0f);
    }
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
}

void Renderer::renderCrosshair() {
    setup2D();
    
    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(2.0f);
    
    int centerX = screenWidth / 2;
    int centerY = screenHeight / 2;
    int size = 10;
    
    glBegin(GL_LINES);
    glVertex2f(centerX - size, centerY);
    glVertex2f(centerX + size, centerY);
    glVertex2f(centerX, centerY - size);
    glVertex2f(centerX, centerY + size);
    glEnd();
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
}

void Renderer::renderText(const std::string& text, int x, int y, float r, float g, float b) {
    // Simple text rendering using raster position
    // In a full game, would use a proper font rendering library
    glColor3f(r, g, b);
    glRasterPos2i(x, y);
    
    // Placeholder - actual text rendering would require a font library
    // For now, just draw a rectangle to indicate text location
    glDisable(GL_LIGHTING);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + text.length() * 8, y);
    glVertex2f(x + text.length() * 8, y + 15);
    glVertex2f(x, y + 15);
    glEnd();
}

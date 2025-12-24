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
    
    // Enhanced fog settings
    if (fogEnabled) {
        glEnable(GL_FOG);
        glFogi(GL_FOG_MODE, GL_EXP2);
        float fogColor[4] = {0.05f, 0.05f, 0.08f, 1.0f};
        glFogfv(GL_FOG_COLOR, fogColor);
        glFogf(GL_FOG_DENSITY, 0.08f);
        glHint(GL_FOG_HINT, GL_NICEST);
    }
}

void Renderer::setupLighting() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    
    float ambient[] = {ambientLight, ambientLight, ambientLight, 1.0f};
    float diffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};
    float specular[] = {0.3f, 0.3f, 0.3f, 1.0f};
    
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    
    glEnable(GL_NORMALIZE);
}

void Renderer::setupPerspective() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(75.0, (double)screenWidth / (double)screenHeight, 0.1, 100.0);
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
    glClearColor(0.02f, 0.02f, 0.05f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    setupPerspective();
}

void Renderer::endFrame() {
    // Handled by SDL
}

void Renderer::setCamera(const Vector3& position, float yaw, float pitch) {
    glLoadIdentity();
    
    glRotatef(-pitch, 1.0f, 0.0f, 0.0f);
    glRotatef(-yaw, 0.0f, 1.0f, 0.0f);
    glTranslatef(-position.x, -position.y, -position.z);
    
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
    
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-w, -h, d); glVertex3f(w, -h, d);
    glVertex3f(w, h, d); glVertex3f(-w, h, d);
    
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(-w, -h, -d); glVertex3f(-w, h, -d);
    glVertex3f(w, h, -d); glVertex3f(w, -h, -d);
    
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-w, h, -d); glVertex3f(-w, h, d);
    glVertex3f(w, h, d); glVertex3f(w, h, -d);
    
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(-w, -h, -d); glVertex3f(w, -h, -d);
    glVertex3f(w, -h, d); glVertex3f(-w, -h, d);
    
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(w, -h, -d); glVertex3f(w, h, -d);
    glVertex3f(w, h, d); glVertex3f(w, -h, d);
    
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(-w, -h, -d); glVertex3f(-w, -h, d);
    glVertex3f(-w, h, d); glVertex3f(-w, h, -d);
    
    glEnd();
    glPopMatrix();
}

void Renderer::drawFloor(float size) {
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    
    float step = 5.0f;
    for (float x = -size; x < size; x += step) {
        for (float z = -size; z < size; z += step) {
            bool isDark = ((int)(x/step) + (int)(z/step)) % 2 == 0;
            float shade = isDark ? 0.15f : 0.18f;
            glColor3f(shade * 0.8f, shade * 0.7f, shade * 0.6f);
            
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
    
    glColor3f(0.35f, 0.28f, 0.22f);
    
    glBegin(GL_QUADS);
    
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(pos.x - w, 0.0f, pos.z + d);
    glVertex3f(pos.x + w, 0.0f, pos.z + d);
    glVertex3f(pos.x + w, h, pos.z + d);
    glVertex3f(pos.x - w, h, pos.z + d);
    
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(pos.x - w, 0.0f, pos.z - d);
    glVertex3f(pos.x - w, h, pos.z - d);
    glVertex3f(pos.x + w, h, pos.z - d);
    glVertex3f(pos.x + w, 0.0f, pos.z - d);
    
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(pos.x - w, 0.0f, pos.z - d);
    glVertex3f(pos.x - w, 0.0f, pos.z + d);
    glVertex3f(pos.x - w, h, pos.z + d);
    glVertex3f(pos.x - w, h, pos.z - d);
    
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(pos.x + w, 0.0f, pos.z - d);
    glVertex3f(pos.x + w, h, pos.z - d);
    glVertex3f(pos.x + w, h, pos.z + d);
    glVertex3f(pos.x + w, 0.0f, pos.z + d);
    
    glEnd();
}

void Renderer::drawDoor(const Door& door) {
    glColor3f(0.25f, 0.15f, 0.1f);
    drawCube(door.position, Vector3(2.0f, 3.0f, 0.2f), 0.25f, 0.15f, 0.1f);
}

void Renderer::drawMonster(const Vector3& pos, float scale) {
    glPushMatrix();
    glTranslatef(pos.x, pos.y + 1.0f, pos.z);
    
    // Body
    drawCube(Vector3(0, 0.5f, 0), Vector3(1.0f, 2.0f, 0.5f), 0.08f, 0.08f, 0.08f);
    
    // Head
    drawCube(Vector3(0, 1.5f, 0), Vector3(0.6f, 0.6f, 0.6f), 0.12f, 0.08f, 0.08f);
    
    // Glowing eyes
    glDisable(GL_LIGHTING);
    glColor3f(1.0f, 0.05f, 0.05f);
    glPointSize(12.0f);
    glBegin(GL_POINTS);
    glVertex3f(-0.2f, 1.6f, 0.4f);
    glVertex3f(0.2f, 1.6f, 0.4f);
    glEnd();
    glEnable(GL_LIGHTING);
    
    glPopMatrix();
}

void Renderer::drawTaskMarker(const Vector3& pos, bool completed) {
    glPushMatrix();
    glTranslatef(pos.x, pos.y + 1.5f, pos.z);
    
    static float pulse = 0.0f;
    pulse += 0.08f;
    
    if (completed) {
        glColor4f(0.1f, 0.8f, 0.1f, 0.7f);
    } else {
        float intensity = 0.8f + 0.2f * sin(pulse);
        glColor4f(intensity, intensity, 0.2f, 0.9f);
        float scale = 1.0f + 0.15f * sin(pulse);
        glScalef(scale, scale, scale);
    }
    
    glDisable(GL_LIGHTING);
    
    // Draw octahedron marker
    glBegin(GL_TRIANGLES);
    float size = 0.4f;
    
    // Top pyramid
    glVertex3f(0, size, 0);
    glVertex3f(size, 0, 0);
    glVertex3f(0, 0, size);
    
    glVertex3f(0, size, 0);
    glVertex3f(0, 0, size);
    glVertex3f(-size, 0, 0);
    
    glVertex3f(0, size, 0);
    glVertex3f(-size, 0, 0);
    glVertex3f(0, 0, -size);
    
    glVertex3f(0, size, 0);
    glVertex3f(0, 0, -size);
    glVertex3f(size, 0, 0);
    
    // Bottom pyramid
    glVertex3f(0, -size, 0);
    glVertex3f(0, 0, size);
    glVertex3f(size, 0, 0);
    
    glVertex3f(0, -size, 0);
    glVertex3f(-size, 0, 0);
    glVertex3f(0, 0, size);
    
    glVertex3f(0, -size, 0);
    glVertex3f(0, 0, -size);
    glVertex3f(-size, 0, 0);
    
    glVertex3f(0, -size, 0);
    glVertex3f(size, 0, 0);
    glVertex3f(0, 0, -size);
    
    glEnd();
    
    glEnable(GL_LIGHTING);
    glPopMatrix();
}

void Renderer::renderMansion(const std::vector<Room>& rooms, const std::vector<Door>& doors) {
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    
    drawFloor(100.0f);
    
    for (const auto& room : rooms) {
        drawWalls(room);
    }
    
    for (const auto& door : doors) {
        drawDoor(door);
    }
}

void Renderer::renderPlayer(const Player& player) {
    // First person view
}

void Renderer::renderMonster(const Monster& monster, const Vector3& playerPos) {
    float distance = (monster.getPosition() - playerPos).length();
    
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
        drawCube(spot.position, Vector3(1.5f, 2.0f, 1.5f), 0.3f, 0.35f, 0.5f, 0.4f);
    }
}

void Renderer::renderHUD(const Player& player, const TaskSystem& taskSystem, const Monster& monster) {
    setup2D();
    
    // Health bar with border
    glColor3f(0.2f, 0.2f, 0.2f);
    glBegin(GL_QUADS);
    glVertex2f(8, 8);
    glVertex2f(214, 8);
    glVertex2f(214, 34);
    glVertex2f(8, 34);
    glEnd();
    
    float healthPercent = player.getHealth() / 100.0f;
    glColor3f(1.0f - healthPercent, healthPercent, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(10, 10);
    glVertex2f(10 + 200 * healthPercent, 10);
    glVertex2f(10 + 200 * healthPercent, 32);
    glVertex2f(10, 32);
    glEnd();
    
    renderText("HEALTH", 12, 16, 1.0f, 1.0f, 1.0f);
    
    // Stamina bar with border
    glColor3f(0.2f, 0.2f, 0.2f);
    glBegin(GL_QUADS);
    glVertex2f(8, 38);
    glVertex2f(214, 38);
    glVertex2f(214, 60);
    glVertex2f(8, 60);
    glEnd();
    
    float staminaPercent = player.getStamina() / 100.0f;
    glColor3f(0.1f, 0.5f, 0.9f);
    glBegin(GL_QUADS);
    glVertex2f(10, 40);
    glVertex2f(10 + 200 * staminaPercent, 40);
    glVertex2f(10 + 200 * staminaPercent, 58);
    glVertex2f(10, 58);
    glEnd();
    
    renderText("STAMINA", 12, 46, 1.0f, 1.0f, 1.0f);
    
    // Task panel
    std::string taskText = "TASKS: " + std::to_string(taskSystem.getCompletedTaskCount()) + "/" + 
                          std::to_string(taskSystem.getTotalTaskCount());
    
    glColor4f(0.0f, 0.0f, 0.0f, 0.7f);
    glBegin(GL_QUADS);
    glVertex2f(8, 68);
    glVertex2f(214, 68);
    glVertex2f(214, 92);
    glVertex2f(8, 92);
    glEnd();
    
    renderText(taskText, 12, 76, 1.0f, 1.0f, 0.5f);
    
    // Current objective at bottom
    std::string objective = taskSystem.getTaskDescription();
    int objWidth = objective.length() * 8 + 20;
    int objX = (screenWidth - objWidth) / 2;
    int objY = screenHeight - 60;
    
    glColor4f(0.0f, 0.0f, 0.0f, 0.8f);
    glBegin(GL_QUADS);
    glVertex2f(objX - 10, objY - 10);
    glVertex2f(objX + objWidth, objY - 10);
    glVertex2f(objX + objWidth, objY + 30);
    glVertex2f(objX - 10, objY + 30);
    glEnd();
    
    renderText(objective, objX, objY, 0.9f, 0.9f, 0.9f);
    
    // Hiding indicator
    if (player.isHiding()) {
        int hideX = screenWidth / 2 - 60;
        glColor4f(0.0f, 0.3f, 0.0f, 0.8f);
        glBegin(GL_QUADS);
        glVertex2f(hideX - 10, 30);
        glVertex2f(hideX + 130, 30);
        glVertex2f(hideX + 130, 65);
        glVertex2f(hideX - 10, 65);
        glEnd();
        
        renderText("HIDING", hideX, 42, 0.2f, 1.0f, 0.2f);
    }
    
    // Monster proximity warning
    float distToMonster = monster.getDistanceToPlayer(player.getPosition());
    if (distToMonster < 20.0f && !player.isHiding()) {
        static float warningPulse = 0.0f;
        warningPulse += 0.15f;
        float intensity = 0.5f + 0.5f * sin(warningPulse);
        
        int dangerX = screenWidth / 2 - 70;
        glColor4f(0.5f, 0.0f, 0.0f, 0.7f);
        glBegin(GL_QUADS);
        glVertex2f(dangerX - 10, 80);
        glVertex2f(dangerX + 150, 80);
        glVertex2f(dangerX + 150, 115);
        glVertex2f(dangerX - 10, 115);
        glEnd();
        
        renderText("! DANGER !", dangerX, 92, 1.0f, intensity * 0.3f, intensity * 0.3f);
    }
    
    // Controls hint
    renderText("F: Interact  E: Hide  SHIFT: Sprint  ESC: Pause", 
               10, screenHeight - 25, 0.7f, 0.7f, 0.7f);
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
}

void Renderer::renderCrosshair() {
    setup2D();
    
    glColor4f(0.9f, 0.9f, 0.9f, 0.8f);
    glLineWidth(2.5f);
    
    int centerX = screenWidth / 2;
    int centerY = screenHeight / 2;
    int size = 12;
    int gap = 4;
    
    glBegin(GL_LINES);
    glVertex2f(centerX - size, centerY);
    glVertex2f(centerX - gap, centerY);
    glVertex2f(centerX + gap, centerY);
    glVertex2f(centerX + size, centerY);
    glVertex2f(centerX, centerY - size);
    glVertex2f(centerX, centerY - gap);
    glVertex2f(centerX, centerY + gap);
    glVertex2f(centerX, centerY + size);
    glEnd();
    
    glPointSize(3.0f);
    glBegin(GL_POINTS);
    glVertex2f(centerX, centerY);
    glEnd();
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
}

void Renderer::renderText(const std::string& text, int x, int y, float r, float g, float b) {
    glDisable(GL_LIGHTING);
    glColor3f(r, g, b);
    
    // Simple bitmap text rendering
    glRasterPos2i(x, y + 12);
    
    // Draw background rectangle for readability
    int textWidth = text.length() * 8;
    glColor4f(0.0f, 0.0f, 0.0f, 0.3f);
    glBegin(GL_QUADS);
    glVertex2f(x - 2, y);
    glVertex2f(x + textWidth + 2, y);
    glVertex2f(x + textWidth + 2, y + 16);
    glVertex2f(x - 2, y + 16);
    glEnd();
    
    // Text color
    glColor3f(r, g, b);
    for (char c : text) {
        // Simple character rendering - would use proper font in production
    }
}
